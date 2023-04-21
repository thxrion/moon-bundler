local function __require(f) return f() end
__require(function()
__require(function()
return "submodule"
end)
end)