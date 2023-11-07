//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuShopProcess.h"
//-- Library
#include "Global.h"
#include "QGLApplication.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResItem.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/ItemBag.h"
#include "CharacterControl.h"
#include "Common/CallLuaFunc.h"
#include "Math.h"
#include "MenuItemProcess.h"
#include "QGL/CharacterControl.h"
#include "Proxy/Service/MessageService.h"
#include "QGL/PointsShopProcess.h"

#define	BUSINESSSHOP_MAX_ITEM_COUNT	40
#define	BUSINESSSHOP_UPDATE_TIME	(1000 * 60 * 5)

CMenuShopProcess::CMenuShopProcess()
{
	Init();
}

CMenuShopProcess::~CMenuShopProcess()
{
	m_ItemLst.clear();
}

VOID CMenuShopProcess::Init()
{
	m_ItemLst.clear();
	m_BuyDiscount = 100;
	m_SellDiscount = 0;
	m_RepairDiscount = 100;
	m_nShopId = -1;
	m_bSellEnable = BOOLEAN_FALSE;
	m_bRepairEnable = BOOLEAN_FALSE;
	m_StartX = 0;
	m_StartY = 0;
	m_bBusinessSell = BOOLEAN_FALSE;
	m_bBusinessItemMode = BOOLEAN_FALSE;
	m_BusinessMoney = 0;

	ResetParam();
}

VOID CMenuShopProcess::ResetParam()
{
	m_nMode = SHOP_NORMAL;

	m_SellPos = -1;
	m_SellCount = 1;

	m_BuyPos = -1;
	m_BuyCount = 1;
}

UInt32 CMenuShopProcess::GetItemId(const UInt8 SlotIndex)
{
	if (!m_bBusinessItemMode)
	{
		Int size = (Int)m_ItemLst.size();
		if ( SlotIndex < size ) 
			return m_ItemLst[SlotIndex];
	}
	else
	{
		Int size = (Int)m_BusinessItemLst.size();
		if ( SlotIndex < size ) 
			return m_BusinessItemLst[SlotIndex];
	}
	return 0; // failed 
}

VOID CMenuShopProcess::ChangeTitle(UInt8 Title)
{
	if ( !(Title > 0 && Title < 7) ) return;

	UIItem* pTitle = NULL;
	StringA sztitle("");
	for ( Int i = 1; i < 7; ++i )
	{
		sztitle.Format("ShopName%d", i);
		pTitle = gGlobal.pUIManager->GetUIItem(sztitle.c_str());
		if ( pTitle )
			pTitle->SetEnable(false);
	}

	sztitle.Format("ShopName%d", Title);
	pTitle = gGlobal.pUIManager->GetUIItem(sztitle.c_str());
	if ( pTitle )
		pTitle->SetEnable(true);
}

VOID CMenuShopProcess::SetButtonDisable(UInt8 Enable)
{
	UInt8 en = Enable;
	if ( (en & 0x01) != 0x01 )
	{
		UIButton* pButton = gGlobal.pUIManager->FindUIButton("ShopSellItemButton");
		if ( pButton )
			pButton->SetState(_ButtonDisable);

		m_bSellEnable = BOOLEAN_FALSE;
	}
	else
		m_bSellEnable = BOOLEAN_TRUE;

	if ( (en & 0x02) != 0x02 )
	{
		//UIButton* pButton = gGlobal.pUIManager->FindUIButton("ShopRepairItemButton");
		//if ( pButton )
		//	pButton->SetState(_ButtonDisable);

		m_bRepairEnable = BOOLEAN_FALSE;
	}
	else
		m_bRepairEnable = BOOLEAN_TRUE;
}

VOID CMenuShopProcess::SetDiscount(UInt16 buy, UInt16 sell, UInt16 repair)
{
	m_BuyDiscount = buy;
	m_SellDiscount = sell;
	m_RepairDiscount = repair;
}

VOID CMenuShopProcess::SetStartUpPos()
{
	if ( gGlobal.g_pCharacter )
	{
		BlockPos charpos = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();

		m_StartX = charpos.PosX;
		m_StartY = charpos.PosY;
	}
}

VOID CMenuShopProcess::OpenShopMenu(Int32 ShopId, UInt8 Title, UInt8 Enable, UInt16 Buy, UInt16 Sell, UInt16 Repair, UInt16 uiShopType, UInt32Vector Lst)
{
	if ( !(Title > 0 && Title < 7) ) return;

	if (uiShopType == 0)
	{
		// - Open Shop Menu
		UIPanel* panel = gGlobal.pUIManager->FindUIPanel("ShopMenuPanel");
		if ( panel == NULL )
		{
			XML_UI::GetInstance().AddFormFile("Data\\ShopMenu.xml", gGlobal.pUIManager);

			panel = gGlobal.pUIManager->FindUIPanel("ShopMenuPanel");
			if ( !panel ) return;

			m_bBusinessItemMode = BOOLEAN_FALSE;
			// - Set Item Id List
			m_ItemLst.clear();
			m_ItemLst = Lst;

			// - Set Shop ID
			m_nShopId = ShopId;

			// - Set Position
			SetStartUpPos();

			// - Set Discount
			SetDiscount(Buy, Sell, Repair); 

			// - Adjust Shop Menu Position
			panel->SetPos(50, 50);

			// - Change Shop Title
			ChangeTitle(Title);
			// - Set Button Disable
			SetButtonDisable(Enable);

			// - Set Item Icon
			StringA IconName("");
			for ( int i = 0; i < (int)Lst.size(); ++i )
			{
				IconName.Format("SIS%03dIcon", i);
				//UIImage* pim = gGlobal.pUIManager->FindUIImage(IconName.c_str());
				UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(IconName.c_str());
				//if ( pim )
				if (pAniIcon)
				{
					const ItemData* pItemData = gGlobal.m_resource->GetItemData(Lst[i]);
					if( pItemData )
					{
						String temp(pItemData->icon2);
						/*StringA imageFilename;
						imageFilename.Format("Data/%s", temp.c_str());
						pim->SetEnable(true);
						pim->SetImageFileA(imageFilename.c_str());
						pim->SetDetectRect();*/
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(temp.c_str());
						pAniIcon->SetEnable(true);
					}
				}
			}

			gGlobal.pUIManager->AddDirtyUIItem(panel);

			// - Open character item menu
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShopOpenCharItemMenu", String(), 0, 0);
		}

	} 
	else /*if (uiShopType == 1)*/
	{
		gGlobal.g_pPointsShopProcess->SetData(ShopId, uiShopType, Lst);
		gGlobal.g_pCharacter->GetCharNetObject()->BuyPointsShopItem(gGlobal.pPeerProxy, gGlobal.g_context, -6, ShopId, uiShopType);

	}
}

