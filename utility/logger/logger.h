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
