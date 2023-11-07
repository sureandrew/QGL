//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MiddleMap.h"
//-- Library
#include "QGL/Global.h"
#include "Proxy/Logic/Party.h"
#include "Resource/ClientResManager.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "QGL/QGLApplication.h"
//#include "MapNPCControl.h"
#include "Configure.h"
#include "QGL/MapControl.h"
#include "QGL/CharacterControl.h"
#include "Math.h"
#include "Common/PathFinder.h"
#include <algorithm>
#include <functional>    
#include "QGL/MenuSearchListProcess.h"
#include "QGL/PathMove.h"
#include "Proxy/Logic/PartyCharInfoClient.h"


typedef StlDeque<BlockPos, ManagedAllocator<BlockPos> > PointList;

CMiddleMap::CMiddleMap()
{
	m_dwRatioX = 0;
	m_dwRatioY = 0;
	m_bLoadMap = false;
	m_bChangeSize = false;
	m_dX = -21;
	m_iDesBlockX = -1;
	m_iDesBlockY = -1;
	m_iCharDesBlockX = -1;
	m_iCharDesBlockY = -1;
	m_iMapID = 0;
	m_pPathIm = NULL;

}

void CMiddleMap::ShowMidMap()
{
	if (!gConfigure.uNetwork)
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseMiddleMap", String());
		return;
	}
	
	gGlobal.g_pSearchListProcess->MakeNPCSearchList();

	String middleMapName(gGlobal.m_mapdb->m_basicinfo->m_mapmapfile);
	middleMapName = middleMapName.substr(0, middleMapName.length() - 4);
	
	middleMapName.append(_T("_middle.pic"));
	
	String strTempDir = gConfigure.strWorkingDir.c_str() + middleMapName;

	UIImage* pMidMap = (UIImage*)gGlobal.pUIManager->GetUIItem("MidMapImage");
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("MidMapPanel");

	UIImage* path = (UIImage*)gGlobal.pUIManager->GetUIItem("MidMapPath");

	if (pMidMap && panel && path )
	{
		if ( g_GdfResource || g_GdfResourceEx )
		{
			UInt32 beginOffset, fileSize;
			BYTE* fbuf = NULL;
			if ( g_GdfResourceEx )
				fbuf = g_GdfResourceEx->GetMapViewOfFile(strTempDir, beginOffset, fileSize);
			if ( (!fbuf) && g_GdfResource )
				fbuf = g_GdfResource->GetMapViewOfFile(strTempDir, beginOffset, fileSize);
			if( fbuf==NULL )
			{
				UnmapViewOfFile(fbuf);
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseMiddleMap", String());
				return;
			}else
				UnmapViewOfFile(fbuf);
		}else if (!Reuben::Platform::File::IsExists(strTempDir.c_str()))
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseMiddleMap", String());
			return ;
		}
		pMidMap->SetImageFile(strTempDir.c_str());
		pMidMap->SetDetectRect();
			
		m_pPathIm = path->GetImage();
		if (m_pPathIm)
			CImageManager::GetInstance().DeleteImage(m_pPathIm);

		m_pPathIm = SafeCreateObject3(CImage, pMidMap->GetWidth(), pMidMap->GetHeight(), 8888);
		CImageManager::GetInstance().CreateImage(m_pPathIm);

		path->SetImage(m_pPathIm, 0, 0, pMidMap->GetWidth(), pMidMap->GetHeight());
		path->SetDetectRect();

		CMapObjectView* mapObjects = QGLApplication::GetInstance().GetRenderMap();

		m_dwRatioX =  (double)mapObjects->GetMapWidth() / pMidMap->GetWidth();
		m_dwRatioY = (double)mapObjects->GetMapHeight() / pMidMap->GetHeight();

		//draw the map and the border
		{
			UIImage* leftUp = (UIImage*)gGlobal.pUIManager->GetUIItem("LeftUpBorder");
			UIImage* leftBottom = (UIImage*)gGlobal.pUIManager->GetUIItem("LeftBottomBorder");
			UIImage* rightUp = (UIImage*)gGlobal.pUIManager->GetUIItem("RightUpBorder");
			UIImage* rightBottom = (UIImage*)gGlobal.pUIManager->GetUIItem("RightBottomBorder");

			UILinerImageV* leftBorder = (UILinerImageV*) gGlobal.pUIManager->GetUIItem("LeftBorder");
			UILinerImageV* rightBorder = (UILinerImageV*) gGlobal.pUIManager->GetUIItem("RightBorder");
			UILinerImage* topBorder = (UILinerImage*)gGlobal.pUIManager->GetUIItem("TopBorder");
			UILinerImage* bottomBorder = (UILinerImage*)gGlobal.pUIManager->GetUIItem("BottomBorder");

			UIImage* npcBar = (UIImage*)gGlobal.pUIManager->GetUIItem("NPCBar");
			UIButton* businessman = (UIButton*)gGlobal.pUIManager->GetUIItem("MidMapBusinessman");
			UIButton* normalPerson = (UIButton*)gGlobal.pUIManager->GetUIItem("MidMapNormalNpc");
			UIButton* special = (UIButton*)gGlobal.pUIManager->GetUIItem("MidMapQuestNpc");

			if (leftUp && leftBottom && rightUp && rightBottom && leftBorder && rightBorder && topBorder && bottomBorder && npcBar && businessman && normalPerson && special)
			{
				rightUp->SetOffsetXY(leftUp->GetOffsetX() + leftUp->GetWidth() + pMidMap->GetWidth(), leftUp->GetOffsetY());
				leftBottom->SetOffsetXY(leftUp->GetOffsetX(), leftUp->GetOffsetY() + leftUp->GetHeight() + pMidMap->GetHeight());
				rightBottom->SetOffsetXY(leftUp->GetOffsetX() + leftUp->GetWidth() + pMidMap->GetWidth(),
					leftUp->GetOffsetY() + leftUp->GetHeight() + pMidMap->GetHeight());

				topBorder->SetOffsetXY(leftUp->GetOffsetX() + leftUp->GetWidth(), leftUp->GetOffsetY() );
				topBorder->SetBaseWidth(pMidMap->GetWidth());
				topBorder->SetLinerWidth(pMidMap->GetWidth());
				topBorder->SetDetectRect();

				bottomBorder->SetOffsetXY(leftUp->GetOffsetX() + leftUp->GetWidth(), leftBottom->GetOffsetY() );
				bottomBorder->SetBaseWidth(pMidMap->GetWidth());
				bottomBorder->SetLinerWidth(pMidMap->GetWidth());
				bottomBorder->SetDetectRect();

				leftBorder->SetOffsetXY(leftUp->GetOffsetX(), leftUp->GetOffsetY() + leftUp->GetHeight());
				leftBorder->SetBaseHeight(pMidMap->GetHeight());
				leftBorder->SetLinerHeight(pMidMap->GetHeight());
				leftBorder->SetDetectRect();

				rightBorder->SetOffsetXY(rightUp->GetOffsetX(), leftUp->GetOffsetY() + leftUp->GetHeight());
				rightBorder->SetBaseHeight(pMidMap->GetHeight());
				rightBorder->SetLinerHeight(pMidMap->GetHeight());
				rightBorder->SetDetectRect();

				npcBar->SetOffsetXY( rightBottom->GetOffsetX() + rightBottom->GetWidth() - npcBar->GetWidth(), rightBottom->GetOffsetY() + rightBottom->GetHeight());

				for (int i = _ButtonNormal; i < _MaxButton_State; i++)
				{
					normalPerson->SetSubOffSet(i , npcBar->GetOffsetX() + 22 ,  npcBar->GetOffsetY());
					businessman->SetSubOffSet(i , npcBar->GetOffsetX() + 69, npcBar->GetOffsetY());
					special->SetSubOffSet(i , npcBar->GetOffsetX() + 115, npcBar->GetOffsetY());
				}
				normalPerson->SetOffsetXY( npcBar->GetOffsetX() + 22 ,  npcBar->GetOffsetY());
				businessman->SetOffsetXY(npcBar->GetOffsetX() + 69, npcBar->GetOffsetY());
				special->SetOffsetXY(npcBar->GetOffsetX() + 115, npcBar->GetOffsetY());

				UIButton* cross = (UIButton*)gGlobal.pUIManager->GetUIItem("MidMapCross");

				if (cross)
				{
					for (int i = _ButtonNormal; i < _MaxButton_State; i++)
						cross->SetSubOffSet(i, rightUp->GetOffsetX() - cross->GetWidth()  , rightUp->GetOffsetY() - cross->GetHeight() / 2);
					
					cross->SetOffsetXY( rightUp->GetOffsetX() - cross->GetWidth() , rightUp->GetOffsetY() - cross->GetHeight() / 2);
				}

				panel->SetImage(NULL, 0, 0, 2 * leftUp->GetWidth() + pMidMap->GetWidth() + leftUp->GetOffsetX() , 2 * leftUp->GetHeight() + pMidMap->GetHeight() + leftUp->GetOffsetY() + npcBar->GetHeight());
				panel->SetTextureWH(2 * leftUp->GetWidth() + pMidMap->GetWidth() + leftUp->GetOffsetX(), 2 * leftUp->GetHeight() + pMidMap->GetHeight() + leftUp->GetOffsetY() + npcBar->GetHeight());
				panel->SetDetectRect();
				panel->SetDragRect(0, 0, panel->GetWidth(), panel->GetHeight());

				panel->SetPos(400 - panel->GetWidth() / 2 , 300 - panel->GetHeight() / 2);
			}
		}

		PixelPos posNow = gGlobal.g_pCharacter->GetCharMainView()->GetEntPixelPos();
		UIImage* charPos = (UIImage*)gGlobal.pUIManager->GetUIItem("CharacterLocation");

		UIImage* jumpPoint = NULL;
		UIImage* jumpHighLight = NULL;

		UIButton* npcLocationButton = NULL;
		UIButton* jumpPointButton = NULL;

		//Map icon
		{
			//UInt i = 0;
			StringA iconName;
			String Filename = _T("Data\\interface0025_main_J.tga");

			UInt normalNPC = 0;
			UInt visibleEmenyForMany = 0;
			UInt businessman = 0;
			UInt questNPC = 0;
			UInt visibleEmenyForOne = 0;
			for (Index i = C_INDEX(0); i < gGlobal.g_pSearchListProcess->GetNPCCount(); ++i)
			{
				NPCInfo npc = gGlobal.g_pSearchListProcess->GetNPCInfo(i);
				if (npc.NPC_type != NPC_INVISIBLE_EMENY)
				{
					npcLocationButton = SafeCreateObject(UIButton);
					RECT rec1, rec2;
					//npcIcon
					{
						switch (npc.NPC_type)
						{
						case NPC_NORAML:
							{
								iconName.Format("NormalNpcIcon%d",normalNPC);
								++normalNPC;
								SetRect(&rec1, 301, 141, 310, 150);
								SetRect(&rec2, 301, 155, 310, 164);
							}
							break;
						case NPC_VISIBLE_EMENY_FOR_MANY_USER:
							{
								iconName.Format("VisibleForManyNpcIcon%d",visibleEmenyForMany);
								++visibleEmenyForMany;
								SetRect(&rec1, 385, 141, 394, 150);
								SetRect(&rec2, 385, 155, 394, 164);
							}
							break;
						case NPC_BUSINESSMAN:
							{
								iconName.Format("BusinessmanNpcIcon%d",businessman);
								++businessman;
								SetRect(&rec1, 331, 141, 340, 150);
								SetRect(&rec2, 331, 155, 340, 164);
							}
							break;
						case NPC_QUEST:
							{
								iconName.Format("QuestNpcIcon%d",questNPC);
								++questNPC;
								SetRect(&rec1, 316, 141, 325, 150);
								SetRect(&rec2, 316, 155, 325, 164);
							}
							break;
						case NPC_VISIBLE_EMENY_FOR_ONE_USER:
							{
								iconName.Format("VisibleForOneNpcIcon%d",visibleEmenyForOne);
								++visibleEmenyForOne;
								SetRect(&rec1, 385, 141, 394, 150);
								SetRect(&rec2, 385, 155, 394, 164);
							}
							break;
						}
						//iconName.Format("NpcIcon%d",i);
						gGlobal.pUIManager->AddUIID(iconName.c_str(), npcLocationButton);
						npcLocationButton->SetName(iconName.c_str());
						npcLocationButton->SetEnable(true);
						npcLocationButton->SetNotDetected(false);
						npcLocationButton->SetDetectImage(true);
						npcLocationButton->SetTimeOutCount(0);

						npcLocationButton->SetLuaFunc(UI_FUNC_MOUSE_MOVE, "MidMapNpcOver");
						npcLocationButton->SetLuaFunc(UI_FUNC_MOUSE_OUT, "MidMapNpcOut");
						npcLocationButton->SetLuaFunc(UI_FUNC_LBUTTON_UP, "MidMapClickNPC");

						CImage* img = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
						npcLocationButton->SetImage(img);

						Int32 relX = npc.BlockPosX * BlockSizeX + BlockSizeX / 2;
						Int32 relY = npc.BlockPosY * BlockSizeY/2 + BlockSizeY / 2;

						npcLocationButton->SetOffsetXY( (Int32)(pMidMap->GetOffsetX() + relX / m_dwRatioX - 4), (Int32)(pMidMap->GetOffsetY() + relY / m_dwRatioY - 4));

						for (int k = 0; k < 3; ++k)
						{
							npcLocationButton->SetSubOffSet( k , npcLocationButton->GetOffsetX(), npcLocationButton->GetOffsetY() );
							if (k < 1)
								npcLocationButton->SetRect( k, rec1.left, rec1.top, rec1.right, rec1.bottom);
							else
								npcLocationButton->SetRect( k, rec2.left, rec2.top, rec2.right, rec2.bottom);
						}

						npcLocationButton->SetOffsetPos(panel->GetX(), panel->GetY());
						npcLocationButton->SetStateDetectRect();
						npcLocationButton->SetLuaState(gGlobal.pUIManager->GetLuaState());
						gGlobal.pUIManager->InsertToInOutEvent(npcLocationButton);
					}
					panel->AddUIItem(npcLocationButton);
				
					m_IconNPCLst.insert(std::make_pair(iconName, i));
				}
			}

			//jump point icon
			{
				BinaryClientJumppointInstance *tmpjp;
				for (Index j = C_INDEX(0); j < gGlobal.m_jplist.size(); ++j)
				{
					tmpjp = gGlobal.m_jplist[j];
					jumpPointButton = SafeCreateObject(UIButton);
					iconName.Format("JumpPoint%d", j);
					gGlobal.pUIManager->AddUIID(iconName.c_str(), jumpPointButton);
					jumpPointButton->SetName(iconName.c_str());
					jumpPointButton->SetEnable(true);
					jumpPointButton->SetNotDetected(false);
					jumpPointButton->SetDetectImage(true);
					jumpPointButton->SetTimeOutCount(0);

					jumpPointButton->SetLuaFunc(UI_FUNC_MOUSE_MOVE, "MidMapJumpPointOver");
					jumpPointButton->SetLuaFunc(UI_FUNC_MOUSE_OUT, "MidMapJumpPointOut");
					jumpPointButton->SetLuaFunc(UI_FUNC_LBUTTON_UP, "MidMapJumpClick");

					Int32 relX = tmpjp->m_posx * BlockSizeX + BlockSizeX / 2;
					Int32 relY = tmpjp->m_posy * BlockSizeY/2 + BlockSizeY / 2;

					if (tmpjp->m_posy % 2 == 1)
						relX += BlockSizeX / 2;

					CImage* img = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
					jumpPointButton->SetImage(img); 
					jumpPointButton->SetOffsetXY( (Int32)(pMidMap->GetOffsetX() + relX / m_dwRatioX - 6), (Int32)(pMidMap->GetOffsetY() + relY / m_dwRatioY - 6));

					for (int k = 0; k < 3; ++k)
					{
						jumpPointButton->SetSubOffSet( k , jumpPointButton->GetOffsetX(), jumpPointButton->GetOffsetY() );

						if (k < 1)
							jumpPointButton->SetRect( k, 346, 140, 358, 152);
						else
							jumpPointButton->SetRect( k, 346, 154, 358, 166);
					}

					jumpPointButton->SetOffsetPos(panel->GetX(), panel->GetY());
					jumpPointButton->SetStateDetectRect();
					jumpPointButton->SetLuaState(gGlobal.pUIManager->GetLuaState());
					gGlobal.pUIManager->InsertToInOutEvent(jumpPointButton);
					panel->AddUIItem(jumpPointButton);

					m_JumpPointLst.insert(std::make_pair(iconName, j));
				}
			}
		}

		if (path)
			DrawPath(m_iDesBlockX, m_iDesBlockY); 

		{//Party members
			StringA iconName;
			String Filename = _T("Data\\interface0025_main_J.tga");
			CImage* img = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
			if (NULL != gGlobal.g_pParty)//in party character 
			{
				UIButton* pTempLeaveMemberButton = NULL;

				Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.begin();
				for (;it != gGlobal.g_mapPartyChar.end();it++) 
				{
					CPartyCharInfoClient* pccic = it->second;
					if (NULL != pccic && pccic->GetU16MapId() == gGlobal.m_mapdb->m_basicinfo->m_mapid) 
					{
						pTempLeaveMemberButton = SafeCreateObject(UIButton);
						iconName.Format("MidMapPartyMember%d",it->first);
						gGlobal.pUIManager->AddUIID(iconName.c_str(), pTempLeaveMemberButton);
						pTempLeaveMemberButton->SetName(iconName.c_str());
						pTempLeaveMemberButton->SetEnable(true);
						pTempLeaveMemberButton->SetNotDetected(false);
						pTempLeaveMemberButton->SetDetectImage(true);
						pTempLeaveMemberButton->SetTimeOutCount(0);

						Int32 relX = pccic->GetU16XPos() * BlockSizeX + BlockSizeX / 2;
						Int32 relY = pccic->GetU16YPos() * BlockSizeY/2 + BlockSizeY / 2;

						if (pccic->GetU16YPos() % 2 == 1)
							relX += BlockSizeX / 2;

						pTempLeaveMemberButton->SetImage(img); 
						pTempLeaveMemberButton->SetOffsetXY( (Int32)(pMidMap->GetOffsetX() + relX / m_dwRatioX - 6), (Int32)(pMidMap->GetOffsetY() + relY / m_dwRatioY - 6));

						for (int k = 0; k < 3; ++k)
						{
							pTempLeaveMemberButton->SetSubOffSet( k , pTempLeaveMemberButton->GetOffsetX(), pTempLeaveMemberButton->GetOffsetY() );

							if (k < 1)
								pTempLeaveMemberButton->SetRect( k, 434, 131, 449, 148);
							else
								pTempLeaveMemberButton->SetRect( k, 434, 151, 449, 168);
						}

						pTempLeaveMemberButton->SetOffsetPos(panel->GetX(), panel->GetY());
						pTempLeaveMemberButton->SetStateDetectRect();
						panel->AddUIItem(pTempLeaveMemberButton);
					}
				}
			}
			else if (NULL != gGlobal.g_pTempLeaveParty)//temporarily leave party character 
			{
				UIButton* pLeaderButton = SafeCreateObject(UIButton);
				iconName.Format("MidMapPartyMember0");
				gGlobal.pUIManager->AddUIID(iconName.c_str(), pLeaderButton);
				pLeaderButton->SetName(iconName.c_str());
				pLeaderButton->SetEnable(true);
				pLeaderButton->SetNotDetected(false);
				pLeaderButton->SetDetectImage(true);
				pLeaderButton->SetTimeOutCount(0);

				img = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
				Global::PartyCharInfoMap::iterator itLeader = gGlobal.g_mapPartyChar.find(0);
				if (itLeader != gGlobal.g_mapPartyChar.end()) 
				{
					CPartyCharInfoClient* pccic = itLeader->second;
					if (NULL != pccic && pccic->GetU16MapId() == gGlobal.m_mapdb->m_basicinfo->m_mapid) 
					{
						Int32 relX = pccic->GetU16XPos() * BlockSizeX + BlockSizeX / 2;
						Int32 relY = pccic->GetU16YPos() * BlockSizeY/2 + BlockSizeY / 2;

						if (pccic->GetU16YPos() % 2 == 1)
							relX += BlockSizeX / 2;

						pLeaderButton->SetImage(img); 
						pLeaderButton->SetOffsetXY( (Int32)(pMidMap->GetOffsetX() + relX / m_dwRatioX - 6), (Int32)(pMidMap->GetOffsetY() + relY / m_dwRatioY - 6));

						for (int k = 0; k < 3; ++k)
						{
							pLeaderButton->SetSubOffSet( k , pLeaderButton->GetOffsetX(), pLeaderButton->GetOffsetY() );

							if (k < 1)
								pLeaderButton->SetRect( k, 404, 131, 419, 148);
							else
								pLeaderButton->SetRect( k, 404, 151, 419, 168);
						}

						pLeaderButton->SetOffsetPos(panel->GetX(), panel->GetY());
						pLeaderButton->SetStateDetectRect();
						panel->AddUIItem(pLeaderButton);
					}
				}

				UIButton* pTempLeaveMemberButton = NULL;
				Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.begin();
				for (;it != gGlobal.g_mapPartyChar.end();it++) 
				{
					CPartyCharInfoClient* pccic = it->second;
					if (NULL != pccic 
						&& gGlobal.g_pTempLeaveParty->GetM_bTempLeave(it->first)
						&& pccic->GetU16MapId() == gGlobal.m_mapdb->m_basicinfo->m_mapid) 
					{
						pTempLeaveMemberButton = SafeCreateObject(UIButton);
						iconName.Format("MidMapPartyMember%d",it->first);
						gGlobal.pUIManager->AddUIID(iconName.c_str(), pTempLeaveMemberButton);
						pTempLeaveMemberButton->SetName(iconName.c_str());
						pTempLeaveMemberButton->SetEnable(true);
						pTempLeaveMemberButton->SetNotDetected(false);
						pTempLeaveMemberButton->SetDetectImage(true);
						pTempLeaveMemberButton->SetTimeOutCount(0);

						Int32 relX = pccic->GetU16XPos() * BlockSizeX + BlockSizeX / 2;
						Int32 relY = pccic->GetU16YPos() * BlockSizeY/2 + BlockSizeY / 2;

						if (pccic->GetU16YPos() % 2 == 1)
							relX += BlockSizeX / 2;

						pTempLeaveMemberButton->SetImage(img); 
						pTempLeaveMemberButton->SetOffsetXY( (Int32)(pMidMap->GetOffsetX() + relX / m_dwRatioX - 6), (Int32)(pMidMap->GetOffsetY() + relY / m_dwRatioY - 6));

						for (int k = 0; k < 3; ++k)
						{
							pTempLeaveMemberButton->SetSubOffSet( k , pTempLeaveMemberButton->GetOffsetX(), pTempLeaveMemberButton->GetOffsetY() );

							if (k < 1)
								pTempLeaveMemberButton->SetRect( k, 434, 131, 449, 148);
							else
								pTempLeaveMemberButton->SetRect( k, 434, 151, 449, 168);
						}

						pTempLeaveMemberButton->SetOffsetPos(panel->GetX(), panel->GetY());
						pTempLeaveMemberButton->SetStateDetectRect();
						panel->AddUIItem(pTempLeaveMemberButton);
					}
				}
			}
			if (charPos)
			{
				charPos->SetEnable(true);
				charPos->SetOffsetXY( (Int32)(pMidMap->GetOffsetX() + posNow.PixelPosX / m_dwRatioX - 6), (Int32)(pMidMap->GetOffsetY() + posNow.PixelPosY / m_dwRatioY - 18));
			}
		}

		gGlobal.pUIManager->AddDirtyUIItem(panel);
		m_bLoadMap = true;
	}
}

