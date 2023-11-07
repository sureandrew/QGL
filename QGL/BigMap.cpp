#include "Common.h"
#include "QGL.h"
//-- Self
#include "BigMap.h"

#include "Resource/ClientResManager.h"
#include "Resource/ResBigMap.h"
#include "QGL/Global.h"
#include "LuaPush.h"
#include "Font.h"
#include "QGL/MapControl.h"
#include "Common/CallLuaFunc.h"

/*
struct MapButtonXWH
{
	UInt16 wordX;
	UInt16 wordWidth;
	UInt16 wordHeight;
};

struct MapButtonY
{
	UInt16 normalY;
	UInt16 overY;
	UInt16 clickY;
};

MapButtonXWH gButtonSetXWH[4] = 
{
	{ 2, 46, 20 },
	{ 52, 60, 20 },
	{ 115, 67, 20},
	{ 186, 79, 20},
	
};

MapButtonY gButtonSetY[6] = 
{
	{152, 176, 201},
	{77, 101, 126},
	{377, 401, 426},
	{2, 26, 51},
	{302, 326, 351},
	{227, 251, 276},

	/*{500, 500, 500},
	{500, 500, 500},
	{500, 500, 500},
	{500, 500, 500},
	{500, 500, 500},
	{500, 500, 500},
};*/

struct MapTextImg
{
	UInt16 x;
	UInt16 y;
	UInt16 width;
	UInt16 height;
};

MapTextImg gTextImgSet[4] = 
{
	{ 4, 4, 49, 15 },
	{ 51, 4, 61, 15 },
	{ 112, 4, 69, 15},
	{ 184, 4, 80, 15},
	
};


UInt8 gButtonX[3] = {29,82,135};
UInt8 gButtonY[6] = {150,184,117,84,217,50}; 

CBigMap::CBigMap()
{
	for (UInt8 i = 0; i < 6; ++i)
		m_MapTypeCount.push_back(0);

	m_iPathStationCount = 0;
	m_bOpenBigMap = false;
}

CBigMap::~CBigMap()
{
	m_ButtonMapIDLst.clear();
	m_MapTypeCount.clear();
}

/*
	BigMapA : ³¥¥~
	BigMapB : «°Âí
	BigMapC : °g®c
	BigMapD : À°¬£
	BigMapE : ¤s²ø
	BigMapF : ªù¬£

*/

