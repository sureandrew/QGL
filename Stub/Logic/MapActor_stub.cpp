//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/MapActor_stub.h"

void MapActor_Stub::SetInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	if (m.sex != sex) { m.sex = sex; modified = true; }
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	if (m.sexClass != sexClass) { m.sexClass = sexClass; modified = true; }
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MapActor_Stub::SetNickName(const String &nickName)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MapActor_Stub::SetSex(const UInt8 &sex)
{
	bool modified = false;
	if (m.sex != sex) { m.sex = sex; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MapActor_Stub::SetCclass(const UInt8 &cclass)
{
	bool modified = false;
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MapActor_Stub::SetSexClass(const UInt8 &sexClass)
{
	bool modified = false;
	if (m.sexClass != sexClass) { m.sexClass = sexClass; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MapActor_Stub::SetFaction(const UInt8 &faction)
{
	bool modified = false;
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MapActor_Stub::SetState(const UInt8 &state)
{
	bool modified = false;
	if (m.state != state) { m.state = state; modified = true; }
	MarkUpdateAll(stateMask, modified);
}

void MapActor_Stub::SetMap(const UInt16 &map_id, const UInt16 &map_x, const UInt16 &map_y, const UInt8 &direction, const UInt32 &map_ownerId)
{
	bool modified = false;
	if (m.map_id != map_id) { m.map_id = map_id; modified = true; }
	if (m.map_x != map_x) { m.map_x = map_x; modified = true; }
	if (m.map_y != map_y) { m.map_y = map_y; modified = true; }
	if (m.direction != direction) { m.direction = direction; modified = true; }
	if (m.map_ownerId != map_ownerId) { m.map_ownerId = map_ownerId; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void MapActor_Stub::SetMap_id(const UInt16 &map_id)
{
	bool modified = false;
	if (m.map_id != map_id) { m.map_id = map_id; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void MapActor_Stub::SetMap_x(const UInt16 &map_x)
{
	bool modified = false;
	if (m.map_x != map_x) { m.map_x = map_x; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void MapActor_Stub::SetMap_y(const UInt16 &map_y)
{
	bool modified = false;
	if (m.map_y != map_y) { m.map_y = map_y; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void MapActor_Stub::SetDirection(const UInt8 &direction)
{
	bool modified = false;
	if (m.direction != direction) { m.direction = direction; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void MapActor_Stub::SetMap_ownerId(const UInt32 &map_ownerId)
{
	bool modified = false;
	if (m.map_ownerId != map_ownerId) { m.map_ownerId = map_ownerId; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void MapActor_Stub::SetReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y)
{
	bool modified = false;
	if (m.reborn_map != reborn_map) { m.reborn_map = reborn_map; modified = true; }
	if (m.reborn_x != reborn_x) { m.reborn_x = reborn_x; modified = true; }
	if (m.reborn_y != reborn_y) { m.reborn_y = reborn_y; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void MapActor_Stub::SetReborn_map(const UInt16 &reborn_map)
{
	bool modified = false;
	if (m.reborn_map != reborn_map) { m.reborn_map = reborn_map; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void MapActor_Stub::SetReborn_x(const UInt16 &reborn_x)
{
	bool modified = false;
	if (m.reborn_x != reborn_x) { m.reborn_x = reborn_x; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void MapActor_Stub::SetReborn_y(const UInt16 &reborn_y)
{
	bool modified = false;
	if (m.reborn_y != reborn_y) { m.reborn_y = reborn_y; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void MapActor_Stub::SetCurPos(const UInt16 &curPosX, const UInt16 &curPosY)
{
	bool modified = false;
	if (m.curPosX != curPosX) { m.curPosX = curPosX; modified = true; }
	if (m.curPosY != curPosY) { m.curPosY = curPosY; modified = true; }
	MarkUpdateAll(curPosMask, modified);
}

void MapActor_Stub::SetCurPosX(const UInt16 &curPosX)
{
	bool modified = false;
	if (m.curPosX != curPosX) { m.curPosX = curPosX; modified = true; }
	MarkUpdateAll(curPosMask, modified);
}

void MapActor_Stub::SetCurPosY(const UInt16 &curPosY)
{
	bool modified = false;
	if (m.curPosY != curPosY) { m.curPosY = curPosY; modified = true; }
	MarkUpdateAll(curPosMask, modified);
}

void MapActor_Stub::SetDstPos(const UInt16 &dstPosX, const UInt16 &dstPosY)
{
	bool modified = false;
	if (m.dstPosX != dstPosX) { m.dstPosX = dstPosX; modified = true; }
	if (m.dstPosY != dstPosY) { m.dstPosY = dstPosY; modified = true; }
	MarkUpdateAll(dstPosMask, modified);
}

void MapActor_Stub::SetDstPosX(const UInt16 &dstPosX)
{
	bool modified = false;
	if (m.dstPosX != dstPosX) { m.dstPosX = dstPosX; modified = true; }
	MarkUpdateAll(dstPosMask, modified);
}

void MapActor_Stub::SetDstPosY(const UInt16 &dstPosY)
{
	bool modified = false;
	if (m.dstPosY != dstPosY) { m.dstPosY = dstPosY; modified = true; }
	MarkUpdateAll(dstPosMask, modified);
}

void MapActor_Stub::SetAction(const UInt16 &action, const UInt32 &actionTime)
{
	bool modified = false;
	if (m.action != action) { m.action = action; modified = true; }
	if (m.actionTime != actionTime) { m.actionTime = actionTime; modified = true; }
	MarkUpdateAll(actionMask, modified);
}

void MapActor_Stub::SetAction(const UInt16 &action)
{
	bool modified = false;
	if (m.action != action) { m.action = action; modified = true; }
	MarkUpdateAll(actionMask, modified);
}

void MapActor_Stub::SetActionTime(const UInt32 &actionTime)
{
	bool modified = false;
	if (m.actionTime != actionTime) { m.actionTime = actionTime; modified = true; }
	MarkUpdateAll(actionMask, modified);
}

void MapActor_Stub::SetSpeed(const UInt32 &speed)
{
	bool modified = false;
	if (m.speed != speed) { m.speed = speed; modified = true; }
	MarkUpdateAll(speedMask, modified);
}

void MapActor_Stub::SetFollowId(const UInt32 &followId, const UInt8 &followOrder)
{
	bool modified = false;
	if (m.followId != followId) { m.followId = followId; modified = true; }
	if (m.followOrder != followOrder) { m.followOrder = followOrder; modified = true; }
	MarkUpdateAll(followIdMask, modified);
}

void MapActor_Stub::SetFollowId(const UInt32 &followId)
{
	bool modified = false;
	if (m.followId != followId) { m.followId = followId; modified = true; }
	MarkUpdateAll(followIdMask, modified);
}

void MapActor_Stub::SetFollowOrder(const UInt8 &followOrder)
{
	bool modified = false;
	if (m.followOrder != followOrder) { m.followOrder = followOrder; modified = true; }
	MarkUpdateAll(followIdMask, modified);
}

void MapActor_Stub::SetAppeared(const Boolean &appeared)
{
	bool modified = false;
	if (m.appeared != appeared) { m.appeared = appeared; modified = true; }
	MarkUpdateAll(appearedMask, modified);
}



void MapActor_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void MapActor_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(infoMask))
	{
		buf.Write<String>(m.nickName);
		buf.Write<UInt8>(m.sex);
		buf.Write<UInt8>(m.cclass);
		buf.Write<UInt8>(m.sexClass);
		buf.Write<UInt8>(m.faction);
	}
	if (mask.IsBit(stateMask))
	{
		buf.Write<UInt8>(m.state);
	}
	if (mask.IsBit(mapMask))
	{
		buf.Write<UInt16>(m.map_id);
		buf.Write<UInt16>(m.map_x);
		buf.Write<UInt16>(m.map_y);
		buf.Write<UInt8>(m.direction);
		buf.Write<UInt32>(m.map_ownerId);
	}
	if (mask.IsBit(rebornMask))
	{
		buf.Write<UInt16>(m.reborn_map);
		buf.Write<UInt16>(m.reborn_x);
		buf.Write<UInt16>(m.reborn_y);
	}
	if (mask.IsBit(curPosMask))
	{
		buf.Write<UInt16>(m.curPosX);
		buf.Write<UInt16>(m.curPosY);
	}
	if (mask.IsBit(dstPosMask))
	{
		buf.Write<UInt16>(m.dstPosX);
		buf.Write<UInt16>(m.dstPosY);
	}
	if (mask.IsBit(actionMask))
	{
		buf.Write<UInt16>(m.action);
		buf.Write<UInt32>(m.actionTime);
	}
	if (mask.IsBit(speedMask))
	{
		buf.Write<UInt32>(m.speed);
	}
	if (mask.IsBit(followIdMask))
	{
		buf.Write<UInt32>(m.followId);
		buf.Write<UInt8>(m.followOrder);
	}
	if (mask.IsBit(appearedMask))
	{
		buf.Write<Boolean>(m.appeared);
	}
}

RPCResult MapActor_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapActor_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult MapActor_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_MapActor_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

