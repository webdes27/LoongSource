//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_quest.h
// author: 
// actor:
// data: 2008-09-11
// last:
// brief: 技能相关
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push)
#pragma pack(1)
//-----------------------------------------------------------------------------


#include "role_data_define.h"

//-----------------------------------------------------------------------------
// 增加技能
//-----------------------------------------------------------------------------
CMD_START(NDBC_AddSkill)						// 增加技能
	DWORD			dwRoleID;
	tagSkillSave	Skill;	
CMD_END

//-----------------------------------------------------------------------------
// 保存技能
//-----------------------------------------------------------------------------
CMD_START(NDBC_UpdateSkill)						// 保存技能
	DWORD			dwRoleID;
	tagSkillSave	Skill;	
CMD_END

//-----------------------------------------------------------------------------
// 删除技能
//-----------------------------------------------------------------------------
CMD_START(NDBC_RemoveSkill)
	DWORD			dwRoleID;
	DWORD			dwSkillID;
CMD_END

#pragma pack(pop)