void CBigMap::DrawButton()
{
 	UIButton* bigMapButton = NULL;
	UIStaticText* bigMapText = NULL;
	UIImage* wordImage = NULL;
	String Filename = _T("Data\\interface0025_main_J2.tga");
	CImage* img = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("BigMapPanel");


	for (UInt8 i = 0; i < 6; ++i)
		m_MapTypeCount[i] = 0;

	if (panel)
	{
		for (UInt32 i = 0; i < gGlobal.m_resource->GetMapCount(); ++i)
		{
			const BigMapData* pBigMap = gGlobal.m_resource->GetBigMapByIndex(i);
			
			if (pBigMap && pBigMap->use == 1)
			{
				StringA iconName = "BigMap";
				char abc = 65 + pBigMap->map_type;

				//bigmapbutton
				{
					iconName.Format("%s%c%dButton", iconName.c_str(), abc, m_MapTypeCount[pBigMap->map_type]);

					bigMapButton = SafeCreateObject(UIButton);
					gGlobal.pUIManager->AddUIID(iconName.c_str(), bigMapButton);
					bigMapButton->SetName(iconName.c_str());
					bigMapButton->SetEnable(true);
					bigMapButton->SetNotDetected(false);
					bigMapButton->SetDetectImage(true);
					bigMapButton->SetTimeOutCount(0);

					bigMapButton->SetLuaFunc(UI_FUNC_LBUTTON_UP, "ShowBigMapPath");
					bigMapButton->SetImage(img);

					UInt16 offsetX = 0;
					UInt16 offsetY = 0;

					if (pBigMap->flag_x > 10 )
						offsetX = pBigMap->flag_x - 10;

					if (pBigMap->flag_y > 10 )
						offsetY = pBigMap->flag_y - 10;

					bigMapButton->SetOffsetXY(offsetX, offsetY );

					UInt16 y = 0;
					for (UInt8 k = 0; k < 3; ++k)
					{
						bigMapButton->SetSubOffSet(k, bigMapButton->GetOffsetX(), bigMapButton->GetOffsetY());
						bigMapButton->SetRect(k, gButtonX[k], gButtonY[pBigMap->map_type], gButtonX[k] + 20, gButtonY[pBigMap->map_type] + 20);
					}

					bigMapButton->SetOffsetPos(0, 0);
					bigMapButton->SetStateDetectRect();
					bigMapButton->SetLuaState(gGlobal.pUIManager->GetLuaState());

					panel->AddUIItem(bigMapButton);		

					m_ButtonMapIDLst.insert(std::make_pair(iconName, pBigMap->map_id));

				}

				iconName = "BigMap";
				iconName.Format("%s%c%d", iconName.c_str(), abc, m_MapTypeCount[pBigMap->map_type]);

				UInt8 wordIndex = 0;
				if ( (UInt8)String(pBigMap->map_name).length() > 2)
					wordIndex = (UInt8)String(pBigMap->map_name).length() - 2;

				wordImage = SafeCreateObject(UIImage);
				gGlobal.pUIManager->AddUIID(iconName.c_str(), wordImage);
				wordImage->SetName(iconName.c_str());
				wordImage->SetEnable(true);
				wordImage->SetNotDetected(true);
				wordImage->SetDetectImage(false);
				wordImage->SetTimeOutCount(0);
				wordImage->SetImage(img, gTextImgSet[wordIndex].x, gTextImgSet[wordIndex].y, gTextImgSet[wordIndex].x + gTextImgSet[wordIndex].width, gTextImgSet[wordIndex].y + gTextImgSet[wordIndex].height);

				UInt16 offsetX = 0;
				UInt16 offsetY = 0;

				if (pBigMap->button_x > (gTextImgSet[wordIndex].width / 2) )
					offsetX = pBigMap->button_x - gTextImgSet[wordIndex].width / 2;

				if (pBigMap->button_y > (gTextImgSet[wordIndex].height / 2) )
					offsetY = pBigMap->button_y - gTextImgSet[wordIndex].height / 2;


				wordImage->SetOffsetXY(offsetX, offsetY);
				wordImage->SetDetectRect();
				wordImage->SetLuaState(gGlobal.pUIManager->GetLuaState());
				panel->AddUIItem(wordImage);

				StringA textName = "BigMap";
				textName.Format("%s%c%dText", textName.c_str(), abc, m_MapTypeCount[pBigMap->map_type]);
				UInt32 shadowColor = HexToDWORD("FF000000");

				bigMapText = SafeCreateObject(UIStaticText);
				//bigMapText->SetColor(TextColor[ColorIndex_White]);
				gGlobal.pUIManager->AddUIID(textName.c_str(), bigMapText);
				bigMapText->SetName(textName.c_str());
				bigMapText->SetEnable(true);
				bigMapText->SetNotDetected(true);
				bigMapText->SetDetectImage(false);
				bigMapText->SetTimeOutCount(0);

				StringA fontName = "Data\\simsun.ttc";
				bigMapText->SetOffsetPos(0, 0);
				bigMapText->SetFont(fontName.c_str(), 14, 14);
				bigMapText->SetAlign(UIStaticText::ALIGN_CENTER);
				bigMapText->SetLinePitch(0);
				bigMapText->SetText(String(pBigMap->map_name).c_str());

				bigMapText->SetFontStyle(EDGE_SHADOW);//EDGE_LIGHT);
				bigMapText->SetFontColor(TextColor[ColorIndex_White]);//Black]);
				bigMapText->SetShadowColor(shadowColor);
				bigMapText->SetOffsetXY(wordImage->GetOffsetX(), wordImage->GetOffsetY() + 1);

				bigMapText->SetImageWH(wordImage->GetWidth(), 16);
				bigMapText->SetLuaState(gGlobal.pUIManager->GetLuaState());
				bigMapText->DrawFont();

				panel->AddUIItem(bigMapText);	
				
				++m_MapTypeCount[pBigMap->map_type];
			}
		}

		UIAniIcon* pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem("BigMapLocation");
		UIAniIcon* pAniIcon2 = (UIAniIcon*)gGlobal.pUIManager->GetUIItem("BigMapDes");
		const BigMapBelongData* pBigMapBelongData = gGlobal.m_resource->GetBigMapBelong(gGlobal.g_pMap->GetMap_id());
		if (pAniIcon && pAniIcon2 && pBigMapBelongData)
		{
			UInt16 mapID = pBigMapBelongData->belongMap_id;
			const BigMapData* pBigMap = gGlobal.m_resource->GetBigMap(mapID);
			if (pBigMap && pBigMap->use == 1)
			{
				pAniIcon->SetOffsetXY(pBigMap->flag_x, pBigMap->flag_y - pAniIcon->GetHeight() / 2);
				panel->LeaveUIItem(pAniIcon);
				panel->AddUIItem(pAniIcon);
			
				panel->LeaveUIItem(pAniIcon2);
				panel->AddUIItem(pAniIcon2);


				if (m_iDesMap != 0)
				{
					UInt16Vector path;
					path.clear();
					FindPath(mapID, m_iDesMap, path);		
					m_iPathStationCount = (UInt16)path.size();
					if (m_iPathStationCount > 1)
					{
						DrawFlag(path);
						UICheckBox* pCheckBox = gGlobal.pUIManager->FindUICheckBox("BigMapCheck8");
						if (pCheckBox)
						{
							pCheckBox->SetState(true);
							CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowAllPath", String());
						}
					}
				}
			}
		}

		gGlobal.pUIManager->AddDirtyUIItem(panel);
	}
}

