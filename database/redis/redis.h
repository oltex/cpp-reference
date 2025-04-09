#pragma once
#include <cpp_redis/cpp_redis>
#pragma comment (lib, "cpp_redis.lib")
#pragma comment (lib, "tacopie.lib")
#pragma comment (lib, "ws2_32.lib")

namespace library::database {
	class redis final : public cpp_redis::client {
	private:
		using base = cpp_redis::client;
	public:
		inline explicit redis(void) noexcept = default;
		inline explicit redis(redis const&) noexcept = delete;
		inline explicit redis(redis&&) noexcept = delete;
		inline auto operator=(redis const&) noexcept -> redis & = delete;
		inline auto operator=(redis&&) noexcept -> redis & = delete;
		inline ~redis(void) noexcept {
			disconnect();
		};

		//inline auto get(char const* const format, ...) noexcept -> std::future<cpp_redis::reply> {
		//	char buffer[256]{};
		//	va_list va_list_;
		//	va_start(va_list_, format);
		//	int length = _vsnprintf_s(buffer, 255, 255, format, va_list_);
		//	va_end(va_list_);
		//	return base::get(buffer);
		//}
		//inline void get(reply_callback_t const& reply_callback, char const* const format, ...) noexcept {
		//	char buffer[256]{};
		//	va_list va_list_;
		//	va_start(va_list_, format);
		//	int length = _vsnprintf_s(buffer, 255, 255, format, va_list_);
		//	va_end(va_list_);
		//	base::get(buffer, reply_callback);
		//}
	};
}