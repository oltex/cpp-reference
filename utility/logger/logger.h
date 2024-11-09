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