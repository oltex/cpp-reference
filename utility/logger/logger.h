#pragma once
#include "../../design-pettern/singleton/singleton.h"
#include "../../system-component/multi/critical_section.h"
#include "../../system-component/file/file.h"
#include <string_view>
#include <optional>
#include <chrono>

#include <iostream>
#include <Windows.h>
#include <unordered_map>

class logger final : public design_pattern::singleton<logger> {
public:
	enum output : unsigned char { console = 0x01, file = 0x02 };
	enum class level : unsigned char { trace, debug, info, warning, error, fatal };
private:
	friend class design_pattern::singleton<logger>;
	using flag = unsigned char;
	using size_type = unsigned int;
	inline static unsigned char constexpr none = static_cast<unsigned char>(level::fatal) + 1;
	struct logged final {
		inline explicit logged(std::wstring_view path) noexcept
			: _path(path) {
		}
		std::wstring _path;
		multi::lock::critical_section _lock;
	};
private:
	inline explicit logger(void) noexcept {
		create_file(L"key", L"test/test/test.log");
	};
	inline explicit logger(logger const& rhs) noexcept = delete;
	inline explicit logger(logger&& rhs) noexcept = delete;
	inline auto operator=(logger const& rhs) noexcept -> logger & = delete;
	inline auto operator=(logger&& rhs) noexcept -> logger & = delete;
	inline ~logger(void) noexcept = default;
public:
	inline void log_message(flag const output_, level const level_, std::wstring_view const key, wchar_t const* const format, ...) noexcept {
		if (0 != _output && level_ >= _level) {
			constexpr wchar_t const* const lm[] = { L"trace", L"debug", L"info", L"warning", L"error", L"fatal" };
			__time64_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::tm tm; _localtime64_s(&tm, &time);
			size_type count = _InterlockedIncrement(&_count);

			wchar_t string[256];
			int length = _snwprintf_s(string, 255, 255, L"[%d-%02d-%02d %02d:%02d:%02d / %s / %08d] ", tm.tm_year - 100, tm.tm_mon + 1, tm.tm_wday, tm.tm_hour, tm.tm_min, tm.tm_sec, lm[static_cast<unsigned char>(level_)], count);
			va_list va_list_;
			va_start(va_list_, format);
			length += _vsnwprintf_s(string + length, 255 - length, 255 - length, format, va_list_);
			va_end(va_list_);
			length += _snwprintf_s(string + length, 255 - length, 255 - length, L"\n");

			if (static_cast<unsigned char>(output::console) & output_)
				wprintf(string);
			if (static_cast<unsigned char>(output::file) & output_) {
				auto& iter = _logged.find(key.data())->second;
				iter._lock.enter();
				HANDLE handle = CreateFileW(iter._path.data(), FILE_WRITE_DATA, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, nullptr);
				SetFilePointerEx(handle, LARGE_INTEGER{ 0 }, nullptr, FILE_END);
				WriteFile(handle, string, static_cast<unsigned long>(length) * 2, nullptr, nullptr);
				CloseHandle(handle);
				iter._lock.leave();
			}
		}
	}
	inline void log_memory(flag const output_, level const level_, std::wstring_view const key, wchar_t const* const print, unsigned char* pointer, size_type size) noexcept {
		if (0 != _output && level_ >= _level) {
			constexpr wchar_t const* const lm[] = { L"trace", L"debug", L"info", L"warning", L"error", L"fatal" };
			__time64_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::tm tm; _localtime64_s(&tm, &time);
			size_type count = _InterlockedIncrement(&_count);

			wchar_t string[256];
			int length = _snwprintf_s(string, 255, 255, L"[%d-%02d-%02d %02d:%02d:%02d / %s / %08d] ", tm.tm_year - 100, tm.tm_mon + 1, tm.tm_wday, tm.tm_hour, tm.tm_min, tm.tm_sec, lm[static_cast<unsigned char>(level_)], count);
			length += _snwprintf_s(string + length, 255 - length, 255 - length, L"%s", print);

			for (size_type index = 0; index < size; index++) {
				if (0 == index % _column)
					length += _snwprintf_s(string + length, 255 - length, 255 - length, L"\n%p  ", pointer + index);
				length += _snwprintf_s(string + length, 255 - length, 255 - length, L"%02x ", pointer[index]);
			}
			size += _snwprintf_s(string + length, 255 - length, 255 - length, L"\n");

			if (static_cast<unsigned char>(output::console) & output_)
				wprintf(string);
			if (static_cast<unsigned char>(output::file) & output_) {
				auto& iter = _logged.find(key.data())->second;
				iter._lock.enter();
				HANDLE handle = CreateFileW(iter._path.data(), FILE_WRITE_DATA, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, nullptr);
				SetFilePointerEx(handle, LARGE_INTEGER{ 0 }, nullptr, FILE_END);
				WriteFile(handle, string, static_cast<unsigned long>(length) * 2, nullptr, nullptr);
				CloseHandle(handle);
				iter._lock.leave();
			}
		}
	}
public:
	inline void set_level(level const level) noexcept {
		_level = level;
	}
	inline void set_output(flag const output) {
		_output = output;
	}
	inline void set_column(size_type const column) noexcept {
		_column = column;
	}
private:
	inline void create_file(std::wstring_view key, std::wstring_view path) noexcept {
		auto& iter = _logged.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(path)).first->second;
		unsigned long attribute = GetFileAttributesW(iter._path.data());
		if (attribute == INVALID_FILE_ATTRIBUTES || (attribute & FILE_ATTRIBUTE_DIRECTORY)) {
			for (size_type index = 0; index < iter._path.size(); ++index) {
				if (L'\\' == path[index]) {
					iter._path[index] = L'\0';
					CreateDirectoryW(iter._path.data(), nullptr);
					iter._path[index] = L'\\';
				}
			}
			HANDLE handle = CreateFileW(iter._path.data(), FILE_WRITE_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
			wchar_t const bom = 0xfeff;
			WriteFile(handle, &bom, sizeof(bom), nullptr, nullptr);
			CloseHandle(handle);
		}
	}
private:
	flag _output = 0;
	level _level = static_cast<level>(none);
	std::unordered_map<std::wstring, logged> _logged;

	volatile size_type _count = 0;
	size_type _column = 4;
};

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




//inline void log(level const level, std::string_view const value) noexcept {
//	if (0 != _output && level >= _level) {
//		constexpr char const* const lm[] = { "trace", "debug", "info", "Warning", "error", "fatal" };
//		__time64_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//		std::tm tm; _localtime64_s(&tm, &time);

//		std::ostringstream stream;
//		stream << "[" << std::put_time(&tm, "%y/%m/%d %H:%M:%S") << "] [" << lm[static_cast<unsigned char>(level)] << "] " << value << "\n";
//		std::string string = stream.str();

//		if (static_cast<unsigned char>(output::console) & _output)
//			std::cout << string;
//		if (static_cast<unsigned char>(output::file) & _output)
//			WriteFile(_file, string.c_str(), static_cast<unsigned long>(string.size()), NULL, NULL);
//	}
//}


//inline void set_output_console(bool const enable) noexcept {
//	switch (enable) {
//	case false:
//		_output &= ~static_cast<flag>(output::console);
//		break;
//	case true:
//		_output |= static_cast<flag>(output::console);
//		break;
//	}
//}
//inline void set_output_file(bool const enable) noexcept {
//	switch (enable) {
//	case false:
//		_output &= ~static_cast<flag>(output::file);
//		break;
//	case true:
//		_output |= static_cast<flag>(output::file);
//		break;
//	}
//}
