package.preload["module1"] = (function()
require("module3")

return 1
end)

package.preload["module3"] = (function()
return 3
end)

package.preload["module2"] = (function()
require("module3")

return 2
end)

require("module1")
require("module2")