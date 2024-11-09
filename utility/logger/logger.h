#pragma once
#include "singleton.h"
#include <string_view>
#include <optional>
#include <chrono>

#include <iostream>
#include <Windows.h>

class logger final : public design_pattern::singleton<logger> {
public:
	enum class output : unsigned char { console = 0x01, file = 0x02 };
	enum class level : unsigned char { trace, debug, info, warning, error, fatal };
private:
	friend class design_pattern::singleton<logger>;
	using flag = unsigned char;
	inline static unsigned char constexpr none = static_cast<unsigned char>(level::fatal) + 1;
private:
	inline explicit logger(void) noexcept = default;
	inline explicit logger(logger const& rhs) noexcept = delete;
	inline explicit logger(logger&& rhs) noexcept = delete;
	inline auto operator=(logger const& rhs) noexcept -> logger & = delete;
	inline auto operator=(logger&& rhs) noexcept -> logger & = delete;
	inline ~logger(void) noexcept {
		CloseHandle(_file);
	};
public:
	inline void log(level const level, std::string_view const value) noexcept {
		if (0 != _output && level >= _level) {
			constexpr char const* const lm[] = { "trace", "debug", "info", "Warning", "error", "fatal" };
			auto now = std::chrono::system_clock::now();
			std::time_t time = std::chrono::system_clock::to_time_t(now);
			std::tm tm;
			localtime_s(&tm, &time);

			std::ostringstream oss;
			oss << "[" << std::put_time(&tm, "%y/%m/%d %H:%M:%S") << "] [" << lm[static_cast<unsigned char>(level)] << "] " << value << "\n";
			std::string result = oss.str();

			if (static_cast<unsigned char>(output::console) & _output)
				std::cout << result;
			if (static_cast<unsigned char>(output::file) & _output)
				WriteFile(_file, result.c_str(), static_cast<unsigned long>(result.size()), NULL, NULL);
		}
	}
public:
	inline void set_level(level const level) noexcept {
		_level = level;
	}
	inline void output_console(bool const enable) noexcept {
		switch (enable) {
		case false:
			_output &= ~static_cast<flag>(output::console);
			break;
		case true:
			_output |= static_cast<flag>(output::console);
			break;
		}
	}
	inline void output_file(bool const enable, std::wstring_view const path = L"") noexcept {
		switch (enable) {
		case false:
			_output &= ~static_cast<flag>(output::file);
			break;
		case true:
			_output |= static_cast<flag>(output::file);
			if (!path.empty()) {
				CloseHandle(_file);
				_file = CreateFileW(path.data(), FILE_WRITE_DATA, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
			}
			break;
		}
	}
private:
	flag _output = 0;
	level _level = static_cast<level>(none);
	HANDLE _file = INVALID_HANDLE_VALUE;
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