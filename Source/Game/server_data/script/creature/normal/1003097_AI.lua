--怪物喊话脚本

-- 怪物进入战斗喊话
function c1003097_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 27)
       end	
end

aux.RegisterCreatureEvent(1003097, 2, "c1003097_OnEnterCombat")


--怪物死亡喊话
function c1003097_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%几率喊话
             cre.MonsterSay(MapID, InstanceID, TargetID, 28)
       end	

end

aux.RegisterCreatureEvent(1003097, 4, "c1003097_OnDie")

