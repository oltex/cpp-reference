#pragma once
#include "../../design-pettern/singleton/singleton.h"
#include <Pdh.h>
#pragma comment(lib,"pdh.lib")
#include <unordered_map>
#include <string>

#include <iostream>

class performance_monitor final : public design_pattern::singleton<performance_monitor> {
private:
	friend class design_pattern::singleton<performance_monitor>;
private:
	inline explicit performance_monitor(void) noexcept {
		PdhOpenQueryW(nullptr, NULL, &_qurey);
	};
	inline explicit performance_monitor(performance_monitor const& rhs) noexcept = delete;
	inline explicit performance_monitor(performance_monitor&& rhs) noexcept = delete;
	inline auto operator=(performance_monitor const& rhs) noexcept -> performance_monitor & = delete;
	inline auto operator=(performance_monitor&& rhs) noexcept -> performance_monitor & = delete;
	inline ~performance_monitor(void) noexcept {
		PdhCloseQuery(&_qurey);
	}
public:
	inline void add_counter(std::wstring_view const key, std::wstring_view const object, std::wstring_view const counter, std::wstring_view const instance) noexcept {
		size_t size = sizeof(wchar_t) * (object.size() + counter.size() + instance.size() + 7);
		wchar_t* path = reinterpret_cast<wchar_t*>(malloc(size));
		swprintf_s(path, size, L"\\%s(%s)\\%% %s", object.data(), instance.data(), counter.data());
		auto& iter = _counter.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple()).first->second;
		PdhAddCounterW(_qurey, path, NULL, &iter);
		free(path);
	}
	inline void add_counter(std::wstring_view const key, std::wstring_view const object, std::wstring_view const counter) noexcept {
		size_t size = sizeof(wchar_t) * (object.size() + counter.size() + 5);
		wchar_t* path = reinterpret_cast<wchar_t*>(malloc(size));
		swprintf_s(path, size, L"\\%s\\%% %s", object.data(), counter.data());
		auto& iter = _counter.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple()).first->second;
		PdhAddCounterW(_qurey, path, NULL, &iter);
		free(path);
	}
	inline void add_counter(std::wstring_view const key, wchar_t const* const path) noexcept {
		auto& iter = _counter.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple()).first->second;
		PdhAddCounterW(_qurey, path, NULL, &iter);
	}
	inline void remove_counter(std::wstring_view const key) noexcept {
		auto iter = _counter.find(key.data());
		PdhRemoveCounter(_counter.find(key.data())->second);
		_counter.erase(iter);
	}
public:
	inline void collect_query(void) noexcept {
		PdhCollectQueryData(_qurey);
	}
	inline auto get_formatted_counter_value(std::wstring_view const key, unsigned long format) noexcept -> PDH_FMT_COUNTERVALUE {
		auto& iter = _counter.find(key.data())->second;
		PDH_FMT_COUNTERVALUE counter_value;
		PdhGetFormattedCounterValue(iter, format, nullptr, &counter_value);
		return counter_value;
	}
	inline auto get_formatted_counter_array(void) noexcept {

	}
	inline void test(void) noexcept {
		unsigned long object_size = 0;
		PdhEnumObjectsW(NULL, NULL, NULL, &object_size, PERF_DETAIL_WIZARD, TRUE);
		wchar_t* object_buffer = reinterpret_cast<wchar_t*>(malloc(sizeof(wchar_t) * object_size));
		PdhEnumObjectsW(NULL, NULL, object_buffer, &object_size, PERF_DETAIL_WIZARD, FALSE);

		wchar_t const* object = object_buffer;
		while (*object) {
			std::wcout << L"object: " << object << std::endl;

			unsigned long counter_size = 0;
			unsigned long instance_size = 0;
			PdhEnumObjectItemsW(NULL, NULL, object, NULL, &counter_size, NULL, &instance_size, PERF_DETAIL_WIZARD, 0);

			wchar_t* counter_buffer = reinterpret_cast<wchar_t*>(malloc(sizeof(wchar_t) * counter_size));
			wchar_t* instance_buffer = reinterpret_cast<wchar_t*>(malloc(sizeof(wchar_t) * instance_size));
			PdhEnumObjectItemsW(NULL, NULL, object, counter_buffer, &counter_size, instance_buffer, &instance_size, PERF_DETAIL_WIZARD, 0);
			wchar_t const* counter = counter_buffer;
			while (counter_size && *counter) {
				std::wcout << L" counter: " << counter << std::endl;
				counter += wcslen(counter) + 1;
			}

			wchar_t const* instnace = instance_buffer;
			while (instance_size && *instnace) {
				std::wcout << L" instnace: " << instnace << std::endl;
				instnace += wcslen(instnace) + 1;
			}
			object += wcslen(object) + 1;
		}
	}
private:
	PDH_HQUERY _qurey;
	std::unordered_map<std::wstring, PDH_HCOUNTER> _counter;
};

//L"\\Processor(_Total)\\% Processor Time"
//PdhExpandCounterPath¶û PdhLookupPerfNameByIndex¶û PdhGetCounterInfo ¸¦ ¾Ë·ÁÁà