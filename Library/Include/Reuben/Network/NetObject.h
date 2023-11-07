#pragma once
#ifndef _REUBEN_NETWORK_NETOBJECT_H_
#define _REUBEN_NETWORK_NETOBJECT_H_

#include "../Simulation/Object.h"
#include "Peer.h"
#include "RPC.h"

#define NetObject_CLASSID	3

#define ERR_INVALID_RPCID		0x0001
#define ERR_INVALID_CLASS		0x0002
#define ERR_INVALID_OBJECT		0x0003
#define ERR_INVALID_PROCEDURE	0x0004
#define ERR_INVALID_PCDTYPE		0x0005

namespace Reuben { namespace Network {

	//-- RPCNetID

	struct RPCNetID			// unique ID of net object
	{
		ServiceID srvId;
		Reuben::Simulation::ObjectID objId;

		static CONST Size SIZE = sizeof(ServiceID) + sizeof(Reuben::Simulation::ObjectID);

		RPCNetID() : srvId(0), objId(0) {}
		RPCNetID(ServiceID sid, Reuben::Simulation::ObjectID oid)
			: srvId(sid), objId(oid) {}

		Boolean IsValid() CONST
		{
			return (srvId != 0 && objId != 0);
		}

		VOID Clear(VOID)
		{
			srvId = 0;
			objId = 0;
		}

		bool operator < (CONST RPCNetID& b) CONST
		{
			return (srvId < b.srvId || (srvId == b.srvId && objId < b.objId));
		}

		bool operator > (CONST RPCNetID& b) CONST
		{
			return (srvId > b.srvId || (srvId == b.srvId && objId > b.objId));
		}

		bool operator != (CONST RPCNetID & b) CONST
		{
			return (srvId != b.srvId || objId != b.objId);
		}

		bool operator == (CONST RPCNetID & b) CONST
		{
			return (srvId == b.srvId && objId == b.objId);
		}
	};

	//-- RPCNetID IO

	template <>
	struct Buffer::Serializer<RPCNetID>
	{
		static INLINE VOID Read(CONST Buffer& sin, RPCNetID& netId)
		{
			sin.Read<ServiceID>(netId.srvId);
			sin.Read<Reuben::Simulation::ObjectID>(netId.objId);
		}

		static INLINE VOID Write(Buffer& sout, CONST RPCNetID& netId)
		{
			sout.Write<ServiceID>(netId.srvId);
			sout.Write<Reuben::Simulation::ObjectID>(netId.objId);
		}
	};

	//-- NetObject

	class NetObject : public Reuben::Simulation::Object
	{
	public:

		enum UpdateMasks
		{
			NoneMask	= 0,
			AllMask		= ALLMASK_BIT,
			NextFreeMask = 1,
		};

		enum ObjectStatus
		{
			NETOBJ_INVALID		= 0x10,
			NETOBJ_CREATED		= 0x11,
			NETOBJ_ALIVE		= 0x12,
			NETOBJ_DEAD			= 0x14,
		};

		enum EventEnum
		{
			EVT_NETOBJ_CREATED		= 0x11,
			EVT_NETOBJ_ALIVE		= 0x12,
			EVT_NETOBJ_DEAD			= 0x13,
		};

	protected:

		typedef Reuben::Simulation::Object Parent;
		RPCNetID	m_netId;		// network object id
		RPCMask		m_updateMask;	// network update mask
		RPCMask		m_sendMask;		// network send mask

	public:
		VOID SetNetId(CONST RPCNetID &netId);		// should only use by LocalRPCManager::Register method
		INLINE virtual VOID ClearAllMask() { m_updateMask.Clear(); m_sendMask.Clear(); }
		INLINE virtual VOID MarkUpdateAll(UInt bit, bool modified) { m_updateMask.MaskBit(bit); }

		INLINE Boolean IsUpdated() { return (!m_updateMask.IsZero()); }
		INLINE Boolean IsUpdated(UInt bit) { return (m_updateMask.IsBit(bit)); }
		INLINE Boolean IsUpdated(CONST RPCMask& mask) { return (m_updateMask.IsMask(mask)); }
		INLINE VOID SetUpdateMask(CONST RPCMask& mask) { m_updateMask = mask; }
		INLINE CONST RPCMask& GetUpdateMask() CONST { return m_updateMask; }
		INLINE VOID ClearUpdateMask() { m_updateMask.Clear(); }
		INLINE VOID MarkUpdate(UInt bit) { m_updateMask.MaskBit(bit); }
		INLINE VOID MarkUpdate(CONST RPCMask& mask) { m_updateMask.Mask(mask); }
		INLINE VOID UnmarkUpdate(UInt bit) { m_updateMask.UnmaskBit(bit); }
		INLINE VOID UnmarkUpdate(CONST RPCMask& mask) { m_updateMask.Unmask(mask); }

