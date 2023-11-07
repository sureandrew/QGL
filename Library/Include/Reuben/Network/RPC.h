#pragma once
#ifndef _REUBEN_NETWORK_RPC_H_
#define _REUBEN_NETWORK_RPC_H_

#include "Buffer.h"

#define RPC_RESULT_OK		0
#define RPC_RESULT_FAIL		-1
#define RPC_NOT_FOUND		-2
#define RPC_NOT_ACCESS		-3

#define INVALID_RPCID		0
#define INVALID_SERVICE_ID	0
#define INVALID_TICKET_ID	0
#define INVALID_SUB_ID		0

#define ALLMASK_BIT				256

namespace Reuben { namespace Network {

	typedef UInt16 RPCID;
	typedef Int16 RPCResult;
	typedef UInt8 ServiceID;
	typedef UInt32 TicketID;
	typedef UInt32 SubID;

	//-- SessionID

	struct SessionID
	{
		ServiceID	serviceId;		// proxy service ID
		SocketID	socketId;		// connected proxy socket ID
		UInt32		ticketId;		// connection time ID

		static CONST Size SIZE = sizeof(ServiceID) + sizeof(SocketID) + sizeof(UInt32);

		SessionID()
			: serviceId(INVALID_SERVICE_ID)
			, socketId(INVALID_SOCKET_ID)
			, ticketId(0)
		{}

		SessionID(CONST SessionID &sid)
			: serviceId(sid.serviceId)
			, socketId(sid.socketId)
			, ticketId(sid.ticketId)
		{}

		SessionID(ServiceID sid, SocketID sockId, UInt32 tid)
			: serviceId(sid)
			, socketId(sockId)
			, ticketId(tid)
		{}

		bool operator == (const SessionID& b) const
		{
			return (serviceId == b.serviceId && socketId == b.socketId && ticketId == b.ticketId);
		}

		bool operator != (const SessionID& b) const
		{
			return (serviceId != b.serviceId || socketId != b.socketId || ticketId != b.ticketId);
		}

		bool operator < (const SessionID& b) const
		{
			return (serviceId < b.serviceId || 
				(serviceId == b.serviceId && (socketId < b.socketId ||
				(socketId == b.socketId && ticketId < b.ticketId))));
		}

		bool operator > (const SessionID& b) const
		{
			return (serviceId > b.serviceId || 
				(serviceId == b.serviceId && (socketId > b.socketId ||
				(socketId == b.socketId && ticketId > b.ticketId))));
		}

		CONST String GetString() const
		{
			Char fmts[16];
			_stprintf(fmts, _T("%02d-%04d-%04d"), serviceId % 100, socketId % 10000, (ticketId / 100) % 10000);
			return (fmts);
		}
	};

	//-- SessionID IO

	template <>
	struct Buffer::Serializer<SessionID>
	{
		static INLINE VOID Read(CONST Buffer& sin, SessionID& sessionId)
		{
			sin.Read<ServiceID>(sessionId.serviceId);
			sin.Read<SocketID>(sessionId.socketId);
			sin.Read<TicketID>(sessionId.ticketId);
		}

		static INLINE VOID Write(Buffer& sout, CONST SessionID& sessionId)
		{
			sout.Write<ServiceID>(sessionId.serviceId);
			sout.Write<SocketID>(sessionId.socketId);
			sout.Write<TicketID>(sessionId.ticketId);
		}
	};

	//-- ProcedureID

	struct ProcedureID
	{
		ServiceID	srvId;
		SubID		subId;

		static CONST Size SIZE = sizeof(ServiceID) + sizeof(SubID);

		ProcedureID()
			: srvId(INVALID_SERVICE_ID)
			, subId(INVALID_SUB_ID)
		{}

		ProcedureID(ServiceID sId, UInt16 subId)
			: srvId(sId)
			, subId(subId)
		{}

		bool operator== (const ProcedureID& b) const
		{
			return (srvId == b.srvId && subId == b.subId);
		}

		bool operator!= (const ProcedureID& b) const
		{
			return (srvId != b.srvId || subId != b.subId);
		}

