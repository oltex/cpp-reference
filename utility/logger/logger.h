#pragma once
#include "../../design-pettern/singleton/singleton.h"
#include "../../system-component/multi/lock/critical_section.h"
#include "../../system-component/file/file.h"
#include "../../system-component/time/unix/unix.h"
#include "../../system-component/time/date/date.h"
#include <string_view>
#include <chrono>
#include <iostream>
#include <Windows.h>

namespace utility {
	class logger final {
	public:
		enum output : unsigned char { console = 0x01, file = 0x02 };
		enum class level : unsigned char { trace, debug, info, warning, error, fatal };
	private:
		friend class library::design_pattern::singleton<logger>;
		using flag = unsigned char;
		using size_type = unsigned int;
		inline static unsigned char constexpr none = static_cast<unsigned char>(level::fatal) + 1;
		inline static constexpr wchar_t const* const lm[] = { L"trace", L"debug", L"info", L"warning", L"error", L"fatal" };
	public:
		inline explicit logger(void) noexcept = default;
		inline explicit logger(std::wstring path) noexcept {
			create(path);
		};
		inline explicit logger(logger const&) noexcept = delete;
		inline explicit logger(logger&&) noexcept = delete;
		inline auto operator=(logger const&) noexcept -> logger & = delete;
		inline auto operator=(logger&&) noexcept -> logger & = delete;
		inline ~logger(void) noexcept = default;
	public:
		inline void create(std::wstring path) noexcept {
			unsigned long attribute = library::system::file::get_attribute(path.data());
			if (attribute == INVALID_FILE_ATTRIBUTES || (attribute & FILE_ATTRIBUTE_DIRECTORY)) {
				for (size_type index = 0; index < path.size(); ++index) {
					if (L'\\' == path[index]) {
						path[index] = L'\0';
						library::system::file::create_directory(path.data());
						path[index] = L'\\';
					}
				}
				_file.create(path.data(), FILE_WRITE_DATA, FILE_SHARE_READ, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH);
				wchar_t const bom = 0xfeff;
				_file.write(&bom, sizeof(bom));
			}
			else {
				_file.create(path.data(), FILE_WRITE_DATA, FILE_SHARE_READ, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH);
				_file.set_pointer_ex(LARGE_INTEGER{ 0 }, FILE_END);
			}
		}
		template <level level_>
		inline void log_message(wchar_t const* const format, ...) noexcept {
			//if (0 != _output && level_ >= _level) {
			library::system::time::unix unix(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
			library::system::time::date date(unix.local_time());

			wchar_t buffer[256];
			int length = _snwprintf_s(buffer, 255, 255, L"[%d-%02d-%02d %02d:%02d:%02d / %s / %08d] ", date.year(), date.month(), date.day_of_month(), date.hour(), date.minute(), date.second(), lm[static_cast<unsigned char>(level_)], _InterlockedIncrement(&_count));
			va_list va_list_;
			va_start(va_list_, format);
			length += _vsnwprintf_s(buffer + length, 255 - length, 255 - length, format, va_list_);
			va_end(va_list_);
			length += _snwprintf_s(buffer + length, 255 - length, 255 - length, L"\n");

			if (static_cast<unsigned char>(output::console) & _output)
				wprintf(buffer);
			if (static_cast<unsigned char>(output::file) & _output) {
				_lock.enter();
				_file.write(buffer, static_cast<unsigned long>(length) * 2);
				_lock.leave();
			}
			//}
		}
		template<level level_>
		inline void log_memory(std::string_view const key, wchar_t const* const print, unsigned char* pointer, size_type size, size_type const column) noexcept {
			if (0 != _output && level_ >= _level) {
				library::system::time::unix unix(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
				library::system::time::date date(unix.local_time());

				wchar_t buffer[256];
				int length = _snwprintf_s(buffer, 255, 255, L"[%d-%02d-%02d %02d:%02d:%02d / %s / %08d] ", date.year(), date.month(), date.day_of_month(), date.hour(), date.minute(), date.second(), lm[static_cast<unsigned char>(level_)], _InterlockedIncrement(&_count));
				length += _snwprintf_s(buffer + length, 255 - length, 255 - length, L"%s", print);
				for (size_type index = 0; index < size; index++) {
					if (0 == index % column)
						length += _snwprintf_s(buffer + length, 255 - length, 255 - length, L"\n%p  ", pointer + index);
					length += _snwprintf_s(buffer + length, 255 - length, 255 - length, L"%02x ", pointer[index]);
				}
				size += _snwprintf_s(buffer + length, 255 - length, 255 - length, L"\n");

				if (static_cast<unsigned char>(output::console) & _output)
					wprintf(buffer);
				if (static_cast<unsigned char>(output::file) & _output) {
					_lock.enter();
					_file.write(buffer, static_cast<unsigned long>(length) * 2);
					_lock.leave();
				}
			}
		}

		inline static void set_output(unsigned char const output_) noexcept {
			_output = output_;
		}
		inline static auto get_output(void) noexcept -> unsigned char {
			return _output;
		}
		inline static void set_level(level const level_) noexcept {
			_level = level_;
		}
		inline static auto get_level(void) noexcept -> level {
			return _level;
		}
	private:
		library::system::file _file;
		library::system::multi::lock::critical_section _lock;

		inline static unsigned char _output;
		inline static level _level = static_cast<level>(none);
		inline static volatile size_type _count = 0;
	};
}

#define log_message(logger_, level_, format, ...)\
{\
	if(0 !=  utility::logger::get_output() && level_ >= utility::logger::get_level()) {\
		logger_.log_message<level_>(format, ##__VA_ARGS__);\
	}\
}

//create_file(L"key", L"test/test/test.log");

//// ��Ʈ OR ������ �����ε�
//constexpr logger::output operator|(logger::output a, logger::output b) {
//	return static_cast<logger::output>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));
//}
//
//// ��Ʈ AND ������ �����ε�
//constexpr bool operator&(logger::output a, logger::output b) {
//	return (static_cast<unsigned char>(a) & static_cast<unsigned char>(b)) != 0;
//}


//TRACE
//DEBUG: ������ ������.
//INFO : �߿��� ���� ���� ��Ͽ�.
//WARN : ��� �޽���.
//ERROR : ���� �߻� �� ���.
//FATAL : ġ������ ������ ���� �ߴ� �� ���.

// �α� ��¿� �Լ�
// �α� �ϴ� c��Ÿ�Ϸ� ���� �������ڷ� �� ���̴�.
// ���� c++ ��Ÿ�Ϸ� ��Ʈ������ ���� �ص��� ���Ǵ�� ����
// �ܺ� ��Ʈ��ũ�� ���� DB�� �����ϴ� ��� �����ϴ�, �ٵ� ���� ���ɼ��� ����
// ���� �α��� ���� Ȯ�� 0�ۼ�Ʈ ������
// �׷��� ���Ͽ� �����ϴ� ���� ��Ģ���� ���°� ����
// 
// ���⼭�� �α״� ������ �����ϱ� �α�Ǵ°���
// ���⼭ �α״� ����͸� ������ �ƴ� �ӵ��� ���� ���ص� ��
// ex) �ϵ� ��ġ�� �ϴϱ� �α��� �ϳ��� �ȵ�����, ��ũ �뷮�� 0
//
// �α� Ŭ��������� ��ũ�� ����ؼ� ����
// ���������ؼ� ���������� ������ �� �ְԲ� ������
// �����Ǵ� ������ �ϳ��ϳ� �α׷� �������
// 
// ���� �̰� ��ó����� �Ȱ� ��Ÿ�ӿ��� �Ǵ��Ѵ�.
// if������ ���Ұ���
// (������ �ٲٱ� ���� ���带 �ٽ��Ѵٶ�� ���� ����)
// (���带 �ٽ��ߴٴ� ���� �׽�Ʈ���� QA���� �ٽ� �� �ؾߵȴ�.)
// (�Ǵ� ���̺꿡�� �ǽð����� ������ �ʿ��� ���� ����)
//
// �������� ������ Ÿ�Կ� ���� ������ ���� �� �־
// ���ϰ� string ���¹��� ����غ���