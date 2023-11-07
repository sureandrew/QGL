///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_QUESTION_PROXY_H_
#define _RPC_QUESTION_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Question_enum.h"

class Question_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_NextQuestion(LPCPEER pPeer, RPCContext &context, const UInt32 &count, const UInt32 &question_id, const String &description, const String &option1, const String &option2, const String &option3, const String &option4);
	virtual RPCResult CB_AnswerResult(LPCPEER pPeer, RPCContext &context, const Boolean &bRight);
	virtual RPCResult CB_StartCountDown(LPCPEER pPeer, RPCContext &context, const UInt32 &count);
	// OnUpdate can be implemented selectively
	virtual void OnUpdatePoint(const UInt32 &point);
	virtual void OnUpdateRanklist(const String &name1, const UInt32 &point1, const String &name2, const UInt32 &point2, const String &name3, const UInt32 &point3, const String &name4, const UInt32 &point4, const String &name5, const UInt32 &point5, const String &name6, const UInt32 &point6, const String &name7, const UInt32 &point7, const String &name8, const UInt32 &point8, const String &name9, const UInt32 &point9, const String &name10, const UInt32 &point10);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		pointMask	= Parent::NextFreeMask + 0,
		ranklistMask	= Parent::NextFreeMask + 1,
		NextFreeMask	= Parent::NextFreeMask + 2
	};
	virtual RPCResult CB_NextQuestion(LPCPEER pPeer, RPCContext &context, const UInt32 &count, const UInt32 &question_id, const String &description, const String &option1, const String &option2, const String &option3, const String &option4) = 0;
	virtual RPCResult Anwser(LPCPEER pPeer, RPCContext &context, const UInt32 &question_id, const UInt16 &anwserIndex);
	virtual RPCResult CB_AnswerResult(LPCPEER pPeer, RPCContext &context, const Boolean &bRight) = 0;
	virtual RPCResult CB_StartCountDown(LPCPEER pPeer, RPCContext &context, const UInt32 &count) = 0;
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
	virtual void OnUpdatePoint(const UInt32 &point) {};
	virtual void OnUpdateRanklist(const String &name1, const UInt32 &point1, const String &name2, const UInt32 &point2, const String &name3, const UInt32 &point3, const String &name4, const UInt32 &point4, const String &name5, const UInt32 &point5, const String &name6, const UInt32 &point6, const String &name7, const UInt32 &point7, const String &name8, const UInt32 &point8, const String &name9, const UInt32 &point9, const String &name10, const UInt32 &point10) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

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
	INLINE Question_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult Question_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Question_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<2060584342>(pPeer, pBuf);
}

INLINE RPCResult Question_Proxy::Anwser(LPCPEER pPeer, RPCContext &context, const UInt32 &question_id, const UInt16 &anwserIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Question_Anwser);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(question_id);
	pBuf->Write<UInt16>(anwserIndex);
	return RPCSend<482361586>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1031225953)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(2060584342)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1688412626)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(482361586)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(529400276)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(922421266)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_QUESTION_PROXY_H_
