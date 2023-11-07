#pragma once
#ifndef _RPC_H_
#define _RPC_H_

#include "Common.h"
#include "LocalRPCManager.h"
///////////////////////////////////
//		Function Prototype		///
///////////////////////////////////

NetObject* CreateNetObject(ClassID cid);
NetObject* CreateNetObject(ClassID cid, const RPCNetID &netId);
NetGroup* CreateNetGroup();
NetGroup* CreateNetGroup(const RPCNetID &netId);
void _DeleteNetObject(NetObject* obj);
void DeleteNetChilds(NetGroup* grp);
NetObject* FindNetObject(const RPCNetID &netId);
RPCContext& GetCbContext(RPCContext &context);

#ifdef CLIENT
	template <unsigned int _V_>
	INLINE RPCResult RPCSend(LPCPEER pPeer, Buffer* pBuf)
	{
		return LocalRPCManager::GetInstance().Send<_V_>(pPeer, pBuf);
	}

	template <unsigned int _V_>
	INLINE RPCResult RPCDecode(LPCPEER pPeer, Buffer* pBuf)
	{
		return LocalRPCManager::GetInstance().Decode<_V_>(pPeer, pBuf);
	}

#else
	template <unsigned int _V_>
	INLINE RPCResult RPCSend(LPCPEER pPeer, Buffer* pBuf)
	{
		return LocalRPCManager::GetInstance().Send(pPeer, pBuf);
	}

	template <unsigned int _V_>
	INLINE RPCResult RPCDecode(LPCPEER pPeer, Buffer* pBuf)
	{
		return LocalRPCManager::GetInstance().Decode(pPeer, pBuf);
	}

#endif // CLIENT

INLINE RPCResult RPCSend(LPCPEER pPeer, Buffer* pBuf)
{
	return LocalRPCManager::GetInstance().Send(pPeer, pBuf);
}

INLINE RPCResult RPCDecode(LPCPEER pPeer, Buffer* pBuf)
{
	return LocalRPCManager::GetInstance().Decode(pPeer, pBuf);
}

INLINE RPCResult SendError(LPCPEER pPeer, RPCContext &context, 
						   const UInt32 &errCode, const String &errMsg)
{
	return LocalRPCManager::GetInstance().SendError(pPeer, context, errCode, errMsg);
}

template<typename _TYPE_>
INLINE _TYPE_* CastNetObject(NetObject *obj)
{
	if (obj && obj->IsSubClass(_TYPE_::_GetClassID()))
		return (_TYPE_*) obj;
	else
		return NULL;
}

template<typename _TYPE_>
INLINE _TYPE_* CreateNetObject()
{
	NetObject* obj = CreateNetObject(_TYPE_::_GetClassID());
	TRACE_ENSURE(obj && obj->GetClassID() == _TYPE_::_GetClassID());
	return (_TYPE_*) obj;
}

template<typename _TYPE_>
INLINE _TYPE_* FindNetObject(const RPCNetID &netId)
{
	NetObject* obj = FindNetObject(netId);
	if (obj && obj->IsSubClass(_TYPE_::_GetClassID()))
		return (_TYPE_*) obj;
	else
		return NULL;
}

template<typename _TYPE_>
INLINE void DeleteNetObject(_TYPE_* &obj)
{
	if (obj == NULL) return;
	_DeleteNetObject(obj);
	obj = NULL;
}

template<typename _TYPE_>
INLINE void DeleteNetGroup(_TYPE_* &grp, Boolean bDelChild = BOOLEAN_FALSE)
{
	if (grp == NULL) return;
	if (bDelChild)
		DeleteNetChilds(grp);
	_DeleteNetObject(grp);
	grp = NULL;
}

// bitstream for NetObject
template <>
struct Reuben::Network::Buffer::Serializer<NetObject*>
{
	static VOID Read(CONST Buffer& sin, NetObject*& obj);
	static VOID Write(Buffer& sout, CONST NetObject* obj);
};

// bitstream for NetGroup
template <>
struct Reuben::Network::Buffer::Serializer<NetGroup*>
{
	static VOID Read(CONST Buffer& sin, NetGroup*& grp);
	static VOID Write(Buffer& sout, CONST NetGroup* grp);
};

//-- include
struct AutoData
{
	UInt16		autoCount;			// remained auto turn count
	UInt8		cmd;			// 
	UInt8		sub_type;			// 
	UInt8		additional;			
	UInt8		target;
};

struct RpcCharInfo
{
	SessionID	sid;				// client session ID
	Boolean		bConnected;			// client is connected or not
	RPCNetID	netId;				// character Net ID
	UInt32		char_id;			// character DB id
	UInt8		team;				// character team
	AutoData	autoData;			// remained auto turn count
	UInt8		petID;				// pet ID
	Boolean		bHasParty;			// check party
	String		buffCache;
};

typedef StlVector<RpcCharInfo, ManagedAllocator<RpcCharInfo> > RpcCharVec;

// bitstream for RpcCharInfo
template <>
struct Reuben::Network::Buffer::Serializer<RpcCharInfo>
{
	static VOID Read(CONST Buffer& sin, RpcCharInfo& obj);
	static VOID Write(Buffer& sout, CONST RpcCharInfo obj);
};

#endif // _RPC_H_
