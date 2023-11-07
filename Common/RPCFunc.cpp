#include "Common.h"
#include "Procedure.h"
#include "RPCFunc.h"

//#define SAFETY_CHECK		1
#define DUMP_SIZE			0

NetObject* CreateNetObject(ClassID cid)
{
	NetObject *obj = (NetObject*) (Reuben::Simulation::CreateObject(cid));
	LocalRPCManager::GetInstance().Register(obj);
	return obj;
}

NetObject* CreateNetObject(ClassID cid, const RPCNetID &netId)
{
	NetObject *obj = (NetObject*) (Reuben::Simulation::CreateObject(cid));
	LocalRPCManager::GetInstance().Register(obj, netId);
	return obj;
}

NetGroup* CreateNetGroup()
{
	NetGroup *obj = Reuben::Simulation::CreateObject<NetGroup>();
	LocalRPCManager::GetInstance().Register(obj);
	return obj;
}

NetGroup* CreateNetGroup(const RPCNetID &netId)
{
	NetGroup *obj = Reuben::Simulation::CreateObject<NetGroup>();
	LocalRPCManager::GetInstance().Register(obj, netId);
	return obj;
}

void _DeleteNetObject(NetObject* obj)
{
	if (obj == NULL)
		return;

	ProcedureManager::GetInstance().RemoveByObject(obj);
	if (LocalRPCManager::GetInstance().Unregister(obj))
		Reuben::Simulation::DeleteObject(obj);
	else
	{
		TRACE_WARNDTL_3(GLOBAL_LOGGER, 
			_FW("unregister failed, object remained, netId %d:%d, objId %d"),
			obj->GetNetId().srvId, obj->GetNetId().objId, obj->GetID())
	}
}

void DeleteNetChilds(NetGroup* grp)
{
	if (grp == NULL || grp->GetNetId().IsValid() && grp != FindNetObject(grp->GetNetId()))
		return;

	NetObject *obj;
	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		obj = grp->GetChild(i);
		if (obj)
			_DeleteNetObject(obj);
	}
	grp->Clear(BOOLEAN_FALSE);
}

NetObject* FindNetObject(const RPCNetID &netId)
{
	return LocalRPCManager::GetInstance().FindNetObject(netId);
}

RPCContext& GetCbContext(RPCContext &context)
{
	ProcedureInfo* pinfo = ProcedureManager::GetInstance().Get(context.pcdId);
	if (pinfo)
		return pinfo->origContext;
	else
		return context;
}

VOID DumpBuffer(CONST Buffer& buffer)
{
	String bstr;
	Size dsize = DUMP_SIZE;
	if (dsize == 0)
		buffer.DumpString(bstr, 0, buffer.GetSize());
	else
		buffer.DumpString(bstr, buffer.GetCursor(), DUMP_SIZE);
	TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("Dump Buffer: cursor %d, dump size %d %s"), 
		buffer.GetCursor(), dsize, bstr.c_str());
}

// bitstream for NetObject
VOID Reuben::Network::Buffer::Serializer<NetObject*>::Read(CONST Buffer& sin, NetObject*& obj)
{
	ClassID classId;
	RPCNetID netId;

	sin.Read<ClassID>(classId);
	if (classId == 0) {									// return null object if class ID = 0
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("RPC Warning: NetObject Read: null object, classId = 0"));
		obj = NULL;
		return;
	}

	sin.Read<RPCNetID>(netId);

	// don't register object if netId is invalid
	if (netId.IsValid()) {
		obj = FindNetObject(netId);
		if (!obj) {					// create NetObject if not found
			obj = Reuben::Simulation::CreateObject<NetObject>(classId);
			if (obj == NULL) {		// cannot create NetObject if not registered or memory out
				TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("RPC Warning: NetObject classId %d not registered")
					_T(" or wrong netId %d,%d at buffer <%d>"), 
					classId, netId.srvId, netId.objId, 
					sin.GetCursor() - sizeof(ClassID) - sizeof(RPCNetID));
				DumpBuffer(sin);
				return;
			}
			LocalRPCManager::GetInstance().Register(obj, netId);
		} else if (obj->GetClassID() != classId) {	// found NetObject Class ID mismatch 
			TRACE_WARNDTL_6(GLOBAL_LOGGER, _F("RPC Warning: found NetObject %d,%d objId %d, classId %d")
				_T(" mismatch received classId %d at buffer <%d>"),
				obj->GetNetId().srvId, obj->GetNetId().objId, obj->GetID(),
				obj->GetClassID(), classId, 
				sin.GetCursor() - sizeof(ClassID) - sizeof(RPCNetID));
			DumpBuffer(sin);
			// replace old object
			DeleteNetObject(obj);
			obj = Reuben::Simulation::CreateObject<NetObject>(classId);
			LocalRPCManager::GetInstance().Register(obj, netId);
		}
	} else {
		// set to one-time used NetObject
		obj = Reuben::Simulation::CreateObject<NetObject>(classId);
	}
	TRACE_ENSURE(obj);
	obj->ReadAttr(sin);
	obj->SetSendMask(obj->GetUpdateMask());
	obj->OnUpdate();

	if (obj->IsSubClass(NetGroup_CLASSID)) {
		NetGroup* grp = CastNetObject<NetGroup>(obj);
		if (grp) {
			NetObject* subObj;
			UInt16 count;

			sin.Read<UInt16>(count);
			for (UInt16 i = 0; i < count; i++) {
				sin.Read<NetObject*>(subObj);
				if (subObj)
					grp->Add(subObj);
				else
					TRACE_WARNDTL_4(GLOBAL_LOGGER, 
						_T("RPC Warning: null NetObject child %d in NetGroup ID %d netId %d,%d"),
						i, grp->GetID(), grp->GetNetId().srvId, grp->GetNetId().objId);
			}
			//TRACE_ENSURE(count == grp->GetCount());
		}
	}
}

