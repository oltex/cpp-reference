#include "rpc.h"
#include "test.h"

void remote_procedure_call::stub(session& session, data_structure::serialize_buffer& serialize_buffer) noexcept {
	type type_;
	serialize_buffer >> (unsigned short&)type_;
    switch (type_) {
    case type::test: {
        int x; int y; int z;
        serialize_buffer >> x >> y >> z;
        ::test(x, y, z);
    }
        break;
	case type::test2: {
        int x; int z;
        serialize_buffer >> x >> z;
        ::test2(x, z);
    }
        break;
	default:
		break;
	}
}