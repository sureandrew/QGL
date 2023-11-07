//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/ChannelType.h"
#include "MessageService/Configure.h"
//-- Self
#include "Stub/Logic/Channel.h"
//-- Library
#include "Stub/Service/MessageService_stub.h"
#include "Stub/Logic/MessageCharacter.h"
#include "MessageService/MessageApplication.h"
#include "MessageService/Global.h"
#include "Proxy/Service/MapService_proxy.h"
//-- Resource
#include "Resource/GameStringTable.h"

REGISTER_CLASSTYPE(CChannel, Channel_CLASSID);

typedef StlMap<LPCPEER, SessionIDVector, std::less<LPCPEER>, 
		ManagedAllocator< std::pair<LPCPEER, SessionIDVector> > > ProxyListenersMap;

CChannel::CChannel()
{
	SetInfo(CHANNELTYPE_MAP);
}

CChannel::~CChannel()
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("channel %d, context size %d"), GetChannelType(),
		m_MapCharsContextByID.size());
	m_MapCharsContextByID.clear();
	m_channelName.clear();

}

VOID CChannel::InsertCharContext(RPCContext context, CMessageCharacter * msgChar)
{
	ChannelCharData TempData;
	TempData.SID = context.sessionId;
	TempData.pMsgChar = msgChar;

	if (msgChar->GetNickName().empty())
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Character with no name insert to channel CharID: %d"), msgChar->GetChar_id());
	ChannelCharMap::iterator ItrID = m_MapCharsContextByID.find(msgChar->GetChar_id());
	if (ItrID != m_MapCharsContextByID.end())
		m_MapCharsContextByID.erase(ItrID);
	
	m_MapCharsContextByID.insert(std::make_pair(msgChar->GetChar_id(), TempData));
	SetOwnObject(context.sessionId, this);
}