VOID CMenuShopProcess::CloseShopMenu()
{
	Init();

	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("ShopMenuPanel");
	if ( panel )
	{
		panel->SetEnable(false);
		gGlobal.pUIManager->DeletePanel("ShopMenuPanel");

		/*UIImage* pim = gGlobal.pUIManager->FindUIImage("ShopItemDragIcon");
		if ( pim && pim->IsEnable() )
			pim->SetEnable(false);*/
		UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon("ShopItemDragIcon");
		if (pAniIcon && pAniIcon->IsEnable())
			pAniIcon->SetEnable(false);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SICB_AnsNo", String(), 0, 0);
		gGlobal.pUIManager->RenderAll();
	}

	if ( gGlobal.g_CursorState && (gGlobal.g_CursorState->GetStateID() == ST_APP_SHOP_SELL) )
	{
		gGlobal.g_CursorState->Unblock();
		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShopReInitParam", String());
}

VOID CMenuShopProcess::ConfirmAction()
{
	if ( m_nMode == SHOP_SELL )
	{
		if (!m_bBusinessItemMode)
		{
			if ( CanSell() && (m_SellPos != -1) && (m_nShopId != -1) )
			{
				CItem* pItem = gGlobal.m_pBag->GetItem((UInt8)m_SellPos);
				if ( pItem )
				{
					// - Get Sell Count
					{
						UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SICB_EditBox");
						if ( pEditBox )
						{
							if ( pEditBox->GetInt() > (Int)pItem->GetStack() )
							{
								ResetParam();
								String ErrMsg =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_INPUT_ERR"));
								gGlobal.PrintMessage(ErrMsg, CHANNELTYPE_SYSTEM);
								return;
							}
							else
								m_SellCount = pEditBox->GetInt();
						}
					}

					if ( m_SellCount <= (Int)(pItem->GetStack()) )
					{
						gGlobal.m_pBag->SellItemToShop(gGlobal.pPeerProxy, gGlobal.g_context, m_nShopId, m_SellPos, m_SellCount);
					}
				}
			}
		}
		else
		{
			if ( m_bBusinessSell && (m_SellPos != -1) && (m_nShopId != -1) )
			{
				CItem* pItem = gGlobal.m_pBag->GetItem((UInt8)m_SellPos);
				if ( pItem )
				{
					// - Get Sell Count
					{
						UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SICB_EditBox");
						if ( pEditBox )
						{
							if ( pEditBox->GetInt() > (Int)pItem->GetStack() )
							{
								ResetParam();
								String ErrMsg =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_INPUT_ERR"));
								gGlobal.PrintMessage(ErrMsg, CHANNELTYPE_SYSTEM);
								return;
							}
							else
								m_SellCount = pEditBox->GetInt();
						}
					}

					if ( m_SellCount <= (Int)(pItem->GetStack()) )
					{
						gGlobal.m_pBag->SellItemToBusinessShop(gGlobal.pPeerProxy, gGlobal.g_context, m_nShopId, m_SellPos, m_SellCount);
					}
				}
			}
		}
	}
	else if ( m_nMode == SHOP_BUY )
	{
		if ( m_BuyPos != -1 && m_nShopId != -1 )
		{
			if (!m_bBusinessItemMode)
			{
				if ( m_BuyPos < (Int)m_ItemLst.size() )
				{
					Int count = 1;
					UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SICB_EditBox");
					if ( pEditBox ) 
					{
						if ( pEditBox->GetInt() > 0 )
							count = pEditBox->GetInt();
					}

					UInt32 itemId = m_ItemLst[m_BuyPos];
					if ( CanBuy(itemId, count, BOOLEAN_TRUE ) == SHOP_BUY_SUCCESS )
					{
						m_BuyCount = count;
						gGlobal.m_pBag->BuyItemFromShop(gGlobal.pPeerProxy, gGlobal.g_context, itemId, m_nShopId, m_BuyPos, m_BuyCount);
					}
				}
			} 
			else
			{
				if ( m_BuyPos < (Int)m_BusinessItemLst.size() )
				{
					Int count = 1;
					UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SICB_EditBox");
					if ( pEditBox ) 
					{
						if ( pEditBox->GetInt() > 0 )
							count = pEditBox->GetInt();
					}

					UInt32 itemId = m_BusinessShopItemLst[m_BuyPos].item_id;
					if ( CanBuy(itemId, count, BOOLEAN_TRUE ) == SHOP_BUY_SUCCESS )
					{
						m_BuyCount = count;
						gGlobal.m_pBag->BuyItemFromBusinessShop(gGlobal.pPeerProxy, gGlobal.g_context, itemId, m_nShopId, m_BuyCount);
					}
				}
			}
		}
	}

	ResetParam();
	/*
	if ( m_nMode == SHOP_SELL )
	{
		if ( CanSell() )
		{
			if ( m_SellPos != -1 && m_nShopId != -1 )
			{
				CItem* pItem = gGlobal.m_pBag->GetItem((UInt8)m_SellPos);
				if ( pItem )
				{
					if ( m_SellCount <= (Int)(pItem->GetStack()) )
					{
						gGlobal.m_pBag->SellItemToShop(gGlobal.pPeerProxy, gGlobal.g_context, m_nShopId, m_SellPos, m_SellCount);
					}
				}
			}	
		}
	}
	else if ( m_nMode == SHOP_BUY )
	{
		if ( m_BuyPos != -1 && m_nShopId != -1 )
		{
			if ( m_BuyPos < (Int)m_ItemLst.size() )
			{
				UInt32 itemId = m_ItemLst[m_BuyPos];
				if ( CanBuy(itemId, m_BuyCount) == SHOP_BUY_SUCCESS )
				{
					gGlobal.m_pBag->BuyItemFromShop(gGlobal.pPeerProxy, gGlobal.g_context, itemId, m_nShopId, m_BuyPos, m_BuyCount);
				}
			}
		}
	}


	ResetParam();
	*/
}

VOID CMenuShopProcess::DragItem(Int Pos)
{
	UIPanel* dragpanel = gGlobal.pUIManager->FindUIPanel("ShopItemDragPanel");
	if ( dragpanel )
	{
		if (!m_bBusinessItemMode)
		{
			if ( (Pos != -1) && ( Pos < (Int)m_ItemLst.size() ) )
			{
				const ItemData* pItemData = gGlobal.m_resource->GetItemData( m_ItemLst[Pos] );
				if ( pItemData )
				{
					// - Set Item Icon
					//UIImage* pim = gGlobal.pUIManager->FindUIImage("ShopItemDragIcon");
					UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon("ShopItemDragIcon");
					//if ( pim )
					if (pAniIcon)
					{
						String temp(pItemData->icon2);
						/*StringA imageFilename;
						imageFilename.Format("Data/%s", temp.c_str());
						pim->SetEnable(true);
						pim->SetImageFileA(imageFilename.c_str());
						pim->SetDetectRect();*/

						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(temp.c_str());
						pAniIcon->SetEnable(true);

						gGlobal.pUIManager->SetDragItemXY(dragpanel, -20, -20);

						gGlobal.pUIManager->RenderAll();
					}
				}
			}
		} 
		else
		{
			if ( (Pos != -1) && ( Pos < (Int)m_BusinessItemLst.size() ) )
			{
				const ItemData* pItemData = gGlobal.m_resource->GetItemData( m_BusinessShopItemLst[Pos].item_id );
				if ( pItemData )
				{
					// - Set Item Icon
					UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon("ShopItemDragIcon");
					if (pAniIcon)
					{
						String temp(pItemData->icon2);

						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(temp.c_str());
						pAniIcon->SetEnable(true);

						gGlobal.pUIManager->SetDragItemXY(dragpanel, -20, -20);

						gGlobal.pUIManager->RenderAll();
					}
				}
			}
		}
	}
}

Boolean CMenuShopProcess::ResetConfirmBoxUI(UInt32 ItemId)
{
	const ItemData* pItemData = gGlobal.m_resource->GetItemData(ItemId);
	if ( pItemData )
	{
		UIPanel* panel = gGlobal.pUIManager->FindUIPanel("ShopConfirm");
		if ( !panel ) return BOOLEAN_FALSE;

		// - Set Confirm Context
		{
			String Text;
			String tempstr;
			if ( m_nMode == SHOP_BUY )
				tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_BUY_CONFIRM"));
			else if ( m_nMode == SHOP_SELL )
				tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_SELL_CONFIRM"));
			else
				tempstr = _T("");
			
			Text.Format(tempstr.c_str(), pItemData->name);
			gGlobal.SetUIStaticText("ShopConfirmText", Text);
		}

		// - Reset Button State To Normal
		{
			UIButton* pButton = gGlobal.pUIManager->FindUIButton("ShopConfirmYes");
			if ( pButton )
				pButton->SetState(_ButtonNormal);

			pButton = gGlobal.pUIManager->FindUIButton("ShopConfirmNo");
			if ( pButton )
				pButton->SetState(_ButtonNormal);
		}

		gGlobal.pUIManager->SetPanel2Top(panel);
		gGlobal.pUIManager->ClearMonoPanel();
		gGlobal.pUIManager->SetMonoPanel(panel);

		panel->SetPos(280, 250);

		panel->SetEnable(true);
		gGlobal.pUIManager->AddDirtyUIItem(panel);

		return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}

VOID CMenuShopProcess::OpenConfirmBox(Int Pos, Int Mode)
{
	Int tempPos = 0;

	if ( Pos != -1 )
		m_nMode = Mode;

	if ( m_nMode == SHOP_SELL )
	{
		if ( !CanSell() )
		{
			ResetParam();
			return;
		}

		if ( Pos == -1 && m_SellPos == -1 )
			return;

		if ( Pos == -1 )
			tempPos = m_SellPos;
		else
			tempPos = Pos;

		CItem* pItem = gGlobal.m_pBag->GetItem((UInt8)tempPos);
		if ( pItem )
		{
			if ((!m_bBusinessItemMode && ITEMTYPE_BUSINESS_ITEM == pItem->GetItemType())
				|| (m_bBusinessItemMode && m_bBusinessSell && ITEMTYPE_BUSINESS_ITEM != pItem->GetItemType()))
			{
				ResetParam();
				return;
			}
			// - Set Sell Count
			if ( Pos != -1 )
				m_SellCount = 1;
			else
			{
				UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SICB_EditBox");
				if ( pEditBox )
				{
					if ( pEditBox->GetInt() > (Int)pItem->GetStack() )
					{
						String ErrMsg =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_INPUT_ERR"));
						gGlobal.PrintMessage(ErrMsg, CHANNELTYPE_SYSTEM);
						return;
					}
					else
						m_SellCount = pEditBox->GetInt();
				}		
			}
			// - Reset Confirm Box
			if ( ResetConfirmBoxUI( pItem->GetItem_id() ) )
			{
				if ( Pos != -1 )
				{
					m_SellPos = Pos;
					m_SellCount = 1;
				}
			}
		}
	}
	else if ( m_nMode == SHOP_BUY )
	{
		if ( Pos == -1 && m_BuyPos == -1 ) return;

		if ( Pos == -1 )
			tempPos = m_BuyPos;
		else
			tempPos = Pos;

		if ( tempPos < (Int)m_ItemLst.size() )
		{
			// - Check Input Count Valid ?
			UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SICB_EditBox");
			if ( !pEditBox ) return;

			UInt32 tempId = m_ItemLst[tempPos];

			{
				const ItemData* pItemData = gGlobal.m_resource->GetItemData(tempId);
				if ( pItemData )
				{
					if ( (pEditBox->GetInt()) > pItemData->overlap )
					{
						String ErrMsg =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_INPUT_ERR"));
						gGlobal.PrintMessage(ErrMsg, CHANNELTYPE_SYSTEM);

						gGlobal.SetUIStaticText("ShopConfirmText", _T("1"));
						return;
					}
				}
			}


			if ( CanBuy(tempId, pEditBox->GetInt(), BOOLEAN_TRUE ) == SHOP_BUY_SUCCESS )
			{
				m_BuyCount = pEditBox->GetInt();
				// - Reset Confirm Box
				if ( ResetConfirmBoxUI( tempId ) )
				{
					if ( Pos != -1 )
					{
						m_BuyPos = Pos;
						m_BuyCount = 1;
					}
				}
			}	
		}
	}		
}

Boolean CMenuShopProcess::ResetCountBoxUI(UInt32 ItemId)
{
	const ItemData* pItemData = gGlobal.m_resource->GetItemData(ItemId);
	if ( pItemData )
	{
		UIPanel* panel = gGlobal.pUIManager->FindUIPanel("ShopItemCountBox");
		if ( !panel ) return BOOLEAN_FALSE;

		// -Set Item Target Name
		{
			String Text;
			String tempstr;
			if ( m_nMode == SHOP_BUY )
				tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_BUY_INPUT"));
			else if ( m_nMode == SHOP_SELL )
				tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_SELL_INPUT"));
			else
				tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_INPUT_COUNT"));

			Text.Format(tempstr.c_str(), pItemData->name);
			gGlobal.SetUIStaticText("SICB_Text", Text);
		}

		// - Set Cost of Item 
		{
			UInt32 cost = 0;
			if (!m_bBusinessItemMode)
			{
				if ( m_nMode == SHOP_BUY )
					cost = (pItemData->g_price) * ( m_BuyDiscount / 100.0 );
				else if ( m_nMode == SHOP_SELL )
					cost = (pItemData->g_price) * ( m_SellDiscount / 100.0 );
			} 
			else
			{
				if ( m_nMode == SHOP_BUY )
					cost = GetBuyPrice(ItemId);
				else if ( m_nMode == SHOP_SELL )
					cost = GetSellPrice(ItemId);
			}
			SetPrice(cost);
		}

		// - Reset Button State
		{
			PCSTRINGA strlist[] = { "SICB_YesButton", "SICB_NoButton", "SICB_MaxButton", "SICB_MinButton", "SICB_AddButton", "SICB_SubButton" };
			Int size = sizeof(strlist) / sizeof(PCSTRINGA);
			for ( Int i = 0; i < size; ++i )
			{
				UIButton* pButton = gGlobal.pUIManager->FindUIButton(strlist[i]);
				if ( pButton )
					pButton->SetState(_ButtonNormal);
			}
		}

		// - Set Edit Box Key In Focus
		gGlobal.pUIManager->SetKeyInItem("SICB_EditBox");

		// - Reset Edit Box Number
		UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SICB_EditBox");
		if ( pEditBox )
		{
			pEditBox->SetText("1");
			pEditBox->Redraw();
			pEditBox->End();
		}

		gGlobal.pUIManager->SetPanel2Top(panel);
		gGlobal.pUIManager->ClearMonoPanel();
		gGlobal.pUIManager->SetMonoPanel(panel);

		panel->SetPos(280, 250);

		panel->SetEnable(true);
		gGlobal.pUIManager->AddDirtyUIItem(panel);

		return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}

VOID CMenuShopProcess::OpenAdjustCountBox(bool bSell, Int nPos)
{
	if ( gGlobal.g_pCharacter )
		gGlobal.g_pCharacter->Stop();

	if ( bSell )
		m_nMode = SHOP_SELL;
	else
		m_nMode = SHOP_BUY;


	if ( m_nMode == SHOP_SELL )
	{
		if (!m_bBusinessItemMode)
		{
			if ( CanSell() ) 
			{
				CItem* pItem = gGlobal.m_pBag->GetItem((UInt8)nPos);
				if ( pItem && ITEMTYPE_BUSINESS_ITEM != pItem->GetItemType() )
				{
					if ( ResetCountBoxUI( pItem->GetItem_id() ) )
					{
						// - Set Sell Item Bag Position
						m_SellPos = nPos;
						m_SellCount = 1;
						return;
					}
				}
			}
		} 
		else
		{
			if (m_bBusinessSell)
			{
				CItem* pItem = gGlobal.m_pBag->GetItem((UInt8)nPos);
				if ( pItem && ITEMTYPE_BUSINESS_ITEM == pItem->GetItemType())
				{
					if ( ResetCountBoxUI( pItem->GetItem_id() ) )
					{
						// - Set Sell Item Bag Position
						m_SellPos = nPos;
						m_SellCount = 1;
						return;
					}
				}
			}
		}
	}
	else if ( m_nMode == SHOP_BUY ) 
	{// buy
		if (!m_bBusinessItemMode)
		{
			Int size = (Int)m_ItemLst.size();
			if ( nPos < size )
			{
				UInt32 tempId = m_ItemLst[nPos];

				// - Check moeny to buy at least one item or not?
				if ( CanBuy(tempId, 1, BOOLEAN_TRUE) == SHOP_BUY_SUCCESS )
				{
					if ( ResetCountBoxUI( tempId ) )
					{
						// - Set Buy Item Shop Pos
						m_BuyPos = nPos;
						m_BuyCount = 1;
						return;
					}
				}	
			}
		}
		else
		{
			Int size = (Int)m_BusinessItemLst.size();
			if ( nPos < size )
			{
				if (m_BusinessShopItemLst[nPos].count > 0)
				{
					UInt32 tempId = m_BusinessShopItemLst[nPos].item_id;

					// - Check moeny to buy at least one item or not?
					if ( CanBuy(tempId, 1, BOOLEAN_TRUE) == SHOP_BUY_SUCCESS )
					{
						if ( ResetCountBoxUI( tempId ) )
						{
							// - Set Buy Item Shop Pos
							m_BuyPos = nPos;
							m_BuyCount = 1;
							return;
						}
					}
				}	
			}
		}
	}	

	ResetParam();
}

VOID CMenuShopProcess::SetMaxNum(bool bMax)
{
	UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SICB_EditBox");
	if ( !pEditBox ) return;

	if ( m_nMode == SHOP_SELL )
	{
		if ( m_SellPos > -1 )
		{
			CItem* pItem = gGlobal.m_pBag->GetItem((UInt8)m_SellPos);
			if ( pItem )
			{
				Int num = 0;
				if ( bMax )
					num = (Int)(pItem->GetStack());
				else
					num = 1;

				// - Set Edit Box
				SetEditNum(num);
				// - Set Price
				UInt32 price(0); 
				if (!m_bBusinessItemMode)
				{
					price = (UInt32)((pItem->m_data->g_price) * ( m_SellDiscount / 100.0 )) * num; 
				} 
				else
				{
					price = GetSellPrice(pItem->GetItem_id()) * num;
				}
				SetPrice(price);
			}
		}
	}
	else if ( m_nMode == SHOP_BUY )
	{
		if (!m_bBusinessItemMode)
		{
			Int size = (Int)m_ItemLst.size();
			if ( m_BuyPos < size && m_BuyPos != - 1 )
			{
				const ItemData* pItemData = gGlobal.m_resource->GetItemData( m_ItemLst[m_BuyPos] );
				if ( pItemData && pItemData->g_price > 0 )
				{
					CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
					if ( pchar )
					{
						UInt32 cost = pItemData->g_price * ( m_BuyDiscount / 100.0 );
						UInt32 max = pchar->GetMoney() / cost;
						Int freeslot = GetTotalFreeSlot();

						Int totalUseSlot = max / (pItemData->overlap);
						if ( (max % (pItemData->overlap)) > 0 )
							totalUseSlot += 1;

						if ( totalUseSlot > freeslot )
						{
							max = freeslot * (pItemData->overlap);
						}

						Int num = 0;
						if ( bMax )
							num = max;
						else
							num = 1;

						// - Set Edit Box
						SetEditNum(num);

						// - Set Price
						UInt32 price = cost * num; 
						SetPrice(price);
					}
				}
			}
		} 
		else
		{
			Int size = (Int)m_BusinessItemLst.size();
			if ( m_BuyPos < size && m_BuyPos != - 1 )
			{
				const ItemData* pItemData = gGlobal.m_resource->GetItemData( m_BusinessShopItemLst[m_BuyPos].item_id );
				if ( pItemData && m_BusinessShopItemLst[m_BuyPos].buyPrice > 0 )
				{
					CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
					if ( pchar )
					{
						UInt32 cost = m_BusinessShopItemLst[m_BuyPos].buyPrice;
						UInt32 max = m_BusinessMoney / cost;
						if (gGlobal.m_pBag)
						{
							Int32 maxBuy = BUSINESSSHOP_MAX_ITEM_COUNT - gGlobal.m_pBag->GetBusinessItemCount();
							if (maxBuy <= 0)
							{
								return;
							}
							max = MIN(max,maxBuy);
							max = MIN(max,m_BusinessShopItemLst[m_BuyPos].count);
						}
						Int freeslot = GetTotalFreeSlot();

						Int totalUseSlot = max / (pItemData->overlap);
						if ( (max % (pItemData->overlap)) > 0 )
							totalUseSlot += 1;

						if ( totalUseSlot > freeslot )
						{
							max = freeslot * (pItemData->overlap);
						}

						Int num = 0;
						if ( bMax )
							num = max;
						else
							num = 1;

						// - Set Edit Box
						SetEditNum(num);

						// - Set Price
						UInt32 price = cost * num; 
						SetPrice(price);
					}
				}
			}
		}
	}
}

VOID CMenuShopProcess::IncCount(bool bInc)
{
	UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SICB_EditBox");
	if ( !pEditBox ) return;

	if ( m_nMode == SHOP_SELL )
	{
		if ( m_SellPos > -1 )
		{
			CItem* pItem = gGlobal.m_pBag->GetItem((UInt8)m_SellPos);
			if ( pItem )
			{
				Int max = pItem->GetStack();
				Int count = pEditBox->GetInt();

				if ( bInc )
				{
					if ( count < max )
						++count;
				}
				else
				{
					if ( count > 1 )
						--count;
				}

				SetEditNum(count);

				// - Set Price
				UInt32 price(0);
				if (!m_bBusinessItemMode)
				{
					price = (UInt32)((pItem->m_data->g_price) * ( m_SellDiscount / 100.0 )) * count; 
				} 
				else
				{
					price = GetSellPrice(pItem->GetItem_id()) * count;
				}
				SetPrice(price);
			}
		}
	}
	else if ( m_nMode == SHOP_BUY )
	{
		if (!m_bBusinessItemMode)
		{
			Int size = (Int)m_ItemLst.size();
			if ( m_BuyPos < size && m_BuyPos != - 1 )
			{
				Int next = pEditBox->GetInt();

				if ( bInc )
					++next;
				else
				{
					if ( next > 1 )
						--next;
				}
				if ( CanBuy(m_ItemLst[m_BuyPos], next) == SHOP_BUY_SUCCESS )
				{
					SetEditNum(next);

					const ItemData* pItemData = gGlobal.m_resource->GetItemData( m_ItemLst[m_BuyPos] );
					if ( pItemData ) 
					{
						UInt32 price = (UInt32)((pItemData->g_price) * ( m_BuyDiscount / 100.0 )) * next; 
						SetPrice(price);
					}
				}
			}
		} 
		else
		{
			Int size = (Int)m_BusinessItemLst.size();
			if ( m_BuyPos < size && m_BuyPos != - 1 )
			{
				Int next = pEditBox->GetInt();

				if ( bInc )
					++next;
				else
				{
					if ( next > 1 )
						--next;
				}
				if ( CanBuy(m_BusinessShopItemLst[m_BuyPos].item_id, next) == SHOP_BUY_SUCCESS )
				{
					SetEditNum(next);

					//const ItemData* pItemData = gGlobal.m_resource->GetItemData( m_BusinessShopItemLst[m_BuyPos] );
					//if ( pItemData ) 
					{
						UInt32 price = m_BusinessShopItemLst[m_BuyPos].buyPrice * next; 
						SetPrice(price);
					}
				}
			}
		}
	}
}

VOID CMenuShopProcess::SetPrice(UInt32 price)
{
	String Text;
	String tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_TOTAL_COST"));
	Text.Format(tempstr.c_str(), price);
	gGlobal.SetUIStaticText("SICB_Price", Text, TRUE);
}

VOID CMenuShopProcess::SetEditNum(Int Num, Boolean bUpdate)
{
	UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SICB_EditBox");
	if ( pEditBox )
	{
		StringA Text("");
		Text.Format("%d", Num);
		Int Pos = pEditBox->GetInsertPos();
		pEditBox->SetText(Text.c_str());
		pEditBox->Home();
		pEditBox->CursorMoveRight(Pos);
		pEditBox->Redraw();

		if ( bUpdate )
			gGlobal.pUIManager->AddDirtyUIItem(gGlobal.pUIManager->GetUIItem("SICB_EditBox"));
	}
}

Boolean CMenuShopProcess::HaveEnoughMoney(UInt32 eachPrice, Int nCount)
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if ( pchar )
	{
		if (m_bBusinessItemMode)
		{
			UInt32 price = eachPrice * nCount;
			if ( price <= m_BusinessMoney )
				return BOOLEAN_TRUE;
		}
		else
		{
			UInt32 price = eachPrice * nCount * ( m_BuyDiscount / 100.0 );
			if ( price <= pchar->GetMoney() )
				return BOOLEAN_TRUE;
		}
	}
	return BOOLEAN_FALSE;
}

Boolean CMenuShopProcess::IsEnoughFreeSlot(Int nCount, Int nMaxOverlap)
{
	if ( gGlobal.m_pBag == NULL )
		return BOOLEAN_FALSE;

	Int totalFree = 0;
	for ( UInt8 i = BAG_CURRENT; i < BAG_TYPE_COUNT; ++i )
	{
		if ( gGlobal.m_pBag->IsBagEnabled(i) )
		{
			UInt8 free = (BAG_ITEM_COUNT) - (gGlobal.m_pBag->GetCount(i));
			totalFree += free;
		}
	}

	Int totalUse = nCount / nMaxOverlap;
	if ( ( nCount % nMaxOverlap ) > 0 )
		totalUse += 1;

	if ( totalUse <= totalFree )
		return BOOLEAN_TRUE;

	return BOOLEAN_FALSE;
}

Int CMenuShopProcess::GetTotalFreeSlot()
{
	Int totalFree = 0;
	for ( UInt8 i = BAG_CURRENT; i < BAG_TYPE_COUNT; ++i )
	{
		if ( gGlobal.m_pBag->IsBagEnabled(i) )
		{
			UInt8 free = (BAG_ITEM_COUNT) - (gGlobal.m_pBag->GetCount(i));
			totalFree += free;
		}
	}
	return totalFree;
}

Int CMenuShopProcess::CanBuy(UInt32 ItemId, Int nCount, Boolean bShowErrMsg)
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if ( pchar )
	{
		const ItemData* pItemData = gGlobal.m_resource->GetItemData( ItemId );
		if ( pItemData && (pItemData->g_price > 0 || m_bBusinessItemMode) )
		{
			// - Check Game Money
			UInt32 price = GetBuyPrice(ItemId);
			if (price == 0)
			{
				price = pItemData->g_price;
			}
			else
			{
				UInt32 max = m_BusinessMoney / price;
				if (gGlobal.m_pBag)
				{
					Int32 maxBuy = BUSINESSSHOP_MAX_ITEM_COUNT - gGlobal.m_pBag->GetBusinessItemCount();
					if (maxBuy <= 0)
					{
						return SHOP_BUY_FAILED;
					}
					max = MIN(max,maxBuy);
				}
				if (nCount > max)
				{
					return SHOP_BUY_FAILED;
				}
			}
			if ( HaveEnoughMoney(price, nCount) == BOOLEAN_FALSE )
			{
				if ( bShowErrMsg )
				{
					String ErrMsg =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_NOT_ENOUGH_MONEY"));
					gGlobal.PrintPlayerMessage(ErrMsg);
					gGlobal.PrintMessage(ErrMsg, CHANNELTYPE_SYSTEM);
				}
				return SHOP_BUY_NOMONEY;
			}

			// - Check Free Space
			if ( IsEnoughFreeSlot(nCount, (Int)(pItemData->overlap) ) == BOOLEAN_FALSE )
			{
				if ( bShowErrMsg )
				{
					String ErrMsg =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_NO_SPACE"));
					gGlobal.PrintPlayerMessage(ErrMsg);
					gGlobal.PrintMessage(ErrMsg, CHANNELTYPE_SYSTEM);
				}
				return SHOP_BUY_NOSPACE;
			}

			return SHOP_BUY_SUCCESS;
		}
		else
			return SHOP_BUY_NOITEM;
	}
	return SHOP_BUY_FAILED;
}

VOID CMenuShopProcess::OnClickSellButton()
{
	if ( gGlobal.g_CursorState && (gGlobal.g_CursorState->GetStateID() != ST_APP_SHOP_SELL) )
	{
		if ( gGlobal.g_CursorState->IsBlocked() )
			gGlobal.g_CursorState->Unblock();

		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CShopSellState));
		gGlobal.g_CursorState->Block();
	}

	// - Open character item menu
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShopOpenCharItemMenu", String(), 0, 0);
}

