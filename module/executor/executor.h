#pragma once
#include "worker.h"
#include "pattern/singleton.h"
#include "container/vector.h"

class executor final {
	using size_type = unsigned int;
	size_type _round_robin;
	library::vector<worker> _worker;
public:
	inline explicit executor(size_type worker) noexcept
		: _round_robin(0) {
		_worker.resize(worker);
	}
	inline explicit executor(executor const&) noexcept = default;
	inline explicit executor(executor&&) noexcept = default;
	inline auto operator=(executor const&) noexcept -> executor&;
	inline auto operator=(executor&&) noexcept -> executor&;
	inline ~executor(void) noexcept = default;

	template <typename function, typename... argument>
	inline void registed(function&& func, argument&&... arg) noexcept {
		coroutine coroutine = std::invoke(std::forward<function>(func), std::forward<argument>(arg)...);
		_worker[_round_robin].enque(coroutine.data());
	}
	inline void registed(task task) noexcept {
		_worker[_round_robin].enque(task);
	}
};
