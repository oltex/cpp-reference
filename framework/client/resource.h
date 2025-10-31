#pragma once
#include "library/pattern/singleton.h"
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/pointer.h"
#include "library/system/guid.h"
#include "library/container/read_copy_update.h"
#include "library/json.hpp"
#include "pool.h"

namespace framework {
	class resource : public base, public library::rcu_base<> {
	protected:
		library::guid _guid;
		library::string _name;
	public:
		explicit resource(void) noexcept = default;
		explicit resource(library::string_view name) noexcept;
		explicit resource(nlohmann::json const& json) noexcept;
		explicit resource(resource const&) noexcept = delete;
		explicit resource(resource&&) noexcept = delete;
		auto operator=(resource const&) noexcept -> resource & = delete;
		auto operator=(resource&&) noexcept -> resource & = delete;
		virtual ~resource(void) noexcept override = default;

		virtual auto type(void) noexcept -> char const* const = 0;
		auto name(void) noexcept -> library::string&;
		auto guid(void) noexcept -> library::guid&;
		virtual void save(nlohmann::json& json) noexcept;
		virtual void open(void) noexcept {};
	};

	class resources : public library::singleton<resources> {
		friend class library::singleton<resources>;

		library::unorder_map<library::guid, pointer<resource>> _guid;
		library::unorder_map<library::string, pointer<resource>> _name;
		std::vector<pointer<resource>> _item;

		explicit resources(void) noexcept;
		explicit resources(resources const&) noexcept = delete;
		explicit resources(resources&&) noexcept = delete;
		auto operator=(resources const&) noexcept -> resources & = delete;
		auto operator=(resources&&) noexcept -> resources & = delete;
		~resources(void) noexcept;
	public:
		template<typename type, typename... argument>
		auto create(library::string_view name, argument&&... arg) noexcept -> pointer<type> {
			auto ptr = pools::instance().allocate<type>(name, std::forward<argument>(arg)...);
			auto rcu_ptr = pointer<type>(ptr);
			_guid.emplace(rcu_ptr->guid(), rcu_ptr);
			_name.emplace(rcu_ptr->name(), rcu_ptr);
			_item.emplace_back(rcu_ptr);
			return rcu_ptr;
		};
		auto create(library::string_view key, nlohmann::json const& json) noexcept -> pointer<resource> {
			auto ptr = pools::instance().allocate(key, json);
			auto rcu_ptr = pointer<resource>(ptr);
			_guid.emplace(rcu_ptr->guid(), rcu_ptr);
			_name.emplace(rcu_ptr->name(), rcu_ptr);
			_item.emplace_back(rcu_ptr);
			return rcu_ptr;
		};

		void destory(library::rcu_pointer<resource> pointer) noexcept;
		template<typename type>
		auto find(library::guid guid) noexcept -> library::rcu_pointer<type> {
			auto result = _guid.find(guid);
			return result->_second;
		}
		template<typename type>
		auto find(library::string_view name) noexcept -> library::rcu_pointer<type> {
			auto result = _name.find(name);
			return result->_second;
		}
		void save(void) noexcept;
		void load(void) noexcept;

		void edit(void) noexcept;
		void create_level(void) noexcept;
		void import_file(void) noexcept;
		void search(void) noexcept;
	};
}