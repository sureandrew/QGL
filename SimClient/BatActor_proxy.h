///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_BATACTOR_PROXY_H_
#define _RPC_BATACTOR_PROXY_H_

#include "LocalRPCManager.h"
#include "Reuben/Network/NetObject.h"

#include "RPCEnum/BatActor_enum.h"

class BatActor_Proxy : public NetObject
{
	typedef NetObject Parent;

private:
	//index
	UInt8 m_index;
	//level
	UInt16 m_level;
	//attrib
	UInt16 m_ATK;
	UInt16 m_DEF;
	UInt16 m_SPD;
	UInt16 m_POW;
	UInt16 m_HIT;
	UInt16 m_EVA;
	//HP
	UInt16 m_HP;
	//mHP
	UInt16 m_mHP;
	//SP
	UInt16 m_SP;
	//mSP
	UInt16 m_mSP;
	//state
	UInt8 m_state;
	//buff
	String m_buff;
	//position
	UInt16 m_posX;
	UInt16 m_posY;
	//command
	UInt8 m_command;
	//target
	UInt8 m_target;
	//subType
	UInt32 m_subType;
	//linkNext
	Boolean m_linkNext;

/*** virtual method for fast copy and paste in real class header ***
public:
	virtual void OnUpdateIndex(const UInt8 &index);
	virtual void OnUpdateLevel(const UInt16 &level);
	virtual void OnUpdateAttrib(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &SPD, const UInt16 &POW, const UInt16 &HIT, const UInt16 &EVA);
	virtual void OnUpdateHP(const UInt16 &HP);
	virtual void OnUpdateMHP(const UInt16 &mHP);
	virtual void OnUpdateSP(const UInt16 &SP);
	virtual void OnUpdateMSP(const UInt16 &mSP);
	virtual void OnUpdateState(const UInt8 &state);
	virtual void OnUpdateBuff(const String &buff);
	virtual void OnUpdatePosition(const UInt16 &posX, const UInt16 &posY);
	virtual void OnUpdateCommand(const UInt8 &command);
	virtual void OnUpdateTarget(const UInt8 &target);
	virtual void OnUpdateSubType(const UInt32 &subType);
	virtual void OnUpdateLinkNext(const Boolean &linkNext);
***/

public:
	enum UpdateMask {
		indexMask	= Parent::NextFreeMask << 0,
		levelMask	= Parent::NextFreeMask << 1,
		attribMask	= Parent::NextFreeMask << 2,
		HPMask	= Parent::NextFreeMask << 3,
		mHPMask	= Parent::NextFreeMask << 4,
		SPMask	= Parent::NextFreeMask << 5,
		mSPMask	= Parent::NextFreeMask << 6,
		stateMask	= Parent::NextFreeMask << 7,
		buffMask	= Parent::NextFreeMask << 8,
		positionMask	= Parent::NextFreeMask << 9,
		commandMask	= Parent::NextFreeMask << 10,
		targetMask	= Parent::NextFreeMask << 11,
		subTypeMask	= Parent::NextFreeMask << 12,
		linkNextMask	= Parent::NextFreeMask << 13,
		NextFreeMask	= Parent::NextFreeMask << 14
	};
	UInt8 GetIndex() { return m_index; }
	UInt16 GetLevel() { return m_level; }
	UInt16 GetATK() { return m_ATK; }
	UInt16 GetDEF() { return m_DEF; }
	UInt16 GetSPD() { return m_SPD; }
	UInt16 GetPOW() { return m_POW; }
	UInt16 GetHIT() { return m_HIT; }
	UInt16 GetEVA() { return m_EVA; }
	UInt16 GetHP() { return m_HP; }
	UInt16 GetMHP() { return m_mHP; }
	UInt16 GetSP() { return m_SP; }
	UInt16 GetMSP() { return m_mSP; }
	UInt8 GetState() { return m_state; }
	String GetBuff() { return m_buff; }
	UInt16 GetPosX() { return m_posX; }
	UInt16 GetPosY() { return m_posY; }
	UInt8 GetCommand() { return m_command; }
	UInt8 GetTarget() { return m_target; }
	UInt32 GetSubType() { return m_subType; }
	Boolean GetLinkNext() { return m_linkNext; }
	void RequestIndex(RPCMask &mask) { mask |= indexMask; }
	void RequestLevel(RPCMask &mask) { mask |= levelMask; }
	void RequestAttrib(RPCMask &mask) { mask |= attribMask; }
	void RequestHP(RPCMask &mask) { mask |= HPMask; }
	void RequestMHP(RPCMask &mask) { mask |= mHPMask; }
	void RequestSP(RPCMask &mask) { mask |= SPMask; }
	void RequestMSP(RPCMask &mask) { mask |= mSPMask; }
	void RequestState(RPCMask &mask) { mask |= stateMask; }
	void RequestBuff(RPCMask &mask) { mask |= buffMask; }
	void RequestPosition(RPCMask &mask) { mask |= positionMask; }
	void RequestCommand(RPCMask &mask) { mask |= commandMask; }
	void RequestTarget(RPCMask &mask) { mask |= targetMask; }
	void RequestSubType(RPCMask &mask) { mask |= subTypeMask; }
	void RequestLinkNext(RPCMask &mask) { mask |= linkNextMask; }
	virtual void OnUpdateIndex(const UInt8 &index) = 0;
	virtual void OnUpdateLevel(const UInt16 &level) = 0;
	virtual void OnUpdateAttrib(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &SPD, const UInt16 &POW, const UInt16 &HIT, const UInt16 &EVA) = 0;
	virtual void OnUpdateHP(const UInt16 &HP) = 0;
	virtual void OnUpdateMHP(const UInt16 &mHP) = 0;
	virtual void OnUpdateSP(const UInt16 &SP) = 0;
	virtual void OnUpdateMSP(const UInt16 &mSP) = 0;
	virtual void OnUpdateState(const UInt8 &state) = 0;
	virtual void OnUpdateBuff(const String &buff) = 0;
	virtual void OnUpdatePosition(const UInt16 &posX, const UInt16 &posY) = 0;
	virtual void OnUpdateCommand(const UInt8 &command) = 0;
	virtual void OnUpdateTarget(const UInt8 &target) = 0;
	virtual void OnUpdateSubType(const UInt32 &subType) = 0;
	virtual void OnUpdateLinkNext(const Boolean &linkNext) = 0;
	virtual void ReadAttr(Buffer &msg);
	virtual RPCResult SendRequest(const Peer &peer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, const Peer &peer, RPCContext &context, RPCMessage &msg);
};