void CMiddleMap::UpdatePosition()
{ 
	if (gGlobal.g_pCharacter && gGlobal.pUIManager)
	{
		BlockPos blockNow = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();
		UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("MidMapPanel");
		if (panel)
		{
			PixelPos posNow = gGlobal.g_pCharacter->GetCharMainView()->GetEntPixelPos();
			BlockPos charDes = gGlobal.g_pCharacter->GetCharMainView()->GetCharDesPos();

			UIImage* pMidMap = (UIImage*)gGlobal.pUIManager->GetUIItem("MidMapImage");
			UIImage* charPos = (UIImage*)gGlobal.pUIManager->GetUIItem("CharacterLocation");
			if (charPos)
			{				
				if (blockNow != charDes)
				{
					if (blockNow.PosX != m_iDesBlockX || blockNow.PosY != m_iDesBlockY )
					{
						Int32 tempx = (Int32)(pMidMap->GetOffsetX() + posNow.PixelPosX / m_dwRatioX - 6);
						Int32 tempy = (Int32)(pMidMap->GetOffsetY() + posNow.PixelPosY / m_dwRatioY - 16);

						if (tempx != charPos->GetOffsetX() || tempy != charPos->GetOffsetY())
							charPos->SetOffsetXY( tempx, tempy );				
					}
				}
				else if ( (blockNow.PosX == m_iDesBlockX && blockNow.PosY == m_iDesBlockY) || (blockNow.PosX == m_iCharDesBlockX && blockNow.PosY == m_iCharDesBlockY) )
				{
					ClearPath();
					ResetCharDes();
				}
				else if (gGlobal.g_pParty || gGlobal.g_pTempLeaveParty)
				{
					Int32 tempx = (Int32)(pMidMap->GetOffsetX() + posNow.PixelPosX / m_dwRatioX - 6);
					Int32 tempy = (Int32)(pMidMap->GetOffsetY() + posNow.PixelPosY / m_dwRatioY - 16);

					if (tempx != charPos->GetOffsetX() || tempy != charPos->GetOffsetY())
						charPos->SetOffsetXY( tempx, tempy );				
				}
			}
		}
		else if (blockNow.PosX == m_iCharDesBlockX && blockNow.PosY == m_iCharDesBlockY)
			ResetCharDes();		
	}
}

