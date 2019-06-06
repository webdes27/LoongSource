function I1330617_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 1340016, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1340014, 1, -1, 8, 420)

	local Exp_Give = role.GetRoleLevelUpExp(MapID, InstanceID, TargetID)
	local Level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	local UseTimes = 12

	Exp_Give = Exp_Give * 30000 / (Level+42)^2 / UseTimes

	role.AddRoleExp(MapID, InstanceID, TargetID, Exp_Give)
end

function I1330617_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330617, 1, "I1330617_GiftBag")
aux.RegisterItemEvent(1330617, 0, "I1330617_CanUseGiftBag")

