import os
import re
from dataclasses import dataclass, field

@dataclass
class info:
    _type: str
    _enum: str
    _function: str
    _parameters : list = field(default_factory=list)

includes_ = set()
infos_ = []

path_ = os.path.dirname(os.path.abspath(__file__))
for root_, dirs_, files_ in os.walk(path_):
    for file_ in files_:
        path_ = os.path.join(root_, file_)
        name_, extension_ = os.path.splitext(file_)
        if '.h' != extension_:
            continue
        with open(path_, 'r') as open_:
            content_ = open_.read().replace('\n', '').replace('\r', '')
            matches_ = re.findall(r'\[\[(.*?rpc.*?)\]\].+?\b(\w+)\s*\((.*?)\)', content_)
            for match_ in matches_:
            
                type_ = re.search(r'rpc::type\((\w+)\)', match_[0]).group(1)
                enum_ = re.search(r'rpc::enum\((\w+)\)', match_[0]).group(1)
                function_ = match_[1]
                parameters_ = re.findall(r'\b(\w+)\s*(?:,|$)', match_[2])

                if 'stub' == type_:
                    includes_.add(file_)
                    print(file_)
                infos_.append(info(type_, enum_, function_, [parameters_]))



print(infos_)
with open('rpc.h', 'w') as open_:
    
    open_.write(
f"""
#pragma once

namespace network {{
class remote_procedure_call final {{
enum type {{
    {', '.join(info_._function +' = ' + info_._enum for info_ in infos_)}
}}
"""
)
