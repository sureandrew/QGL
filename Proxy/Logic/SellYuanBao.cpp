//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/SellYuanBao.h"
#include "QGL/MenuSellYuanBaoProcess.h"
#include "QGL/Global.h"

REGISTER_CLASSTYPE(CSellYuanBao, SellYuanBao_CLASSID);

UInt CSellYuanBao::GetRemain()
{
	return GetTotalYBT() - GetSoldYBT();
}

void CSellYuanBao::OnUpdateSoldYBT(const UInt32 &soldYBT)
{
	if (gGlobal.g_pMenuSellYBProcess )
		gGlobal.g_pMenuSellYBProcess->UpdateSellYBRecord();
}