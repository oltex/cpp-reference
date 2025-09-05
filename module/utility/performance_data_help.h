#pragma once
#include "library/pattern/singleton.h"
#include "library/container/pair.h"
#include "library/container/hash_table.h"
#include "library/container/unique_pointer.h"
#include "library/container/string.h"
#include <Pdh.h>
#pragma comment(lib,"pdh.lib")
#include <list>
#include <iostream>
#include <optional>

namespace library {
	class pdh_counter final {
		PDH_HCOUNTER _counter;
	public:
		inline explicit pdh_counter(PDH_HCOUNTER counter) noexcept
			: _counter(counter) {
		};
		inline explicit pdh_counter(pdh_counter const&) noexcept = default;
		inline pdh_counter(pdh_counter&& rhs) noexcept
			: _counter(rhs._counter) {
			rhs._counter = INVALID_HANDLE_VALUE;
		};
		inline auto operator=(pdh_counter const&) noexcept -> pdh_counter & = default;
		inline auto operator=(pdh_counter&& rhs) noexcept -> pdh_counter& {
			_counter = rhs._counter;
			rhs._counter = INVALID_HANDLE_VALUE;
			return *this;
		};
		inline ~pdh_counter(void) noexcept {
			PdhRemoveCounter(_counter);
		};
	public:
		template<typename type>
			requires library::any_of_type<type, long, long long, double>
		inline auto get_format_value(unsigned long format = 0) noexcept -> type {
			if constexpr (library::same_type<type, long>)
				return get_format_value(PDH_FMT_LONG | format).longValue;
			else if constexpr (library::same_type<type, long long>)
				return get_format_value(PDH_FMT_LARGE | format).largeValue;
			else
				return get_format_value(PDH_FMT_DOUBLE | format).doubleValue;
		};
		inline auto data(void) noexcept -> PDH_HCOUNTER& {
			return _counter;
		}
	private:
		inline auto get_format_value(unsigned long format) noexcept -> PDH_FMT_COUNTERVALUE {
			PDH_FMT_COUNTERVALUE value;
			::PdhGetFormattedCounterValue(_counter, format, nullptr, &value);
			return value;
		}
	};
	class pdh_query final : public library::singleton<pdh_query> {
		friend class library::singleton<pdh_query>;
		PDH_HQUERY _qurey;
		library::unorder_map<library::wstring, pdh_counter> _counter;

		inline explicit pdh_query(void) noexcept {
			::PdhOpenQueryW(nullptr, NULL, &_qurey);
		};
		inline explicit pdh_query(pdh_query const&) noexcept = delete;
		inline explicit pdh_query(pdh_query&&) noexcept = delete;
		inline auto operator=(pdh_query const&) noexcept -> pdh_query & = delete;
		inline auto operator=(pdh_query&&) noexcept -> pdh_query & = delete;
		inline ~pdh_query(void) noexcept {
			::PdhCloseQuery(&_qurey);
		}
	public:
		inline auto add_counter(wchar_t const* path) noexcept -> pdh_counter& {
			auto result = _counter.find(path);
			if (_counter.end() == result) {
				PDH_HCOUNTER handle;
				::PdhAddCounterW(_qurey, path, NULL, &handle);
				result = _counter.emplace(path, handle);
			}
			return (*result)._second;
		}
		inline void collect_query_data(void) noexcept {
			PdhCollectQueryData(_qurey);
		}
		inline void collect_query_data_ex(void) noexcept {
			//PdhCollectQueryDataEx()
		}
		inline auto expand_counter_path(std::wstring_view const path) noexcept {
		}

	};

