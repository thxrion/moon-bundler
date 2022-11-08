local function __call(f)
  return f()
end
__call(function()
  return "module1"
end)
local module2 = __call(function()
  return "module2"
end)
local module3 = require("module3")