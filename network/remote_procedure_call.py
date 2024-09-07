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



with open('remote_procedure_call.h', 'w') as open_:
    open_.write(
f"""#pragma once
#include "serialize_buffer.h"
#include <list>

class session;
class remote_procedure_call final {{
enum type {{
    {", ".join(function_._name + " = " + function_._enum for function_ in functions_)}
}};
public:
    inline explicit remote_procedure_call(void) noexcept = default;
    inline explicit remote_procedure_call(remote_procedure_call const& rhs) noexcept = delete;
    inline auto operator=(remote_procedure_call const& rhs) noexcept -> remote_procedure_call & = delete;
    inline explicit remote_procedure_call(remote_procedure_call&& rhs) noexcept = delete;
    inline auto operator=(remote_procedure_call&& rhs) noexcept -> remote_procedure_call & = delete;
    inline ~remote_procedure_call(void) noexcept = default;
public:
    {"\n\t".join("void "+ function_._name + "(std::list<session*> session, " + function_._parameter + ") noexcept;" for function_ in functions_)}
}};"""
    )

with open('remote_procedure_call.cpp', 'w') as open_:
    open_.write(
f"""#include "remote_procedure_call.h"
#include "session.h"
{"\n".join("#include \"" + include_ + "\"" for include_ in includes_)}

{"\n\t".join(
f"""void remote_procedure_call::{function_._name}(std::list<session*> session, {function_._parameter}) noexcept {{
    data_structure::serialize_buffer serialize_buffer;
    serialize_buffer <<{" <<".join(" " + argument_[1] for argument_ in function_._arguments)};
    for(auto& iter : session)
        iter->send(serialize_buffer);
}}"""
for function_ in functions_)}
"""
    )