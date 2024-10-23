#pragma once

namespace thread {
	class peterson_algorithm final {
	public:
		inline explicit peterson_algorithm(void) noexcept = default;
		inline explicit peterson_algorithm(peterson_algorithm const& rhs) noexcept = delete;
		inline explicit peterson_algorithm(peterson_algorithm&& rhs) noexcept = delete;
		inline auto operator=(peterson_algorithm const& rhs) noexcept -> peterson_algorithm & = delete;
		inline auto operator=(peterson_algorithm&& rhs) noexcept -> peterson_algorithm & = delete;
		inline ~peterson_algorithm() noexcept = default;
	public:
		template<bool index>
		inline void lock(void) noexcept {
			_flag[index] = true;
			_turn = index;
			//while (true == _flag[!index] && index == _turn) { 
			//}
			for (;;) {
				if (false == _flag[!index]) {
					a = 0;
					break;
				}
				if (index != _turn) {
					a = 1;
					break;
				}
			}
		};
		template<bool index>
		inline void unlock(void) noexcept {
			_flag[index] = false;
		}
	private:
		volatile bool _flag[2]{};
		volatile bool _turn = 0;

		volatile bool a =0;
	};
}