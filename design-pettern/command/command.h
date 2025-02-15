#pragma once
#include <vector>
#include <string>
#include <functional>

class command final {
public:
	class parameter final {
	private:
		using size_type = unsigned int;
	public:
		template<typename... argument>
		inline explicit parameter(argument&&... arg) noexcept {
			(_argument.emplace_back(std::forward<argument>(arg)), ...);
		}
		inline explicit parameter(parameter const& rhs) noexcept = delete;
		inline explicit parameter(parameter&& rhs) noexcept = delete;
		inline auto operator=(parameter const& rhs) noexcept -> parameter & = delete;
		inline auto operator=(parameter&& rhs) noexcept -> parameter & = delete;
		inline ~parameter(void) noexcept = default;
	public:
		inline auto get_string(size_type index) noexcept -> std::string& {
			return _argument[index];
		}
		inline auto get_int(size_type index) noexcept -> int {
			return std::stoi(_argument[index]);
		}
		inline auto get_float(size_type index) noexcept -> float {
			return std::stof(_argument[index]);
		}
		inline auto get_bool(size_type index) noexcept -> bool {
			std::string& arg = _argument[index];
			if ("true" == arg || "on" == arg || "1" == arg)
				return true;
			return false;
		}
	private:
		std::vector<std::string> _argument;
	};
public:
	inline explicit command(std::function<int(parameter*)> function) noexcept
		: _function(function) {
	}
	inline explicit command(command const& rhs) noexcept = delete;
	inline explicit command(command&& rhs) noexcept = delete;
	inline auto operator=(command const& rhs) noexcept -> command & = delete;
	inline auto operator=(command&& rhs) noexcept -> command & = delete;
	inline ~command(void) noexcept = default;
public:
	inline int execute(parameter* par) noexcept {
		return _function(par);
	}
	std::function<int(parameter*)> _function;

};