//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/QuestLog_proxy.h"


void QuestLog_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.char_id);
		buf.Read<UInt16>(m.quest_id);
	}
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<UInt16>(m.Type_id);
		buf.Read<String>(m.name);
		buf.Read<String>(m.descript);
	}
	if (m_updateMask.IsBit(beginNpcMask))
	{
		buf.Read<UInt32>(m.beginNpc);
		buf.Read<UInt16>(m.beginNpc_Map);
		buf.Read<UInt16>(m.beginNpc_x);
		buf.Read<UInt16>(m.beginNpc_y);
	}
	if (m_updateMask.IsBit(finishNpcMask))
	{
		buf.Read<UInt32>(m.finishNpc);
		buf.Read<UInt16>(m.finishNpc_Map);
		buf.Read<UInt16>(m.finishNpc_x);
		buf.Read<UInt16>(m.finishNpc_y);
	}
	if (m_updateMask.IsBit(target1Mask))
	{
		buf.Read<UInt32>(m.target1_id);
		buf.Read<UInt16>(m.target1_v1);
		buf.Read<UInt16>(m.target1_v2);
		buf.Read<UInt16>(m.target1_v3);
		buf.Read<UInt8>(m.target1_vtype);
		buf.Read<UInt16>(m.target1_num);
		buf.Read<UInt16>(m.target1_max);
	}
	if (m_updateMask.IsBit(target2Mask))
	{
		buf.Read<UInt32>(m.target2_id);
		buf.Read<UInt16>(m.target2_v1);
		buf.Read<UInt16>(m.target2_v2);
		buf.Read<UInt16>(m.target2_v3);
		buf.Read<UInt8>(m.target2_vtype);
		buf.Read<UInt16>(m.target2_num);
		buf.Read<UInt16>(m.target2_max);
	}
	if (m_updateMask.IsBit(target3Mask))
	{
		buf.Read<UInt32>(m.target3_id);
		buf.Read<UInt16>(m.target3_v1);
		buf.Read<UInt16>(m.target3_v2);
		buf.Read<UInt16>(m.target3_v3);
		buf.Read<UInt8>(m.target3_vtype);
		buf.Read<UInt16>(m.target3_num);
		buf.Read<UInt16>(m.target3_max);
	}
	if (m_updateMask.IsBit(target4Mask))
	{
		buf.Read<UInt32>(m.target4_id);
		buf.Read<UInt16>(m.target4_v1);
		buf.Read<UInt16>(m.target4_v2);
		buf.Read<UInt16>(m.target4_v3);
		buf.Read<UInt8>(m.target4_vtype);
		buf.Read<UInt16>(m.target4_num);
		buf.Read<UInt16>(m.target4_max);
	}
	if (m_updateMask.IsBit(target5Mask))
	{
		buf.Read<UInt32>(m.target5_id);
		buf.Read<UInt16>(m.target5_v1);
		buf.Read<UInt16>(m.target5_v2);
		buf.Read<UInt16>(m.target5_v3);
		buf.Read<UInt8>(m.target5_vtype);
		buf.Read<UInt16>(m.target5_num);
		buf.Read<UInt16>(m.target5_max);
	}
	if (m_updateMask.IsBit(timerMask))
	{
		buf.Read<UInt32>(m.timeLimit);
		buf.Read<UInt16>(m.interval);
	}
	if (m_updateMask.IsBit(buffMask))
	{
		buf.Read<UInt16>(m.buff);
	}
	if (m_updateMask.IsBit(stateMask))
	{
		buf.Read<UInt8>(m.state);
	}
	if (m_updateMask.IsBit(featureMask))
	{
		buf.Read<UInt16>(m.feature);
	}
	if (m_updateMask.IsBit(statusMask))
	{
		buf.Read<UInt8>(m.status);
	}
	if (m_updateMask.IsBit(target1_nameMask))
	{
		buf.Read<String>(m.target1_name);
	}
	if (m_updateMask.IsBit(target2_nameMask))
	{
		buf.Read<String>(m.target2_name);
	}
	if (m_updateMask.IsBit(target3_nameMask))
	{
		buf.Read<String>(m.target3_name);
	}
	if (m_updateMask.IsBit(target4_nameMask))
	{
		buf.Read<String>(m.target4_name);
	}
	if (m_updateMask.IsBit(target5_nameMask))
	{
		buf.Read<String>(m.target5_name);
	}
	if (m_updateMask.IsBit(updateIconMask))
	{
		buf.Read<Boolean>(m.updateIcon);
	}
}

void QuestLog_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void QuestLog_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.char_id, m.quest_id);
	if (IsUpdated(infoMask)) OnUpdateInfo(m.Type_id, m.name, m.descript);
	if (IsUpdated(beginNpcMask)) OnUpdateBeginNpc(m.beginNpc, m.beginNpc_Map, m.beginNpc_x, m.beginNpc_y);
	if (IsUpdated(finishNpcMask)) OnUpdateFinishNpc(m.finishNpc, m.finishNpc_Map, m.finishNpc_x, m.finishNpc_y);
	if (IsUpdated(target1Mask)) OnUpdateTarget1(m.target1_id, m.target1_v1, m.target1_v2, m.target1_v3, m.target1_vtype, m.target1_num, m.target1_max);
	if (IsUpdated(target2Mask)) OnUpdateTarget2(m.target2_id, m.target2_v1, m.target2_v2, m.target2_v3, m.target2_vtype, m.target2_num, m.target2_max);
	if (IsUpdated(target3Mask)) OnUpdateTarget3(m.target3_id, m.target3_v1, m.target3_v2, m.target3_v3, m.target3_vtype, m.target3_num, m.target3_max);
	if (IsUpdated(target4Mask)) OnUpdateTarget4(m.target4_id, m.target4_v1, m.target4_v2, m.target4_v3, m.target4_vtype, m.target4_num, m.target4_max);
	if (IsUpdated(target5Mask)) OnUpdateTarget5(m.target5_id, m.target5_v1, m.target5_v2, m.target5_v3, m.target5_vtype, m.target5_num, m.target5_max);
	if (IsUpdated(timerMask)) OnUpdateTimer(m.timeLimit, m.interval);
	if (IsUpdated(buffMask)) OnUpdateBuff(m.buff);
	if (IsUpdated(stateMask)) OnUpdateState(m.state);
	if (IsUpdated(featureMask)) OnUpdateFeature(m.feature);
	if (IsUpdated(statusMask)) OnUpdateStatus(m.status);
	if (IsUpdated(target1_nameMask)) OnUpdateTarget1_name(m.target1_name);
	if (IsUpdated(target2_nameMask)) OnUpdateTarget2_name(m.target2_name);
	if (IsUpdated(target3_nameMask)) OnUpdateTarget3_name(m.target3_name);
	if (IsUpdated(target4_nameMask)) OnUpdateTarget4_name(m.target4_name);
	if (IsUpdated(target5_nameMask)) OnUpdateTarget5_name(m.target5_name);
	if (IsUpdated(updateIconMask)) OnUpdateUpdateIcon(m.updateIcon);
}

#pragma optimize("", off)
RPCResult QuestLog_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_QuestLog_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult QuestLog_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_QuestLog_UP_STATE:
			_result = RPCDecode<786441412>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
