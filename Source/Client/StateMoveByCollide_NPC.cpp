#include "StdAfx.h"
#include "StateMoveByCollide_NPC.h"
#include "NPC.h"
#include "FSM_NPC.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\action.h"

StateMoveByCollide_NPC::StateMoveByCollide_NPC(void)
{
}

StateMoveByCollide_NPC::~StateMoveByCollide_NPC(void)
{
}

void StateMoveByCollide_NPC::Active( tagNetCmd* pNetCmd,NavMap* pNav,NPC* pRole )
{
	ASSERT(pNetCmd->dwID==pNetCmd->Crc32("NS_SyncWalk"));
	tagNS_SyncWalk* pCmd=(tagNS_SyncWalk*)pNetCmd;

	//--初始化碰撞检测器
	m_collider.Init(pCmd->srcPos,pCmd->dstPos,pRole->GetRoleSize(),pCmd->fXZSpeed,false);
	m_lastCDTime=Kernel::Inst()->GetAccumTime()-pCmd->curTime;

	//--设置朝向
	Vector3 dir=pCmd->dstPos-pCmd->srcPos;
	pRole->SetYaw(CalcYaw(dir));

	//--播放动作
	const TCHAR* szTrackName=CombatSysUtil::Inst()->GetNPCMoveTrackName(pCmd->fXZSpeed);
	pRole->PlayTrack(szTrackName,true);
}

void StateMoveByCollide_NPC::Update( NavMap* pNav,NPC* pRole )
{
	//--碰撞检测
	Vector3 nowPos;
	DWORD dwCarrierObjID;
	float endTime;

	ENavResult result=m_collider.Update(pNav->GetCollider(),m_lastCDTime,Kernel::Inst()->GetAccumTime()-m_lastCDTime,nowPos,endTime,dwCarrierObjID);
	m_lastCDTime=Kernel::Inst()->GetAccumTime();

	pRole->SetPos(nowPos);

	//--
	if(result!=ENR_ToBeContinued)
		m_pFSM->Change2IdleState();
}

void StateMoveByCollide_NPC::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_MoveSpeedChange"))//速度变化消息
	{
		m_pFSM->Change2IdleState();
	}
	else
		m_pFSM->Change2State(pNetCmd);
}
