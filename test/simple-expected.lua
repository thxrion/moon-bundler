package.preload["dep1"] = (function()
-- dep1
end)

package.preload["dep2"] = (function()

end)

require("dep1")
require("dep2")

while true do
    wait(0)
    print("______")
end