UInt32 CBigMap::GetButtonCount(int index)
{
	if (index < 6 && index >= 0)
	{
		return m_MapTypeCount[index];
	}
	return 0;
}

UInt16 CBigMap::GetMinPathMap(const std::map<UInt16, UInt16> openList)
{
	if (openList.size() != 0)
	{
		UInt16 tempMapID = openList.begin()->first;
		UInt16 step = openList.begin()->second;
		for (std::map<UInt16, UInt16>::const_iterator itr = openList.begin(); itr != openList.end(); ++itr)
		{
			if (step > itr->second)
			{
				step = itr->second;
				tempMapID = itr->first;
			}
		}

		return tempMapID;
	}
	return 0;
	
}


void CBigMap::FindPath(UInt16 src, UInt16 des, UInt16Vector &path)
{
	const BigMapData* pSrcBigMap = gGlobal.m_resource->GetBigMap(src);

	std::map<UInt16, UInt16> openList;
	std::map<UInt16, bool> closeList;
	std::map<UInt16, UInt16> prevList;

	std::map<UInt16, UInt16>::iterator iter;
	std::map<UInt16, UInt16>::iterator iter2;
	std::map<UInt16, bool>::iterator iter3;

	path.clear();
	
	UInt16 count = 0;
	if (pSrcBigMap)
	{
		const BigMapData* pBigMap = pSrcBigMap;
		
		openList.clear();
		closeList.clear();
		prevList.clear();

		openList.insert(std::make_pair(pSrcBigMap->map_id, 0));
		prevList.insert(std::make_pair(pSrcBigMap->map_id, pSrcBigMap->map_id));
		closeList.insert(std::make_pair(pSrcBigMap->map_id, false));
		
		UInt16 tempMapID = 0;

		while (!openList.empty() && des != tempMapID)
		{
			iter = openList.find(GetMinPathMap(openList));

			pBigMap = gGlobal.m_resource->GetBigMap(iter->first);

			if (pBigMap)
			{
				for (UInt8 i = 0; i < 12; ++i)
				{
					UInt32 mapID = 0;
					switch (i)
					{
					case 0:	mapID = pBigMap->adj_map1;	break;
					case 1:	mapID = pBigMap->adj_map2;	break;
					case 2: mapID = pBigMap->adj_map3;	break;
					case 3: mapID = pBigMap->adj_map4;	break;
					case 4: mapID = pBigMap->adj_map5;	break;
					case 5: mapID = pBigMap->adj_map6;	break;
					case 6: mapID = pBigMap->adj_map7;	break;
					case 7: mapID = pBigMap->adj_map8;	break; 
					case 8: mapID = pBigMap->adj_map9;	break;
					case 9: mapID = pBigMap->adj_map10;	break;
					case 10: mapID = pBigMap->adj_map11;	break;
					case 11: mapID = pBigMap->adj_map12;	break;
					}

					if (mapID == 0)
						break;
					
					iter3 = closeList.find(mapID);
					if ( iter3 != closeList.end() && iter3->second)
						continue;
					
					
					
					iter2 = openList.find(mapID);
					if ( iter2 != openList.end())
					{
						if (iter2->second > (iter->second + 1 ) )
						{
							iter2->second = iter->second + 1;
							prevList[iter2->first] = iter->first;
						}
					}
					else
					{
						openList.insert(std::make_pair(mapID, iter->second + 1) );
						prevList.insert(std::make_pair(mapID, iter->first) );
					}
				}

				tempMapID = iter->first;
				openList.erase(iter);
				
				iter3 = closeList.find(tempMapID);
				if (iter3 != closeList.end())
				{
					iter3->second = true;
				}
				else 
				{
					closeList.insert(std::make_pair(tempMapID, true));
				}			
			}
			else
			{
				break;
			}
		}
	
		tempMapID = des;
		path.push_back(des);
		while(!prevList.empty() && tempMapID != src)
		{
			iter2 = prevList.find(tempMapID);
			
			if (iter2 != prevList.end() )
			{
				tempMapID = iter2->second;
				path.push_back(tempMapID);
				prevList.erase(iter2);
			}
			else
			{
				break;
			}
		}
	}
}

