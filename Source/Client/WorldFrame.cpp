#include "stdafx.h"
#include "WorldFrame.h"
#include "MapMgr.h"
#include "RoleMgr.h"
#include "PetManager.h"
#include "LocalPlayer.h"
#include "..\WorldDefine\selectrole.h"
#include "ItemMgr.h"
#include "DragBtn.h"
//#include "ToolTipScript.h"
#include "..\WorldDefine\role_att.h"
#include ".\ChatFrame.h"
#include ".\MilleSoundFrame.h"
#include "PlayerNameTab.h"
#include "RoleFloatTipsFrame.h"
#include "QuestMgr.h"
#include "SkillMgr.h"
#include "ComposeMgr.h"
#include "CursorMgr.h"
#include "MapLogicData.h"
#include "SkillProgressFrame.h"
#include "SceneCenterInfoFrame.h"
#include "RoleFloatTipsFrame.h"
#include "StallSignboardTipsFrame.h"
#include "DakMgr.h"
#include "GroundItemMgr.h"
#include "CurrencyMgr.h"
#include "AudioSys.h"
#include "GameSet.h"
#include "TradeFrame.h"
#include "TeamSys.h"
#include "MallMgr.h"
#include "StallMgr.h"
#include "DigBaoMgr.h"
#include "InstanceMgr.h"
#include "ReputeMgr.h"
#include "HackShield.h"
#include "TreasureMgr.h"
#include "YuanbaoTradeMgr.h"
#include "PetManager.h"
#include "TreasureChestFrame.h"
#include "ServerTime.h"
#include "ActivityMgr.h"
#include "NoticeFrame.h"
#include "OnlineTips.h"
#include "GuildMgr.h"
#include "TargetStateFrame.h"
#include "InsuranceFrame.h"
#include "HelpFrame.h"
#include "VipNetBarMgr.h"
#include "ActivityPvPMgr.h"
#include "BroadCastMgr.h"
#include "ProfessionMgr.h"
#include "MarriageMgr.h"
#include "FishMgr.h"
#include "LoongBenedictionFrame.h"

WorldFrame::WorldFrame():m_Trunk(this)
{
}

WorldFrame::~WorldFrame()
{
	
}