void CMiddleMap::SetLocationBoxPos(Int32 x, Int32 y, Int32 textPos)
{
	UIImage* locationBoxLeft = gGlobal.pUIManager->FindUIImage("LocationBoxLeft");
	UILinerImage* locationBox = gGlobal.pUIManager->FindUILinerImage("LocationBox");
	UIImage* locationBoxRight = gGlobal.pUIManager->FindUIImage("LocationBoxRight");
	UIPanel* locationBoxPanel = gGlobal.pUIManager->FindUIPanel("LocationBoxPanel");

	if (locationBoxLeft && locationBox && locationBoxRight && locationBoxPanel)
	{
		if (locationBoxPanel->GetX() != x || locationBoxPanel->GetY() != y)
		{
			RECT realRect;
			locationBoxPanel->SetRealRect(realRect);
			gGlobal.pUIManager->GetDirtyRectMan()->AddDirtyRectToMan(realRect);
			locationBoxPanel->SetPos(x , y);
			locationBoxLeft->SetOffsetXY(0,0);
			locationBox->SetOffsetXY(locationBoxLeft->GetOffsetX() + locationBoxLeft->GetWidth(), locationBoxLeft->GetOffsetY());
			locationBoxRight->SetOffsetXY(locationBox->GetOffsetX() + locationBox->GetWidth(), locationBoxLeft->GetOffsetY());

			UIStaticText* text = (UIStaticText*)gGlobal.pUIManager->GetUIItem("LocationString");
			UIStaticText* npcName = (UIStaticText*)gGlobal.pUIManager->GetUIItem("TargetName");

			if (text && npcName)
			{
				text->SetOffsetXY(textPos, locationBox->GetOffsetY() + 15);
				npcName->SetOffsetXY(textPos, locationBox->GetOffsetY() + 2);
			}

			if (!locationBoxPanel->IsEnable())
				locationBoxPanel->SetEnable(true);

			gGlobal.pUIManager->AddDirtyUIItem(locationBoxPanel);
		}
	}
}

