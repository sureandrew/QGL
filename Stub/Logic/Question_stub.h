///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_QUESTION_STUB_H_
#define _RPC_QUESTION_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Question_enum.h"

class Question_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult Anwser(LPCPEER pPeer, RPCContext &context, const UInt32 &question_id, const UInt16 &anwserIndex);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		pointMask	= Parent::NextFreeMask + 0,
		ranklistMask	= Parent::NextFreeMask + 1,
		NextFreeMask	= Parent::NextFreeMask + 2
	};
	virtual RPCResult CB_NextQuestion(LPCPEER pPeer, RPCContext &context, const UInt32 &count, const UInt32 &question_id, const String &description, const String &option1, const String &option2, const String &option3, const String &option4);
	virtual RPCResult Anwser(LPCPEER pPeer, RPCContext &context, const UInt32 &question_id, const UInt16 &anwserIndex) = 0;
	virtual RPCResult CB_AnswerResult(LPCPEER pPeer, RPCContext &context, const Boolean &bRight);
	virtual RPCResult CB_StartCountDown(LPCPEER pPeer, RPCContext &context, const UInt32 &count);
	const UInt32& GetPoint() const { return m.point; }
	const String& GetName1() const { return m.name1; }
	const UInt32& GetPoint1() const { return m.point1; }
	const String& GetName2() const { return m.name2; }
	const UInt32& GetPoint2() const { return m.point2; }
	const String& GetName3() const { return m.name3; }
	const UInt32& GetPoint3() const { return m.point3; }
	const String& GetName4() const { return m.name4; }
	const UInt32& GetPoint4() const { return m.point4; }
	const String& GetName5() const { return m.name5; }
	const UInt32& GetPoint5() const { return m.point5; }
	const String& GetName6() const { return m.name6; }
	const UInt32& GetPoint6() const { return m.point6; }
	const String& GetName7() const { return m.name7; }
	const UInt32& GetPoint7() const { return m.point7; }
	const String& GetName8() const { return m.name8; }
	const UInt32& GetPoint8() const { return m.point8; }
	const String& GetName9() const { return m.name9; }
	const UInt32& GetPoint9() const { return m.point9; }
	const String& GetName10() const { return m.name10; }
	const UInt32& GetPoint10() const { return m.point10; }
	void SetPoint(const UInt32 &point);
	void SetRanklist(const String &name1, const UInt32 &point1, const String &name2, const UInt32 &point2, const String &name3, const UInt32 &point3, const String &name4, const UInt32 &point4, const String &name5, const UInt32 &point5, const String &name6, const UInt32 &point6, const String &name7, const UInt32 &point7, const String &name8, const UInt32 &point8, const String &name9, const UInt32 &point9, const String &name10, const UInt32 &point10);
	void SetName1(const String &name1);
	void SetPoint1(const UInt32 &point1);
	void SetName2(const String &name2);
	void SetPoint2(const UInt32 &point2);
	void SetName3(const String &name3);
	void SetPoint3(const UInt32 &point3);
	void SetName4(const String &name4);
	void SetPoint4(const UInt32 &point4);
	void SetName5(const String &name5);
	void SetPoint5(const UInt32 &point5);
	void SetName6(const String &name6);
	void SetPoint6(const UInt32 &point6);
	void SetName7(const String &name7);
	void SetPoint7(const UInt32 &point7);
	void SetName8(const String &name8);
	void SetPoint8(const UInt32 &point8);
	void SetName9(const String &name9);
	void SetPoint9(const UInt32 &point9);
	void SetName10(const String &name10);
	void SetPoint10(const UInt32 &point10);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//point
	UInt32 point;
	//ranklist
	String name1;
	UInt32 point1;
	String name2;
	UInt32 point2;
	String name3;
	UInt32 point3;
	String name4;
	UInt32 point4;
	String name5;
	UInt32 point5;
	String name6;
	UInt32 point6;
	String name7;
	UInt32 point7;
	String name8;
	UInt32 point8;
	String name9;
	UInt32 point9;
	String name10;
	UInt32 point10;
} m;

public:
	INLINE Question_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_QUESTION_STUB_H_