BOOL WorldFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	DWORD dwTime = timeGetTime();
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_camera.SetDefault(20000.0f);
	m_camera.SetFov(55.0f);
	
	//--初始化角色管理器
	RoleMgr::Inst()->Init(dwParam);
	IMSG(_T("TIME1=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	//--初始化场景管理器
	MapMgr::Inst()->Init();
	IMSG(_T("TIME2=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	//--初始化物品管理器
	ItemMgr::Inst()->Init();
	IMSG(_T("TIME3=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	//--初始化氏族珍宝管理
	TreasureMgr::Inst()->Init();
	//--初始化技能管理器
	SkillMgr::Inst()->Init();
	IMSG(_T("TIME4=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	PlayerNameTab::Inst()->Init();
	IMSG(_T("TIME5=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	QuestMgr::Inst()->Init();
	IMSG(_T("TIME6=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	ComposeMgr::Inst()->Init();
	IMSG(_T("TIME7=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	DakMgr::Inst()->Init();
	IMSG(_T("TIME8=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	TeamSys::Inst()->Init();
	IMSG(_T("TIME9=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	//--初始化地面物品管理器
	GroundItemMgr::Inst()->Init();

	IMSG(_T("TIME10=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	//--初始化货币管理器
	CurrencyMgr::Inst()->Init();
	IMSG(_T("TIME11=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	MallMgr::Inst()->Init();

	StallMgr::Inst()->Init();
	IMSG(_T("TIME12=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	InstanceMgr::Inst()->Init();

	IMSG(_T("TIME13=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	
	DigBaoMgr::Inst()->Init();

	ReputeMgr::Inst()->Init();

    YuanbaoTradeMgr::Inst()->Init();

    PetManager::Inst()->Init();

    ProfessionMgr::Inst().Init();

    MarriageMgr::Inst().Init();

	ActivityMgr::Inst()->Init();

	GuildMgr::Inst()->Init();

	VipNetBarMgr::Inst()->Init();

	ActivityPvPMgr::Inst()->Init();

	BroadCastMgr::Inst()->Init();

	//--创建子Frame
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("SysMenu"), _T("SysMenuFrame"), 0);
	IMSG(_T("TIME14=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("ShortcutBar"), _T("ShortcutBarFrame"), 0);
	IMSG(_T("TIME15=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("Chat"), _T("ChatFrame"), 0);	
	IMSG(_T("TIME16=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("MilleSound"), _T("MilleSoundFrame"), 0);	
	IMSG(_T("TIME16=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("MiniMap"), _T("MiniMapFrame"), 0);
	IMSG(_T("TIME17=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();	
	
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("ZoneMap"), _T("ZoneMapFrame"), 0);
	IMSG(_T("TIME17=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();	

	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("PlayerState"), _T("PlayerStateFrame"), 0);
	IMSG(_T("TIME18=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("TargetState"), _T("TargetStateFrame"), 0);
	IMSG(_T("TIME19=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("TargetSpellState"), _T("TargetSpellStateFrame"), 0);
	IMSG(_T("TIME19=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("Item"), _T("ItemFrame"), 0);
	IMSG(_T("TIME20=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("Talent"), _T("TalentFrame"), 0);
	IMSG(_T("TIME21=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("WorldPicker"), _T("WorldPickerFrame"), 0);
	IMSG(_T("TIME22=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("Trade"), _T("TradeFrame"), 0);
	IMSG(_T("TIME23=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("Character"), _T("CharacterFrame"), 0);
	IMSG(_T("TIME24=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

    m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("DispRoleInfo"), _T("DispRoleInfoFrame"), 0);
    IMSG(_T("TIME24=%d\r\n"), timeGetTime()-dwTime);
    dwTime = timeGetTime();

    m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("TargetCharacter"), _T("TargetCharacterFrame"), 0);
    IMSG(_T("TIME24=%d\r\n"), timeGetTime()-dwTime);
    dwTime = timeGetTime();
	
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("RoleFloatTips"), _T("RoleFloatTipsFrame"), 0);
	IMSG(_T("TIME25=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("SkillProgress"), _T("SkillProgressFrame"), 0);
	IMSG(_T("TIME26=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("SceneCenterInfo"), _T("SceneCenterInfoFrame"), 0);
	IMSG(_T("TIME27=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("RoleHeadTips"), _T("RoleHeadTipsFrame"), 0);
	IMSG(_T("TIME28=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("StallSignboard"), _T("StallSignboardTipsFrame"), 0);
	IMSG(_T("TIME28=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("CombatSys"), _T("CombatSysFrame"), 0);
	IMSG(_T("TIME29=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

    // 创建宠物界面
    m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("PetFrame"), _T("PetFrame"), 0);
    IMSG(_T("TIME24=%d\r\n"), timeGetTime()-dwTime);
    dwTime = timeGetTime();

    // 创建宠物技能设置界面
    m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("PetSkillFrame"), _T("PetSkillFrame"), 0);
    IMSG(_T("TIME24=%d\r\n"), timeGetTime()-dwTime);
    dwTime = timeGetTime();

    // 宠物交易
    m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("PetTradeFrame"), _T("PetTradeFrame"), 0);
    IMSG(_T("TIME23=%d\r\n"), timeGetTime()-dwTime);
    dwTime = timeGetTime();

	//--创建任务相关界面
	m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("QuestMain"), _T("QuestMainFrame"), 0 );
	IMSG(_T("TIME30=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	//--好友系统
	m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("SocialMgr"), _T("SocialMgrFrame"), 0 );
	IMSG(_T("TIME31=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	// 队友状态
	m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("TeammatesState"), _T("TeammatesStateFrame"), 0);
	IMSG(_T("TIME32=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	// 宝箱
	m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("TreasureChest"), _T("TreasureChestFrame"), 0);
	IMSG(_T("TIME34=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	// 40级以下玩家登陆提示...
	m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("OnlineTips"), _T("OnlineTipsFrame"), 0);

	m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("Notice"), _T("NoticeFrame"), 0);
	IMSG(_T("TIME35=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("GroundItemTips"), _T("GroundItemTipsFrame"), 0);
	IMSG(_T("TIME36=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("HelpFrame"), _T("HelpFrame"), 0);


	m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("Insurance"), _T("InsuranceFrame"), 0);
	IMSG(_T("TIME37=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("MouseIntoTips"), _T("MouseIntoTipsFrame"), 0);
	IMSG(_T("TIME38=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("EquipRecency"), _T("EquipRecencyFrame"), 0);
	IMSG(_T("TIME39=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("SpecialSkill"), _T("SpecialSkillFrame"), 0);
	IMSG(_T("TIME40=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("StyleAction"), _T("StyleActionFrame"), 0);
	IMSG(_T("TIME41=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

    // 婚姻frame
    m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("MarriageFrame"), _T("MarriageFrame"), 0);
    IMSG(_T("TIME40=%d\r\n"), timeGetTime()-dwTime);
    dwTime = timeGetTime();

    m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("DivorceFrame"), _T("DivorceFrame"), 0);
    IMSG(_T("TIME40=%d\r\n"), timeGetTime()-dwTime);
    dwTime = timeGetTime();

	QuestMgr::Inst()->OpenTrackFrame();

	DragBtn::NewInstance(m_pSys);

	switchToState(EWState_GotoNewMap);

	//--	
	m_pCmdMgr->Register("NS_GotoNewMap",	(NETMSGPROC)m_Trunk.sfp2(&WorldFrame::NetRecvGotoMap),		_T("NS_GotoNewMap"));
	/*m_pCmdMgr->Register("NS_RoleGetName",	(NETMSGPROC)m_Trunk.sfp2(&WorldFrame::NetRecvRoleGetName),	_T("NS_RoleGetName"));*/
	m_pCmdMgr->Register("NS_RoleGetID",		(NETMSGPROC)m_Trunk.sfp2(&WorldFrame::NetRecvRoleGetID),	_T("NS_RoleGetID"));
	m_pCmdMgr->Register("NS_LoongBenedictionInit", (NETMSGPROC)m_Trunk.sfp2(&WorldFrame::NetRecvLoongBeneInit), _T("NS_LoongBenedictionInit") );

	m_pMgr->RegisterEventHandle( _T("DisconnectionEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&WorldFrame::OnDisconnectionEvent));

	//--发送初始化消息
	tagNC_GetRoleInitState m;
	m.eType = ERIT_CompleteQuest;
	if(m_pSession->IsConnect())
		m_pSession->Send(&m);

	IMSG(_T("TIME33=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();


	TObjRef<GameInputMap>()->Register(TObjRef<Util>()->Crc32( _T("EscCancel") ), g_StrTable[_T("HotKey_CloseWnd")],	
		(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&WorldFrame::HandleESC), DIK_ESCAPE, FALSE, FALSE, 0 );

	TObjRef<GameInputMap>()->LoadFromFile();	// 读取热键配置

	return TRUE;
}

BOOL WorldFrame::Destroy()
{
	TObjRef<GameInputMap>()->SaveToFile();	// 保存热键配置

	TObjRef<GameInputMap>()->SetEnable( g_StrTable[_T("HotKey_CloseWnd")], FALSE );

	//删除自定义键位界面
	TObjRef<GameInputMap>()->UnloadGUI();

	//--任务管理器释放数据
	QuestMgr::Inst()->Destroy();

	GameFrame::Destroy();

	//--
	RoleMgr::Inst()->OnCloseMap(MapMgr::Inst()->GetSceneGraph());

	//--场景管理器释放数据
	MapMgr::Inst()->Destroy();

	//--角色管理器释放数据
	RoleMgr::Inst()->Destroy();

	//--物品管理器释放数据
	ItemMgr::Inst()->Destroy();

	//--氏族珍宝
	TreasureMgr::Inst()->Destroy();
	
	//--技能管理器释放数据
	SkillMgr::Inst()->Destroy();

	//--强化合成释放数据
	ComposeMgr::Inst()->Destroy();

	//--驿站释放数据
	DakMgr::Inst()->Destroy();

	//--地面物品管理器释放数据
	GroundItemMgr::Inst()->Destroy();
	
	StallMgr::Inst()->Destroy();
	//--货币管理器释放数据
	CurrencyMgr::Inst()->Destroy();

	TeamSys::Inst()->Destroy();

	InstanceMgr::Inst()->Destroy();
	//--
	PlayerNameTab::Inst()->Destroy();

	MallMgr::Inst()->Destroy();

	DigBaoMgr::Inst()->Destroy();

	ReputeMgr::Inst()->Destroy();

    YuanbaoTradeMgr::Inst()->Destroy();

    PetManager::Inst()->Destroy();

    ProfessionMgr::Inst().Destroy();

    MarriageMgr::Inst().Destroy();

	ActivityMgr::Inst()->Destroy();
	
	GuildMgr::Inst()->Destroy();

	VipNetBarMgr::Inst()->Destroy();

	ActivityPvPMgr::Inst()->Destroy();

	BroadCastMgr::Inst()->Destroy();

	DragBtn::DestroyDragBtn();

	m_pCmdMgr->UnRegister("NS_GotoNewMap",	(NETMSGPROC)m_Trunk.sfp2(&WorldFrame::NetRecvGotoMap));
	/*m_pCmdMgr->UnRegister("NS_RoleGetName",	(NETMSGPROC)m_Trunk.sfp2(&WorldFrame::NetRecvRoleGetName));*/
	m_pCmdMgr->UnRegister("NS_RoleGetID",	(NETMSGPROC)m_Trunk.sfp2(&WorldFrame::NetRecvRoleGetID));
	m_pCmdMgr->UnRegister("NS_LoongBenedictionInit", (NETMSGPROC)m_Trunk.sfp2(&WorldFrame::NetRecvLoongBeneInit) );

	m_pMgr->UnRegisterEventHandler( _T("DisconnectionEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&WorldFrame::OnDisconnectionEvent));
	if( P_VALID(GetObj("DisconnectionMsgBox")) )
		KillObj("DisconnectionMsgBox");
	if( P_VALID(GetObj("SetRebornMsgBox")) )
		KillObj("SetRebornMsgBox");

	return TRUE;
}

VOID WorldFrame::Render3D()
{
	//--
	float deltaTime=Kernel::Inst()->GetDeltaTime();
	SceneGraph* pSG=MapMgr::Inst()->GetSceneGraph();
	if(pSG)
		pSG->Render(deltaTime);

	//--让子GameFrame渲染3D场景
	GameFrame::Render3D();
}

VOID WorldFrame::Update()
{
	BeginSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe"));

	switch(m_curState)
	{
	case EWState_GotoNewMap:
		break;
	case EWState_Playing:
		{
			BeginSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.nametab"));
			PlayerNameTab::Inst()->Update();
			EndSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.nametab"));

			//--mapmgr
			BeginSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.mapmgr"));
			MapMgr::Inst()->Update();
			EndSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.mapmgr"));

			//--rolemgr
			BeginSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.rolemgr"));
			RoleMgr::Inst()->Update(MapMgr::Inst()->GetSceneGraph());
			EndSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.rolemgr"));

            //--petmgr
			BeginSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.petmgr"));
            PetManager::Inst()->Update();
			EndSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.petmgr"));

            //--ProfessionMgr
            BeginSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.ProfessionMgr"));
            ProfessionMgr::Inst().Update();
            EndSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.ProfessionMgr"));

            //--MarriageMgr
            BeginSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.MarriageMgr"));
            MarriageMgr::Inst().Update();
            EndSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.MarriageMgr"));

			//--TeamSys
			BeginSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.teamsys"));
			TeamSys::Inst()->Updata();
			EndSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.teamsys"));

			//--QuestMgr
			BeginSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.questmgr"));
			QuestMgr::Inst()->Update();
			EndSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.questmgr"));

			//--DibBaoMgr
			BeginSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.dibaomgr"));
			DigBaoMgr::Inst()->Update();
			EndSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.dibaomgr"));

			GuildMgr::Inst()->Update();

			BroadCastMgr::Inst()->Update();

			//--camera
			BeginSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.camera"));
			m_camera.Update(RoleMgr::Inst()->GetLocalPlayer(),MapMgr::Inst()->GetNavMap());
			EndSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe.camera"));

			VipNetBarMgr::Inst()->Update();

			//主要是为了节日活动上线提示
			ActivityMgr::Inst()->Update();	

		}
		break;
	}

	//--AudioSys
	if( GameSet::Inst()->Get( EGS_Music ) || GameSet::Inst()->Get( EGS_Sound ) )
	{
		m_pAudioSys->SetCamera( m_camera );
	}

	if(!m_pSession->IsConnect())
	{
		if( P_VALID(GetObj("DisconnectionMsgBox")) )
			return;
		CreateObj("DisconnectionMsgBox", "MsgBoxEx");

		TObjRef<MsgBoxEx>("DisconnectionMsgBox")->Init(_T(""), g_StrTable[_T("DisconnectServer")], 
			_T("DisconnectionEvent"), (MsgBoxFlag)(MBF_OK), TRUE, TRUE);
	}

	EndSpeedTest(_T("Client::Mainloop.update.gameframe.worldframe"));


	//--children
#if 1
	GameFrame::Update();
#else
	GameFrame* pFrame = NULL;
	m_listChild.ResetIterator();
	TCHAR szTile[200];
	while(m_listChild.PeekNext(pFrame))
	{
		_stprintf(szTile,_T("Client::Mainloop.update.gameframe.%s"),pFrame->GetName().c_str());
		BeginSpeedTest(szTile);
		pFrame->Update();
		EndSpeedTest(szTile);
	}
#endif
}

void WorldFrame::switchToState(EWorldState state)
{	
	m_curState = state;
	switch(m_curState)
	{	
	case EWState_GotoNewMap:
		break;
	case EWState_Playing:
		//LoadRes(_T("333"), Vector3(0,0,0), Vector3(0,0,0));
		//RoleMgr::Inst()->GetLocalPlayer()->SetPos(Vector3(0, 0, 0));
		break;
	}
}

BOOL WorldFrame::LoadRes(const TCHAR* szMapName, DWORD dwMapID, const Vector3& localPlayerPos, const Vector3& localPlayerFaceto)
{
	GameFrame* pFrame = m_pFrameMgr->CreateFrame(_T("Root"), _T("Loading"), _T("LoadingFrame"), 0);

	//--关闭当前地图
	if(MapMgr::Inst()->GetSceneGraph()!=NULL)
	{
		RoleMgr::Inst()->OnCloseMap(MapMgr::Inst()->GetSceneGraph());
		MapMgr::Inst()->CloseMap();
	}

	//--加载新的地图
	ResMgr::Inst()->ZeroLoadCount();
	MapMgr::Inst()->OpenMap(szMapName, dwMapID, &m_camera);

	RoleMgr::Inst()->OnOpenMap(MapMgr::Inst()->GetSceneGraph(), localPlayerPos, localPlayerFaceto);


	//--发送初始化任务脚本消息
	m_pFrameMgr->SendEvent( &tagGameEvent(_T("Quest_LoadNewMap"), this) );
	return TRUE;
}

DWORD WorldFrame::NetRecvGotoMap(tagNS_GotoNewMap* pMsg, DWORD pPrama)
{
	// 如果目前地图就仅移动人物和镜头并等待读图
	if( pMsg->dwMapID == MapMgr::Inst()->GetCurMapID() )
	{
		// 设置角色位置、朝向和状态
		RoleMgr::Inst()->GetLocalPlayer()->OnGotoMap(pMsg->pos,pMsg->faceTo);

		// 设置镜头位置等
		m_camera.Update(RoleMgr::Inst()->GetLocalPlayer(),MapMgr::Inst()->GetNavMap());

		// 更新场景视野区域
		MapMgr::Inst()->SyncInvalidViewZone();
	}
	else
	{
		const tagMapLogicData* pMapLogicData=MapLogicData::Inst()->FindMapLogicData(pMsg->dwMapID);
		if(P_VALID(pMapLogicData))
		{
			LoadRes(pMapLogicData->szName.c_str(), pMsg->dwMapID, pMsg->pos, pMsg->faceTo);
			switchToState(EWState_Playing);
		}
	}

	return 0;
}

//DWORD WorldFrame::NetRecvRoleGetName( tagNS_RoleGetName* pNetCmd, DWORD dwParam )
//{
//	PlayerNameTab::Inst()->OnNetRoleGetName(pNetCmd, dwParam);
//	if( pNetCmd->bResult )
//	{
//		const map<DWORD,Role*>& mapRole = RoleMgr::Inst()->GetRoleMap();
//		for( map<DWORD,Role*>::const_iterator itr = mapRole.begin(); itr != mapRole.end(); itr++ )
//		{
//			if( itr->first == pNetCmd->dwRoleID && P_VALID( itr->second ) )
//				itr->second->SetRoleName( pNetCmd->szRoleName );
//		}
//	}
//}

DWORD WorldFrame::NetRecvRoleGetID( tagNS_RoleGetID* pNetCmd, DWORD dwParam )
{
	PlayerNameTab::Inst()->OnNetRoleGetID(pNetCmd, dwParam);

	return 0;
}

DWORD WorldFrame::NetRecvLoongBeneInit( tagNS_LoongBenedictionInit* pNetCmd, DWORD dwParam )
{
	m_pFrameMgr->CreateFrame( _T("World"), _T("LoongBenediction"), _T("LoongBenedictionFrame"), 0 );
	return 0;
}

DWORD WorldFrame::OnDisconnectionEvent( tagMsgBoxEvent* pEvent )
{
	if(pEvent->eResult == MBF_OK)
	{
		if( !m_pMgr->CreateFrame(_T("Root"), _T("Login"), _T("LoginFrame"), 0) )
			return 0;

		m_pMgr->AddToDestroyList( GetWorldFrm() );
		m_pSession->Disconnect();
		HackShield::Inst()->StopApexClient();
	}

	return 0;
}

// 处理ESC键
DWORD WorldFrame::HandleESC( DWORD dwID, BOOL bDown )
{
	GameFrame* pFrame = NULL;

	if( bDown )
	{
		GameFrame* pSysOptMenuFrame = m_pMgr->GetFrame( _T("SysOptionMenu") );
		if( P_VALID( pSysOptMenuFrame ) )
		{
			pSysOptMenuFrame->EscCancel();
			return 0;
		}

		// Esc优先级
		// 1、战斗过程中停止普通攻击或起手（正在施放的技能在完成后不会接续普通攻击）
		GameFrame* pCombatFrame = m_pMgr->GetFrame( _T("CombatSys") );
		if( P_VALID(pCombatFrame) && pCombatFrame->EscCancel() )
			return 0;

		//	2、取消目标
		TargetStateFrame* pTargetFrame = (TargetStateFrame*)m_pMgr->GetFrame(_T("TargetState"));
		if( P_VALID(pTargetFrame) && pTargetFrame->EscCancel() )
			return 0;

		//	3、关闭除主界面外的界面
		TMap<DWORD, GameFrame*>& map = m_pMgr->GetMap();
		pFrame = NULL;
		map.ResetIterator();

		INT nCounter = 0;
		while( map.PeekNext(pFrame) )
		{
			if( pFrame == pCombatFrame )
				continue;

			if( pFrame == pSysOptMenuFrame )
				continue;

			if( pFrame == pTargetFrame )
				continue;

			 if( pFrame->EscCancel() )
				 ++nCounter;
		}

		if( nCounter > 0 )
			return 0;


		//	4、弹出系统选单
		pFrame = m_pMgr->GetFrame( _T("SysOptionMenu") );
		if( !P_VALID( pFrame ) )
		{
			m_pMgr->CreateFrame( m_strName.c_str(), _T("SysOptionMenu"), _T("SysOptionMenuFrame"), 0);
		}

	}

	return 0;
}