void CMiddleMap::MoveCharacterTo()
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("MidMapPanel");
	UIImage* pMidMap = gGlobal.pUIManager->FindUIImage("MidMapImage");

	UIImage* npcPoint = gGlobal.pUIManager->FindUIImage(m_sPrevIconName.c_str());

	if (panel && m_bLoadMap && gGlobal.g_pCharacter )
	{
		UInt16 relX =  gGlobal.pUIManager->GetCursorX() - panel->GetX() - pMidMap->GetOffsetX();
		UInt16 relY =  gGlobal.pUIManager->GetCursorY() - panel->GetY() - pMidMap->GetOffsetY();

		if (relX > 0 && relX < pMidMap->GetWidth() && relY > 0 && relY < pMidMap->GetHeight())
		{
			CMapObjectView* mapObjects = QGLApplication::GetInstance().GetRenderMap();
			BlockPos DesPos;

			if ( QGLMath::FindBlockByPixel( (DWORD)(relX * m_dwRatioX), (DWORD)(relY * m_dwRatioY), DesPos, mapObjects->GetMapWidth(), 
				mapObjects->GetMapHeight(), mapObjects->GetMapMaxBlockX(), mapObjects->GetMapMaxBlockY()) )
			{
				
				gGlobal.g_pCharacter->SetEntityDesPos(DesPos.PosX, DesPos.PosY, true);
				DrawPath();				
			}
		}
	}
}

