//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/MapNPC.h"
//-- Library
#include "QGL/MapNPCControl.h"

REGISTER_CLASSTYPE(CMapNPC, MapNPC_CLASSID);

CMapNPC::CMapNPC()
{
	m_pMapNPCCtrl = NULL;
}

CMapNPC::~CMapNPC()
{
	if (m_pMapNPCCtrl)
		m_pMapNPCCtrl->SetMapActorObject(NULL);
}

VOID CMapNPC::SetMapNPCControl(CMapNPCControl * pMapNPC)
{
	m_pMapNPCCtrl = pMapNPC;
}

VOID CMapNPC::ResponseExecuteAll(String filename)
{
	if ( m_pMapNPCCtrl )
		m_pMapNPCCtrl->ResponseExecuteAllCtrl(filename);
}

VOID CMapNPC::CancelTalkingTarget()
{
	if ( m_pMapNPCCtrl )
		m_pMapNPCCtrl->CancelTalkingTargetHandle();
}

VOID CMapNPC::OnUpdateVisible(const bool &visible)
{
	if (m_pMapNPCCtrl)
	{
		m_pMapNPCCtrl->SetEnable(visible);
	}
}