#include "remote_procedure_call.h"
#include "session.h"
#include "test.h"

void remote_procedure_call::test(std::list<session*> session, int x, int y, int z) noexcept {
    data_structure::serialize_buffer serialize_buffer;
    serialize_buffer << x << y << z;
    for(auto& iter : session)
        iter->send(serialize_buffer);
}
	void remote_procedure_call::test2(std::list<session*> session, int x, int z) noexcept {
    data_structure::serialize_buffer serialize_buffer;
    serialize_buffer << x << z;
    for(auto& iter : session)
        iter->send(serialize_buffer);
}