void CMiddleMap::MoveCharacterToNPC()
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("MidMapPanel");
	UIImage* pMidMap = gGlobal.pUIManager->FindUIImage("MidMapImage");
	IconNpcIDMap::iterator itr = m_IconNPCLst.find(m_sPrevIconName);

	if (panel && m_bLoadMap && itr != m_IconNPCLst.end() && gGlobal.g_pCharacter)
	{
		BlockPos DesPos;
		
		NPCInfo npc = gGlobal.g_pSearchListProcess->GetNPCInfo(itr->second);
		DesPos.PosX = npc.BlockPosX;
		DesPos.PosY = npc.BlockPosY;

		gGlobal.g_pCharacter->SetEntityDesPos(DesPos.PosX , DesPos.PosY);
		
		PointList ptList;
		ptList = gGlobal.g_pCharacter->GetCharMainView()->m_PathFindData.m_ptList;

		if (ptList.size() > 10)
			gGlobal.g_pCharacter->SetEntityDesPos(ptList[ptList.size() - 10].PosX , ptList[ptList.size() - 10].PosY, true);
		else
			gGlobal.g_pCharacter->SetEntityDesPos(DesPos.PosX , DesPos.PosY, true);

		DrawPath(DesPos.PosX, DesPos.PosY);
	}
}


