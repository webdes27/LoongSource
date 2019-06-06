-- 怪物进入战斗触发示警
function c1008035_OnEnterCombat(MapID, InstanceID, CreatureID)
    --得到怪物的当前目标
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --同步仇恨给范围为10个格子，高度为20的友方怪物
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008035, 2, "c1008035_OnEnterCombat")


--怪物喊话脚本


--怪物死亡喊话
function c1008035_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%几率喊话
             cre.MonsterSay(MapID, InstanceID, TargetID, 40037)
       end

end

aux.RegisterCreatureEvent(1008035, 4, "c1008035_OnDie")


