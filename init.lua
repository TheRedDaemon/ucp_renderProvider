local exports = {}

exports.enable = function(self, moduleConfig, globalConfig)

  --[[
  local procAddress = core.AOBScan("83 EC 48 A1 ? ? ? ? 33 C4 89 44 ? ? 8B 44 ? ? 8B 4C ? ? 53 55 56 57", 0x400000)
  if procAddress == nil then
    log(ERROR, "'winProcHandler' was unable to find the main WindowProc address.")
    error("'winProcHandler' can not be initialized.")
  end
  ]]--

  --[[ load module ]]--
  
  local requireTable = require("renderProvider.dll") -- loads the dll in memory and runs luaopen_renderProvider

end

exports.disable = function(self, moduleConfig, globalConfig) error("not implemented") end

return exports