#pragma once
#ifndef _ACCOUNTCONTROL_H_
#define _ACCOUNTCONTROL_H_


#include "QGL.h"
#include "Reuben/Simulation/Object.h"
#include "Proxy/Logic/Account.h"

class CAccountControl : 
	public Reuben::Simulation::Object
{
public:

	CAccount*		m_pAccount;
	NetGroup*		m_characters;
	bool			m_needReLogin;
	UInt			m_iChangeMapLineTimer;

	CAccountControl();
	~CAccountControl();

	Boolean IsSelectCharacterFrozen();

	UInt16 GetSelectedCharacterLevel();	

	Boolean CheckCharacterFrozenStatus(UInt32 index);	

	VOID Login(const String &name, const ByteArray &password);
	RPCResult Login(RPCContext &context);
	RPCResult LoginAccount(RPCContext &context);

	VOID Logout();
	RPCResult Logout(RPCContext &context);

	VOID EnterGame(const Int &Index);
	RPCResult EnterGame(RPCContext &context);

	virtual VOID FrozenCharacter();
	RPCResult FrozenCharacter(RPCContext &context);	

	virtual VOID RecoverCharacter();
	RPCResult RecoverCharacter(RPCContext &context);	

	virtual VOID CheckCharacter();
	RPCResult CheckCharacter(RPCContext &context);

	virtual VOID CreateCharacter();
	RPCResult CreateCharacter(RPCContext &context);

	virtual VOID CheckVersion();
	RPCResult CheckVersion(RPCContext &context);

	virtual VOID DeleteCharacter();
	RPCResult DeleteCharacter(RPCContext &context);

	virtual VOID CheckCharacterName();
	RPCResult CheckCharacterName(RPCContext &context);

	virtual VOID UpdateMapLine();
	RPCResult UpdateMapLine(RPCContext &context);

	VOID ChangeMapLine(UInt8 lineIndex);
	RPCResult ConfirmKick(RPCContext &context);

	DEFINE_PARENT_CLASS(Reuben::Simulation::Object);
	DEFINE_CLASS(0);			// don't care class id
};
#endif
