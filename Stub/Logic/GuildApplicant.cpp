//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/GuildApplicant.h"
#include "Stub/Logic/MessageCharacter.h"

REGISTER_CLASSTYPE(CGuildApplicant, GuildApplicant_CLASSID);


void CGuildApplicant::SetData(CMessageCharacter * pMsgChar)
{
	SetId(pMsgChar->GetChar_id());
	SetLevel(pMsgChar->GetLevel());
	SetName(pMsgChar->GetNickName());
	ClearUpdateMask();
}

void CGuildApplicant::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}