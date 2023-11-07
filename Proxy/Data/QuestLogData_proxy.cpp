//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/QuestLogData_proxy.h"

void QuestLogData_Proxy::SetId(const UInt32 &char_id, const UInt16 &quest_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	if (m.quest_id != quest_id) { m.quest_id = quest_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestLogData_Proxy::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestLogData_Proxy::SetQuest_id(const UInt16 &quest_id)
{
	bool modified = false;
	if (m.quest_id != quest_id) { m.quest_id = quest_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestLogData_Proxy::SetInfo(const UInt16 &Type_id, const String &name, const String &descript)
{
	bool modified = false;
	if (m.Type_id != Type_id) { m.Type_id = Type_id; modified = true; }
	if (m.name != name) { m.name = name; modified = true; }
	if (m.descript != descript) { m.descript = descript; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void QuestLogData_Proxy::SetType_id(const UInt16 &Type_id)
{
	bool modified = false;
	if (m.Type_id != Type_id) { m.Type_id = Type_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void QuestLogData_Proxy::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void QuestLogData_Proxy::SetDescript(const String &descript)
{
	bool modified = false;
	if (m.descript != descript) { m.descript = descript; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void QuestLogData_Proxy::SetBeginNpc(const UInt32 &beginNpc, const UInt16 &beginNpc_Map, const UInt16 &beginNpc_x, const UInt16 &beginNpc_y)
{
	bool modified = false;
	if (m.beginNpc != beginNpc) { m.beginNpc = beginNpc; modified = true; }
	if (m.beginNpc_Map != beginNpc_Map) { m.beginNpc_Map = beginNpc_Map; modified = true; }
	if (m.beginNpc_x != beginNpc_x) { m.beginNpc_x = beginNpc_x; modified = true; }
	if (m.beginNpc_y != beginNpc_y) { m.beginNpc_y = beginNpc_y; modified = true; }
	MarkUpdateAll(beginNpcMask, modified);
}

void QuestLogData_Proxy::SetBeginNpc(const UInt32 &beginNpc)
{
	bool modified = false;
	if (m.beginNpc != beginNpc) { m.beginNpc = beginNpc; modified = true; }
	MarkUpdateAll(beginNpcMask, modified);
}

void QuestLogData_Proxy::SetBeginNpc_Map(const UInt16 &beginNpc_Map)
{
	bool modified = false;
	if (m.beginNpc_Map != beginNpc_Map) { m.beginNpc_Map = beginNpc_Map; modified = true; }
	MarkUpdateAll(beginNpcMask, modified);
}

void QuestLogData_Proxy::SetBeginNpc_x(const UInt16 &beginNpc_x)
{
	bool modified = false;
	if (m.beginNpc_x != beginNpc_x) { m.beginNpc_x = beginNpc_x; modified = true; }
	MarkUpdateAll(beginNpcMask, modified);
}

void QuestLogData_Proxy::SetBeginNpc_y(const UInt16 &beginNpc_y)
{
	bool modified = false;
	if (m.beginNpc_y != beginNpc_y) { m.beginNpc_y = beginNpc_y; modified = true; }
	MarkUpdateAll(beginNpcMask, modified);
}

void QuestLogData_Proxy::SetFinishNpc(const UInt32 &finishNpc, const UInt16 &finishNpc_Map, const UInt16 &finishNpc_x, const UInt16 &finishNpc_y)
{
	bool modified = false;
	if (m.finishNpc != finishNpc) { m.finishNpc = finishNpc; modified = true; }
	if (m.finishNpc_Map != finishNpc_Map) { m.finishNpc_Map = finishNpc_Map; modified = true; }
	if (m.finishNpc_x != finishNpc_x) { m.finishNpc_x = finishNpc_x; modified = true; }
	if (m.finishNpc_y != finishNpc_y) { m.finishNpc_y = finishNpc_y; modified = true; }
	MarkUpdateAll(finishNpcMask, modified);
}

void QuestLogData_Proxy::SetFinishNpc(const UInt32 &finishNpc)
{
	bool modified = false;
	if (m.finishNpc != finishNpc) { m.finishNpc = finishNpc; modified = true; }
	MarkUpdateAll(finishNpcMask, modified);
}

void QuestLogData_Proxy::SetFinishNpc_Map(const UInt16 &finishNpc_Map)
{
	bool modified = false;
	if (m.finishNpc_Map != finishNpc_Map) { m.finishNpc_Map = finishNpc_Map; modified = true; }
	MarkUpdateAll(finishNpcMask, modified);
}

void QuestLogData_Proxy::SetFinishNpc_x(const UInt16 &finishNpc_x)
{
	bool modified = false;
	if (m.finishNpc_x != finishNpc_x) { m.finishNpc_x = finishNpc_x; modified = true; }
	MarkUpdateAll(finishNpcMask, modified);
}

void QuestLogData_Proxy::SetFinishNpc_y(const UInt16 &finishNpc_y)
{
	bool modified = false;
	if (m.finishNpc_y != finishNpc_y) { m.finishNpc_y = finishNpc_y; modified = true; }
	MarkUpdateAll(finishNpcMask, modified);
}

void QuestLogData_Proxy::SetTarget1(const UInt32 &target1_id, const UInt16 &target1_v1, const UInt16 &target1_v2, const UInt16 &target1_v3, const UInt8 &target1_vtype, const UInt16 &target1_num, const UInt16 &target1_max)
{
	bool modified = false;
	if (m.target1_id != target1_id) { m.target1_id = target1_id; modified = true; }
	if (m.target1_v1 != target1_v1) { m.target1_v1 = target1_v1; modified = true; }
	if (m.target1_v2 != target1_v2) { m.target1_v2 = target1_v2; modified = true; }
	if (m.target1_v3 != target1_v3) { m.target1_v3 = target1_v3; modified = true; }
	if (m.target1_vtype != target1_vtype) { m.target1_vtype = target1_vtype; modified = true; }
	if (m.target1_num != target1_num) { m.target1_num = target1_num; modified = true; }
	if (m.target1_max != target1_max) { m.target1_max = target1_max; modified = true; }
	MarkUpdateAll(target1Mask, modified);
}

void QuestLogData_Proxy::SetTarget1_id(const UInt32 &target1_id)
{
	bool modified = false;
	if (m.target1_id != target1_id) { m.target1_id = target1_id; modified = true; }
	MarkUpdateAll(target1Mask, modified);
}

void QuestLogData_Proxy::SetTarget1_v1(const UInt16 &target1_v1)
{
	bool modified = false;
	if (m.target1_v1 != target1_v1) { m.target1_v1 = target1_v1; modified = true; }
	MarkUpdateAll(target1Mask, modified);
}

void QuestLogData_Proxy::SetTarget1_v2(const UInt16 &target1_v2)
{
	bool modified = false;
	if (m.target1_v2 != target1_v2) { m.target1_v2 = target1_v2; modified = true; }
	MarkUpdateAll(target1Mask, modified);
}

void QuestLogData_Proxy::SetTarget1_v3(const UInt16 &target1_v3)
{
	bool modified = false;
	if (m.target1_v3 != target1_v3) { m.target1_v3 = target1_v3; modified = true; }
	MarkUpdateAll(target1Mask, modified);
}

void QuestLogData_Proxy::SetTarget1_vtype(const UInt8 &target1_vtype)
{
	bool modified = false;
	if (m.target1_vtype != target1_vtype) { m.target1_vtype = target1_vtype; modified = true; }
	MarkUpdateAll(target1Mask, modified);
}

void QuestLogData_Proxy::SetTarget1_num(const UInt16 &target1_num)
{
	bool modified = false;
	if (m.target1_num != target1_num) { m.target1_num = target1_num; modified = true; }
	MarkUpdateAll(target1Mask, modified);
}

void QuestLogData_Proxy::SetTarget1_max(const UInt16 &target1_max)
{
	bool modified = false;
	if (m.target1_max != target1_max) { m.target1_max = target1_max; modified = true; }
	MarkUpdateAll(target1Mask, modified);
}

void QuestLogData_Proxy::SetTarget2(const UInt32 &target2_id, const UInt16 &target2_v1, const UInt16 &target2_v2, const UInt16 &target2_v3, const UInt8 &target2_vtype, const UInt16 &target2_num, const UInt16 &target2_max)
{
	bool modified = false;
	if (m.target2_id != target2_id) { m.target2_id = target2_id; modified = true; }
	if (m.target2_v1 != target2_v1) { m.target2_v1 = target2_v1; modified = true; }
	if (m.target2_v2 != target2_v2) { m.target2_v2 = target2_v2; modified = true; }
	if (m.target2_v3 != target2_v3) { m.target2_v3 = target2_v3; modified = true; }
	if (m.target2_vtype != target2_vtype) { m.target2_vtype = target2_vtype; modified = true; }
	if (m.target2_num != target2_num) { m.target2_num = target2_num; modified = true; }
	if (m.target2_max != target2_max) { m.target2_max = target2_max; modified = true; }
	MarkUpdateAll(target2Mask, modified);
}

void QuestLogData_Proxy::SetTarget2_id(const UInt32 &target2_id)
{
	bool modified = false;
	if (m.target2_id != target2_id) { m.target2_id = target2_id; modified = true; }
	MarkUpdateAll(target2Mask, modified);
}

void QuestLogData_Proxy::SetTarget2_v1(const UInt16 &target2_v1)
{
	bool modified = false;
	if (m.target2_v1 != target2_v1) { m.target2_v1 = target2_v1; modified = true; }
	MarkUpdateAll(target2Mask, modified);
}

void QuestLogData_Proxy::SetTarget2_v2(const UInt16 &target2_v2)
{
	bool modified = false;
	if (m.target2_v2 != target2_v2) { m.target2_v2 = target2_v2; modified = true; }
	MarkUpdateAll(target2Mask, modified);
}

void QuestLogData_Proxy::SetTarget2_v3(const UInt16 &target2_v3)
{
	bool modified = false;
	if (m.target2_v3 != target2_v3) { m.target2_v3 = target2_v3; modified = true; }
	MarkUpdateAll(target2Mask, modified);
}

void QuestLogData_Proxy::SetTarget2_vtype(const UInt8 &target2_vtype)
{
	bool modified = false;
	if (m.target2_vtype != target2_vtype) { m.target2_vtype = target2_vtype; modified = true; }
	MarkUpdateAll(target2Mask, modified);
}

void QuestLogData_Proxy::SetTarget2_num(const UInt16 &target2_num)
{
	bool modified = false;
	if (m.target2_num != target2_num) { m.target2_num = target2_num; modified = true; }
	MarkUpdateAll(target2Mask, modified);
}

void QuestLogData_Proxy::SetTarget2_max(const UInt16 &target2_max)
{
	bool modified = false;
	if (m.target2_max != target2_max) { m.target2_max = target2_max; modified = true; }
	MarkUpdateAll(target2Mask, modified);
}

void QuestLogData_Proxy::SetTarget3(const UInt32 &target3_id, const UInt16 &target3_v1, const UInt16 &target3_v2, const UInt16 &target3_v3, const UInt8 &target3_vtype, const UInt16 &target3_num, const UInt16 &target3_max)
{
	bool modified = false;
	if (m.target3_id != target3_id) { m.target3_id = target3_id; modified = true; }
	if (m.target3_v1 != target3_v1) { m.target3_v1 = target3_v1; modified = true; }
	if (m.target3_v2 != target3_v2) { m.target3_v2 = target3_v2; modified = true; }
	if (m.target3_v3 != target3_v3) { m.target3_v3 = target3_v3; modified = true; }
	if (m.target3_vtype != target3_vtype) { m.target3_vtype = target3_vtype; modified = true; }
	if (m.target3_num != target3_num) { m.target3_num = target3_num; modified = true; }
	if (m.target3_max != target3_max) { m.target3_max = target3_max; modified = true; }
	MarkUpdateAll(target3Mask, modified);
}

void QuestLogData_Proxy::SetTarget3_id(const UInt32 &target3_id)
{
	bool modified = false;
	if (m.target3_id != target3_id) { m.target3_id = target3_id; modified = true; }
	MarkUpdateAll(target3Mask, modified);
}

void QuestLogData_Proxy::SetTarget3_v1(const UInt16 &target3_v1)
{
	bool modified = false;
	if (m.target3_v1 != target3_v1) { m.target3_v1 = target3_v1; modified = true; }
	MarkUpdateAll(target3Mask, modified);
}

void QuestLogData_Proxy::SetTarget3_v2(const UInt16 &target3_v2)
{
	bool modified = false;
	if (m.target3_v2 != target3_v2) { m.target3_v2 = target3_v2; modified = true; }
	MarkUpdateAll(target3Mask, modified);
}

void QuestLogData_Proxy::SetTarget3_v3(const UInt16 &target3_v3)
{
	bool modified = false;
	if (m.target3_v3 != target3_v3) { m.target3_v3 = target3_v3; modified = true; }
	MarkUpdateAll(target3Mask, modified);
}

void QuestLogData_Proxy::SetTarget3_vtype(const UInt8 &target3_vtype)
{
	bool modified = false;
	if (m.target3_vtype != target3_vtype) { m.target3_vtype = target3_vtype; modified = true; }
	MarkUpdateAll(target3Mask, modified);
}

void QuestLogData_Proxy::SetTarget3_num(const UInt16 &target3_num)
{
	bool modified = false;
	if (m.target3_num != target3_num) { m.target3_num = target3_num; modified = true; }
	MarkUpdateAll(target3Mask, modified);
}

void QuestLogData_Proxy::SetTarget3_max(const UInt16 &target3_max)
{
	bool modified = false;
	if (m.target3_max != target3_max) { m.target3_max = target3_max; modified = true; }
	MarkUpdateAll(target3Mask, modified);
}

void QuestLogData_Proxy::SetTarget4(const UInt32 &target4_id, const UInt16 &target4_v1, const UInt16 &target4_v2, const UInt16 &target4_v3, const UInt8 &target4_vtype, const UInt16 &target4_num, const UInt16 &target4_max)
{
	bool modified = false;
	if (m.target4_id != target4_id) { m.target4_id = target4_id; modified = true; }
	if (m.target4_v1 != target4_v1) { m.target4_v1 = target4_v1; modified = true; }
	if (m.target4_v2 != target4_v2) { m.target4_v2 = target4_v2; modified = true; }
	if (m.target4_v3 != target4_v3) { m.target4_v3 = target4_v3; modified = true; }
	if (m.target4_vtype != target4_vtype) { m.target4_vtype = target4_vtype; modified = true; }
	if (m.target4_num != target4_num) { m.target4_num = target4_num; modified = true; }
	if (m.target4_max != target4_max) { m.target4_max = target4_max; modified = true; }
	MarkUpdateAll(target4Mask, modified);
}

void QuestLogData_Proxy::SetTarget4_id(const UInt32 &target4_id)
{
	bool modified = false;
	if (m.target4_id != target4_id) { m.target4_id = target4_id; modified = true; }
	MarkUpdateAll(target4Mask, modified);
}

void QuestLogData_Proxy::SetTarget4_v1(const UInt16 &target4_v1)
{
	bool modified = false;
	if (m.target4_v1 != target4_v1) { m.target4_v1 = target4_v1; modified = true; }
	MarkUpdateAll(target4Mask, modified);
}

void QuestLogData_Proxy::SetTarget4_v2(const UInt16 &target4_v2)
{
	bool modified = false;
	if (m.target4_v2 != target4_v2) { m.target4_v2 = target4_v2; modified = true; }
	MarkUpdateAll(target4Mask, modified);
}

void QuestLogData_Proxy::SetTarget4_v3(const UInt16 &target4_v3)
{
	bool modified = false;
	if (m.target4_v3 != target4_v3) { m.target4_v3 = target4_v3; modified = true; }
	MarkUpdateAll(target4Mask, modified);
}

void QuestLogData_Proxy::SetTarget4_vtype(const UInt8 &target4_vtype)
{
	bool modified = false;
	if (m.target4_vtype != target4_vtype) { m.target4_vtype = target4_vtype; modified = true; }
	MarkUpdateAll(target4Mask, modified);
}

void QuestLogData_Proxy::SetTarget4_num(const UInt16 &target4_num)
{
	bool modified = false;
	if (m.target4_num != target4_num) { m.target4_num = target4_num; modified = true; }
	MarkUpdateAll(target4Mask, modified);
}

void QuestLogData_Proxy::SetTarget4_max(const UInt16 &target4_max)
{
	bool modified = false;
	if (m.target4_max != target4_max) { m.target4_max = target4_max; modified = true; }
	MarkUpdateAll(target4Mask, modified);
}

void QuestLogData_Proxy::SetTarget5(const UInt32 &target5_id, const UInt16 &target5_v1, const UInt16 &target5_v2, const UInt16 &target5_v3, const UInt8 &target5_vtype, const UInt16 &target5_num, const UInt16 &target5_max)
{
	bool modified = false;
	if (m.target5_id != target5_id) { m.target5_id = target5_id; modified = true; }
	if (m.target5_v1 != target5_v1) { m.target5_v1 = target5_v1; modified = true; }
	if (m.target5_v2 != target5_v2) { m.target5_v2 = target5_v2; modified = true; }
	if (m.target5_v3 != target5_v3) { m.target5_v3 = target5_v3; modified = true; }
	if (m.target5_vtype != target5_vtype) { m.target5_vtype = target5_vtype; modified = true; }
	if (m.target5_num != target5_num) { m.target5_num = target5_num; modified = true; }
	if (m.target5_max != target5_max) { m.target5_max = target5_max; modified = true; }
	MarkUpdateAll(target5Mask, modified);
}

void QuestLogData_Proxy::SetTarget5_id(const UInt32 &target5_id)
{
	bool modified = false;
	if (m.target5_id != target5_id) { m.target5_id = target5_id; modified = true; }
	MarkUpdateAll(target5Mask, modified);
}

void QuestLogData_Proxy::SetTarget5_v1(const UInt16 &target5_v1)
{
	bool modified = false;
	if (m.target5_v1 != target5_v1) { m.target5_v1 = target5_v1; modified = true; }
	MarkUpdateAll(target5Mask, modified);
}

void QuestLogData_Proxy::SetTarget5_v2(const UInt16 &target5_v2)
{
	bool modified = false;
	if (m.target5_v2 != target5_v2) { m.target5_v2 = target5_v2; modified = true; }
	MarkUpdateAll(target5Mask, modified);
}

void QuestLogData_Proxy::SetTarget5_v3(const UInt16 &target5_v3)
{
	bool modified = false;
	if (m.target5_v3 != target5_v3) { m.target5_v3 = target5_v3; modified = true; }
	MarkUpdateAll(target5Mask, modified);
}

void QuestLogData_Proxy::SetTarget5_vtype(const UInt8 &target5_vtype)
{
	bool modified = false;
	if (m.target5_vtype != target5_vtype) { m.target5_vtype = target5_vtype; modified = true; }
	MarkUpdateAll(target5Mask, modified);
}

void QuestLogData_Proxy::SetTarget5_num(const UInt16 &target5_num)
{
	bool modified = false;
	if (m.target5_num != target5_num) { m.target5_num = target5_num; modified = true; }
	MarkUpdateAll(target5Mask, modified);
}

void QuestLogData_Proxy::SetTarget5_max(const UInt16 &target5_max)
{
	bool modified = false;
	if (m.target5_max != target5_max) { m.target5_max = target5_max; modified = true; }
	MarkUpdateAll(target5Mask, modified);
}

void QuestLogData_Proxy::SetTimer(const UInt32 &timeLimit, const UInt16 &interval)
{
	bool modified = false;
	if (m.timeLimit != timeLimit) { m.timeLimit = timeLimit; modified = true; }
	if (m.interval != interval) { m.interval = interval; modified = true; }
	MarkUpdateAll(timerMask, modified);
}

void QuestLogData_Proxy::SetTimeLimit(const UInt32 &timeLimit)
{
	bool modified = false;
	if (m.timeLimit != timeLimit) { m.timeLimit = timeLimit; modified = true; }
	MarkUpdateAll(timerMask, modified);
}

void QuestLogData_Proxy::SetInterval(const UInt16 &interval)
{
	bool modified = false;
	if (m.interval != interval) { m.interval = interval; modified = true; }
	MarkUpdateAll(timerMask, modified);
}

void QuestLogData_Proxy::SetBuff(const UInt16 &buff)
{
	bool modified = false;
	if (m.buff != buff) { m.buff = buff; modified = true; }
	MarkUpdateAll(buffMask, modified);
}

void QuestLogData_Proxy::SetState(const UInt8 &state)
{
	bool modified = false;
	if (m.state != state) { m.state = state; modified = true; }
	MarkUpdateAll(stateMask, modified);
}

void QuestLogData_Proxy::SetFeature(const UInt16 &feature)
{
	bool modified = false;
	if (m.feature != feature) { m.feature = feature; modified = true; }
	MarkUpdateAll(featureMask, modified);
}

void QuestLogData_Proxy::SetStatus(const UInt8 &status)
{
	bool modified = false;
	if (m.status != status) { m.status = status; modified = true; }
	MarkUpdateAll(statusMask, modified);
}


void QuestLogData_Proxy::ReadAttr(CONST Buffer &buf)
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
}

void QuestLogData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.char_id);
		buf.Write<UInt16>(m.quest_id);
	}
	if (mask.IsBit(infoMask))
	{
		buf.Write<UInt16>(m.Type_id);
		buf.Write<String>(m.name);
		buf.Write<String>(m.descript);
	}
	if (mask.IsBit(beginNpcMask))
	{
		buf.Write<UInt32>(m.beginNpc);
		buf.Write<UInt16>(m.beginNpc_Map);
		buf.Write<UInt16>(m.beginNpc_x);
		buf.Write<UInt16>(m.beginNpc_y);
	}
	if (mask.IsBit(finishNpcMask))
	{
		buf.Write<UInt32>(m.finishNpc);
		buf.Write<UInt16>(m.finishNpc_Map);
		buf.Write<UInt16>(m.finishNpc_x);
		buf.Write<UInt16>(m.finishNpc_y);
	}
	if (mask.IsBit(target1Mask))
	{
		buf.Write<UInt32>(m.target1_id);
		buf.Write<UInt16>(m.target1_v1);
		buf.Write<UInt16>(m.target1_v2);
		buf.Write<UInt16>(m.target1_v3);
		buf.Write<UInt8>(m.target1_vtype);
		buf.Write<UInt16>(m.target1_num);
		buf.Write<UInt16>(m.target1_max);
	}
	if (mask.IsBit(target2Mask))
	{
		buf.Write<UInt32>(m.target2_id);
		buf.Write<UInt16>(m.target2_v1);
		buf.Write<UInt16>(m.target2_v2);
		buf.Write<UInt16>(m.target2_v3);
		buf.Write<UInt8>(m.target2_vtype);
		buf.Write<UInt16>(m.target2_num);
		buf.Write<UInt16>(m.target2_max);
	}
	if (mask.IsBit(target3Mask))
	{
		buf.Write<UInt32>(m.target3_id);
		buf.Write<UInt16>(m.target3_v1);
		buf.Write<UInt16>(m.target3_v2);
		buf.Write<UInt16>(m.target3_v3);
		buf.Write<UInt8>(m.target3_vtype);
		buf.Write<UInt16>(m.target3_num);
		buf.Write<UInt16>(m.target3_max);
	}
	if (mask.IsBit(target4Mask))
	{
		buf.Write<UInt32>(m.target4_id);
		buf.Write<UInt16>(m.target4_v1);
		buf.Write<UInt16>(m.target4_v2);
		buf.Write<UInt16>(m.target4_v3);
		buf.Write<UInt8>(m.target4_vtype);
		buf.Write<UInt16>(m.target4_num);
		buf.Write<UInt16>(m.target4_max);
	}
	if (mask.IsBit(target5Mask))
	{
		buf.Write<UInt32>(m.target5_id);
		buf.Write<UInt16>(m.target5_v1);
		buf.Write<UInt16>(m.target5_v2);
		buf.Write<UInt16>(m.target5_v3);
		buf.Write<UInt8>(m.target5_vtype);
		buf.Write<UInt16>(m.target5_num);
		buf.Write<UInt16>(m.target5_max);
	}
	if (mask.IsBit(timerMask))
	{
		buf.Write<UInt32>(m.timeLimit);
		buf.Write<UInt16>(m.interval);
	}
	if (mask.IsBit(buffMask))
	{
		buf.Write<UInt16>(m.buff);
	}
	if (mask.IsBit(stateMask))
	{
		buf.Write<UInt8>(m.state);
	}
	if (mask.IsBit(featureMask))
	{
		buf.Write<UInt16>(m.feature);
	}
	if (mask.IsBit(statusMask))
	{
		buf.Write<UInt8>(m.status);
	}
}

void QuestLogData_Proxy::OnUpdate()
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
}

#pragma optimize("", off)
RPCResult QuestLogData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_QuestLogData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult QuestLogData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_QuestLogData_UP_STATE:
			_result = RPCDecode<1894523640>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