RPCResult CChannel::SendGrpMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId, const Boolean &guildSysMsg)
{
	if (Msg.IsEmpty())
		return RPC_RESULT_OK;
	

	UInt sizeofMsg= NoCodeStringSize(String(Msg));
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendGrpMessage: Msg address 0x%08x Msg: %s"), Msg.c_str(), Msg.c_str());

	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendGrpMessage: SourceId %d Msg: %s"), SourceId, Msg.c_str());
	//don't allow to send message in official channels
	UInt32 CurrTime = time(NULL);
	String Name;

	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendGrpMessage: Name address 0x%08x Msg: %s"), Name.c_str(), Name.c_str());
	ChannelCharMap::iterator itSrc = m_MapCharsContextByID.find(SourceId);
	if (itSrc != m_MapCharsContextByID.end())
	{
		CMessageCharacter * pMsgChar = itSrc->second.pMsgChar;

		Name = pMsgChar->GetNickName();
		LPCPEER pPeer = GetProxy(itSrc->second.SID.serviceId);
		if (pPeer) 
		{
			//check block channel
			UInt32 now = (UInt32) time(NULL);
			if (pMsgChar->GetSpeakBlock() > now)
			{
				CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), MSG_RETURN_FAIL_NO_RIGHT_BLOCK, 0); 
				return RPC_RESULT_OK;
			}
			//check speak count
			if (GetChannelType() == CHANNELTYPE_GLOBAL)
			{
				if (pMsgChar->GetMoney() < gConfigure.moneyChannelGlobal)
				{
					CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), MSG_RETURN_FAIL_NO_RIGHT_MONEY, 0); 
					return RPC_RESULT_OK;
				}
				if (pMsgChar->GetLevel() < 10)
				{
					CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), MSG_RETURN_FAIL_NO_RIGHT_LV, 0); 
					return RPC_RESULT_OK;
				}
			}
			if (GetChannelType() == CHANNELTYPE_GANG)
			{
				if (pMsgChar->GetMoney() < gConfigure.moneyChannelGang)
				{
					CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), MSG_RETURN_FAIL_NO_RIGHT_MONEY_GANG, 0); 
					return RPC_RESULT_OK;
				}
				
			}
			//check speak count
			if (GetChannelType() == CHANNELTYPE_SPEAKER)
			{
				if (pMsgChar->GetGlobalSpeakCount() == 0)
				{
					CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), MSG_RETURN_FAIL_NO_RIGHT_NO_COUNT, 0); 
					return RPC_RESULT_OK;
				}
				else if (pMsgChar->GetLevel() < 10)
				{
					CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), MSG_RETURN_FAIL_NO_RIGHT_LV, 0); 
					return RPC_RESULT_OK;
				}
			}
			//No Speak Right
			else if (!pMsgChar->m_speakRights[GetChannelType()])
			{
				CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), MSG_RETURN_FAIL_NO_RIGHT_BLOCK, 0); 
				return RPC_RESULT_OK;
			}
			//speak too frequently
			else if (GetChannelType() < MAX_CHANNELTYPE && 
				CurrTime < SpeakRestTime[GetChannelType()] + itSrc->second.PreSpeakTime && !guildSysMsg)
			{
				CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), MSG_RETURN_FAIL_TOO_FREQUENT, SpeakRestTime[GetChannelType()] + itSrc->second.PreSpeakTime - CurrTime); 
				return RPC_RESULT_OK;
			}
			else if (sizeofMsg > MaxMsgLongLen)
			{
					CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), MSG_RETURN_FAIL_TOO_LONG, 0); 
					return RPC_RESULT_OK;
			}	
			else
			{
				itSrc->second.PreSpeakTime = CurrTime;
				//BYTE Result = MSG_RETURN_SUCCESS;
				//CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), Result); 
			}

		}
		else
			return RPC_RESULT_OK;

		if (Name.empty())
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Character with no name send grp message CharID: %d"), pMsgChar->GetChar_id());

		String blockedMsg = Msg;
		//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendGrpMessage: blockedMsg address 0x%08x Msg: %s"), blockedMsg.c_str(), blockedMsg.c_str());
		gGlobal.BlockWord(blockedMsg);
		//gGlobal.BlockColorWord(blockedMsg);

		if (GetChannelType() == CHANNELTYPE_SPEAKER)
		{
			pMsgChar->SetSpeakCount(pMsgChar->GetGlobalSpeakCount() - 1);
			pMsgChar->UpdateObject();
		}

		if (GetChannelType() == CHANNELTYPE_GLOBAL)
		{
			for (PeerSet::iterator ItrMap = gGlobal.m_mapPeers.begin(); 
				ItrMap != gGlobal.m_mapPeers.end(); ++ItrMap)
			{
				MapService_Proxy::ChangeMoney(*ItrMap, RPCContext(), pMsgChar->GetChar_id(), -gConfigure.moneyChannelGlobal);
			}
		}

		if (GetChannelType() == CHANNELTYPE_GANG)
		{
			for (PeerSet::iterator ItrMap = gGlobal.m_mapPeers.begin(); 
				ItrMap != gGlobal.m_mapPeers.end(); ++ItrMap)
			{
				MapService_Proxy::ChangeMoney(*ItrMap, RPCContext(), pMsgChar->GetChar_id(), -gConfigure.moneyChannelGang);
			}
		}

		ProxyListenersMap proxys;
		UInt32Vector listenerID;
		for (ChannelCharMap::iterator it = m_MapCharsContextByID.begin(); it != m_MapCharsContextByID.end(); ++it)
		{
			if (!it->second.Listen)
				continue;

			LPCPEER pPeer = GetProxy(it->second.SID.serviceId);
			if (pPeer)
				proxys[pPeer].push_back(it->second.SID);
			listenerID.push_back(it->first);
		}

		if (GetChannelType() == CHANNELTYPE_GUILD && guildSysMsg)
		{
			Name = String(); // do not display the name if it is guild broadcast
		}

		for (ProxyListenersMap::iterator it = proxys.begin(); it != proxys.end(); ++it)
			CB_ReceiveMessage(it->first, RPCContext(), blockedMsg, Name, it->second);


		if (GetChannelType() == CHANNELTYPE_GLOBAL)
		{
			CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), MSG_RETURN_SUCCESS_COST_MONEY, 0); 
		}
		else if (GetChannelType() == CHANNELTYPE_GANG)
		{
			CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), MSG_RETURN_SUCCESS_COST_MONEY_GANG, 0); 
		}
				
		switch( GetChannelType())
		{
		case CHANNELTYPE_PARTY:			
		case CHANNELTYPE_GANG:		
		case CHANNELTYPE_GLOBAL:		
		case CHANNELTYPE_OFFICIAL:
		case CHANNELTYPE_CHATROOM:
		case CHANNELTYPE_NOTICE:
		case CHANNELTYPE_SPEAKER:
		case CHANNELTYPE_GUILD:
		case CHANNELTYPE_FRIEND:
		case CHANNELTYPE_INSTANCE:
			WriteMsgLog(context, GetChannelType(), pMsgChar->GetChar_id(), Name, listenerID, m_channelName, blockedMsg);
			break;		
		}
	}
	
	return RPC_RESULT_OK;
}

