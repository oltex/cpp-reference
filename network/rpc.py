import os
import re
from dataclasses import dataclass, field

pattern = '|'.join(f'{re.escape(word)}' for word in {
    'void', 'int', 'char', 'float', 'double', 'short', 'long', 'long long', 'bool', 'auto', 'decltype', 'nullptr_t',
    'size_t', 'ptrdiff_t', 'wchar_t', 'std::string', 'std::vector'
})

@dataclass
class function:
    _type: str
    _enum: str
    _name: str
    _parameter: str
    _arguments : list = field(default_factory=list)

includes_ = set()
functions_ = []

path_ = os.path.dirname(os.path.abspath(__file__))
for root_, dirs_, files_ in os.walk(path_):
    for file_ in files_:
        path_ = os.path.join(root_, file_)
        name_, extension_ = os.path.splitext(file_)
        if '.h' != extension_:
            continue
        with open(path_, 'r') as open_:
            content_ = open_.read().replace('\n', '').replace('\r', '')
        matches_ = re.findall(r'\[\[(.*?rpc.*?)\]\].+?\b(\w+)\s*?\((.*?)\)', content_)
        for match_ in matches_:
            type_ = re.search(r'rpc::type\((\w+)\)', match_[0]).group(1)
            enum_ = re.search(r'rpc::enum\((\w+)\)', match_[0]).group(1)
            name_ = match_[1]
            parameter_ = match_[2]
            arguments_ = re.findall( r'\b(' + pattern + r')\b.*?\b(\w+)\s*(?=,|$)', match_[2])

            if 'stub' == type_:
                includes_.add(file_)
            functions_.append(function(type_, enum_, name_, parameter_, arguments_))



with open('rpc.h', 'w') as open_:
    open_.write(
f"""#pragma once
#include "session.h"
#include "serialize_buffer.h"
#include <list>

class remote_procedure_call final {{
	struct header {{
		unsigned short _size;
	}};
    enum class type : unsigned short {{
        {", ".join(function_._name + " = " + function_._enum for function_ in functions_)}
    }};
private:
    inline explicit remote_procedure_call(void) noexcept = delete;
    inline explicit remote_procedure_call(remote_procedure_call const& rhs) noexcept = delete;
    inline auto operator=(remote_procedure_call const& rhs) noexcept -> remote_procedure_call & = delete;
    inline explicit remote_procedure_call(remote_procedure_call&& rhs) noexcept = delete;
    inline auto operator=(remote_procedure_call&& rhs) noexcept -> remote_procedure_call & = delete;
    inline ~remote_procedure_call(void) noexcept = delete;
public:
    inline static void accept(session& session) noexcept {{
    }};
    inline static void receive(session& session, data_structure::serialize_buffer& serialize_buffer) noexcept {{
    	while (true) {{
			if (sizeof(header) > session._recv_ring_buffer.size())
				break;
			header header_;
            session._recv_ring_buffer.peek((unsigned char*)&header_, sizeof(header));
			if (header_._size > session._recv_ring_buffer.size())
				break;
            session._recv_ring_buffer.pop(sizeof(header));
			data_structure::serialize_buffer serialize_buffer;
			session._recv_ring_buffer.peek((unsigned char*)serialize_buffer.data(), header_._size);
			session._recv_ring_buffer.pop(header_._size);
			serialize_buffer.move_rear(header_._size);
		}}
    }};
    inline static void send(session& session, data_structure::serialize_buffer& serialize_buffer) noexcept {{
        header header_;
        header_._size = serialize_buffer.rear();
        session._send_ring_buffer.push((unsigned char*)&header_, sizeof(header));
        session._send_ring_buffer.push(serialize_buffer.data(), serialize_buffer.rear());
    }};
	inline static void close(session& session) noexcept {{
    }};
public:
    //stub
    static void stub(session& session, data_structure::serialize_buffer& serialize_buffer) noexcept;
public:
    //proxy
    {"\n\t".join(
    f"""inline static void {function_._name}(std::list<session*> session, {function_._parameter}) noexcept {{
        data_structure::serialize_buffer serialize_buffer;
        serialize_buffer <<{" <<".join(" " + argument_[1] for argument_ in function_._arguments)};
        for (auto& iter : session)
            send(*iter, serialize_buffer);
    }}"""
    for function_ in functions_)}
}};"""
    )



with open('rpc.cpp', 'w') as open_:
    open_.write(
f"""#include "rpc.h"
{"\n".join("#include \"" + include_ + "\"" for include_ in includes_)}

void remote_procedure_call::stub(session& session, data_structure::serialize_buffer& serialize_buffer) noexcept {{
	type type_;
	serialize_buffer >> (unsigned short&)type_;
    switch (type_) {{
    {"\n\t".join(
    f"""case type::{function_._name}: {{
        {function_._parameter.replace(",", ";")};
        serialize_buffer >>{" >>".join(" " + argument_[1] for argument_ in function_._arguments)};
        ::{function_._name}({", ".join(argument_[1] for argument_ in function_._arguments)});
    }}
        break;"""
    for function_ in functions_)}
	default:
		break;
	}}
}}"""
    )

# {"\n\t".join("static void "+ function_._name + "(std::list<session*> session, " + function_._parameter + ") noexcept;" for function_ in functions_)}

# {"\n".join(
# f"""void remote_procedure_call::{function_._name}(std::list<session*> session, {function_._parameter}) noexcept {{
#     data_structure::serialize_buffer serialize_buffer;
#     serialize_buffer <<{" <<".join(" " + argument_[1] for argument_ in function_._arguments)};
#     for (auto& iter : session)
#         send(*iter, serialize_buffer);
# }}"""
# for function_ in functions_)}