VOID CMenuShopProcess::OnLButtonUpForSell()
{
	UIPanel* itempanel = gGlobal.pUIManager->FindUIPanel("ItemMenuPanel");
	if ( itempanel && itempanel->IsEnable() )
	{
		UIItem* uim = gGlobal.pUIManager->GetUIItem("ItemSlot000Icon");
		if ( uim )
		{
			Int32 leftupX = uim->GetX();
			Int32 leftupY = uim->GetY();

			Int32 mX = gGlobal.pUIManager->GetCursorX();
			Int32 mY = gGlobal.pUIManager->GetCursorY();

			Int32 diffX = mX - leftupX;
			if ( diffX < 1 )
				diffX = 0;
			Int32 diffY = mY - leftupY;
			if ( diffY < 1 )
				diffY = 0;
			if ( ( (diffX > 0) && (diffX < (41 * 6)) ) && 
				 ( (diffY > 0 ) && (diffY < (41 * 4)) ) )
			{
				Int currPage = 0;
				if ( gGlobal.g_pItemProcess )
					currPage = gGlobal.g_pItemProcess->GetBagPage();

				Int slotIndex = (currPage * 24) + ((diffY / 41) * 6) + ( diffX / 41 );
				if ( slotIndex >= 0 )
				{
					CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ItemDetailMouseOut", String());
					OpenAdjustCountBox(true, slotIndex);
				}
			}
		}
	}
}

