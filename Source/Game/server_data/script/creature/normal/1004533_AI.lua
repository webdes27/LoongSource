--怪物喊话脚本/示警脚本

-- 怪物进入战斗喊话
function c1004533_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 10074)
	--得到怪物的当前目标
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --同步仇恨给范围为8个格子，高度为20的友方怪物
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1004533, 2, "c1004533_OnEnterCombat")

--怪物死亡喊话
function c1004533_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10076)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end

aux.RegisterCreatureEvent(1004533, 4, "c1004533_OnDie")


