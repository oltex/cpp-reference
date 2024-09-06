
#pragma once

namespace network {
    class remote_procedure_call final {
    enum type {
        test = 1, test2 = 2
    }
    public:
	    inline explicit remote_procedure_call(void) noexcept = default;
	    inline ~remote_procedure_call(void) noexcept = default;
