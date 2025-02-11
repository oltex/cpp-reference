#pragma once
#include "../../design-pettern/singleton/singleton.h"
#include "../../system-component/multi/lock/critical_section.h"
#include "../../system-component/file/file.h"
#include "../../system-component/time/unix.h"
#include "../../system-component/time/date.h"
#include <string_view>
#include <chrono>
#include <iostream>
#include <Windows.h>
#include <unordered_map>

namespace utility {
	class logger final : public design_pattern::singleton<logger> {
	public:
		enum output : unsigned char { console = 0x01, file = 0x02 };
		enum class level : unsigned char { trace, debug, info, warning, error, fatal };
	private:
		friend class design_pattern::singleton<logger>;
		using flag = unsigned char;
		using size_type = unsigned int;
		inline static unsigned char constexpr none = static_cast<unsigned char>(level::fatal) + 1;
		inline static constexpr wchar_t const* const lm[] = { L"trace", L"debug", L"info", L"warning", L"error", L"fatal" };
		struct logged final {
			inline explicit logged(void) noexcept = default;
			inline explicit logged(logged const& rhs) noexcept = delete;
			inline explicit logged(logged&& rhs) noexcept = delete;
			inline auto operator=(logged const& rhs) noexcept -> logged & = delete;
			inline auto operator=(logged&& rhs) noexcept -> logged & = delete;
			inline ~logged(void) noexcept = default;
		public:
			system_component::file _file;
			system_component::multi::lock::critical_section _lock;
		};
	private:
		inline explicit logger(void) noexcept = default;
		inline explicit logger(logger const&) noexcept = delete;
		inline explicit logger(logger&&) noexcept = delete;
		inline auto operator=(logger const&) noexcept -> logger & = delete;
		inline auto operator=(logger&&) noexcept -> logger & = delete;
		inline ~logger(void) noexcept = default;
	public:
		inline void create(std::string_view key, std::wstring path) noexcept {
			auto& iter = _logged.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple()).first->second;
			unsigned long attribute = system_component::file::get_attribute(path.data());
			if (attribute == INVALID_FILE_ATTRIBUTES || (attribute & FILE_ATTRIBUTE_DIRECTORY)) {
				for (size_type index = 0; index < path.size(); ++index) {
					if (L'\\' == path[index]) {
						path[index] = L'\0';
						system_component::file::create_directory(path.data());
						path[index] = L'\\';
					}
				}
				iter._file.create(path.data(), FILE_WRITE_DATA, FILE_SHARE_READ, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH);
				wchar_t const bom = 0xfeff;
				iter._file.write(&bom, sizeof(bom));
			}
			else {
				iter._file.create(path.data(), FILE_WRITE_DATA, FILE_SHARE_READ, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH);
				iter._file.set_pointer_ex(LARGE_INTEGER{ 0 }, FILE_END);
			}
		}
		template <level level_>
		inline void log_message(std::string_view const key, wchar_t const* const format, ...) noexcept {
			//if (0 != _output && level_ >= _level) {
			system_component::time::unix unix(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
			system_component::time::date date(unix.local_time());

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
				auto& iter = _logged.find(key.data())->second;
				iter._lock.enter();
				iter._file.write(buffer, static_cast<unsigned long>(length) * 2);
				iter._lock.leave();
			}
			//}
		}
		template<level level_>
		inline void log_memory(std::string_view const key, wchar_t const* const print, unsigned char* pointer, size_type size, size_type const column) noexcept {
			if (0 != _output && level_ >= _level) {
				system_component::time::unix unix(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
				system_component::time::date date(unix.local_time());

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
					auto& iter = _logged.find(key.data())->second;
					iter._lock.enter();
					iter._file.write(buffer, static_cast<unsigned long>(length) * 2);
					iter._lock.leave();
				}
			}
		}
		inline void set_output(unsigned char const output_) noexcept {
			_output = output_;
		}
		inline auto get_output(void) const noexcept -> unsigned char {
			return _output;
		}
		inline void set_level(level const level_) noexcept {
			_level = level_;
		}
		inline auto get_level(void) const noexcept -> level {
			return _level;
		}
	private:
		unsigned char _output = 0;
		level _level = static_cast<level>(none);
		volatile size_type _count = 0;
		std::unordered_map<std::string, logged> _logged;
	};
}

#define log_message(key, level, format, ...)\
{\
	auto& logger = utility::logger::instance();\
	if(0 != logger.get_output() && level >= logger.get_level()) {\
		logger.log_message<level>(key, format, ##__VA_ARGS__);\
	}\
}

//create_file(L"key", L"test/test/test.log");

//// 비트 OR 연산자 오버로딩
//constexpr logger::output operator|(logger::output a, logger::output b) {
//	return static_cast<logger::output>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));
//}
//
//// 비트 AND 연산자 오버로딩
//constexpr bool operator&(logger::output a, logger::output b) {
//	return (static_cast<unsigned char>(a) & static_cast<unsigned char>(b)) != 0;
//}


//TRACE
//DEBUG: 개발자 디버깅용.
//INFO : 중요한 실행 정보 기록용.
//WARN : 경고성 메시지.
//ERROR : 오류 발생 시 기록.
//FATAL : 치명적인 오류로 인한 중단 시 기록.

// 로그 출력용 함수
// 로깅 일단 c스타일로 가면 가변인자로 들어갈 것이다.
// 나는 c++ 스타일로 스트링으로 간다 해도됨 편의대로 가자
// 외부 네트워크로 전송 DB에 저장하는 방법 간단하다, 근데 유실 가능성이 있음
// 파일 로깅은 유실 확률 0퍼센트 보장함
// 그래서 파일에 저장하는 것을 원칙으로 보는게 맞음
// 
// 여기서의 로그는 문제가 있으니까 로깅되는거임
// 여기서 로그는 모니터링 목적이 아님 속도는 걱정 안해도 됨
// ex) 하도 방치를 하니까 로깅이 하나도 안돼있음, 디스크 용량이 0
//
// 로그 클래스만들고 매크로 사용해서 느낌
// 레벨지정해서 레벨단위로 설정할 수 있게끔 만들자
// 가동되는 과정은 하나하나 로그로 남길거임
// 
// 지금 이걸 전처리기로 안감 런타임에서 판단한다.
// if문으로 비교할거임
// (설정을 바꾸기 위해 빌드를 다시한다라는 것은 없다)
// (빌드를 다시했다는 것은 테스트부터 QA까지 다시 다 해야된다.)
// (또는 라이브에서 실시간으로 변경이 필요할 때가 있음)
//
// 가변인자 했을때 타입에 따른 오류가 있을 수 있어서
// 편하게 string 쓰는법도 고려해보자