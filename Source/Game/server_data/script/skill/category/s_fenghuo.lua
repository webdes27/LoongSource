--技能——风火雷雨 1010701
--只能在修炼场使用，使用一次消耗一个风火石

function s1010701_CanCast(MapID, InstanceID, SkillID, OwnerID, Dummy)
    if MapID == 2291722097 or MapID == 2291721329 or MapID == 2291721585 or MapID == 2291720817 or MapID == 2291721073 or MapID == 2291720305 or MapID == 2291720561 then
        if role.GetRoleItemNum(OwnerID, 3200464) > 0 then
     		return 0
        else
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 13, 0)
		    msg.AddMsgEvent(MsgID, 1, 600)       --每次使用风火雷雨都需要消耗一枚风火石，您可以在游戏商城购买该道具。�
		    msg.DispatchRoleMsgEvent(OwnerID, MsgID)
            return 7
        end
	else
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 601)       --风火雷雨只能在修炼场中使用。
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
	    return 7
	end
end

function s1010701_Cast(MapID, InstanceID, SkillID, OwnerID)
	role.RemoveFromRole(MapID, InstanceID, OwnerID, 3200464, 1, 420)
	return 0
end


--注册

aux.RegisterSkillEvent(1010701, 1, "s1010701_Cast")
aux.RegisterSkillEvent(1010701, 0, "s1010701_CanCast")