void CBigMap::ShowPath()
{
	const BigMapBelongData* pBigMapBelongData = gGlobal.m_resource->GetBigMapBelong(gGlobal.g_pMap->GetMap_id());
	if (pBigMapBelongData == NULL)
		return;

	UInt16 mapID = pBigMapBelongData->belongMap_id;
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("BigMapPanel");
	String Filename = _T("Data\\interface0025_main_J.tga");
	//CImage* cimg = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);

	if (panel)// && cimg)
	{
		UIButton* button = (UIButton*)panel->GetMouseOver();//gGlobal.pUIManager->FindUIButton(panel->GetLuaOnLButtonUp() GetMouseOver());
		if (button)
		{
			MapNameList::iterator itr = m_ButtonMapIDLst.find(*button->GetName());

			if (itr != m_ButtonMapIDLst.end())
			{
				m_iDesMap = itr->second;
				if (m_iDesMap != 0)
				{
					UInt16Vector path;
					path.clear();
					FindPath(mapID, m_iDesMap, path);

					DisableAllFlag();

					m_iPathStationCount = (UInt16)path.size();

					if (m_iPathStationCount > 1)
					{
						DrawFlag(path);
					}
				}
			}
		}
	}
}

void CBigMap::StartBigMapDescMode(bool start)
{
	if (start)
	{
		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBigMapDescState));
		gGlobal.g_CursorState->Block();
	}
	else
	{
		gGlobal.g_CursorState->Unblock();
		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}
}

