function I2220104_OnUse(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 2000226, 1, -1, 8, 420)
end

function I2220104_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
		return bRet, bIgnore
	end

	if 0==role.RemoveFromRole(MapID, InstanceID, TargetID, 2220004, 10, 420) then
		return 0, false
	else
		return 32, false
	end

end

aux.RegisterItemEvent(2220104, 1, "I2220104_OnUse")
aux.RegisterItemEvent(2220104, 0, "I2220104_CanUse")
