local function __require(module)
    return module()
end
__require(function()
__require(function()
return "submodule"
end)
end)