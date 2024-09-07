#pragma once
#include "serialize_buffer.h"
#include <list>

class session;
class remote_procedure_call final {
enum type {
    test = 1, test2 = 2
};
public:
    inline explicit remote_procedure_call(void) noexcept = default;
    inline explicit remote_procedure_call(remote_procedure_call const& rhs) noexcept = delete;
    inline auto operator=(remote_procedure_call const& rhs) noexcept -> remote_procedure_call & = delete;
    inline explicit remote_procedure_call(remote_procedure_call&& rhs) noexcept = delete;
    inline auto operator=(remote_procedure_call&& rhs) noexcept -> remote_procedure_call & = delete;
    inline ~remote_procedure_call(void) noexcept = default;
public:
    void stub
public:
    void test(std::list<session*> session, int x, int y, int z) noexcept;
	void test2(std::list<session*> session, int x, int z) noexcept;
};