VOID CMenuShopProcess::OnRButtonUpForCancelSell()
{
	if ( gGlobal.g_CursorState && (gGlobal.g_CursorState->GetStateID() == ST_APP_SHOP_SELL) )
	{
		UIPanel* panel = gGlobal.pUIManager->FindUIPanel("ShopItemCountBox");
		if ( panel && panel->IsEnable() == false ) 
		{
			ResetParam();
			gGlobal.g_CursorState->Unblock();
			State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
		}
	}
}

VOID CMenuShopProcess::OnDisappear()
{
	UIPanel* pShop = gGlobal.pUIManager->FindUIPanel("ShopMenuPanel");
	if ( gGlobal.g_pCharacter && pShop && pShop->IsEnable() )
	{
		BlockPos currPos = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();
		BlockPos center;
		center.PosX = m_StartX;
		center.PosY = m_StartY;

		PixelDirection dummy;

		if (QGLMath::CalculateBlockPosDistance(currPos, center, dummy) > 300)
		{
			CloseShopMenu();
		}
	}
}


VOID CMenuShopProcess::UpdatePrice()
{
	UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SICB_EditBox");

	if ( pEditBox )
	{
		if ( m_nMode == SHOP_SELL )
		{
			CItem* pItem = gGlobal.m_pBag->GetItem((UInt8)m_SellPos);
			if ( pItem )
			{
				SetPrice((UInt32)((pItem->m_data->g_price) * ( m_SellDiscount / 100.0 )) * pEditBox->GetInt());
			}
		}
		else
		{
			const ItemData* pItemData = gGlobal.m_resource->GetItemData( m_ItemLst[m_BuyPos] );
			if ( pItemData ) 
			{
				SetPrice((UInt32)((pItemData->g_price) * ( m_BuyDiscount / 100.0 )) * pEditBox->GetInt());
			}
		}
	}
}