void CBigMap::ShowDesc()
{
	UIImage* npcIcon = gGlobal.pUIManager->FindUIImage("NPCBigMapImage");
	UIImage* chatBoxImage = gGlobal.pUIManager->FindUIImage("BigMapDesBox");
	UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel("BigMapDesc");
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("BigMapPanel");
	if (panel && npcIcon && pChatPanel && chatBoxImage)
	{
		UIButton* button = (UIButton*)panel->GetMouseOver();//gGlobal.pUIManager->FindUIButton(panel->GetLuaOnLButtonUp() GetMouseOver());
		if (button)
		{
			MapNameList::iterator itr = m_ButtonMapIDLst.find(*button->GetName());

			if (itr != m_ButtonMapIDLst.end())
			{
				const BigMapData* pBigMap = gGlobal.m_resource->GetBigMap(itr->second);
				if (pBigMap)
				{
					String fileName;
					fileName.Format(_T("Data\\NPCFACE\\%s.tga"), pBigMap->map_image_file);
					npcIcon->SetImageFile(fileName.c_str());
					
					pChatPanel->ClearAllRow();
					pChatPanel->AddStringW(String(pBigMap->description).c_str());
					pChatPanel->EndRow();
					pChatPanel->Redraw();
					
					pChatPanel->SetOffsetXY(pChatPanel->GetOffsetX(), npcIcon->GetHeight() + 14);
					chatBoxImage->SetOffsetXY(chatBoxImage->GetOffsetX(), npcIcon->GetHeight());

					gGlobal.pUIManager->AddDirtyUIItem(npcIcon);
					gGlobal.pUIManager->AddDirtyUIItem(pChatPanel);
					gGlobal.pUIManager->AddDirtyUIItem(chatBoxImage);

				}
			}

		}
	}
}

void CBigMap::DrawFlag(UInt16Vector path)
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("BigMapPanel");
	if (path.size() > 1 && panel)
	{
		UIAniIcon* des = (UIAniIcon*)gGlobal.pUIManager->GetUIItem("BigMapDes");
		const BigMapData* pDesBigMap = gGlobal.m_resource->GetBigMap(path[0]);
		if (des && pDesBigMap)
		{
			des->SetOffsetXY(pDesBigMap->flag_x + des->GetWidth() / 2 - 8, pDesBigMap->flag_y - des->GetHeight() / 2);
			des->SetEnable(true);
			des->GetAniEnt()->SetStopAnimation(false);
		}

		for (UInt16 i = 1; i < path.size() - 1; ++i)
		{
			StringA imgName;
			imgName.Format("BigPathStation%d", i);
			
			UIImage* img = gGlobal.pUIManager->FindUIImage(imgName.c_str());
			UIAniIcon* flag = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(imgName.c_str());

			const BigMapData* pBigMap = gGlobal.m_resource->GetBigMap(path[i]);
			if (pBigMap)
			{
				if (flag)
				{
					flag->SetOffsetXY(pBigMap->flag_x + flag->GetWidth() / 2 - 6, pBigMap->flag_y - flag->GetHeight() / 2);
					flag->SetEnable(true);
					flag->GetAniEnt()->SetStopAnimation(false);
				}
				else
				{
					flag = SafeCreateObject(UIAniIcon);
					gGlobal.pUIManager->AddUIID(imgName.c_str(), flag);
					flag->SetName(imgName.c_str());
					flag->SetEnable(true);
					flag->SetNotDetected(false);
					
					flag->SetTimeOutCount(0);
					flag->SetAlign(1);
					flag->SetAnimationIndex(0);
					flag->SetRotation(0);
				
					flag->SetAnimationData(String(_T("CursorP.ent")).c_str());
					flag->SetOffsetXY(pBigMap->flag_x + flag->GetWidth() / 2 - 6, pBigMap->flag_y - flag->GetHeight() / 2);
					flag->SetDetectRect();
					flag->SetLuaState(gGlobal.pUIManager->GetLuaState());
					gGlobal.pUIManager->AddUpdateUIItem(flag);
					panel->AddUIItem(flag);

				}
			}
		}
	}
}

void CBigMap::DisableAllFlag()
{
	UIAniIcon* des = (UIAniIcon*)gGlobal.pUIManager->GetUIItem("BigMapDes");
	if (des)
	{
		des->SetOffsetXY(0, 0);
		des->SetEnable(false);
		des->GetAniEnt()->SetStopAnimation(true);
	}

	for (UInt16 i = 1; i < m_iPathStationCount; ++i)
	{
		StringA imgName;
		imgName.Format("BigPathStation%d", i);
		UIAniIcon* flag = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(imgName.c_str());
		if (flag)
		{
			flag->SetOffsetXY(0,0);
			flag->SetEnable(false);
			flag->GetAniEnt()->SetStopAnimation(true);
		}
	}

	m_iPathStationCount = 0;
}
