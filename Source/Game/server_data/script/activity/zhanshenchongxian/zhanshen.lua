--固定活动“战神重现“
zhanshen_monster = {}
zhanshen_monster[1] = {TypeID=1533009}
zhanshen_monster[2] = {TypeID=1533010}
zhanshen_monster[3] = {TypeID=1533011}
zhanshen_monster[4] = {TypeID=1533012}
zhanshen_monster[5] = {TypeID=1533013}
zhanshen_monster[6] = {TypeID=1533014}
zhanshen_monster[7] = {TypeID=1533015}
zhanshen_monster[8] = {TypeID=1533016}
zhanshen_monster[9] = {TypeID=1533017}
zhanshen_monster[10] = {TypeID=1533018}

zhanshen_npc = {}
zhanshen_npc[1] = {TypeID=3021009, x=2020, y=6365, z=1800}
zhanshen_npc[2] = {TypeID=3021010, x=2021, y=6365, z=1791}
zhanshen_npc[3] = {TypeID=3021011, x=2020, y=6365, z=1729}
zhanshen_npc[4] = {TypeID=3021012, x=2020, y=6365, z=1745}
zhanshen_npc[5] = {TypeID=3021013, x=2020, y=6365, z=1737}

zhanshen_MaxNum = 10

zhanshen_Wuyi_Pos = {}
zhanshen_Wuyi_Pos[1] = {x = 2308 ,y = 4299 ,z = 2030}
zhanshen_Wuyi_Pos[2] = {x = 2640 ,y = 6000 ,z = 1953}
zhanshen_Wuyi_Pos[3] = {x = 2471 ,y = 4907 ,z = 1368}
zhanshen_Wuyi_Pos[4] = {x = 2028 ,y = 3250 ,z = 926}
zhanshen_Wuyi_Pos[5] = {x = 2326 ,y = 7214 ,z = 569}
zhanshen_Wuyi_Pos[6] = {x = 1916 ,y = 3100 ,z = 600}
zhanshen_Wuyi_Pos[7] = {x = 1459 ,y = 3143 ,z = 786}
zhanshen_Wuyi_Pos[8] = {x = 958 ,y = 7181 ,z = 597}
zhanshen_Wuyi_Pos[9] = {x = 1352 ,y = 3100 ,z = 300}
zhanshen_Wuyi_Pos[10] = {x = 1248 ,y = 7157 ,z = 1847}
zhanshen_Wuyi_Pos[11] = {x = 1945 ,y = 3492 ,z = 2562}
zhanshen_Wuyi_Pos[12] = {x = 1686 ,y = 5800 ,z = 1061}
zhanshen_Wuyi_Pos[13] = {x = 2128 ,y = 3000 ,z = 1452}
zhanshen_Wuyi_Pos[14] = {x = 1827 ,y = 5583 ,z = 1878}
zhanshen_Wuyi_Pos[15] = {x = 843 ,y = 5500,z = 439}


zhanshen_PanLong_Pos = {}
zhanshen_PanLong_Pos[1] = {x = 713 ,y = 13600 ,z = 1439}
zhanshen_PanLong_Pos[2] = {x = 1137 ,y = 12526 ,z = 1541}
zhanshen_PanLong_Pos[3] = {x = 1479 ,y = 3693 ,z = 950}
zhanshen_PanLong_Pos[4] = {x = 1318 ,y = 3070 ,z = 571}
zhanshen_PanLong_Pos[5] = {x = 2016 ,y = 3726 ,z = 844}
zhanshen_PanLong_Pos[6] = {x = 1877 ,y = 3456 ,z = 1500}
zhanshen_PanLong_Pos[7] = {x = 1363 ,y = 9779 ,z = 2069}
zhanshen_PanLong_Pos[8] = {x = 1788 ,y = 5760 ,z = 2627}
zhanshen_PanLong_Pos[9] = {x = 1999 ,y = 5760 ,z = 2339}
zhanshen_PanLong_Pos[10] = {x = 2502 ,y = 3490 ,z = 1513}
zhanshen_PanLong_Pos[11] = {x = 2222 ,y = 3456 ,z = 1252}
zhanshen_PanLong_Pos[12] = {x = 847 ,y = 14501 ,z = 1244}
zhanshen_PanLong_Pos[13] = {x = 1554 ,y = 5396 ,z = 1924}
zhanshen_PanLong_Pos[14] = {x = 600 ,y = 14789 ,z = 1335}
zhanshen_PanLong_Pos[15] = {x = 1845 ,y = 5504 ,z = 2122}