VOID CMenuShopProcess::OnEditBoxKeyUp(Int keyCode)
{
	UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SICB_EditBox");
	if ( pEditBox )
	{
		if ( pEditBox->GetInt() == 0 )
		{
			SetPrice(0);
			return;
		}

		if ( m_nMode == SHOP_SELL )
		{
			if ( m_SellPos > -1 )
			{
				CItem* pItem = gGlobal.m_pBag->GetItem((UInt8)m_SellPos);
				if ( pItem )
				{
					Int count = pEditBox->GetInt();
					Int OldCount = count;
					if ( count > pItem->GetStack() )
					{
						count = pItem->GetStack();
						PCSTRINGW	pText = pEditBox->GetText();
						if(count != OldCount || (count && pText[0] == _T('0')))
							SetEditNum(count);
					}
					else if ( count < 1 )
					{
						count = 1;
						SetEditNum(count);
						pEditBox->End();
					}

					UInt32 price(0);
					if (!m_bBusinessItemMode)
					{
						price = (UInt32)((pItem->m_data->g_price) * ( m_SellDiscount / 100.0 )) * count;
					} 
					else
					{
						price = GetSellPrice(pItem->GetItem_id()) * count;
					}
					SetPrice(price);
				}
			}
		}
		else if ( m_nMode == SHOP_BUY )
		{
			if (!m_bBusinessItemMode)
			{
				Int size = (Int)m_ItemLst.size();
				if ( m_BuyPos < size && m_BuyPos != - 1 )
				{
					const ItemData* pItemData = gGlobal.m_resource->GetItemData( m_ItemLst[m_BuyPos] );
					if ( pItemData ) 
					{
						Int count = pEditBox->GetInt();
						UInt32 eachCost = ( pItemData->g_price ) * ( m_BuyDiscount / 100.0 );
						CCharacter* pchar = gGlobal.g_pCharacter->GetCharNetObject();
						if ( pchar )
						{
							Int max = (pchar->GetMoney()) / eachCost;
							if ( count > max )
								count = max;

							Int freeslot = GetTotalFreeSlot();
							Int totaluse = count / (pItemData->overlap);
							if ( (count % (pItemData->overlap)) > 0 )
								totaluse += 1;
							if ( totaluse > freeslot )
							{
								count = freeslot * (pItemData->overlap);
							}

							SetEditNum(count);
						}
						else
							SetEditNum(0);

						UInt32 cost =  (UInt32)(( pItemData->g_price ) * ( m_BuyDiscount / 100.0 )) * count;
						SetPrice(cost);
					}
				}
			} 
			else
			{
				Int size = (Int)m_BusinessItemLst.size();
				if ( m_BuyPos < size && m_BuyPos != - 1 )
				{
					const ItemData* pItemData = gGlobal.m_resource->GetItemData( m_BusinessShopItemLst[m_BuyPos].item_id );
					if ( pItemData ) 
					{
						Int count = pEditBox->GetInt();
						UInt32 eachCost = m_BusinessShopItemLst[m_BuyPos].buyPrice;
						CCharacter* pchar = gGlobal.g_pCharacter->GetCharNetObject();
						if ( pchar )
						{
							Int max = m_BusinessMoney / eachCost;
							if (gGlobal.m_pBag)
							{
								Int32 maxBuy = BUSINESSSHOP_MAX_ITEM_COUNT - gGlobal.m_pBag->GetBusinessItemCount();
								if (maxBuy <= 0)
								{
									return;
								}
								max = MIN(max,maxBuy);
								max = MIN(max,m_BusinessShopItemLst[m_BuyPos].count);
							}
							if ( count > max )
								count = max;

							Int freeslot = GetTotalFreeSlot();
							Int totaluse = count / (pItemData->overlap);
							if ( (count % (pItemData->overlap)) > 0 )
								totaluse += 1;
							if ( totaluse > freeslot )
							{
								count = freeslot * (pItemData->overlap);
							}

							SetEditNum(count);
						}
						else
							SetEditNum(0);

						UInt32 cost =  eachCost * count;
						SetPrice(cost);
					}
				}
			}
		}	
	}
}

