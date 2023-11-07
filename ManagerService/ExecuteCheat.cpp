//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "ExecuteCheat.h"
//-- Others
#include "Configure.h"
#include "Global.h"
#include <Reuben/Network/NetGroup.h>
#include "Proxy/Service/WorldService_Proxy.h"
#include "Proxy/Service/ManagerService_Proxy.h"
#include "Stub/Data/ExecuteCheatData.h"

#include "Reuben/Simulation/ObjectManager.h"
#include "Reuben/Simulation/ScheduleManager.h"
#include "Reuben/Simulation/Event.h"

#define ExecuteCheat_CLASSID 54000

REGISTER_CLASSTYPE(CExecuteCheat, ExecuteCheat_CLASSID);

CExecuteCheat::CExecuteCheat()
{
}

CExecuteCheat::~CExecuteCheat()
{
}

void CExecuteCheat::Start()
{
	gGlobal.CheatCommandDBFail();
	RaiseUniqEventID(EVT_UPDATE_GETNEWCHEAT_TIMEOUT, (UInt32)gConfigure.cheatRetrievalTime);
}

void CExecuteCheat::PrepareCheat(NetGroup* grp)
{
	if (grp == NULL)
		return;

	CExecuteCheatData* pExeCheat;

	for (CheatCommandMap::iterator itr = gGlobal.m_cheatCommandMap.begin(); 
		itr != gGlobal.m_cheatCommandMap.end(); ++itr )
	{
		CheatCommandData& cheat = itr->second;
		if( !cheat.requestSent )
		{
			cheat.requestSent = TRUE;

			pExeCheat = (CExecuteCheatData*) CreateNetObject(ExecuteCheatData_CLASSID);
			pExeCheat->SetExe_id(cheat.exeId);
			pExeCheat->SetAcct_id((UInt32)cheat.acctId);
			pExeCheat->SetTarget_acct_id((UInt32)cheat.targetAcctId);
			pExeCheat->SetTarget_char_id((UInt32)cheat.targetCharId);
			pExeCheat->SetCommand(cheat.command);
			pExeCheat->SetSendMask(CExecuteCheatData::AllMask);
			grp->Add(pExeCheat);
		}
	}
}

Boolean CExecuteCheat::SendCheat(NetGroup* grp)
{
	Boolean success = BOOLEAN_FALSE;

	if (grp == NULL || grp->GetCount() == 0)
	{
		DeleteNetGroup(grp, BOOLEAN_TRUE);
		return BOOLEAN_TRUE;
	}

	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		CExecuteCheatData* pCheat = grp->GetChild<CExecuteCheatData>(i);
		if (pCheat)
		{
			TRACE_INFODTL_5(GLOBAL_LOGGER, 
				_F("exe_id %d, src_acct_id %d, tgt_acct_id %d, tgt_char_id %d, command %s"),
				pCheat->GetExe_id(), pCheat->GetAcct_id(), pCheat->GetTarget_acct_id(), 
				pCheat->GetTarget_char_id(), pCheat->GetCommand().c_str());
			pCheat->SetSendMask(CExecuteCheatData::AllMask);
		}
	}

	for(ServiceExMap::iterator itr = gGlobal.m_srvExMap.begin(); 
		itr != gGlobal.m_srvExMap.end(); ++itr )
	{
		ServiceEx &service = itr->second;
		if (service.type == SERVICE_WORLD)
		{
			if (service.bLocal)
			{
				// if world service in local, send directly
				WorldService_Proxy::SendCheatCommand(service.pPeer, gGlobal.m_context, grp);
				success = BOOLEAN_TRUE;
			}
			else
			{
				// otherwise, forward to other managers service to handle
				ManagerMap::iterator it = gGlobal.m_managerMap.find(service.address.GetIP());
				if (it != gGlobal.m_managerMap.end())
				{
					Service &mgrsrv = it->second;
					if (mgrsrv.bConnected)
					{
						ManagerService_Proxy::ForwardCheats(mgrsrv.pPeer, gGlobal.m_context, grp);
						success = BOOLEAN_TRUE;
					}
				}
			}
			break;
		}
	}

	DeleteNetGroup(grp, BOOLEAN_TRUE);

	return success;
}

void CExecuteCheat::GetCheatCommand()
{
	gGlobal.GetDBCheatCommand();
}

void CExecuteCheat::PostCheatCommandSent()
{
	gGlobal.MarkDBExecuteCheatRead();
}

void CExecuteCheat::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_UPDATE_GETNEWCHEAT_TIMEOUT:				
		{
			NetGroup* grp = CreateNetGroup();
			GetCheatCommand();
			PrepareCheat(grp);
			if (SendCheat(grp))
				PostCheatCommandSent();
			RaiseUniqEventID(EVT_UPDATE_GETNEWCHEAT_TIMEOUT, (UInt32)gConfigure.cheatRetrievalTime);
		}
		break;
	}
}