void CMiddleMap::ResetText()
{
	UIStaticText* NameText = gGlobal.pUIManager->FindUIStaticText("TargetName");

	if (NameText)
	{
		if (m_bChangeSize)
		{
			SetLocationPanelSize(68);
			UIPanel* locationBoxPanel = (UIPanel*)gGlobal.pUIManager->GetUIItem("LocationBoxPanel");
			gGlobal.pUIManager->AddDirtyUIItem(locationBoxPanel);
			m_dX = -21;

			m_bChangeSize = false;
		}
		NameText->SetEnable(false);
	}
}


void CMiddleMap::ShowNpcName()
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("MidMapPanel");
	UIImage* pMidMap = gGlobal.pUIManager->FindUIImage("MidMapImage");
	UIImage* charPos = gGlobal.pUIManager->FindUIImage("CharacterLocation");

	if (panel && pMidMap)
	{
		UIButton* npcIconButton = (UIButton*)panel->GetMouseOver();

		if (npcIconButton && ((UIItem*)npcIconButton != (UIItem*)pMidMap) && ((UIItem*)npcIconButton != (UIItem*)charPos) )
		{
			m_sPrevIconName = *npcIconButton->GetName();
			IconNpcIDMap::iterator itr = m_IconNPCLst.find(*npcIconButton->GetName());

			if (itr != m_IconNPCLst.end())
			{
				NPCInfo npc = gGlobal.g_pSearchListProcess->GetNPCInfo(itr->second);
				String npcName = npc.ChiName;
				SetLocationBoxText(npcName);
				SetLocationBoxPosText(npc.BlockPosX, npc.BlockPosY);
			}
		}
	}
}

void CMiddleMap::SetLocationBoxPosText(UInt16 x, UInt16 y)
{
	String temp;
	temp.Format(_T("(%d,%d)"), x, y);
	UIStaticText* text = gGlobal.pUIManager->FindUIStaticText("LocationString");
	if (text)
	{
		if (text->GetText() != temp.c_str())
		{
			text->SetText(temp.c_str());
			text->Redraw();
		}
	}
}

void CMiddleMap::SetLocationBoxText(String text)
{
	UIStaticText* NameText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("TargetName");

	if (NameText && NameText->GetText() != text.c_str())
	{	
		UInt textLenght = (UInt)text.length();
		if (textLenght > 5 )
		{
			SetLocationPanelSize(textLenght * 12);
			UIPanel* locationBoxPanel = (UIPanel*)gGlobal.pUIManager->GetUIItem("LocationBoxPanel");
			UILinerImage* locationBox = (UILinerImage*)gGlobal.pUIManager->GetUIItem("LocationBox");
			m_dX = -1 * (55 - locationBox->GetWidth() / 2 ); 
			NameText->SetOffsetXY( m_dX, NameText->GetOffsetY() );

			UIStaticText* posText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("LocationString");
			posText->SetOffsetXY( m_dX, posText->GetOffsetY() );

			gGlobal.pUIManager->AddDirtyUIItem(locationBoxPanel);

			m_bChangeSize = true;
		}

		NameText->SetText(text.c_str());
		NameText->SetEnable(true);
		NameText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(NameText);
	}
}

void CMiddleMap::ShowJumpPointName()
{
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("MidMapPanel");
	UIImage* pMidMap = (UIImage*)gGlobal.pUIManager->GetUIItem("MidMapImage");
	UIImage* charPos = (UIImage*)gGlobal.pUIManager->GetUIItem("CharacterLocation");

	if (panel && pMidMap && charPos)
	{
		UIButton* jumpPointButton = (UIButton*)panel->GetMouseOver();

		if (jumpPointButton && ((UIItem*)jumpPointButton != (UIItem*)pMidMap) && ((UIItem*)jumpPointButton != (UIItem*)charPos))
		{
			m_sPrevIconName = *jumpPointButton->GetName();
			IconNpcIDMap::iterator itr = m_JumpPointLst.find(*jumpPointButton->GetName());
			if (itr != m_JumpPointLst.end())
			{
				BinaryClientJumppointInstance *tmpjp;
				tmpjp = gGlobal.m_jplist[itr->second];
				String jpName = tmpjp->szName;

				SetLocationBoxText(jpName);
				SetLocationBoxPosText(tmpjp->m_posx, tmpjp->m_posy);
			}
		}
	}
}