RPCResult CChannel::SendEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const UInt32 &SourceId, const UInt32 &TargetID, const bool &isPet)
{
	//don't allow to send message in official channels
	time_t now;
	time(&now);
	UInt32 CurrTime = (UInt32)now;
	String Name;
	String TargetName;
	String Msg;

	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendEmotion: Name address 0x%08x Msg: %s"), Name.c_str(), Name.c_str());
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendEmotion: TargetName address 0x%08x Msg: %s"), TargetName.c_str(), TargetName.c_str());
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendEmotion: Msg address 0x%08x Msg: %s"), Msg.c_str(), Msg.c_str());
	ChannelCharMap::iterator itSrc = m_MapCharsContextByID.find(SourceId);
	ChannelCharMap::iterator itTarget = m_MapCharsContextByID.find(TargetID);
	if (itSrc != m_MapCharsContextByID.end() &&
		itTarget != m_MapCharsContextByID.end())
	{
		Name = itSrc->second.pMsgChar->GetNickName();
		TargetName = itTarget->second.pMsgChar->GetNickName();
		LPCPEER pPeer = GetProxy(itSrc->second.SID.serviceId);

		for (ChannelCharMap::iterator it = m_MapCharsContextByID.begin(); it != m_MapCharsContextByID.end(); it++)
		{
			LPCPEER pPeer = GetProxy(it->second.SID.serviceId);
			if (pPeer) 
			{	
				CB_ReceiveEmotion(pPeer, RPCContext(it->second.SID), emo_id, Name, TargetName, isPet); 
			}
		}
	}
	
	return RPC_RESULT_OK;
}
RPCResult CChannel::SendGrpFlushMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId)
{
	if (Msg.IsEmpty())
		return RPC_RESULT_OK;

	
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendGrpFlushMessage: Msg address %d Msg: %s"), Msg.c_str(), Msg.c_str());
	time_t now;
	time(&now);
	UInt32 CurrTime = now;
	String Name;

	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendGrpFlushMessage: Name address 0x%08x Msg: %s"), Name.c_str(), Name.c_str());
	ChannelCharMap::iterator itSrc = m_MapCharsContextByID.find(SourceId);
	if (itSrc != m_MapCharsContextByID.end())
	{
		Name = itSrc->second.pMsgChar->GetNickName();
		LPCPEER pPeer = GetProxy(itSrc->second.SID.serviceId);
		if (pPeer) 
		{
			CMessageCharacter* pMsgChar = gGlobal.GetMsgCharRef(SourceId);
			if (pMsgChar == NULL)
				return RPC_RESULT_OK;
			//check block channel
			UInt32 now = (UInt32) time(NULL);
			if (pMsgChar->GetSpeakBlock() > now)
			{
				BYTE Result = MSG_RETURN_FAIL_NO_RIGHT_BLOCK;
				CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), Result, 0); 
				return RPC_RESULT_OK;
			}

			itSrc->second.PreSpeakTime = CurrTime;
			BYTE Result = MSG_RETURN_SUCCESS;
			UInt sizeofMsg= NoCodeStringSize(String(Msg));
			if (sizeofMsg > gConfigure.maxMsgLen)
			{
				BYTE Result = MSG_RETURN_FAIL_TOO_LONG;
				CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), Result, 0); 
				return RPC_RESULT_OK;
			}
			CB_SendGrpMessage(pPeer, RPCContext(itSrc->second.SID), Result, 0); 
		}
	}

	ProxyListenersMap proxys;
	for (ChannelCharMap::iterator it = m_MapCharsContextByID.begin(); it != m_MapCharsContextByID.end(); it++)
	{
		LPCPEER pPeer = GetProxy(it->second.SID.serviceId);
		if (pPeer)
			proxys[pPeer].push_back(it->second.SID);
	}

	for (ProxyListenersMap::iterator it = proxys.begin(); it != proxys.end(); ++it)
		CB_ReceiveMessage(it->first, RPCContext(), Msg, Name, it->second);
	
	return RPC_RESULT_OK;
}

RPCResult CChannel::LeaveChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &SourceId)
{
	ChannelCharMap::iterator itID = m_MapCharsContextByID.find(SourceId);
	if (itID != m_MapCharsContextByID.end())
	{
		LPCPEER pPeer = GetProxy(itID->second.SID.serviceId);
		if (pPeer) 
		{	
			CB_LeaveChannel(pPeer, RPCContext(itID->second.SID));
		}

		UnsetOwnObject(itID->second.SID, this);
		m_MapCharsContextByID.erase(itID);

	}
	return RPC_RESULT_OK;
}

RPCResult CChannel::SetListen(LPCPEER pPeer, RPCContext &context, const BOOL &IsListen, const UInt32 &char_Id)
{
	ChannelCharMap::iterator itID = m_MapCharsContextByID.find(char_Id);
	if (itID != m_MapCharsContextByID.end())
	{
		itID->second.Listen = IsListen;
	}
	CB_SetListen(pPeer, context); 
	return RPC_RESULT_OK;
}

UInt CChannel::NumberOfPlayer()
{
	return (UInt)m_MapCharsContextByID.size();
}

VOID CChannel::ClearSession(SessionID &sid)
{
	for (ChannelCharMap::iterator it = m_MapCharsContextByID.begin(); it != m_MapCharsContextByID.end(); it++)
	{
		if (it->second.SID == sid)
		{
			m_MapCharsContextByID.erase(it);
			UnsetOwnObject(sid, this);
			break;
		}
	}
}

VOID CChannel::RemoveCharFromChannel(UInt32 char_id)
{
	ChannelCharMap::iterator it = m_MapCharsContextByID.find(char_id);
	if (it != m_MapCharsContextByID.end())
	{
		m_MapCharsContextByID.erase(it);
	}
}

void CChannel::SendShutdownMsg(UInt32 downTime)
{
	for (ChannelCharMap::iterator it = m_MapCharsContextByID.begin(); 
		it != m_MapCharsContextByID.end(); it++)
	{
		LPCPEER pPeer = GetProxy(it->second.SID.serviceId);
		if (pPeer) 
			MessageService_Stub::CB_SendShutdownMessage(pPeer, 
				RPCContext(it->second.SID), downTime); 
	}
}