--怪物喊话脚本、示警脚本

-- 怪物进入战斗喊话
function c1003120_OnEnterCombat(MapID, InstanceID, CreatureID)

		   cre.MonsterSay(MapID, InstanceID, CreatureID, 79)
	--得到怪物的当前目标
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --同步仇恨给范围为8个格子，高度为20的友方怪物
	g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1003120, 2, "c1003120_OnEnterCombat")

--怪物死亡
function c1003120_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 1, "jinjieshili")
end

aux.RegisterCreatureEvent(1003120, 4, "c1003120_OnDie")