void CMiddleMap::SetLocationPanelSize(UInt16 size)
{
	UILinerImage* locationBox = (UILinerImage*)gGlobal.pUIManager->GetUIItem("LocationBox");
	locationBox->SetBaseWidth(size);
	locationBox->SetLinerWidth(size);
	locationBox->SetDetectRect();

	UIImage* locationBoxRight = (UIImage*)gGlobal.pUIManager->GetUIItem("LocationBoxRight");
	locationBoxRight->SetOffsetXY(locationBox->GetOffsetX() + locationBox->GetWidth(), locationBox->GetOffsetY());

	UIPanel* locationBoxPanel = (UIPanel*)gGlobal.pUIManager->GetUIItem("LocationBoxPanel");
	locationBoxPanel->SetImage(NULL, 0, 0, 10 + locationBox->GetWidth(), 30);
	locationBoxPanel->SetTextureWH(10 + locationBox->GetWidth(), 30);
	locationBoxPanel->SetDetectRect();
}


void CMiddleMap::MoveCharacterToJumpPoint()
{
	UIImage* pMidMap = (UIImage*)gGlobal.pUIManager->GetUIItem("MidMapImage");
	IconNpcIDMap::iterator itr = m_JumpPointLst.find(m_sPrevIconName);
		
	if (pMidMap && m_bLoadMap && itr != m_JumpPointLst.end() && gGlobal.g_pCharacter)
	{
		BinaryClientJumppointInstance *tmpjp;
		tmpjp = gGlobal.m_jplist[itr->second];

		BlockPos DesPos;
		DesPos.PosX = tmpjp->m_posx;
		DesPos.PosY = tmpjp->m_posy;
		gGlobal.g_pCharacter->SetEntityDesPos(DesPos.PosX, DesPos.PosY, true);
		DrawPath();
	}
}


void CMiddleMap::DrawPath(Int32 desBlockX, Int32 desBlockY)
{
	UIImage* pMidMap = (UIImage*)gGlobal.pUIManager->GetUIItem("MidMapImage");
	UIImage* path = (UIImage*)gGlobal.pUIManager->GetUIItem("MidMapPath");
	
	ClearPath();

	if (pMidMap && path && (gGlobal.g_pParty == NULL || gGlobal.g_pParty->IsLeader()))
	{
		PointList ptList;

		ptList = gGlobal.g_pCharacter->GetCharMainView()->m_PathFindData.m_ptList;
		if (desBlockX > 0 || desBlockY > 0) 
		{
			if (!gGlobal.g_PathFinder->IsBlocked(desBlockX, desBlockY))
			{
				BlockPos tempDes;
				tempDes.PosX = desBlockX;
				tempDes.PosY = desBlockY;
				ptList.push_back(tempDes);
			}
		}
		CImage* img =path->GetImage();

		Int pixelX = 0;
		Int pixelY = 0;
		DirtyPoint point;

		for (UInt i = 0; i < ptList.size(); ++i)
		{
			QGLMath::BlockPosToPixel(ptList[i].PosX, ptList[i].PosY, pixelX, pixelY);

			double realX = pixelX / m_dwRatioX;
			pixelX = (Int)realX;
			realX = realX - pixelX;
			if (realX > 0.49)
				++pixelX;

			double realY = pixelY / m_dwRatioY;
			pixelY = (Int)realY;
			realY = realY - pixelY;
			if (realY > 0.49)
				++pixelY;

			Int desX = 0;
			Int desY = 0;

			Int diffX = 0;
			Int diffY = 0;
			bool loop = true;
			Int StartPixelX = pixelX;
			Int StartPixelY = pixelY;
			
			if ( (i + 1) < ptList.size())
			{
				QGLMath::BlockPosToPixel(ptList[i+1].PosX, ptList[i+1].PosY, desX, desY);
		
				realX = desX / m_dwRatioX;
				desX = (Int)realX;
				realX = realX - desX;
				if (realX > 0.49)
					++desX;

				realY = desY / m_dwRatioY;
				desY = (Int)realY;
				realY = realY - desY;
				if (realY > 0.49)
					++desY;

				diffX = abs(desX - pixelX);
				diffY = abs(desY - pixelY);
			}
			else
			{
				desX = pixelX;
				desY = pixelY;
			}

			if (desX == pixelX)
			{
				while (loop)
				{
					UInt32 *tempBit = (UInt32 *)(img->GetPtrXY(StartPixelX, StartPixelY));

					point.pixelX = StartPixelX;
					point.pixelY = StartPixelY;

					m_DirtyPointList.push_back(point);

					tempBit[0] = 0xFFFF0000;

					tempBit = (UInt32 *)(img->GetPtrXY(StartPixelX + 1, StartPixelY));

					point.pixelX = StartPixelX + 1;
					point.pixelY = StartPixelY;

					m_DirtyPointList.push_back(point);

					tempBit[0] = 0xFFFF0000;

					if (StartPixelY == desY)
						loop = false;

					if (StartPixelY < desY)
						++StartPixelY;
					else
						--StartPixelY;
				}

			}
			else if (desY == pixelY)
			{
				while (loop)
				{
					UInt32 *tempBit = (UInt32 *)(img->GetPtrXY(StartPixelX, StartPixelY));

					point.pixelX = StartPixelX;
					point.pixelY = StartPixelY;

					m_DirtyPointList.push_back(point);

					tempBit[0] = 0xFFFF0000;

					tempBit = (UInt32 *)(img->GetPtrXY(StartPixelX , StartPixelY + 1));

					point.pixelX = StartPixelX;
					point.pixelY = StartPixelY + 1;

					m_DirtyPointList.push_back(point);

					tempBit[0] = 0xFFFF0000;

					if (StartPixelX == desX)
						loop = false;

					if (StartPixelX < desX)
						++StartPixelX;
					else 
						--StartPixelX;
				}
			}
			else
			{
				double slope = (double)(desY - pixelY) / (desX - pixelX) ;

				Int sum = 1;

				if (diffY > diffX)
				{
					while (loop)
					{
						StartPixelX = (Int)((StartPixelY - desY) / slope + desX);

						UInt32 *tempBit = (UInt32 *)(img->GetPtrXY(StartPixelX, StartPixelY));

						point.pixelX = StartPixelX;
						point.pixelY = StartPixelY;

						m_DirtyPointList.push_back(point);

						tempBit[0] = 0xFFFF0000;

						tempBit = (UInt32 *)(img->GetPtrXY(StartPixelX + sum , StartPixelY));

						point.pixelX = StartPixelX + sum;
						point.pixelY = StartPixelY;

						m_DirtyPointList.push_back(point);

						tempBit[0] = 0xFFFF0000;

						if (StartPixelY == desY)
							loop = false;

						if (StartPixelY > desY)
							--StartPixelY;
						else
							++StartPixelY;
					}
				}
				else
				{
					while (loop)
					{
						StartPixelY = (Int)(slope * (StartPixelX - desX) + desY);

						UInt32 *tempBit = (UInt32 *)(img->GetPtrXY(StartPixelX, StartPixelY));

						point.pixelX = StartPixelX;
						point.pixelY = StartPixelY;

						m_DirtyPointList.push_back(point);

						tempBit[0] = 0xFFFF0000;

						tempBit = (UInt32 *)(img->GetPtrXY(StartPixelX  , StartPixelY + sum));

						point.pixelX = StartPixelX;
						point.pixelY = StartPixelY + sum;

						m_DirtyPointList.push_back(point);

						tempBit[0] = 0xFFFF0000;

						if (StartPixelX == desX)
							loop = false;

						if (StartPixelX > desX)
							--StartPixelX;
						else
							++StartPixelX;
					}
				}
			}			
		}
		path->SetEnable(true);
		gGlobal.pUIManager->AddDirtyUIItem(path);

		if (ptList.size() > 0)
		{
			m_iDesBlockX = ptList[ptList.size() - 1].PosX;
			m_iDesBlockY = ptList[ptList.size() - 1].PosY;

			if ( (desBlockX > 0 || desBlockY > 0) && ptList.size() > 1)
				SetCharDes(ptList[ptList.size() - 2].PosX, ptList[ptList.size() - 2].PosY);			
			else			
				SetCharDes( m_iDesBlockX, m_iDesBlockY);
		}

		UIImage* targetPoint = (UIImage*)gGlobal.pUIManager->GetUIItem("TargetPoint");
		if (targetPoint)
		{
			BlockPos blockNow = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();
			if ( m_iCharDesBlockX >= 0 && m_iCharDesBlockY >= 0 &&
				(m_iCharDesBlockX != blockNow.PosX || m_iCharDesBlockY != blockNow.PosY) && 
				(m_iDesBlockX != blockNow.PosX || m_iDesBlockY != blockNow.PosY))
			{
				targetPoint->SetEnable(true);
				targetPoint->SetOffsetXY(pixelX + path->GetOffsetX() - 2, pixelY + path->GetOffsetY() - 23);
				
			}
		}
	}
}