Boolean CMenuShopProcess::OnSellClickInValid()
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("ShopItemCountBox");
	if ( panel && panel->IsEnable() ) 
		return BOOLEAN_TRUE;
	
	{
		PCSTRINGA strlist[] = { "ItemCurrentBag", "ItemSideBag", "ItemBagExtra01", "ItemBagExtra02", "ItemBagExtra03" };
		Int size = sizeof(strlist) / sizeof(PCSTRINGA);
		for ( Int i = 0 ; i < size; ++i )
		{
			UIItem* uim = gGlobal.pUIManager->GetUIItem(strlist[i]);
			if ( uim )
			{
				if ( uim->Detect(gGlobal.pUIManager->GetCursorX(), gGlobal.pUIManager->GetCursorY() ) )
					return BOOLEAN_TRUE;
			}
		}
	}
	return BOOLEAN_FALSE;
}

VOID CMenuShopProcess::OpenBusinessShopMenu(Boolean sell, UInt16 shop_id,	UInt8 shopName, 
											const UInt32 &SynTime, const UInt32 &BusinessMoney,
											const BusinessShopItemInfoVector &ItemInfoList)
{
	if ( !(shopName > 0 && shopName < 7) ) return;

	// - Open Shop Menu
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("ShopMenuPanel");
	if ( panel == NULL )
	{
		XML_UI::GetInstance().AddFormFile("Data\\ShopMenu.xml", gGlobal.pUIManager);

		panel = gGlobal.pUIManager->FindUIPanel("ShopMenuPanel");
		if ( !panel ) return;

		m_bBusinessItemMode = BOOLEAN_TRUE;
		m_bBusinessSell = sell;
		m_BusinessMoney = BusinessMoney;
		m_NextBusinessItemShopSynTime = ::timeGetTime() + SynTime;
		// - Set Item Id List
		m_ItemLst.clear();
		m_BusinessItemLst.clear();
		m_BusinessShopItemLst.clear();
		UInt16Vector	LstCount;
		for (size_t i=0; i<ItemInfoList.size(); i++)
		{
			if (ItemInfoList[i].buyPrice > 0)
			{
				m_BusinessShopItemLst.push_back(ItemInfoList[i]);
				m_BusinessItemLst.push_back(ItemInfoList[i].item_id);
				LstCount.push_back(ItemInfoList[i].count);
			}
		}
		for (size_t i=0; i<ItemInfoList.size(); i++)
		{
			if (ItemInfoList[i].buyPrice == 0)
			{
				m_BusinessShopItemLst.push_back(ItemInfoList[i]);
			}
		}

		// - Set Shop ID
		m_nShopId = shop_id;

		// - Set Position
		SetStartUpPos();

		// - Set Discount
		SetDiscount(100, 0, 100); 

		// - Adjust Shop Menu Position
		panel->SetPos(50, 50);

		// - Change Shop Title
		ChangeTitle(shopName);
		// - Set Button Disable
		SetButtonDisable(sell);

		// - Set Item Icon
		StringA IconName("");
		for ( int i = 0; i < (int)m_BusinessItemLst.size(); ++i )
		{
			IconName.Format("SIS%03dIcon", i);
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(IconName.c_str());
			if (pAniIcon)
			{
				const ItemData* pItemData = gGlobal.m_resource->GetItemData(m_BusinessItemLst[i]);
				if( pItemData )
				{
					String temp(pItemData->icon2);
					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(temp.c_str());
					pAniIcon->SetEnable(true);
				}
			}
			IconName.Format("SIS%03dCount", i);
			UIStaticText* pStaticText = gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String strNum;
				strNum.Format(_T("%d"),LstCount[i]);
				pStaticText->SetText(strNum);
				pStaticText->Redraw();
				pStaticText->SetEnable(true);
			}
		}
		UIStaticText* pStaticText = gGlobal.pUIManager->FindUIStaticText("BusinessMoneyText");
		if (pStaticText)
		{
			String strText;
			strText.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUSINESSSHOP_TOTAL_MONEY")),m_BusinessMoney);
			pStaticText->SetText(strText);
			pStaticText->Redraw();
			pStaticText->SetEnable(true);
		}

		gGlobal.pUIManager->AddDirtyUIItem(panel);

		// - Open character item menu
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShopOpenCharItemMenu", String(), 0, 0);
	}
}

