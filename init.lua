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

local function readAddressFromSingleOpcodeUsage(usageAddress)
  return core.readInteger(usageAddress + 1)
end

local function receiveAbsoluteAddressFromCallOffsetAddress(usageAddress)
  local offsetAddress = readAddressFromSingleOpcodeUsage(usageAddress)
  return offsetAddress + usageAddress + 5
end

local GAME_ADDRESSES = {
  -- structs
  PencilRenderCore = { "B9 ? ? ? ? E8 ? ? ? ? 53 B9 ? ? ? ? E8 ? ? ? ? 6A 08", readAddressFromSingleOpcodeUsage, nil },
  TextManager = { "B9 ? ? ? ? E8 ? ? ? ? 53 6A 03", readAddressFromSingleOpcodeUsage, nil },
  TextureRenderCore = { "B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? E8 ? ? ? ? 53 6A 03", readAddressFromSingleOpcodeUsage, nil },
  WindowAndDirectDraw = { "B9 ? ? ? ? E8 ? ? ? ? 39 ? ? ? ? ? 74 18", readAddressFromSingleOpcodeUsage, nil },
  
  -- TextManager
  RenderSinglelineBlendableTextWithShadow = { "8B 44 24 20 8B 54 24 18 53", nil, nil },
  RenderSinglelineBlendableText = { "83 7C 24 1C 00 53 56 8B F1 75 06 C7 06 00 00 00 00 8B 5C 24 0C 85 DB 74 7C", nil, nil },
  RenderMultilineBlendableTextWithShadow = { "E8 ? ? ? ? 5F 5E C2 10 00 8B 4C 24 08", receiveAbsoluteAddressFromCallOffsetAddress, nil },
  RenderMultilineBlendableText = { "8B 44 24 04 85 C0 56 8B F1 74 2C", nil, nil },
  ComputeTextWidth = { "56 8B 74 24 08 85 F6 75 06", nil, nil },
  
  -- PencilRenderCore
  DrawLine = { "E8 ? ? ? ? 0F ? ? ? ? ? ? 52 8D 45 01", receiveAbsoluteAddressFromCallOffsetAddress, nil },
  DrawRectangle = { "E8 ? ? ? ? 0F ? ? ? ? ? ? 52 8D 6F 0A", receiveAbsoluteAddressFromCallOffsetAddress, nil },
  DrawFilledRectangle = { "E8 ? ? ? ? 8B 44 24 14 8B 4C 24 10 33 D2", receiveAbsoluteAddressFromCallOffsetAddress, nil },
  
  -- TextureRenderCore
  Transform24bitBGRToDisplayFormat = { "8B 44 24 04 8B D0 C1 EA 13", nil, nil },
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