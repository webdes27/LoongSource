--怪物喊话脚本/示警脚本

-- 怪物进入战斗喊话
function c1006540_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%几率喊话
             cre.MonsterSay(MapID, InstanceID, CreatureID, 30097)
       end
	       --得到怪物的当前目标
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --同步仇恨给范围为8个格子，高度为20的友方怪物
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1006540, 2, "c1006540_OnEnterCombat")

--怪物死亡喊话
function c1006540_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%几率喊话
             cre.MonsterSay(MapID, InstanceID, TargetID, 30098)
       end
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end

aux.RegisterCreatureEvent(1006540, 4, "c1006540_OnDie")