VOID CMenuShopProcess::UpdateBusinessShopMenu(const BusinessShopItemInfoVector &ItemInfoList)
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("ShopMenuPanel");
	if (panel)
	{
		m_bBusinessItemMode = BOOLEAN_TRUE;
		// - Set Item Id List
		m_ItemLst.clear();
		m_BusinessItemLst.clear();
		m_BusinessShopItemLst.clear();
		UInt16Vector	LstCount;
		for (size_t i=0; i<ItemInfoList.size(); i++)
		{
			if (ItemInfoList[i].buyPrice > 0)
			{
				m_BusinessShopItemLst.push_back(ItemInfoList[i]);
				m_BusinessItemLst.push_back(ItemInfoList[i].item_id);
				LstCount.push_back(ItemInfoList[i].count);
			}
		}
		for (size_t i=0; i<ItemInfoList.size(); i++)
		{
			if (ItemInfoList[i].buyPrice == 0)
			{
				m_BusinessShopItemLst.push_back(ItemInfoList[i]);
			}
		}

		// - Set Item Icon
		StringA IconName("");
		for ( int i = 0; i < (int)m_BusinessItemLst.size(); ++i )
		{
			IconName.Format("SIS%03dIcon", i);
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(IconName.c_str());
			if (pAniIcon)
			{
				const ItemData* pItemData = gGlobal.m_resource->GetItemData(m_BusinessItemLst[i]);
				if( pItemData )
				{
					String temp(pItemData->icon2);
					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(temp.c_str());
					pAniIcon->SetEnable(true);
				}
			}
			IconName.Format("SIS%03dCount", i);
			UIStaticText* pStaticText = gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String strNum;
				strNum.Format(_T("%d"),LstCount[i]);
				pStaticText->SetText(strNum);
				pStaticText->Redraw();
				pStaticText->SetEnable(true);
			}
		}
		UIStaticText* pStaticText = gGlobal.pUIManager->FindUIStaticText("BusinessMoneyText");
		if (pStaticText)
		{
			String strText;
			strText.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUSINESSSHOP_TOTAL_MONEY")),m_BusinessMoney);
			pStaticText->SetText(strText);
			pStaticText->Redraw();
			pStaticText->SetEnable(true);
		}

		gGlobal.pUIManager->AddDirtyUIItem(panel);
	}
}