		bool operator< (const ProcedureID& b) const
		{
			return (srvId < b.srvId || (srvId == b.srvId && subId < b.subId));
		}

		bool operator> (const ProcedureID& b) const
		{
			return (srvId > b.srvId || (srvId == b.srvId && subId > b.subId));
		}
	};

	//-- ProcedureID IO

	template <>
	struct Buffer::Serializer<ProcedureID>
	{
		static INLINE VOID Read(CONST Buffer& sin, ProcedureID& procedureId)
		{
			sin.Read<ServiceID>(procedureId.srvId);
			sin.Read<SubID>(procedureId.subId);
		}

		static INLINE VOID Write(Buffer& sout, CONST ProcedureID& procedureId)
		{
			sout.Write<ServiceID>(procedureId.srvId);
			sout.Write<SubID>(procedureId.subId);
		}
	};

	//-- RPCHeader

	struct RPCHeader
	{
		UInt32 uCRC;
		RPCID rpcId;
		
		static CONST Size SIZE = sizeof(UInt32) + sizeof(RPCID);

		RPCHeader(VOID)
			: uCRC(0)
			, rpcId(INVALID_RPCID) {}

		RPCHeader(CONST RPCID rpcid)
			: uCRC(0)
			, rpcId(rpcid) {}
	};

	//-- RPCHeader IO

	template <>
	struct Buffer::Serializer<RPCHeader>
	{
		static INLINE VOID Read(CONST Buffer& sin, RPCHeader& header)
		{
			sin.Read<UInt32>(header.uCRC);
			sin.Read<RPCID>(header.rpcId);
		}

		static INLINE VOID Write(Buffer& sout, CONST RPCHeader& header)
		{
			sout.Write<UInt32>(header.uCRC);
			sout.Write<RPCID>(header.rpcId);
		}
	};

	//-- RPCContext

	struct RPCContext
	{
		SessionID sessionId;
		ProcedureID pcdId;		// callback source object ID
		Boolean bCallback;		// need to return callback context?

		static CONST Size SIZE = SessionID::SIZE + ProcedureID::SIZE;

		RPCContext(VOID) : bCallback(BOOLEAN_TRUE) {}

		RPCContext(const SessionID& sessid, const ProcedureID &pid = ProcedureID())
			: sessionId(sessid), pcdId(pid), bCallback(BOOLEAN_TRUE) {}
	};

	//-- RPCContext IO

	template <>
	struct Buffer::Serializer<RPCContext>
	{
		static INLINE VOID Read(CONST Buffer& sin, RPCContext& context)
		{
			sin.Read<SessionID>(context.sessionId);
			sin.Read<ProcedureID>(context.pcdId);
		}

		static INLINE VOID Write(Buffer& sout, CONST RPCContext& context)
		{
			sout.Write<SessionID>(context.sessionId);
			sout.Write<ProcedureID>(context.pcdId);
		}
	};

	//-- RPCMask

	struct RPCMask
	{
	public:

		UInt low;
		UInt high;

		static CONST Size SIZE = sizeof(UInt) + sizeof(UInt);

		INLINE RPCMask() : low(0), high(0) {}
		INLINE RPCMask(CONST RPCMask &mask) : low(mask.low), high(mask.high) {}
		INLINE RPCMask(UInt l, UInt h) : low(l), high(h) {}
		INLINE RPCMask(Int bit)
		{
			if (bit < 32)
			{
				low = 1 << bit;
				high = 0;
			}
			else if (bit < 64)
			{
				low = 0;
				high = 1 << (bit - 32);
			}
			else if (bit == ALLMASK_BIT)
			{
				low = 0xFFFFFFFF;
				high = 0xFFFFFFFF;
			}
		}

		INLINE VOID Clear()
		{
			low = 0;
			high = 0;
		}

		INLINE VOID Mask(CONST RPCMask& mask)
		{
			low |= mask.low;
			high |= mask.high;
		}

		INLINE VOID Unmask(CONST RPCMask& mask)
		{
			low &= ~mask.low;
			high &= ~mask.high;
		}