inline void BatActor_Proxy::ReadAttr(Buffer &msg)
{
	Parent::ReadAttr(msg);
	if (msg.ReadBit())		// index
	{
		MarkUpdate(indexMask);
		msg.Read<UInt8>(m_index);
		OnUpdateIndex(m_index);
	}
	if (msg.ReadBit())		// level
	{
		MarkUpdate(levelMask);
		msg.Read<UInt16>(m_level);
		OnUpdateLevel(m_level);
	}
	if (msg.ReadBit())		// attrib
	{
		MarkUpdate(attribMask);
		msg.Read<UInt16>(m_ATK);
		msg.Read<UInt16>(m_DEF);
		msg.Read<UInt16>(m_SPD);
		msg.Read<UInt16>(m_POW);
		msg.Read<UInt16>(m_HIT);
		msg.Read<UInt16>(m_EVA);
		OnUpdateAttrib(m_ATK, m_DEF, m_SPD, m_POW, m_HIT, m_EVA);
	}
	if (msg.ReadBit())		// HP
	{
		MarkUpdate(HPMask);
		msg.Read<UInt16>(m_HP);
		OnUpdateHP(m_HP);
	}
	if (msg.ReadBit())		// mHP
	{
		MarkUpdate(mHPMask);
		msg.Read<UInt16>(m_mHP);
		OnUpdateMHP(m_mHP);
	}
	if (msg.ReadBit())		// SP
	{
		MarkUpdate(SPMask);
		msg.Read<UInt16>(m_SP);
		OnUpdateSP(m_SP);
	}
	if (msg.ReadBit())		// mSP
	{
		MarkUpdate(mSPMask);
		msg.Read<UInt16>(m_mSP);
		OnUpdateMSP(m_mSP);
	}
	if (msg.ReadBit())		// state
	{
		MarkUpdate(stateMask);
		msg.Read<UInt8>(m_state);
		OnUpdateState(m_state);
	}
	if (msg.ReadBit())		// buff
	{
		MarkUpdate(buffMask);
		msg.Read<String>(m_buff);
		OnUpdateBuff(m_buff);
	}
	if (msg.ReadBit())		// position
	{
		MarkUpdate(positionMask);
		msg.Read<UInt16>(m_posX);
		msg.Read<UInt16>(m_posY);
		OnUpdatePosition(m_posX, m_posY);
	}
	if (msg.ReadBit())		// command
	{
		MarkUpdate(commandMask);
		msg.Read<UInt8>(m_command);
		OnUpdateCommand(m_command);
	}
	if (msg.ReadBit())		// target
	{
		MarkUpdate(targetMask);
		msg.Read<UInt8>(m_target);
		OnUpdateTarget(m_target);
	}
	if (msg.ReadBit())		// subType
	{
		MarkUpdate(subTypeMask);
		msg.Read<UInt32>(m_subType);
		OnUpdateSubType(m_subType);
	}
	if (msg.ReadBit())		// linkNext
	{
		MarkUpdate(linkNextMask);
		msg.Read<Boolean>(m_linkNext);
		OnUpdateLinkNext(m_linkNext);
	}
}

inline RPCResult BatActor_Proxy::SendRequest(const Peer &peer, RPCContext &context, const RPCMask &mask)
{
	RPCMessage msg;
	msg.Write<RPCID>(RPC_BatActor_REQ_STATE);
	msg.Write<RPCContext>(context);
	msg.Write<RPCNetID>(m_netId);
	msg.Write<RPCMask>(mask);

	return RPCSend(peer,context,msg);
}

inline RPCResult BatActor_Proxy::Process(const RPCID rpcId, const Peer &peer, RPCContext &context, RPCMessage &msg)
{
	switch(rpcId)
	{
	case RPC_BatActor_UP_STATE:
		{
			ReadAttr(msg);
		}
		break;
	default:
		return RPC_RESULT_FAIL;
	}
	return RPC_RESULT_OK;
}


#endif //_RPC_BATACTOR_PROXY_H_