		INLINE VOID SetSendMask(CONST RPCMask& mask) { m_sendMask = mask; }
		INLINE CONST RPCMask& GetSendMask() CONST { return m_sendMask; }
		INLINE VOID ClearSendMask() { m_sendMask.Clear(); }
		INLINE VOID MarkSend(UInt bit) { m_sendMask.MaskBit(bit); }
		INLINE VOID MarkSend(CONST RPCMask& mask) { m_sendMask.Mask(mask); }
		INLINE VOID UnmarkSend(UInt bit) { m_sendMask.UnmaskBit(bit); }
		INLINE VOID UnmarkSend(CONST RPCMask& mask) { m_sendMask.Unmask(mask); }

		INLINE RPCNetID GetNetId() CONST { return m_netId; }
		RPCResult SendAttr(CONST Peer* pPeer, RPCContext & context);
		virtual RPCResult RequestAttr(CONST Peer* pPeer, RPCContext & context);
		virtual RPCResult SendAttr(CONST Peer* pPeer, RPCContext & context, CONST RPCMask& mask);
		virtual RPCResult CB_Error(CONST Peer* pPeer, RPCContext &context, CONST UInt32 &errCode, CONST String &errMsg);
		virtual VOID ReadAttr(CONST Buffer &buffer);
		virtual VOID WriteAttr(Buffer &buffer, CONST RPCMask& mask) CONST;
		virtual VOID OnUpdate() {}
		virtual VOID ClearAttr() {}
		INLINE VOID Clone(NetObject* obj, CONST RPCMask &mask) {}
		virtual RPCResult Process(CONST RPCID rpcId, CONST Peer* pPeer, 
								RPCContext & context, Buffer* pBuf);
		static RPCResult Decode(CONST RPCID rpcId, CONST Peer* pPeer, Buffer* pBuf) { return RPC_RESULT_OK; }
		virtual String DebugInfo();

		DEFINE_CLASS(NetObject_CLASSID);
	};

	INLINE VOID NetObject::SetNetId(CONST RPCNetID &netId)
	{
		m_netId = netId;
		// shouldn't override constructor to set mask
		//m_updateMask.Clear();
		//m_sendMask.Clear();
	}

	INLINE VOID NetObject::ReadAttr(CONST Buffer &buffer)
	{
		buffer.Read<RPCMask>(m_updateMask);
	}

	INLINE VOID NetObject::WriteAttr(Buffer &buffer, CONST RPCMask& mask) CONST
	{
		buffer.Write<RPCMask>(mask);
	}

	INLINE RPCResult NetObject::SendAttr(CONST Peer* pPeer, RPCContext & context, CONST RPCMask& mask)
	{
		return (RPC_RESULT_OK);
	}

	INLINE RPCResult NetObject::SendAttr(CONST Peer* pPeer, RPCContext & context)
	{
		return (SendAttr(pPeer, context, m_sendMask));
	}

	INLINE RPCResult NetObject::RequestAttr(CONST Peer* pPeer, RPCContext & context)
	{
		return (RPC_RESULT_OK);
	}

	INLINE RPCResult NetObject::CB_Error(CONST Peer* pPeer, RPCContext &context, CONST UInt32 &errCode, CONST String &errMsg)
	{ 
		_tprintf(_T("NetObject CB_Error: errCode: %d, %s\n"), errCode, errMsg.c_str());
		return (RPC_RESULT_OK);
	}

	INLINE RPCResult NetObject::Process(CONST RPCID rpcId, CONST Peer* pPeer, RPCContext & context, Buffer* pBuf)
	{
		return (RPC_NOT_FOUND);
	}

	INLINE String NetObject::DebugInfo()
	{
		String s(Parent::DebugInfo());
		s.AppendFormat(_T(", netId %d,%d"), m_netId.srvId, m_netId.objId);

		return s;
	}

};}; // Reuben::Network

#endif // _REUBEN_NETWORK_NETOBJECT_H_
