#!python3

import xml.etree.ElementTree as ET

tree = ET.parse("scripts/gl.xml")
root = tree.getroot()

required_commands = []
required_enums = []

for feature in root.findall("feature"):
    if feature.attrib.get("api") == "gl":
        for require in feature.findall("require"):
            for command in require.findall("command"):
                required_commands.append(command.attrib.get("name").strip())
            for enum in require.findall("enum"):
                required_enums.append(enum.attrib.get("name").strip())
        for remove in feature.findall("remove"):
            for command in remove.findall("command"):
                required_commands.remove(command.attrib.get("name").strip())
            for enum in remove.findall("enum"):
                required_enums.remove(enum.attrib.get("name").strip())

required_commands = list(set(required_commands))
required_enums = list(set(required_enums))

required_commands.sort()
required_enums.sort()

command_defs = {}
enum_defs = {}

def extract_type(element):
    s = ""
    for x in list(element.itertext())[:-1]:
        s += x
    s = s.strip()
    if s.endswith(" *"):
        s = s[:-2] + s[-1:]
    return s

for commands in root.findall("commands"):
    for command in commands.findall("command"):
        proto = command.find("proto")
        name = proto.find("name").text.strip()
        if name not in required_commands:
            continue
        return_type = extract_type(proto)
        params = []
        for param in command.findall("param"):
            pname = param.find("name").text.strip()
            ptype = extract_type(param)
            params.append({"type": ptype, "name": pname})
        command_defs[name] = {"rtype": return_type, "name": name, "params": params}

def params_str(params):
    s = ""
    if params:
        for param in params[:-1]:
            s += param["type"] + ' ' + param["name"] + ", "
        param = params[-1]
        s += param["type"] + ' ' + param["name"]
    return s

def param_names(params):
    s = ""
    if (params):
        for param in params[:-1]:
            s += param["name"] + ", "
        param = params[-1]
        s += param["name"]
    return s

def proto_str(command_def):
    return command_def["rtype"] + ' ' + command_def["name"] + '(' + params_str(command_def["params"]) + ");"

def command_ptr(command_def):
    return command_def["rtype"] + " (GLAPIENTRY* " + command_def["name"] + ")(" + params_str(command_def["params"]) + ")"

def tdef_str(command_def):
    return "typedef " + command_def["rtype"] + "(GLAPIENTRY* " + command_def["name"] + "_proc)(" + params_str(command_def["params"]) + ");"

def call_command_ptr(command_def):
    s = command_def["rtype"] + "(*Call_" + command_def["name"] + ")(const Context* context"
    if command_def["params"]:
        s += ", " + params_str(command_def["params"])
    s += ")"
    return s

def call_command_loaded_proto(command_def):
    s = command_def["rtype"] + " Call_" + command_def["name"] + "_Loaded(const Context* context"
    if command_def["params"]:
        s += ", " + params_str(command_def["params"])
    s += ")"
    return s

def call_command_unloaded_proto(command_def):
    s = command_def["rtype"] + " Call_" + command_def["name"] + "_Unloaded(const Context* context"
    if command_def["params"]:
        s += ", " + params_str(command_def["params"])
    s += ")"
    return s

def call_command_loaded(command_def):
    ret = command_def["rtype"]
    name = command_def["name"]
    params = params_str(command_def["params"])
    pnames = param_names(command_def["params"])
    s = call_command_loaded_proto(command_def) + "\n{\n"
    s += "\tcontext->PreGLCall(\"" + name + "\");\n"
    if ret == "void":
        s += "\tcontext->mInternalContext->" + name + "(" + pnames + ");\n"
    else:
        s += "\t" + ret + " result = context->mInternalContext->" + name + "(" + pnames + ");\n"
    s += "\tcontext->PostGLCall(\"" + name + "\");\n"
    if ret != "void":
        s += "\treturn result;\n"
    s += "}\n"
    return s

def call_command_unloaded(command_def):
    ret = command_def["rtype"]
    name = command_def["name"]
    params = params_str(command_def["params"])
    s = call_command_unloaded_proto(command_def) + "\n{\n"
    s += "\tcontext->UnloadedGLFunctionCalled(\"" + name + "\");\n"
    if ret != "void":
        s += "\treturn static_cast<" + ret + ">(0);\n"
    s += "}\n"
    return s

def load_command(name):
    s = "\t\t" + name + " = reinterpret_cast<decltype(" + name + ")>(context->Load(\"" + name + "\")); "
    s += "if (" + name + ") "
    s += "Call_" + name + " = Call_" + name + "_Loaded; "
    s += "else fails++; attempts++;\n"
    return s

def load_all_commands(commands):
    s = "\tint LoadAll()\n"
    s += "\t{\n"
    s += "\t\tint attempts = 0;\n"
    s += "\t\tint fails = 0;\n\n"
    for command in commands:
        s += load_command(command)
    s += "\t\tif (fails == attempts) return -1;\n"
    s += "\t\treturn fails;\n"
    s += "\t}\n"
    return s

def call_command_proto(command_def):
    name = command_def["name"]
    #name = name[2:]
    s = "\t" + command_def["rtype"] + " " + name + "(" + params_str(command_def["params"]) + ") const;\n"
    return s

def call_command(command_def):
    name = command_def["name"]
    newname = name#[2:]
    rtype = command_def["rtype"]
    s = rtype + " Context::" + newname + "(" + params_str(command_def["params"]) + ") const { "
    if (rtype != "void"):
        s += "return "
    s += "mInternalContext->Call_" + name + "(this"
    pnames = param_names(command_def["params"])
    if pnames:
        s += ", "
        s += pnames
    s += "); }\n"
    return s

file = open("src/InternalContext.h", "w")
for command in required_commands:
    file.write(call_command_unloaded_proto(command_defs[command]) + ";\n")
file.write("\n")
for command in required_commands:
    file.write(call_command_loaded_proto(command_defs[command]) + ";\n")
file.write("\n")

file.write("class InternalContext\n{\npublic:\n\tContext* context;\n\n")
for command in required_commands:
    command_def = command_defs[command]
    file.write("\t" + command_ptr(command_def) + " = nullptr;\n")
    file.write("\t" + call_command_ptr(command_def) + " = Call_" + command_def["name"] + "_Unloaded;\n")
file.write("\n" + load_all_commands(required_commands))
file.write("};\n\n")

for command in required_commands:
    file.write(call_command_unloaded(command_defs[command]) + "\n")
for command in required_commands:
    file.write(call_command_loaded(command_defs[command]) + "\n")

file.write('\n')
for command in required_commands:
    file.write(call_command(command_defs[command]))
file.close()

file = open("src/ContextGLFuncProtos.h", "w")
for command in required_commands:
    file.write(call_command_proto(command_defs[command]))