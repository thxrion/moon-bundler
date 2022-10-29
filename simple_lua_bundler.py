import os
import sys
import ntpath
import re

REQUIRE_STATEMENT = re.compile("(require\s*(?:[\',\"](.+)[\',\"]|\(*\s*[\',\"](.+)[\',\"]\s*\)+))")
FILE_WITH_EXTENSION = re.compile("(.+)\.+(.+)")
WRAPPED_MODULE_CODE_FORMAT = """(function()
  {0}
end)()"""

def replace_last_occurance_of_substring(string, substring, replacement):
  if substring not in string: return string
  index = string.rfind(substring)
  return string[:index] + replacement + string[index + len(substring):]

def read_file(_path):
  if not os.path.isfile(_path): exit()
  stream = open(_path, "r")
  file_content = stream.read()
  stream.close()
  return file_content

def write_file(_path, text):
  stream = open(_path, "w")
  stream.write(text)
  stream.close()

def find_require_statement_referenced_module_path(statement, project_root = ".\\"):
  base_module_path = REQUIRE_STATEMENT.match(statement[0])
  if not base_module_path: return

  base_module_path = (base_module_path[2] or base_module_path[3]).replace(".", "\\")
  for modifier in [".lua", "\init.lua"]:
    module_path = project_root + base_module_path + modifier
    if os.path.exists(module_path): return module_path

def find_next_require_statement_referencing_module(lua_code, project_root = ".\\"):
  require_statement = REQUIRE_STATEMENT.search(lua_code)
  if not require_statement: return
  require_module_path = find_require_statement_referenced_module_path(require_statement, project_root)
  if require_module_path: return require_statement
  end_of_require_statement = require_statement.span()[1]
  rest_of_lua_code = lua_code[end_of_require_statement:]
  return find_next_require_statement_referencing_module(rest_of_lua_code, project_root)

def replace_require_statements_with_module_code(lua_code, project_root = ".\\"):
  require_statement = find_next_require_statement_referencing_module(lua_code, project_root)
  if not require_statement: return lua_code
  require_module_path = find_require_statement_referenced_module_path(require_statement, project_root)
  module_code = read_file(require_module_path)
  lua_code = lua_code.replace(require_statement[0], WRAPPED_MODULE_CODE_FORMAT.format(module_code))
  return replace_require_statements_with_module_code(lua_code, project_root)

def get_project_root(_path):
  project_root = replace_last_occurance_of_substring(_path, ntpath.basename(_path), "")
  return project_root

def get_bundle_path(_path):
  base_name = ntpath.basename(_path)
  if not base_name: exit()
  match_result = FILE_WITH_EXTENSION.match(base_name)
  bundle_name = "{0}-bundle.{1}".format(match_result[1], match_result[2]) if match_result else "{0}-bundle".format(base_name)
  return replace_last_occurance_of_substring(_path, base_name, bundle_name)

def get_bundle_code(_path):
  lua_code = read_file(_path)
  project_root = get_project_root(_path)
  return replace_require_statements_with_module_code(lua_code, project_root)

def create_bundle(_path):
  bundle_path = get_bundle_path(_path)
  bundle_code = get_bundle_code(_path)
  write_file(bundle_path, bundle_code)

if __name__ == "__main__":
  _path = sys.argv[1]
  if _path:
    create_bundle(_path)
  os.system("pause")
