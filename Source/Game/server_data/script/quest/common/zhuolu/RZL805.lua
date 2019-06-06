--�������ɣ�12805
--����1������������ʯ����ʹ����Ʒ���ʯ��[2612702]�ɷֱ�ˢ�³�������ױ���[1005507]������ɽ��[1005508]�����湫��[1005509]
--���꣺
--[m04: 2663, 2435]
--[m04: 2063, 1979]
--[m04: 1811, 2514]

--��Ʒ���ʯ��ʹ���жϺ���
function i2612702_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false

	--������¹����ʹ��
	if MapID ~= 3017299663 then
	    bRet = 43
	else
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
        --����Ƿ�����Щ�㸽��
        if x>2647 and x<2679 and z>2419 and z<2451 then
            bRet = 0
        elseif x>2047 and x<2079 and z>1963 and z<1995 then
            bRet = 0
        elseif x>1795 and x<1827 and z>2498 and z<2530 then
            bRet = 0
        else
            bRet = 43
        end
    end

    --����
	return bRet, bIgnore
end

--��Ʒ���ʯ��ʹ��Ч������
function i2612702_QuestUsable(MapID, InstanceID, TypeID, TargetID)
    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)

	if x>2647 and x<2679 and z>2419 and z<2451 then
		map.MapCreateCreature(MapID, InstanceID, 1005507, 2663, 20783, 2435)
	elseif x>2047 and x<2079 and z>1963 and z<1995 then
		map.MapCreateCreature(MapID, InstanceID, 1005508, 2063,13898, 1979)
	elseif x>1795 and x<1827 and z>2498 and z<2530 then
		map.MapCreateCreature(MapID, InstanceID, 1005509, 1811, 12354, 2514)
	end
end

--ע��
aux.RegisterItemEvent(2612702, 1, "i2612702_QuestUsable")
aux.RegisterItemEvent(2612702, 0, "i2612702_CanUse")