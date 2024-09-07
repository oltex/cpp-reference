#include "remote_procedure_call.h"
#include "session.h"
#include "test.h"

void remote_procedure_call::stub(session& session, data_structure::serialize_buffer& serialize_buffer) const noexcept {
	type type_;
	serialize_buffer >> (unsigned short)type_;
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

void remote_procedure_call::test(std::list<session*> session, int x, int y, int z) const noexcept {
	data_structure::serialize_buffer serialize_buffer;
	serialize_buffer << x << y << z;
	for (auto& iter : session)
		iter->send(serialize_buffer);
}
void remote_procedure_call::test2(std::list<session*> session, int x, int z) const noexcept {
	data_structure::serialize_buffer serialize_buffer;
	serialize_buffer << x << z;
	for (auto& iter : session)
		iter->send(serialize_buffer);
}
