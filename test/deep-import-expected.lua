package.preload["module"] = (function()
require("samp.events")
require("module.sub_module.module")
end)

package.preload["module.sub_module.module"] = (function()
return "submodule"
end)

require("module")