zhanshen_TimerMin = 0
zhanshen_WuYi_Num = 0 --武夷击杀怪物数量
zhanshen_PanLong_num = 0 -- 盘龙击杀怪物数量

function zhanshen_OnStart(actID)
	--服务器武夷、盘龙谷全场景通知[屏幕顶部滚动文字提示]：“10个战神化身重现世间，请大家赶快在本地图内寻找，获取宝石合成符等丰厚道具奖励。”
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 800)
	msg.DispatchMapMsgEvent(MsgID, 3017299151, -1)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 800)
	msg.DispatchMapMsgEvent(MsgID, 1146339967, -1)

	--重置武夷和盘龙的战神化身已击杀数量
	map.SetMapScriptData(3017299151,-1,1,7,0)
	map.SetMapScriptData(1146339967,-1,1,1,0)
	--刷出5个善使NPC
	for k=1, 5 do
		map.MapCreateCreature(3017299151, -1, zhanshen_npc[k].TypeID, zhanshen_npc[k].x, zhanshen_npc[k].y, zhanshen_npc[k].z)
	end

	--更新时间重置为0
	zhanshen_TimerMin = 0
	local zhanshen_M06_MonsterPos = deepcopy(zhanshen_Wuyi_Pos)
	local zhanshen_M06_1_MonsterPos = deepcopy(zhanshen_PanLong_Pos)
	local i = 0
	local j = 15
		--在15个坐标中随机刷出10个战神分身怪物，地点不重复，怪物类型可重复

		for i = 1,10 do
			local Index_pos = math.random(1,j)
			local Index_mon = math.random(1,10)
			local Temp1 = table.remove(zhanshen_M06_MonsterPos,Index_pos)
			local Temp2 = table.remove(zhanshen_M06_1_MonsterPos,Index_pos)
			map.MapCreateColCreature(3017299151, -1, zhanshen_monster[Index_mon].TypeID, Temp1.x, Temp1.y, Temp1.z, 1)
			map.MapCreateColCreature(1146339967, -1, zhanshen_monster[Index_mon].TypeID, Temp2.x, Temp2.y, Temp2.z, 1)
			j = j-1
		end

end

function zhanshen_OnTimerMin(actID)

	zhanshen_TimerMin = zhanshen_TimerMin + 1

	local broadmin={}
	broadmin[1] = 9
	broadmin[2] = 19
	broadmin[3] = 29
	broadmin[4] = 39
	broadmin[5] = 49

	local respawnmin={}
	respawnmin[1] = 10
	respawnmin[2] = 20
	respawnmin[3] = 30
	respawnmin[4] = 40
	respawnmin[5] = 50

	for i=1, 5 do
		if zhanshen_TimerMin == broadmin[i] then
			--服务器武夷、盘龙全场景通知[屏幕顶部滚动文字提示]：“服务器将于1分钟后重新刷新10个战神化身，请各位侠士做好准备。”
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 801)
			msg.DispatchMapMsgEvent(MsgID, 3017299151, -1)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 800)
			msg.DispatchMapMsgEvent(MsgID, 1146339967, -1)
		end
	end

	for i=1, 5 do
		if zhanshen_TimerMin == respawnmin[i] then

		--服务器武夷、盘龙谷全场景通知[屏幕顶部滚动文字提示]：“10个战神化身重现世间，请大家赶快在本地图内寻找，获取宝石合成符等丰厚道具奖励。”
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 800)
		msg.DispatchMapMsgEvent(MsgID, 3017299151, -1)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 800)
		msg.DispatchMapMsgEvent(MsgID, 1146339967, -1)

			--重置武夷和盘龙的战神化身已击杀数量
			map.SetMapScriptData(3017299151,-1,1,7,0)
			map.SetMapScriptData(1146339967,-1,1,1,0)

			local zhanshen_M06_MonsterPos = deepcopy(zhanshen_Wuyi_Pos)
			local zhanshen_M06_1_MonsterPos = deepcopy(zhanshen_PanLong_Pos)
			local k = 0
			local j = 15
			--在15个坐标中随机刷出10个战神分身怪物，地点不重复，怪物类型可重复
			for k = 1,10 do
				local Index_pos = math.random(1,j)
				local Index_mon = math.random(1,10)
				local Temp1 = table.remove(zhanshen_M06_MonsterPos,Index_pos)
				local Temp2 = table.remove(zhanshen_M06_1_MonsterPos,Index_pos)
				map.MapCreateColCreature(3017299151, -1, zhanshen_monster[Index_mon].TypeID, Temp1.x, Temp1.y, Temp1.z, 1)
				map.MapCreateColCreature(1146339967, -1, zhanshen_monster[Index_mon].TypeID, Temp2.x, Temp2.y, Temp2.z, 1)
				j = j-1
			end
		end
	end