		INLINE Boolean IsBit(Int bit) CONST
		{
			if (bit < 32)
				return ((low & (1 << bit)) > 0);
			else if (bit < 64)
				return ((high & (1 << (bit - 32))) > 0);
			else if (bit == ALLMASK_BIT)
				return BOOLEAN_TRUE;
			else
				return BOOLEAN_FALSE;
		}

		INLINE VOID MaskBit(Int bit)
		{
			if (bit < 32)
				low |= 1 << bit;
			else if (bit < 64)
				high |= 1 << (bit - 32);
			else if (bit == ALLMASK_BIT) {
				low = 0xFFFFFFFF;
				high = 0xFFFFFFFF;
			}
		}

		INLINE VOID UnmaskBit(Int bit)
		{
			if (bit < 32)
				low &= ~(1 << bit);
			else if (bit < 64)
				high &= ~(1 << (bit - 32));
			else if (bit == ALLMASK_BIT) {
				low = 0;
				high = 0;
			}
		}

		//-- variable argument list, last argument must be -1 to stop

		INLINE Boolean IsBits(Int bit, ...) CONST
		{
			Boolean bBit = BOOLEAN_FALSE;
			va_list ap;
			va_start(ap, bit);
			while (bit != -1)
			{
				if (bit < 32)
					bBit = ((low & (1 << bit)) > 0);
				else if (bit < 64)
					bBit = ((high & (1 << (bit - 32))) > 0);
				else if (bit == ALLMASK_BIT)
					bBit = BOOLEAN_TRUE;
				if (bBit)
					break;
				bit = va_arg(ap, Int);
			}
			va_end(ap);
			return bBit;
		}

		INLINE VOID MaskBits(Int bit, ...)
		{
			va_list ap;
			va_start(ap, bit);
			while (bit != -1)
			{
				if (bit < 32)
					low |= 1 << bit;
				else if (bit < 64)
					high |= 1 << (bit - 32);
				else if (bit == ALLMASK_BIT) {
					low = 0xFFFFFFFF;
					high = 0xFFFFFFFF;
					break;
				}
				bit = va_arg(ap, Int);
			}
			va_end(ap);
		}

		INLINE VOID UnmaskBits(Int bit, ...)
		{
			va_list ap;
			va_start(ap, bit);
			while (bit != -1)
			{
				if (bit < 32)
					low &= ~(1 << bit);
				else if (bit < 64)
					high &= ~(1 << (bit - 32));
				else if (bit == ALLMASK_BIT) {
					low = 0;
					high = 0;
					break;
				}
				bit = va_arg(ap, Int);
			}
			va_end(ap);
		}

		INLINE Boolean IsMask(CONST RPCMask& mask) CONST
		{
			return ((low & mask.low) || (high & mask.high));
		}

		INLINE Boolean IsZero() CONST
		{
			return (low == 0 && high == 0);
		}

		//-- operator

		INLINE CONST RPCMask& operator = (UInt& IntValue)
		{
			low = IntValue;
			high = 0;
			return (*this);
		}

		INLINE CONST RPCMask& operator = (CONST RPCMask& mask)
		{
			low = mask.low;
			high = mask.high;
			return (*this);
		}

		INLINE CONST RPCMask& operator &= (CONST RPCMask& mask)
		{
			low &= mask.low;
			high &= mask.high;
			return (*this);
		}

		INLINE CONST RPCMask& operator |= (CONST RPCMask& mask)
		{
			low |= mask.low;
			high |= mask.high;
			return (*this);
		}

		INLINE CONST bool operator == (CONST RPCMask& mask)
		{
			return (low == mask.low && high == mask.high);
		}

	};

	//-- RPCMask IO

	template <>
	struct Buffer::Serializer<RPCMask>
	{
		static INLINE VOID Read(CONST Buffer& sin, RPCMask& mask)
		{
			sin.Read<UInt>(mask.low);
			sin.Read<UInt>(mask.high);
		}

		static INLINE VOID Write(Buffer& sout, CONST RPCMask& mask)
		{
			sout.Write<UInt>(mask.low);
			sout.Write<UInt>(mask.high);
		}
	};

};}; // Reuben::Network

#endif // _REUBEN_NETWORK_RPC_H_
