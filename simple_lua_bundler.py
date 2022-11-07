import os
import sys
import ntpath
import re

ITERATIONS_LIMIT = 64
# for your computer not to die on require loop

REQUIRE_STATEMENT = re.compile("(require\s*(?:[\',\"](.+)[\',\"]|\(*\s*[\',\"](.+)[\',\"]\s*\)+))")
FILE_WITH_EXTENSION = re.compile("(.+)\.+(.+)")
WRAPPED_MODULE_CODE_FORMAT = """(function()
  {0}
end)()"""

def replace_last_occurance_of_substring(string, substring, replacement):
  if substring not in string: return string
  index = string.rfind(substring)
  return string[:index] + replacement + string[index + len(substring):]

def read_file(file_path):
  if not os.path.isfile(file_path): exit()
  stream = open(file_path, "r")
  file_content = stream.read()
  stream.close()
  return file_content

def write_file(file_path, text):
  stream = open(file_path, "w")
  stream.write(text)
  stream.close()

def get_module_path_referenced_in_require_statement(require_statement, project_root = ".\\"):
  base_module_path = REQUIRE_STATEMENT.match(require_statement[0])
  if not base_module_path: return
  base_module_path = (base_module_path[2] or base_module_path[3]).replace(".", "\\")
  for modifier in [".lua", "\init.lua"]:
    module_path = project_root + base_module_path + modifier
    if os.path.exists(module_path): return module_path

def get_next_require_statement_referencing_any_module(lua_code, project_root = ".\\"):
  require_statement = REQUIRE_STATEMENT.search(lua_code)
  if not require_statement: return
  module_path = get_module_path_referenced_in_require_statement(require_statement, project_root)
  if module_path: return require_statement
  end_of_require_statement = require_statement.span()[1]
  rest_of_lua_code = lua_code[end_of_require_statement:]
  return get_next_require_statement_referencing_any_module(rest_of_lua_code, project_root)

def replace_every_require_statement_with_referenced_module_code(lua_code, project_root = ".\\", iterations_occured = 0):
  if iterations_occured > ITERATIONS_LIMIT: exit()
  next_require_statement = get_next_require_statement_referencing_any_module(lua_code, project_root)
  if not next_require_statement: return lua_code
  module_path = get_module_path_referenced_in_require_statement(next_require_statement, project_root)
  module_code = read_file(module_path)
  lua_code = lua_code.replace(next_require_statement[0], WRAPPED_MODULE_CODE_FORMAT.format(module_code))
  return replace_every_require_statement_with_referenced_module_code(lua_code, project_root, iterations_occured + 1)

def get_file_directory(file_path):
  file_name = ntpath.basename(file_path)
  directory = replace_last_occurance_of_substring(file_path, file_name, "")
  return directory

def get_bundle_path(file_path):
  file_name = ntpath.basename(file_path)
  if not file_name: exit()
  match_result = FILE_WITH_EXTENSION.match(file_name)
  bundle_name = "{0}-bundle.{1}".format(match_result[1], match_result[2]) if match_result else "{0}-bundle".format(base_name)
  return replace_last_occurance_of_substring(file_path, file_name, bundle_name)

def get_bundle_code(file_path):
  lua_code = read_file(file_path)
  project_root = get_file_directory(file_path)
  return replace_every_require_statement_with_referenced_module_code(lua_code, project_root)

def create_bundle(file_path):
  bundle_path = get_bundle_path(file_path)
  bundle_code = get_bundle_code(file_path)
  write_file(bundle_path, bundle_code)

if __name__ == "__main__":
  file_path = sys.argv[1]
  if file_path:
    create_bundle(file_path)
  os.system("pause")
