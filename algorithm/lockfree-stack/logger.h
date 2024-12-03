#pragma once
#include "../../module/multi/spin.h"
#include "../../design-pettern/singleton/singleton.h"
#include <string_view>
#include <chrono>
#include <Windows.h>
#include <list>

class logger final : public design_pattern::singleton<logger> {
private:
	friend class design_pattern::singleton<logger>;
	using size_type = unsigned int;
private:
	inline explicit logger(void) noexcept {
		_file = CreateFileW(L"test.txt", FILE_WRITE_DATA, FILE_SHARE_READ, nullptr, OPEN_ALWAYS, 0, nullptr);
		wchar_t const bom = 0xfeff;
		WriteFile(_file, &bom, sizeof(bom), nullptr, nullptr);

		_heap = HeapCreate(0, 1024 * 1024, 0);
	};
	inline explicit logger(logger const& rhs) noexcept = delete;
	inline explicit logger(logger&& rhs) noexcept = delete;
	inline auto operator=(logger const& rhs) noexcept -> logger & = delete;
	inline auto operator=(logger&& rhs) noexcept -> logger & = delete;
	inline ~logger(void) noexcept {
		CloseHandle(_file);
	}
public:
	inline void log(wchar_t const* const format, ...) noexcept {
		//return;

		//wchar_t* string = reinterpret_cast<wchar_t*>(HeapAlloc(_heap, 0, sizeof(wchar_t) * 256));
		wchar_t* string = reinterpret_cast<wchar_t*>(malloc(sizeof(wchar_t) * 256));
		va_list va_list_;
		va_start(va_list_, format);
		int length = _vsnwprintf_s(string, 255, 255, format, va_list_);
		va_end(va_list_);
		length += _snwprintf_s(string + length, 255 - length, 255 - length, L"\n");

		//_log_list[count] = string;
		_spin.lock();
		//_log_list.push_front(string);
		WriteFile(_file, string, static_cast<unsigned long>(length) * 2, nullptr, nullptr);
		_spin.unlock();
	}
private:
	HANDLE _file;
	HANDLE _heap;
	volatile size_type _count = 0;
	multi::lock::spin _spin;
public:
	std::list<wchar_t*> _log_list;
};