VOID Reuben::Network::Buffer::Serializer<NetObject*>::Write(Buffer& sout, CONST NetObject* obj)
{
	if (obj == NULL
#ifdef SAFETY_CHECK
		|| obj->GetNetId().IsValid() && FindNetObject(obj->GetNetId()) != obj	// safety check
#endif // SAFETY_CHECK
		)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, 
			_T("RPC Warning: NetObject Write: NetObject %d already deleted"), obj);
		sout.Write<ClassID>(0);					// write a null object
		return;
	}

	sout.Write<ClassID>(obj->GetClassID());
	sout.Write<RPCNetID>(obj->GetNetId());
	obj->WriteAttr(sout, obj->GetSendMask());
	if (obj->IsSubClass(NetGroup_CLASSID)) {
		NetGroup* grp = CastNetObject<NetGroup>((NetObject*) obj);
		if (grp) {
			grp->Validate();					// validate before send
			sout.Write<UInt16>(grp->GetCount());
			for (UInt16 i = 0; i < grp->GetCount(); i++) {
				NetObject *subObj = grp->GetChild(i);
				if (subObj) {
					sout.Write<NetObject*>(subObj);
				}
			}
		}
	}
}

// bitstream for NetGroup
VOID Reuben::Network::Buffer::Serializer<NetGroup*>::Read(CONST Buffer& sin, NetGroup*& grp)
{
	NetObject *obj;
	Serializer<NetObject*>::Read(sin, obj);
	grp = (NetGroup*) obj;
}

VOID Reuben::Network::Buffer::Serializer<NetGroup*>::Write(Buffer& sout, CONST NetGroup* grp)
{
	Serializer<NetObject*>::Write(sout, grp);
}

VOID Reuben::Network::Buffer::Serializer<RpcCharInfo>::Read(CONST Buffer& sin, RpcCharInfo& obj)
{
	sin.Read<SessionID>(obj.sid);
	sin.Read<Boolean>(obj.bConnected);
	sin.Read<RPCNetID>(obj.netId);
	sin.Read<UInt32>(obj.char_id);
	sin.Read<UInt8>(obj.team);
	sin.Read<AutoData>(obj.autoData);
	sin.Read<UInt8>(obj.petID);
	sin.Read<Boolean>(obj.bHasParty);
	sin.Read<String>(obj.buffCache);
}

VOID Reuben::Network::Buffer::Serializer<RpcCharInfo>::Write(Buffer& sout, CONST RpcCharInfo obj)
{
	sout.Write<SessionID>(obj.sid);
	sout.Write<Boolean>(obj.bConnected);
	sout.Write<RPCNetID>(obj.netId);
	sout.Write<UInt32>(obj.char_id);
	sout.Write<UInt8>(obj.team);
	sout.Write<AutoData>(obj.autoData);
	sout.Write<UInt8>(obj.petID);
	sout.Write<Boolean>(obj.bHasParty);
	sout.Write<String>(obj.buffCache);
}