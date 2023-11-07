//-- Common
#include "Common.h"
//-- Self
#include "SimClient/SellYuanBao.h"

REGISTER_CLASSTYPE(CSellYuanBao, SellYuanBao_CLASSID);

UInt CSellYuanBao::GetRemain()
{
	return GetTotalYBT() - GetSoldYBT();
}

void CSellYuanBao::OnUpdateSoldYBT(const UInt32 &soldYBT)
{
	
}