local exports = {}

local function getAddress(aob, errorMsg, modifierFunc)
  local address = core.AOBScan(aob, 0x400000)
  if address == nil then
    if type(errorMsg) == "function" then
      log(ERROR, errorMsg())
    else
      log(ERROR, errorMsg)
    end
    error("'renderProvider' can not be initialized.")
  end
  if modifierFunc == nil then
    return address;
  end
  return modifierFunc(address)
end

local function fillAddress(address, fill)
  core.writeCode(address, {fill})
end

local function readAddressFromStructUsage(usageAddress)
  return core.readInteger(usageAddress + 1)
end

local GAME_ADDRESSES = {
  -- structs
  PencilRenderCore = { "B9 ? ? ? ? E8 ? ? ? ? 53 B9 ? ? ? ? E8 ? ? ? ? 6A 08", readAddressFromStructUsage, nil },
  TextManager = { "B9 ? ? ? ? E8 ? ? ? ? 53 6A 03", readAddressFromStructUsage, nil },
  TextureRenderCore = { "B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? E8 ? ? ? ? 53 6A 03", readAddressFromStructUsage, nil },
  WindowAndDirectDraw = { "B9 ? ? ? ? E8 ? ? ? ? 39 ? ? ? ? ? 74 18", readAddressFromStructUsage, nil },
  
  -- TextManager
  --RenderGameInGameText = { "", nil, nil },

  --ComputeGameTextWidth = { "", nil, nil },
  --ComputeTextWidth = { "", nil, nil },
}

exports.enable = function(self, moduleConfig, globalConfig)

  -- get game structs and functions
  for name, addressData in pairs(GAME_ADDRESSES) do
    addressData[3] = getAddress(
      addressData[1],
      function()
        return string.format("'renderProvider' was unable to find the address for '%s'.", name)
      end,
      addressData[2]
    )
  end
  
  local addrOfCallAddrOfTestFunctionMenuToMapSurface = getAddress(
    "E8 ? ? ? ? B9 ? ? ? ? E8 ? ? ? ? 53 B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? E8 ? ? ? ? 39",
    "'renderProvider' was unable to find the address for the 'MenuToMapSurface'-function used for testing.",
    function(foundAddress) return foundAddress + 1 end
  )
   local addrOfTestFunctionMenuToMapSurface = core.readInteger(addrOfCallAddrOfTestFunctionMenuToMapSurface) + addrOfCallAddrOfTestFunctionMenuToMapSurface + 4
  

  --[[ load module ]]--
  
  local requireTable = require("renderProvider.dll") -- loads the dll in memory and runs luaopen_renderProvider
  
  -- fill addresses
  for name, addressToFill in pairs(requireTable.gamePtr) do
    local addressData = GAME_ADDRESSES[name]
    if addressData == nil then
      error(string.format("No data about requested address to fill '%s'. 'renderProvider' can not be initialized.", name))
    end
    local address = addressData[3]
    if addressData == nil then
      error(string.format("No address for requested address to fill '%s'. 'renderProvider' can not be initialized.", name))
    end
    fillAddress(addressToFill, address)
  end
  
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