--RLC102  保证完成时的道德为0
--任务完成时
function q10102_OnComplete(MapID, InstanceID, QuestID, OwnerID, EnderID)
	local i=role.GetRoleAttValue(MapID, InstanceID, OwnerID, 74)
	if i > -45 then
	    role.ModRoleAttValue(MapID, InstanceID, OwnerID, 74, -45-i)
	end
end

--注册
aux.RegisterQuestEvent(10102, 1, "q10102_OnComplete")

