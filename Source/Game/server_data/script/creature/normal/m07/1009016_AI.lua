--���ﺰ���ű�

-- �������ս������
function c1009016_OnEnterCombat(MapID, InstanceID, CreatureID)
        local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
	      cre.MonsterSay(MapID, InstanceID, CreatureID, 50004)
	end
end

aux.RegisterCreatureEvent(1009016, 2, "c1009016_OnEnterCombat")

