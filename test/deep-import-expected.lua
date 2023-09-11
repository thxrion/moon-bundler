__MODULES = {}

local __require = require
function require(path)
	local module = __MODULES[path]
	if module then
		return module()
	end

	return __require(path)
end
__MODULES["module"] = function()
require("samp.events")
require("module.sub_module.module")
end

__MODULES["module.sub_module.module"] = function()
return "submodule"
end

require("module")