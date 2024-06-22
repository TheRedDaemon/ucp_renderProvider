local exports = {}

local function getAddress(aob, errorMsg, modifierFunc)
  local address = core.AOBScan(aob, 0x400000)
  if address == nil then
    log(ERROR, errorMsg)
    error("'timeProvider' can not be initialized.")
  end
  if modifierFunc == nil then
    return address;
  end
  return modifierFunc(address)
end

local function fillAddress(address, fill)
  core.writeCode(address, {fill})
end

exports.enable = function(self, moduleConfig, globalConfig)

  local addrOfPencilRenderCore = getAddress(
    "B9 ? ? ? ? E8 ? ? ? ? 53 B9 ? ? ? ? E8 ? ? ? ? 6A 08",
    "'renderProvider' was unable to find the address for the 'PencilRenderCore' object.",
    function(foundAddress) return core.readInteger(foundAddress + 1) end
  )

  local addrOfTextManager = getAddress(
    "B9 ? ? ? ? E8 ? ? ? ? 53 6A 03",
    "'renderProvider' was unable to find the address for the 'TextManager' object.",
    function(foundAddress) return core.readInteger(foundAddress + 1) end
  )
  
  local addrOfTextureRenderCore = getAddress(
    "B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? E8 ? ? ? ? 53 6A 03",
    "'renderProvider' was unable to find the address for the 'TextureRenderCore' object.",
    function(foundAddress) return core.readInteger(foundAddress + 1) end
  )
  
  local addrOfWindowAndDirectDraw = getAddress(
    "B9 ? ? ? ? E8 ? ? ? ? 39 ? ? ? ? ? 74 18 ",
    "'renderProvider' was unable to find the address for the 'WindowAndDirectDraw' object.",
    function(foundAddress) return core.readInteger(foundAddress + 1) end
  )
  
  local addrOfCallAddrOfTestFunctionMenuToMapSurface = getAddress(
    "E8 ? ? ? ? B9 ? ? ? ? E8 ? ? ? ? 53 B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? E8 ? ? ? ? 39",
    "'renderProvider' was unable to find the address for the 'MenuToMapSurface'-function used for testing.",
    function(foundAddress) return foundAddress + 1 end
  )
   local addrOfTestFunctionMenuToMapSurface = core.readInteger(addrOfCallAddrOfTestFunctionMenuToMapSurface) + addrOfCallAddrOfTestFunctionMenuToMapSurface + 4
  

  --[[ load module ]]--
  
  local requireTable = require("renderProvider.dll") -- loads the dll in memory and runs luaopen_renderProvider
  
  fillAddress(requireTable.address_PencilRenderCore, addrOfPencilRenderCore)
  fillAddress(requireTable.address_TextManager, addrOfTextManager)
  fillAddress(requireTable.address_TextureRenderCore, addrOfTextureRenderCore)
  fillAddress(requireTable.address_WindowAndDirectDraw, addrOfWindowAndDirectDraw)
  
  core.writeCode(
    requireTable.address_ActualMenuToMapSurface,
    {addrOfTestFunctionMenuToMapSurface}
  )
  
  core.writeCode(
    addrOfCallAddrOfTestFunctionMenuToMapSurface,
    {requireTable.funcAddress_DetouredMenuToMapSurface - addrOfCallAddrOfTestFunctionMenuToMapSurface - 4}
  )

end

exports.disable = function(self, moduleConfig, globalConfig) error("not implemented") end

return exports