end

--注册该活动的相关事件
aux.RegisterActEvent(57, 2, "zhanshen_OnStart")
aux.RegisterActEvent(58, 2, "zhanshen_OnStart")
aux.RegisterActEvent(59, 2, "zhanshen_OnStart")
aux.RegisterActEvent(60, 2, "zhanshen_OnStart")

aux.RegisterActEvent(57, 4, "zhanshen_OnTimerMin")
aux.RegisterActEvent(58, 4, "zhanshen_OnTimerMin")
aux.RegisterActEvent(59, 4, "zhanshen_OnTimerMin")
aux.RegisterActEvent(60, 4, "zhanshen_OnTimerMin")


function zhanshen_Dead(MapID, InstanceID, CreatureID, TypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 10,"jinjieshili_A")

	--检测Killer是否有小队
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)

	--如果没有小队
	if 4294967295 == TeamID  then
		--添加战神力量BUFF
		unit.AddBuff(MapID, InstanceID, RoleID, 2023901, RoleID)
	else
		--获得小队玩家ID
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)

		for k in pairs(Role) do
			if 4294967295 ~= Role[k] then
				--添加战神力量BUFF
				unit.AddBuff(MapID, InstanceID, Role[k], 2023901, Role[k])
			end
		end
	end
	if MapID == 3017299151 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 805)--战神重现活动过程中战神化身&lt;p1&gt;被玩家&lt;p2&gt;击杀，请再接再厉。
		msg.AddMsgEvent(MsgID, 5, TypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchMapMsgEvent(MsgID, 3017299151, -1)
	elseif MapID == 1146339967 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 805)--战神重现活动过程中战神化身&lt;p1&gt;被玩家&lt;p2&gt;击杀，请再接再厉。
		msg.AddMsgEvent(MsgID, 5, TypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchMapMsgEvent(MsgID, 1146339967, -1)
	end

	--记录已击杀的战神化身数量
	if MapID == 3017299151 then
		local m06_num = map.GetMapScriptData(MapID,InstanceID,1,7)
		if m06_num == zhanshen_MaxNum - 1 then
			--服务器武夷全场景通知[屏幕顶部滚动文字提示]：“武夷地图内的战神化身已全部被击败！请等待下次刷新”
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 802)
			msg.DispatchMapMsgEvent(MsgID, 3017299151, -1)
		else
			map.SetMapScriptData(MapID,InstanceID,1,7,m06_num + 1)
		end

	elseif MapID == 1146339967 then
		local m06_1_num = map.GetMapScriptData(MapID,InstanceID,1,1)
		if m06_1_num == zhanshen_MaxNum - 1 then
			--服务器盘龙全场景通知[屏幕顶部滚动文字提示]：“盘龙谷地图内的战神化身已全部被击败！请等待下次刷新”
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 803)
			msg.DispatchMapMsgEvent(MsgID, 1146339967, -1)
		else
			map.SetMapScriptData(MapID,InstanceID,1,7,m06_1_num + 1)
		end
	end


	local k = math.random(1,150)

	if k >= 148 then
		local x,y,z = unit.GetPosition(MapID, InstanceID, CreatureID)
		map.MapCreateCreature(MapID, InstanceID, 1533019, x, y, z)
		--战神重现活动过程中九黎珍宝守护兽下凡 位于<p1>的[<p2>,<p3>]
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 804)
		msg.AddMsgEvent(MsgID, 6, MapID)
		msg.AddMsgEvent(MsgID, 9, x)
		msg.AddMsgEvent(MsgID, 9, z)
		msg.DispatchMapMsgEvent(MsgID, 3017299151, -1) --m06

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 804)
		msg.AddMsgEvent(MsgID, 6, MapID)
		msg.AddMsgEvent(MsgID, 9, x)
		msg.AddMsgEvent(MsgID, 9, z)
		msg.DispatchMapMsgEvent(MsgID, 1146339967, -1) --m06_1
	end