	inline static auto enum_object(void) noexcept -> std::list<std::wstring> {
		unsigned long object_size = 0;
		PdhEnumObjectsW(nullptr, nullptr, nullptr, &object_size, PERF_DETAIL_WIZARD, TRUE);
		wchar_t* object_buffer = reinterpret_cast<wchar_t*>(malloc(sizeof(wchar_t) * object_size));
		PdhEnumObjectsW(nullptr, nullptr, object_buffer, &object_size, PERF_DETAIL_WIZARD, FALSE);
		wchar_t const* object = object_buffer;

		std::list<std::wstring> result;
#pragma warning(suppress: 6011)
		for (wchar_t const* object = object_buffer; *object != false; object += wcslen(object) + 1)
			result.emplace_back(object);
		free(object_buffer);

		result.sort();
		return result;
	}
	inline static auto enum_item_instance(std::wstring_view const object) noexcept -> std::pair<std::list<std::wstring>, std::list<std::wstring>> {
		unsigned long item_size = 0;
		unsigned long instance_size = 0;
		PdhEnumObjectItemsW(nullptr, nullptr, object.data(), nullptr, &item_size, nullptr, &instance_size, PERF_DETAIL_WIZARD, 0);
		wchar_t* item_buffer = reinterpret_cast<wchar_t*>(malloc(sizeof(wchar_t) * item_size));
		wchar_t* instance_buffer = reinterpret_cast<wchar_t*>(malloc(sizeof(wchar_t) * instance_size));
		PdhEnumObjectItemsW(nullptr, nullptr, object.data(), item_buffer, &item_size, instance_buffer, &instance_size, PERF_DETAIL_WIZARD, 0);

		std::pair<std::list<std::wstring>, std::list<std::wstring>> result;
#pragma warning(suppress: 6011)
		for (wchar_t const* item = item_buffer; *item != false; item += wcslen(item) + 1)
			result.first.emplace_back(item);
#pragma warning(suppress: 6011)
		for (wchar_t const* instance = instance_buffer; *instance != false; instance += wcslen(instance) + 1)
			result.second.emplace_back(instance);

		free(item_buffer);
		free(instance_buffer);

		result.first.sort();
		result.second.sort();
		return result;
	}
}

// 1.PdhExpandCounterPath
// ī���� ��ο��� ���ϵ�ī��(*)�� ���Ե� ��� �̸� Ȯ���Ͽ� ���� ��� ������ ��� ī���� ��θ� ��ȯ�մϴ�.
// ���� ��� Processor(*)\\ % Processor Time ���� ��θ� Ȯ���Ͽ� Processor(0)\\ % Processor Time, Processor(1)\\% Processor Time ���� ���� ��η� ��ȯ�մϴ�.

// 2. PdhLookupPerfNameByIndex
// ���� ī���� �̸��� ������ �ε����� ���� ��ȸ�մϴ�.
// Ư�� ��Ķ(locale)�� ���� ���� ī���� �̸��� �������ų�, �⺻ ��Ķ�� ����� �� �ֽ��ϴ�.

// 3. PdhGetCounterInfo
// Ư�� ī���Ϳ� ���� �� ������ �����ɴϴ�.
// ī���� Ÿ��, �⺻ ����, ����, �⺻��, �ν��Ͻ� ���� ���� ������ ��Ÿ�����͸� Ȯ���� �� �ֽ��ϴ�.

//inline auto add_counter(std::wstring_view const object, std::wstring_view const item, std::wstring_view const instance) noexcept -> pdh_counter {
//	size_t size = sizeof(wchar_t) * (object.size() + item.size() + instance.size() + 7);
//	wchar_t* path = reinterpret_cast<wchar_t*>(malloc(size));
//#pragma warning(suppress: 6387)
//	swprintf_s(path, size / sizeof(wchar_t), L"\\%s(%s)\\%% %s", object.data(), instance.data(), item.data());
//	pdh_counter counter_;
//#pragma warning(suppress: 6387)
//	PdhAddCounterW(_qurey, path, NULL, &counter_.data());
//	free(path);
//	return counter_;
//}
//inline auto add_counter(std::wstring_view const object, std::wstring_view const item) noexcept -> pdh_counter {
//	size_t size = object.size() + item.size() + 5;
//	wchar_t* path = reinterpret_cast<wchar_t*>(malloc(sizeof(wchar_t) * size));
//#pragma warning(suppress: 6387)
//	swprintf_s(path, size / sizeof(wchar_t), L"\\%s\\%% %s", object.data(), item.data());
//	pdh_counter counter_;
//#pragma warning(suppress: 6387)
//	PdhAddCounterW(_qurey, path, NULL, &counter_.data());
//	free(path);
//	return counter_;
//}

//using value_item = PDH_FMT_COUNTERVALUE_ITEM_W;
//inline auto get_formatted_array(unsigned long format) noexcept -> library::pair<unsigned long, library::unique_pointer<value_item[]>> {
//	unsigned long buffer_size = 0;
//	unsigned long item_count;
//	PdhGetFormattedCounterArrayW(_counter, format, &buffer_size, &item_count, nullptr);
//	value_item* counter_value_item_ = reinterpret_cast<value_item*>(malloc(buffer_size));
//	PdhGetFormattedCounterArrayW(_counter, format, &buffer_size, &item_count, counter_value_item_);
//
//	return library::pair<unsigned long, library::unique_pointer<value_item[]>>(item_count, counter_value_item_);
//}