void CMiddleMap::ShowLocationBox()
{
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("MidMapPanel");
	UIImage* pMidMap = (UIImage*)gGlobal.pUIManager->GetUIItem("MidMapImage");

	if (panel && m_bLoadMap)
	{
		Int32 px = gGlobal.pUIManager->GetCursorX();
		Int32 py = gGlobal.pUIManager->GetCursorY();

		UInt16 X =  (UInt16)( (gGlobal.pUIManager->GetCursorX() - panel->GetX() - pMidMap->GetOffsetX()) * m_dwRatioX );
		UInt16 Y =  (UInt16)( (gGlobal.pUIManager->GetCursorY() - panel->GetY() - pMidMap->GetOffsetY()) * m_dwRatioY );

		CMapObjectView* mapObjects = QGLApplication::GetInstance().GetRenderMap();
		BlockPos DesPos;
		if ( QGLMath::FindBlockByPixel(X, Y, DesPos, mapObjects->GetMapWidth(), 
			mapObjects->GetMapHeight(), mapObjects->GetMapMaxBlockX(), mapObjects->GetMapMaxBlockY()) )
		{
			SetLocationBoxPosText(DesPos.PosX, DesPos.PosY);
		}

		UILinerImage* locationBox = (UILinerImage*)gGlobal.pUIManager->GetUIItem("LocationBox");
		if (locationBox)
			SetLocationBoxPos(px  - 5 - locationBox->GetWidth() / 2, py - 40, m_dX);
	}
}

void CMiddleMap::ClearPath()
{
	UIImage* path = (UIImage*)gGlobal.pUIManager->GetUIItem("MidMapPath");

	if (path)
	{
		CImage* img = path->GetImage();
		if (img)
		{
			img->FillUInt32();
			m_DirtyPointList.clear();
			path->SetEnable(false);
			gGlobal.pUIManager->AddDirtyUIItem(path);
		}

		UIImage* targetPoint = (UIImage*)gGlobal.pUIManager->GetUIItem("TargetPoint");
		if (targetPoint)
		{
			targetPoint->SetEnable(false);
			gGlobal.pUIManager->AddDirtyUIItem(targetPoint);
		}
	}
}


bool CMiddleMap::HasPath()
{
	if (m_DirtyPointList.size() > 0)
		return true;
	else
		return false;
}

bool CMiddleMap::IsOpenMiddleMap()
{
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("MidMapPanel");
	if (panel)
		return panel->IsEnable();
	return false;
}

void CMiddleMap::ResetValue()
{
	m_bLoadMap = false;
	m_bChangeSize = false;
	m_dwRatioX = 0;
	m_dwRatioY = 0;
	m_IconNPCLst.clear();
	m_JumpPointLst.clear();

	if (m_DirtyPointList.size() > 0)
		ClearPath();
	m_DirtyPointList.clear();
	m_sPrevIconName = _T("");
	m_dX = -21;

}

CMiddleMap::~CMiddleMap()
{
	ResetValue();
	m_DirtyPointList.clear();
	m_sPrevIconName = _T("");
	m_iDesBlockX = -1;
	m_iDesBlockY = -1;
	m_iCharDesBlockX = -1;
	m_iCharDesBlockY = -1;
}
