__MODULES = {}

local __require = require
function require(path)
	local module = __MODULES[path]
	if module then
		return module()
	end

	return __require(path)
end
__MODULES["dep1"] = function()
-- dep1
end

__MODULES["dep2"] = function()

end

require("dep1")
require("dep2")

while true do
    wait(0)
    print("______")
end