end
function jiulizhenbao_Dead(MapID, InstanceID, CreatureID, TypeID, RoleID)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 805)--战神重现活动过程中�&lt;p1&gt;被玩家&lt;p2&gt;击杀，请再接再厉。
		msg.AddMsgEvent(MsgID, 5, TypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchMapMsgEvent(MsgID, 3017299151, -1)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 805)--战神重现活动过程中&lt;p1&gt;被玩家&lt;p2&gt;击杀，请再接再厉。
		msg.AddMsgEvent(MsgID, 5, TypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchMapMsgEvent(MsgID, 1146339967, -1)
end

--注册死亡事件
aux.RegisterCreatureEvent(1533009, 4, "zhanshen_Dead")
aux.RegisterCreatureEvent(1533010, 4, "zhanshen_Dead")
aux.RegisterCreatureEvent(1533011, 4, "zhanshen_Dead")
aux.RegisterCreatureEvent(1533012, 4, "zhanshen_Dead")
aux.RegisterCreatureEvent(1533013, 4, "zhanshen_Dead")
aux.RegisterCreatureEvent(1533014, 4, "zhanshen_Dead")
aux.RegisterCreatureEvent(1533015, 4, "zhanshen_Dead")
aux.RegisterCreatureEvent(1533016, 4, "zhanshen_Dead")
aux.RegisterCreatureEvent(1533017, 4, "zhanshen_Dead")
aux.RegisterCreatureEvent(1533018, 4, "zhanshen_Dead")
aux.RegisterCreatureEvent(1533019, 4, "jiulizhenbao_Dead")

--完成5个兑换任务后20%几率给予 1个菩提叶
function zhanshen_quest_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local r = math.random(1,100)
	if r > 80 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 2612694, 1, -1, 8, 420)
	end

	Increase_shili(MapID, InstanceID, RoleID, 0, 5,"jinjieshili_A")
end

--注册
aux.RegisterQuestEvent(20162, 1, "zhanshen_quest_OnComplete")
aux.RegisterQuestEvent(20163, 1, "zhanshen_quest_OnComplete")
aux.RegisterQuestEvent(20164, 1, "zhanshen_quest_OnComplete")
aux.RegisterQuestEvent(20165, 1, "zhanshen_quest_OnComplete")
aux.RegisterQuestEvent(20166, 1, "zhanshen_quest_OnComplete")


function zhanshen_quest_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	--判断背包空闲空间是否足够
	if(role.GetBagFreeSize(RoleID) < 1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 142)
		msg.DispatchRoleMsgEvent(MsgID)
		return 0
	end

	return 1
end
aux.RegisterQuestEvent(20162, 5, "zhanshen_quest_OnCheckComplete")
aux.RegisterQuestEvent(20163, 5, "zhanshen_quest_OnCheckComplete")
aux.RegisterQuestEvent(20164, 5, "zhanshen_quest_OnCheckComplete")
aux.RegisterQuestEvent(20165, 5, "zhanshen_quest_OnCheckComplete")
aux.RegisterQuestEvent(20166, 5, "zhanshen_quest_OnCheckComplete")



--完成一次菩提叶兑换后，增加次数
function PuTiYe_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local Num = role.GetRoleScriptData(RoleID, 1, RoleDataType["PuTiYe_Num"])
	local CurTime = tonumber(os.date("%j"))

		role.SetRoleScriptData(RoleID, 1, RoleDataType["PuTiYe_Num"], Num + 1)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["PuTiYe_Update"], CurTime)

end

--注册
aux.RegisterQuestEvent(20167, 1, "PuTiYe_OnComplete")


function PuTiYe_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
--检测任务上次更新日期，若与当前日期不一致，则重置总环数及更新日期
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local Num = role.GetRoleScriptData(RoleID, 1, RoleDataType["PuTiYe_Num"])
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["PuTiYe_Update"])
	local CurTime = tonumber(os.date("%j"))
	if CurTime~=UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["PuTiYe_Num"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["PuTiYe_Update"], CurTime)
	end
	--若本日已接取50次，则无法再接取
	if level>= 90 then
		if Num >= 80 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 147)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		else
			return 1
		end
	elseif level>= 80 then
		if Num >= 40 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 147)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		else
			return 1
		end
	elseif level>= 70 then
		if Num >= 20 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 147)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		else
			return 1
		end
	end
end
aux.RegisterQuestEvent(20167, 4, "PuTiYe_OnCheckAccept")
