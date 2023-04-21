local function __require(f) return f() end
__require(function()
__require(function()
return 3
end)

return 1
end)
__require(function()
__require(function()
return 3
end)

return 2
end)