UInt32 CMenuShopProcess::GetBuyPrice(UInt32 item_id)
{
	if (m_bBusinessItemMode)
	{
		for (size_t i=0; i<m_BusinessShopItemLst.size(); i++)
		{
			if (item_id == m_BusinessShopItemLst[i].item_id)
			{
				return m_BusinessShopItemLst[i].buyPrice;
			}
		}
	}
	return 0;
}

UInt32 CMenuShopProcess::GetSellPrice(UInt32 item_id)
{
	if (m_bBusinessItemMode)
	{
		for (size_t i=0; i<m_BusinessShopItemLst.size(); i++)
		{
			if (item_id == m_BusinessShopItemLst[i].item_id)
			{
				return m_BusinessShopItemLst[i].sellPrice;
			}
		}
	}
	return 0;
}

void CMenuShopProcess::UpdateBusinessShopItem(UInt32 CurrentTime)
{
	if (m_bBusinessItemMode && CurrentTime > m_NextBusinessItemShopSynTime)
	{
		if (gGlobal.pPeerProxy)
		{
			MessageService_Proxy::UpdateBusinessShopList(gGlobal.pPeerProxy, gGlobal.g_context,m_nShopId);
			m_NextBusinessItemShopSynTime += BUSINESSSHOP_UPDATE_TIME;
		}
		else
			m_bBusinessItemMode = false;
	}
}

void CMenuShopProcess::ChangeBusinessShopMoney(Int32 nChange)
{
	m_BusinessMoney += nChange;
	if (m_bBusinessItemMode && gGlobal.pPeerProxy)
	{
		MessageService_Proxy::UpdateBusinessShopList(gGlobal.pPeerProxy, gGlobal.g_context,m_nShopId);
	}
}