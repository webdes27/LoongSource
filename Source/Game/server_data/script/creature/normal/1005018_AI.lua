--怪物喊话脚本

-- 怪物进入战斗喊话
function c1005018_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20002)
       end	
end

aux.RegisterCreatureEvent(1005018, 2, "c1005018_OnEnterCombat")

