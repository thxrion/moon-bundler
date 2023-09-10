__MODULES = {}

local __require = require
function require(path)
	local module = __MODULES[path]
	if module then
		return module()
	end

	return __require(path)
end
__MODULES["module1"] = function()
require("module3")

return 1
end

__MODULES["module3"] = function()
return 3
end

__MODULES["module2"] = function()
require("module3")

return 2
end

require("module1")
require("module2")