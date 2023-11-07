//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "BattleControl.h"
//-- Library
#include "Math.h"
#include "TextEntManager.h"
#include "QGLApplication.h"
#include "Global.h"
#include "Proxy/Logic/BatActor.h"
#include "Proxy/Logic/Battle.h"
#include "Proxy/Logic/Pet.h"
#include "Proxy/Logic/PetGroup.h"
#include "Common/StateCode.h"
#include "Proxy/Logic/BatMob.h"
#include "Proxy/Logic/BatCharacter.h"
#include "Proxy/Logic/Party.h"
#include "Proxy/Logic/ItemBag.h"
#include "Configure.h"
#include "AnimatedEntity.h"
#include "Common/CallLuaFunc.h"
#include "MessageBox.h"
#include "Resources.h"
#include "MiddleMap.h"
#include "MenuSkillProcess.h"
#include "Resource/resskill.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "MenuPartnerProcess.h"
#include "Common/common_binary.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResBuff.h"
#include "EntityBase.h"
#include "BatBubbleControl.h"
#include "BatScriptControl.h"
#include "MenuSystemSetting.h"
#include "Resource/ResPet.h"
#include "AntiHack.h"
#include "CrossMapPathFinder.h"

enum BatEndEnum {				// battle result enum
	END_WIN = 1,				// player team win
	END_LOSE,					// player team lose
	END_ESCAPED,				// player team escaped
	END_DRAW,					// no team win or lose 
	END_MONITOR,				// monitor end
};

#define ENEMY_TEAM 0
#define PLAYER_TEAM 1

typedef StlMap<UInt32, UInt32, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, UInt32> > > ItemCountMap;

static UInt counterTime = ::timeGetTime();

PROTO_LuaPush(CBattleControl)
PROTO_LuaPush(CCharacterControl)

CONST Int SameTargetDelay = 500;
CONST Int HpBarOffsetY = 15;
CONST Int skipX = 4;
CONST Int skipY = 4;
CONST Int BaseX = -3;
CONST Int BaseY = 34;
CONST Int ChessboardMaxX = 25;
CONST Int ChessboardMaxY = 37;
CONST Byte SubValue = 0;
CONST Int BattleGridSizeX = 9;
CONST Int BattleGridSizeY = 9;
CONST Int HitPoint = -99;
CONST Int FinishWaitTime = 500;
CONST Int FadeOutTime = 1000;
CONST UInt32 CountDownTotalTime = 20000;
CONST UInt32 SkillDelayTime = 0;
CONST UInt MAXSkill = 7;
CONST PCSTRINGW fontAddEnt = _T("font_hurt_yellow.ent");
CONST PCSTRINGW fontSubEnt = _T("font_hurt_blue.ent");
CONST PCSTRINGW fontChiEnt = _T("font_hurt_chi.ent");
CONST PCSTRINGW CrtHitEnt = _T("effHurt_critical.ent");
CONST PCSTRINGW testComEffect = _T("effAtk0069.ent");

CONST PCSTRINGW hurtNum = _T("font_hurt_red.ent");
CONST PCSTRINGW criHurtNum = _T("font_hurt_crt.ent");
CONST PCSTRINGW healNum = _T("font_hurt_green.ent");
CONST PCSTRINGW countNum = _T("font_count.ent");
CONST PCSTRINGW defSpark = _T("effDef0001.ent");
CONST PCSTRINGW Spark = _T("effHurt0000.ent");
CONST PCSTRINGW escapeEffect = _T("effQuit.ent");
CONST PCSTRINGW hpBarRed = _T("HpBarRed.ent");
CONST PCSTRINGW hpBarDRed = _T("HpBarDRed.ent");
CONST PCSTRINGW hpBarBack = _T("HpBarBack.ent");


enum PLAYER_SOUND_TYPE
{
	PLAYER_SOUND_SKILL = 0,
	PLAYER_SOUND_CATCH,
	PLAYER_SOUND_HURT,
	PLAYER_SOUND_DIE,
	PLAYER_SOUND_ATK,
	MAX_PLAYER_SOUND_TYPE,
};
CONST PCSTRINGW SoundNameArray[SEXCLASS_F_ASSASSIN + 1][MAX_PLAYER_SOUND_TYPE] = 
{
	{_T(""),_T(""),_T(""),_T(""),_T("")},
	{_T("SoundB0021_mskill_class1.wav"),_T("SoundB0023_mcatch_class1.wav"),_T("SoundB0025_mhurt_class1.wav"),_T("SoundB0027_mdie_class1.wav"),_T("SoundB0070_mattack_class1.wav")}, 
	{_T("SoundB0071_wattack_class2.wav"),_T("SoundB0024_wcatch_class2.wav"),_T("SoundB0026_whurt_class2.wav"),_T("SoundB0028_wdie_class2.wav"),_T("SoundB0071_wattack_class2.wav")}, 
	{_T("SoundB0072_mskill_class3.wav"),_T("SoundB0074_mcatch_class3.wav"),_T("SoundB0076_mhurt_class3.wav"),_T("SoundB0078_mdie_class3.wav"),_T("SoundB0080_mattack_class3.wav")},
	{_T("SoundB0073_wskill_class4.wav"),_T("SoundB0075_wcatch_class4.wav"),_T("SoundB0077_whurt_class4.wav"),_T("SoundB0079_wdie_class4.wav"),_T("SoundB0081_wattack_class4.wav")},
	{_T("SoundB0082_mskill_class5.wav"),_T("SoundB0084_mcatch_class5.wav"),_T("SoundB0086_mhurt_class5.wav"),_T("SoundB0088_mdie_class5.wav"),_T("SoundB0090_mattack_class5.wav")}, 
	{_T("SoundB0083_wskill_class6.wav"),_T("SoundB0085_wcatch_class6.wav"),_T("SoundB0087_whurt_class5.wav"),_T("SoundB0089_wdie_class6.wav"),_T("SoundB0091_wattack_class6.wav")},
	{_T("SoundB0092_mskill_class7.wav"),_T("SoundB0094_mcatch_class7.wav"),_T("SoundB0096_mhurt_class7.wav"),_T("SoundB0098_mdie_class7.wav"),_T("SoundB0100_mattack_class7.wav")},
	{_T("SoundB0093_wskill_class8.wav"),_T("SoundB0095_wcatch_class8.wav"),_T("SoundB0097_whurt_class8.wav"),_T("SoundB0099_wdie_class8.wav"),_T("SoundB0101_wattack_class8.wav")},
};
INLINE float GetRand(float fmax)
{
	float r = (float) gGlobal.m_rand.GetRandom(INT32_MAX_VALUE);

	return ((r / INT32_MAX_VALUE) * fmax);
}

INLINE int GetRand(int imax)
{
	return gGlobal.m_rand.GetRandom(imax);
}

CBattleControl::CBattleControl():m_S1(""),m_S2("")
{
	m_BattleParticle = SafeCreateObject(CParticleSystem);
	pControlPlayer = NULL;
	m_BattleFinishTime = 0;
	pBattle = NULL;
	m_LeaveBattle = TRUE;
	m_CommandAvial = FALSE;
	m_PlayingBattleAnim = FALSE;
	pControlPartner = NULL;
	m_PreUpdateTime = 0;
	m_CountDownTime = 0;
	m_pStaticTextist = NULL;
	m_pStaticTextsnd = NULL;
	m_iShowState=0;
	
	m_CountDownEnt.SetFileNameW(countNum);
	for (Int i = 0; i < (Int)m_CountDownEnt.m_NumEntLst.size(); i ++)
	{
		m_CountDownEnt.m_NumEntLst[i]->LockResources();
	}
	m_CountDownEnt.SetExtraZValue(0x20000000);

	m_NextAttackQueueTime = 0;
	m_ExtraWaitTime = 0;

	m_BattleItemUseIndex = -1;

	m_CurrAutoCount = 0;

	m_objects = SafeCreateObject(NetGroup);

	m_StartCountDown = FALSE;

	m_PlayerTeamIndex = 0;
	m_EnemyTeamIndex = 0;

	m_pBatBubbleCtrl = SafeCreateObject(CBatBubbleControl);

	m_CurrTeam = 0;
	m_AutoCountBox.FreeString();

	m_tempPartnerObject = NULL;

}

CBattleControl::~CBattleControl()
{
	FreeBattleEntData();
	
	if (m_BattleMapData.BgTexture != NULL)
		m_BattleMapData.BgTexture->Release();

	if (m_BattleMapData.pInFile)
	{
		UnmapViewOfFile(m_BattleMapData.pInFile);
		m_BattleMapData.pInFile = NULL;
	}
	if (m_BattleMapData.hMapFile)
	{
		CloseHandle(m_BattleMapData.hMapFile);
		m_BattleMapData.hMapFile = NULL;
	}
	if (m_BattleMapData.hFile)
	{
		CloseHandle(m_BattleMapData.hFile);
		m_BattleMapData.hFile = NULL;
	}

	if (m_BattleMapData.pBgOrgTextureData != NULL)
		SafeDeallocate(m_BattleMapData.pBgOrgTextureData);

	SafeDeleteObject(m_BattleParticle);

	DeleteNetChilds(m_objects);
	SafeDeleteObject(m_objects);

	SafeDeleteObject(m_pBatBubbleCtrl);
}

VOID CBattleControl::FreeBattleEntData()
{
	for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].pChar->GetBatActorObject() && 
			m_BattlePlayerLst[i].pChar->GetBatActorObject()->GetClassID() == BatMob_CLASSID)
			m_BattlePlayerLst[i].pChar->GetCharMainView()->FreeEntityData();
		QGLApplication::GetInstance().RemoveEntFromDisposalList(m_BattlePlayerLst[i].pChar->GetCharMainView());
		SafeDeleteObject(m_BattlePlayerLst[i].pChar);
		if (m_BattlePlayerLst[i].pPet)
		SafeDeleteObject(m_BattlePlayerLst[i].pPet);
	}
	m_BattlePlayerLst.clear();

	for (Int i = 0; i < (Int)m_BattlePlayerHPBarBackLst.size(); i++)
	{
		SafeDeleteObject(m_BattlePlayerHPBarBackLst[i]);
	}

	m_BattlePlayerHPBarBackLst.clear();

	for (Int i = 0; i < (Int)m_BattlePlayerHPBarLst.size(); i++)
	{
		SafeDeleteObject(m_BattlePlayerHPBarLst[i]);
	}

	m_BattlePlayerHPBarLst.clear();

	pControlPartner = NULL;

	pControlPlayer = NULL;
	DeleteNetChilds(m_objects);

	m_EffectDataLst.clear();

	for (StlMap<String, EffectData>::iterator Itr = m_PlayedEffect.begin(); 
		Itr != m_PlayedEffect.end(); ++Itr)
	{
		CResourcesManager::GetInstance().ReleaseEntityData(Itr->first);
	}

	m_PlayedEffect.clear();

	if (m_BattleMapData.pBgOrgTextureData != NULL)
	{
		SafeDeallocate(m_BattleMapData.pBgOrgTextureData);
		m_BattleMapData.pBgOrgTextureData = NULL;
	}
	

}

VOID CBattleControl::AddCharacterToBattleLst(CCharacterControl * pGivChar, LPDIRECT3DDEVICE9	pD3DDevice, Int BattlePosX, 
		Int BattlePosY, BOOL AddHpBar, CCharacterControl * pGivPet)
{
	if (pGivChar == NULL)
		return;

	CCharacterControl * tempChar = pGivChar;

	BattleEntData EntData;
	EntData.pChar = tempChar;
	EntData.pPet = pGivPet;
	EntData.BattlePoxX = BattlePosX;
	EntData.BattlePoxY = BattlePosY;
	EntData.OrgRotation = pGivChar->GetCharMainView()->GetAnimationRotation();

	if (AddHpBar)
	{
		CAnimatedEntity * HpBarBack = SafeCreateObject(CAnimatedEntity);
		HpBarBack->LoadEntityFile(hpBarBack, pD3DDevice);
		HpBarBack->SetEntParent(tempChar->GetCharMainView());
		HpBarBack->SetExtraZBuffer(1);
		HpBarBack->SetEntityType(ENTITY_TYPE_PARTICLE_WITHOUTEFFECT);
		HpBarBack->SetCharPos(0 , 0);
		HpBarBack->SetEntOffsetY(HpBarOffsetY);

		m_BattlePlayerHPBarBackLst.push_back(HpBarBack);
		EntData.pEntHpBarBack = HpBarBack;

		CAnimatedEntity * HpBar = SafeCreateObject(CAnimatedEntity);
		HpBar->LoadEntityFile(hpBarRed, pD3DDevice);
		HpBar->SetEntParent(tempChar->GetCharMainView());
		HpBar->SetExtraZBuffer(2);
		HpBar->SetEntityType(ENTITY_TYPE_PARTICLE_WITHOUTEFFECT);
		HpBar->SetCharPos(0 , 0);
		HpBar->SetEntOffsetY(HpBarOffsetY);

		CAnimatedEntity * HpDBar = SafeCreateObject(CAnimatedEntity);
		HpDBar->LoadEntityFile(hpBarDRed, pD3DDevice);
		HpDBar->SetEntParent(tempChar->GetCharMainView());
		HpDBar->SetExtraZBuffer(1);
		HpDBar->SetEntityType(ENTITY_TYPE_PARTICLE_WITHOUTEFFECT);
		HpDBar->SetCharPos(0 , 0);
		HpDBar->SetEntOffsetY(HpBarOffsetY);

		

		Real32 ScaleX = (Real32)pGivChar->GetBattleHp() / 
			(Real32)pGivChar->GetBattleMaxHp();

		HpBar->SetScaleX(ScaleX);

		ScaleX = (Real32)pGivChar->GetBattleMHp() / 
			(Real32)pGivChar->GetBattleMaxHp();

		HpDBar->SetScaleX(ScaleX);

		m_BattlePlayerHPBarLst.push_back(HpBar);
		m_BattlePlayerHPBarLst.push_back(HpDBar);
		EntData.pEntHpBar = HpBar;
		EntData.pEntHpDBar = HpDBar;
		
	}

	EntData.Index = (Int)(m_BattlePlayerLst.size());
	m_BattlePlayerLst.push_back(EntData);

	//add back bat actor object
	if (pGivChar->GetBatActorObject())
	{
		m_objects->Add(pGivChar->GetBatActorObject());
	}

	pGivChar->GetCharMainView()->SetEntityType(ENTITY_TYPE_BATTLE_PLAYER);
	pGivChar->GetCharShadowView()->SetEntityType(ENTITY_TYPE_BATTLE_PLAYER);
}

VOID CBattleControl::ChangeWeapon(Int Index)
{
	for (int i = 0 ; i < (int)m_BattlePlayerLst.size(); i++)
	{
		m_BattlePlayerLst[i].pChar->GetCharMainView()->SetAnimationIndexByIndex(Index, TRUE);
	}
}

VOID CBattleControl::InitBattleData(CRender * pRender, LPCTSTR filename)
{
	if (m_BattleMapData.BgTexture != NULL)
		m_BattleMapData.BgTexture->Release();

	if( FAILED(D3DXCreateTexture(pRender->GetDevice(), pRender->GetWinWidth(), pRender->GetWinHeight()
		, 0, 0 ,D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH  , 
		&m_BattleMapData.BgTexture)))
		DebugBreak();

	if( g_GdfResource || g_GdfResourceEx )
	{
		String bdfilename(filename);
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(bdfilename, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(bdfilename, beginOffset, fileSize);
		if( fbuf )
		{
			m_BattleMapData.pInFile = fbuf;
			m_BattleMapData.width = *(DWORD *)(m_BattleMapData.pInFile + beginOffset);
			m_BattleMapData.height = *(DWORD *)(m_BattleMapData.pInFile + 4 + beginOffset);
			m_BattleMapData.pPicData = m_BattleMapData.pInFile + 8 + beginOffset;
			
		}
	}
	else
	{
		m_BattleMapData.hFile = CreateFile(filename, FILE_READ_DATA, FILE_SHARE_READ, 
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

		String fileNameStr = filename;
		String::size_type pos = fileNameStr.find_last_of(_T("\\"));

		if (pos != String::npos)
		{
			fileNameStr = fileNameStr.substr(pos + 1);
		}

		m_BattleMapData.hMapFile = CreateFileMapping(m_BattleMapData.hFile, NULL, PAGE_READONLY, 
			0, 0, fileNameStr.c_str());

		m_BattleMapData.pInFile = (Byte *)MapViewOfFile(m_BattleMapData.hMapFile, FILE_MAP_READ, 0, 0, 0);
		if (m_BattleMapData.pInFile && m_BattleMapData.hMapFile && m_BattleMapData.hFile)
		{
			m_BattleMapData.width = *(DWORD *)(m_BattleMapData.pInFile);
			m_BattleMapData.height = *(DWORD *)(m_BattleMapData.pInFile + 4);
			m_BattleMapData.pPicData = m_BattleMapData.pInFile + 8;

		}
	}
}
VOID CBattleControl::CreateBgTexture(CMapObjectView * pMap, CRender * pRender)
{
	Int SubColor = 0; 
	if (m_BattleMapData.pBgOrgTextureData != NULL)
		SafeDeallocate(m_BattleMapData.pBgOrgTextureData);

	m_BattleMapData.Alpha = 255;

	RECT WinRect;
	WinRect.left = WinRect.top = 0;
	WinRect.right = pRender->GetWinWidth();
	WinRect.bottom = pRender->GetWinHeight();

	pRender->LockTexture();
	//draw the background first
	pRender->AddAllDirtyRect();
	StlMultiMap<UInt32, CEntityBase *> Empty;
	pRender->CameraMoveDraw(QGLApplication::GetInstance().GetRenderMap(), Empty , QGLApplication::GetInstance().GetRenderCamera(),
		gGlobal.g_NightMaskTextureData);
	RECT SrcRect = pRender->GetCurrDisRect();
	pRender->DrawBgMapByCurrDirtyRect(QGLApplication::GetInstance().GetRenderMap(), QGLApplication::GetInstance().GetRenderCamera());
	if (gGlobal.Night && gGlobal.NightMask)
	{
		D3DSURFACE_DESC  pDescSrc;
		D3DLOCKED_RECT d3dltSrc;

		if (CRender::LockSystemTexture(pDescSrc, d3dltSrc, &gGlobal.g_NightMaskTextureData.m_CurrDisArea
			, gGlobal.g_NightMaskTextureData.Texture))
		{
			CRender::BlitPic((Byte *)d3dltSrc.pBits, 0, pRender->GetWinWidth(), pRender->GetWinHeight(),
				pDescSrc.Width, pRender->GetSystemCombinedData()->pData + (SrcRect.top * MaxEntSizeX + SrcRect.left) * 4
				, 0, MaxEntSizeX, FALSE, 255 - gGlobal.g_NightMaskAlpha, 4, 4);
			gGlobal.g_NightMaskTextureData.Texture->UnlockRect(0);
		}

	}
	pRender->AddAllDirtyRect();

	D3DSURFACE_DESC  pDescDes;

	D3DLOCKED_RECT d3dltDes;

	
	{

		BYTE * pSrcTData = pRender->GetSystemCombinedData()->pData + (SrcRect.top * MaxEntSizeX + SrcRect.left) * 4;

		if(CRender::LockSystemTexture(pDescDes, d3dltDes, 0, m_BattleMapData.BgTexture))
		{
			BYTE * pDesTData = (BYTE *)d3dltDes.pBits;

			for ( Int i = 0; i <  SrcRect.bottom - SrcRect.top; i ++)
			{
				memcpy(pDesTData, pSrcTData, (SrcRect.right - SrcRect.left) * 4);
				pDesTData += d3dltDes.Pitch;
				pSrcTData += MaxEntSizeX * 4;
			}

			if (SubColor > 0)
			{
				//substrative blend
				pDesTData = (BYTE *)d3dltDes.pBits;

				for ( Int y = 0; y <  (Int)pDescDes.Height; y ++)
				{
					for ( Int x = 0; x <  (Int)pDescDes.Width; x ++)
					{
						pDesTData[0] = gGlobal.SubtractiveTable[pDesTData[0]][(BYTE)SubColor];
						pDesTData[1] = gGlobal.SubtractiveTable[pDesTData[1]][(BYTE)SubColor];
						pDesTData[2] = gGlobal.SubtractiveTable[pDesTData[2]][(BYTE)SubColor];
						pDesTData += 4;
					}
				}
			}

			pDesTData = (BYTE *)d3dltDes.pBits;
			m_BattleMapData.pBgOrgTextureData = SafeAllocate(Byte, d3dltDes.Pitch * pDescDes.Height);
			memcpy(m_BattleMapData.pBgOrgTextureData, pDesTData, d3dltDes.Pitch * pDescDes.Height);

			m_BattleMapData.BgTexture->UnlockRect(0);
		}
	}

	pRender->UnLockTexture();
	QGLApplication::GetInstance().AddParticleSystem(m_BattleParticle);

	m_BattleMapData.State = BATTLE_STATE_FADE_IN;
	m_BattleMapData.StartBattleTime = ::timeGetTime();
	m_BattleMapData.FadeInOutTime = 500;

}

VOID CBattleControl::UpdateBattleBgMap(UInt32 CurrTime)
{
	if (m_BattleMapData.State == BATTLE_STATE_FADE_IN)
	{
		if (CurrTime - m_BattleMapData.StartBattleTime < m_BattleMapData.FadeInOutTime)
		{
			Byte AlphaValue = (Byte)((1 - (float)(CurrTime - m_BattleMapData.StartBattleTime) / 
				(float)m_BattleMapData.FadeInOutTime) * 255);

			if (m_BattleMapData.Alpha != AlphaValue)
			{
				QGLApplication::GetInstance().GetRenderClass()->AddAllDirtyRect();
				m_BattleMapData.Alpha = AlphaValue;

				D3DSURFACE_DESC  pDescDes;
				D3DLOCKED_RECT d3dltDes;
				if(CRender::LockSystemTexture(pDescDes, d3dltDes, 0, m_BattleMapData.BgTexture))
				{
					Byte * pDesTData = (byte *)d3dltDes.pBits;
					memcpy(pDesTData, m_BattleMapData.pBgOrgTextureData, d3dltDes.Pitch * pDescDes.Height);

					{
						Byte * pSrcData = m_BattleMapData.pPicData;

						CRender::BlitPic(pSrcData, 0, (WORD)pDescDes.Width, (WORD)pDescDes.Height,
							(WORD)m_BattleMapData.width, pDesTData, 0, 
							(WORD)pDescDes.Width, false, m_BattleMapData.Alpha
							, sizeof(Color32), sizeof(Color32));
					}
					m_BattleMapData.BgTexture->UnlockRect(0);
				}
			}

		}
		else
		
		{
			m_BattleMapData.Alpha = 0;
			QGLApplication::GetInstance().GetRenderClass()->AddAllDirtyRect();
			m_BattleMapData.State = BATTLE_STATE_INWAR;

			D3DSURFACE_DESC  pDescDes;
			D3DLOCKED_RECT d3dltDes;

			if(CRender::LockSystemTexture(pDescDes, d3dltDes, 0, m_BattleMapData.BgTexture))
			{
				Byte * pDesTData = (byte *)d3dltDes.pBits;
				memcpy(pDesTData, m_BattleMapData.pBgOrgTextureData, d3dltDes.Pitch * pDescDes.Height);

				{
					Byte * pSrcData = m_BattleMapData.pPicData;

					
					CRender::BlitPic(pSrcData, 0, (WORD)pDescDes.Width, (WORD)pDescDes.Height,
						(WORD)m_BattleMapData.width, pDesTData, 0, 
						(WORD)pDescDes.Width, false, m_BattleMapData.Alpha
						, sizeof(Color32), sizeof(Color32));
				}
				m_BattleMapData.BgTexture->UnlockRect(0);
			}
			if (m_BattleMapData.pBgOrgTextureData != NULL)
			{
				SafeDeallocate(m_BattleMapData.pBgOrgTextureData);
				m_BattleMapData.pBgOrgTextureData = NULL;
			}
			
		}
	}
	else if (m_BattleMapData.State == BATTLE_STATE_FADE_OUT)
	{
		if (CurrTime - m_BattleMapData.StartBattleTime < m_BattleMapData.FadeInOutTime)
		{
			Byte AlphaValue = (Byte)(((float)(CurrTime - m_BattleMapData.StartBattleTime) / 
				(float)m_BattleMapData.FadeInOutTime) * 255);

			if (m_BattleMapData.Alpha != AlphaValue)
			{
				QGLApplication::GetInstance().GetRenderClass()->AddAllDirtyRect();
				m_BattleMapData.Alpha = AlphaValue;
			}
		}
		else
		{
			QGLApplication::GetInstance().GetRenderClass()->AddAllDirtyRect();
			m_BattleMapData.Alpha = 255;
			m_BattleMapData.State = BATTLE_STATE_NO_BATTLE;
			QGLApplication::GetInstance().SetBgTexture(NULL);
			
		}
	}
	
}
VOID CBattleControl::UpdateAttackQueue(UInt32 CurrTime)
{
	StlDeque<AttackQuene>::iterator Itr = m_AttackQueneData.begin();
	if (Itr != m_AttackQueneData.end() && CurrTime > m_NextAttackQueueTime)
	{
		//if (m_EntityBeingAttackLst.size() > 0)
		//	return;

		//TRACE_INFODTL_1(GLOBAL_LOGGER, _F("AnimTime: attack elapsed time %d"), 
		//CurrTime -   m_NextAttackQueueTime);

		for (Int i = 0; i < (Int)Itr->AttackerLst.size(); i++)
		{
			if (Itr->AttackerLst[i].AttackerIndex >= (Int)m_BattlePlayerLst.size())
			{
				m_AttackQueneData.erase(Itr);
				return;
			}
			//find whether the entity is moving
			if (Itr->AttackerLst[i].AttackerIndex >= (Int)m_BattlePlayerLst.size())
			{
				m_AttackQueneData.erase(Itr);
				return;
			}
			if (m_BattlePlayerLst[Itr->AttackerLst[i].AttackerIndex].pChar->GetCharMainView()->m_animationset.size() > 0)
				return;

			if (m_BattlePlayerLst[Itr->AttackerLst[i].AttackerIndex].pChar->GetCharMainView()->IsMoving())
				return;
			
			if (m_BattlePlayerLst[Itr->AttackerLst[i].AttackerIndex].pChar->GetCharMainView()->GetAnimationIndex() == ANIM_DIE && 
				!m_BattlePlayerLst[Itr->AttackerLst[i].AttackerIndex].pChar->GetCharMainView()->FrameEndNoOthers(::timeGetTime()))
				return;

		}

		for (Int i = 0; i < (Int)Itr->DefenserLst.size(); i++)
		{
			if (Itr->DefenserLst[i].DefenserIndex >= (Int)m_BattlePlayerLst.size())
			{
				m_AttackQueneData.erase(Itr);
				return;
			}
			//find whether the entity is moving
			if (m_BattlePlayerLst[Itr->DefenserLst[i].DefenserIndex].pChar->GetCharMainView()->IsMoving())
				return;

			if (m_BattlePlayerLst[Itr->DefenserLst[i].DefenserIndex].pChar->GetCharMainView()->m_animationset.size() > 0)
				return;

			if (m_BattlePlayerLst[Itr->DefenserLst[i].DefenserIndex].pChar->GetCharMainView()->GetAnimationIndex() == ANIM_DIE && 
				!m_BattlePlayerLst[Itr->DefenserLst[i].DefenserIndex].pChar->GetCharMainView()->FrameEndNoOthers(::timeGetTime()))
				return;
		}

		m_NextAttackQueueTime = CurrTime;
		//check escape
		if (Itr->AttackerLst.size() > 0 && 
			Itr->AttackerLst[0].AtkAction == BATCMD_ESCAPE)
		{
			CCharacterControl* pCharCtrl = m_BattlePlayerLst[Itr->AttackerLst[0].AttackerIndex].pChar;
			CBatActor* pActor = pCharCtrl->GetBatActorObject();

			// show escape bubble msg
			if (pActor)
			{
				if (pActor->GetState() != CHAR_ST_DEAD)
				{
					ShowBattleBubbleMsg(Itr->AttackerLst[0].AttackerIndex, BATBUBBLE_ESCAPE);
					
					PerformEscape(m_BattlePlayerLst[Itr->AttackerLst[0].AttackerIndex].pChar, 
						Itr->AttackerLst[0].petActive);

				}
				else
				{
					pCharCtrl->GetCharMainView()->SetVisible(FALSE);
				}
			}
			//if (m_BattlePlayerLst[Itr->AttackerLst[0].AttackerIndex].pChar == pControlPartner)
			//{
			//	gGlobal.g_pPartnerProcess->UpdateSmallIcon(false);
			//	pControlPartner = NULL;
			//}

			//if (m_BattlePlayerLst[Itr->AttackerLst[0].AttackerIndex].pChar == pControlPlayer)
			//{
			//	m_PlayingBattleAnim = FALSE;
			//	if (pBattle)
			//	{
			//		pBattle->EndTurn(gGlobal.pPeerProxy, RPCContext());
			//	}
			//	m_AttackQueneData.clear();
			//	return;
			//}
			
		}
		else if (Itr->AttackerLst.size() > 0 && 
			Itr->AttackerLst[0].AtkAction == BATCMD_ESCAPE_FAIL)
		{
			PerformEscapeFail(m_BattlePlayerLst[Itr->AttackerLst[0].AttackerIndex].pChar);
		}
		else if (Itr->AttackerLst.size() > 0 && 
			Itr->AttackerLst[0].AtkAction == BATCMD_CATCH)
		{
			if (Itr->DefenserLst[0].DefReaction == BATCMD_CATCHED)
				PerformCatch(m_BattlePlayerLst[Itr->AttackerLst[0].AttackerIndex].pChar, m_BattlePlayerLst[Itr->DefenserLst[0].DefenserIndex].pChar, true, Itr->AttackerLst[0].petActive);
			else
			{
				PerformCatch(m_BattlePlayerLst[Itr->AttackerLst[0].AttackerIndex].pChar, m_BattlePlayerLst[Itr->DefenserLst[0].DefenserIndex].pChar, false, Itr->AttackerLst[0].petActive);
			}
			

			// show catch bubble msg
			ShowBattleBubbleMsg(Itr->DefenserLst[0].DefenserIndex, BATBUBBLE_CATCH);
		}
		else if (Itr->AttackerLst.size() > 0 && (Itr->AttackerLst[0].Skill != 0 || Itr->AttackerLst[0].Item != 0))
		{
			if (Itr->DefenserLst.size() > 0 && (Itr->DefenserLst[0].Attri.OldState !=  CHAR_ST_DEAD || Itr->DefenserLst[0].Attri.HPChange > 0))
				OnSkillAttack(*Itr);
			else
			{
				CCharacterControl * pChar = m_BattlePlayerLst[Itr->AttackerLst[0].AttackerIndex].pChar;
				if (pChar->GetCharMainView()->GetEntOffsetX() != 0 ||
					pChar->GetCharMainView()->GetEntOffsetY() != 0)
				{
					Int RunSpeed = gConfigure.uRunSpeed;
					Int RunTime = (Int)sqrt(pow((double)pChar->GetCharMainView()->GetEntOffsetX() , (int)2) + 
						pow((double)pChar->GetCharMainView()->GetEntOffsetY(), (int)2)) * 1000 / RunSpeed;

					Int OrgRotation = pChar->GetCharMainView()->GetAnimationRotation();
					pChar->GetCharMainView()->AppendAnimationSet(ANIM_RUN, 
						-1 * pChar->GetCharMainView()->GetEntOffsetX(), 
						-1 * pChar->GetCharMainView()->GetEntOffsetY(), 0, 
						RunTime, TRUE, QGLMath::GetOppRotation(OrgRotation), CurrTime,0,
						FALSE, TRUE,  0, 0, 0, 0, 0, 0, 0);

					pChar->GetCharMainView()->AppendAnimationSet(pChar->GetCharMainView()->GetBattleAnimation(), 
						0, 0, 0, 0, TRUE, OrgRotation, CurrTime +  RunTime,0,
						FALSE,FALSE,  0, 0, 0, 0, 0, 0, 0);
				}
			}
		}
		else if (Itr->DefenserLst.size() > 0 && Itr->DefenserLst[0].DefReaction == BATCMD_SHOW_FRIEND)
		{
			CBatActor * pActor = m_BattlePlayerLst[Itr->DefenserLst[0].DefenserIndex].pChar->GetBatActorObject();

			if (pActor)
			{				
				if (m_tempPartnerObject)
				{
					bool found = false;
					for (UInt i = 0; i < m_tempPartnerObject->GetCount(); ++i)
					{
						CBatMob* partner = m_tempPartnerObject->GetChild<CBatMob>(i);
						if (partner && pActor->GetClassID() == BatCharacter_CLASSID)
						{
							CBatCharacter* charTemp = CastNetObject<CBatCharacter>(pActor);
							if (charTemp && partner->GetOwner_id() == charTemp->GetChar_id())
							{
								NetGroup temp;
								temp.Add(m_tempPartnerObject->GetChild(i));							
								PerformJumpEnterBattle(m_BattlePlayerLst[partner->GetIndex()].pChar);
								gGlobal.g_pPartnerProcess->UpdateSmallIcon(true, partner->GetPartner_id());
								found = true;
								break;
							}
						}
					}

					if (!found)
					{
						PerformJumpEnterBattle(m_BattlePlayerLst[pActor->GetTarget()].pChar);					
					}
				}
				
			}
		}
		else if (Itr->DefenserLst.size() > 0 && Itr->DefenserLst[0].DefReaction == BATCMD_INVISIBLE)
		{
			CCharacterControl * pChar = m_BattlePlayerLst[Itr->DefenserLst[0].DefenserIndex].pChar;
			if (pChar)
			{
				pChar->GetCharMainView()->SetEntOffsetX(-1000);
				pChar->GetCharMainView()->SetEntOffsetY(-1000);

				if (pChar == pControlPartner && gGlobal.g_pPartnerProcess)
					gGlobal.g_pPartnerProcess->UpdateSmallIcon(false);
			}

		}
		else if (Itr->DefenserLst.size() > 0 && Itr->DefenserLst[0].DefReaction == BATCMD_BUFF_EFFECT_HIT)
		{
			OnBuffActive(*Itr);
		}
		else
		{
			// show attack bubble msg
			if (Itr->AttackerLst.size() > 0)
				ShowBattleBubbleMsg(Itr->AttackerLst[0].AttackerIndex, BATBUBBLE_ATTACK);

			OnAttack(*Itr);
		}

		AttackQuene TempData = *Itr;
		m_AttackQueneData.erase(Itr);
		if (TempData.DefenserLst.size()  && TempData.AttackerLst.size() &&
			(TempData.AttackerLst[0].Attri.HPChange < 0 || TempData.AttackerLst[0].Attri.SPChange < 0 ||
			TempData.DefenserLst[0].DefReaction == BATCMD_COUNTER_ATTACK_HIT ||
			TempData.DefenserLst[0].DefReaction == BATCMD_COUNTER_ATTACK_DEF_HIT))
		{
			AttackQuene counterAttackData;
			for (Int i = 0; i < (Int)TempData.DefenserLst.size(); i ++)
			{
				if (TempData.DefenserLst[i].DefReaction == BATCMD_COUNTER_ATTACK_HIT ||
					TempData.DefenserLst[i].DefReaction == BATCMD_COUNTER_ATTACK_DEF_HIT)
				{
					AttackerData Data;
					Data.AttackDes = m_BattlePlayerLst[TempData.DefenserLst[i].DefenserIndex].pChar->GetCharMainView()->GetEntPixelPos();
					Data.AtkAction = BATCMD_ATTACK;
					Data.AttackIndex = m_BattlePlayerLst[TempData.DefenserLst[i].DefenserIndex].pChar->GetCharMainView()->GetBattleAttack1Animation();
					Data.AttackerIndex = TempData.DefenserLst[i].DefenserIndex;
					Data.Attri = TempData.DefenserLst[i].Attri;
					Data.Attri.HPChange = 0;
					Data.Attri.SPChange = 0;
					Data.ShowNum = FALSE;
					counterAttackData.AttackerLst.push_back(Data);
				}
			}
			DefenserData DefData;
			DefData.DefReaction = BATCMD_NORM_HIT;
			DefData.Attri = TempData.AttackerLst[0].Attri;
			TempData.AttackerLst[0].Attri.HPChange = 0;
			TempData.AttackerLst[0].Attri.SPChange = 0;
			DefData.DefenserIndex = TempData.AttackerLst[0].AttackerIndex;
			BOOL AfterFound = FALSE;
			StlDeque<AttackQuene>::iterator Itr = m_AttackQueneData.begin();
			if (Itr != m_AttackQueneData.end())
			{
				for (Int i = 0; i < (Int)Itr->AttackerLst.size(); i ++)
				{
					if (Itr->AttackerLst[i].AttackerIndex == TempData.AttackerLst[0].AttackerIndex)
					{
						AfterFound = TRUE;
						break;
					}
				}
			}
			if (AfterFound)
			{
				DefData.Back = FALSE;
				DefData.die = FALSE;
			}
			counterAttackData.DefenserLst.push_back(DefData);
			m_AttackQueneData.push_front(counterAttackData);
		}
		
	}
}
BOOL CBattleControl::OnBuffActive(AttackQuene QueneData)
{
	for (Int k = 0; k < (Int)QueneData.DefenserLst.size(); k++)
	{
		BeingAttackData AtkData;

		
		AtkData.HitAnimationIndex = ANIM_HURT;

		AtkData.StartTime = ::timeGetTime();

		AtkData.DefData = QueneData.DefenserLst[k];

		AtkData.DefData.ShowNum = TRUE;

		AtkData.TimeToHit = 0;

		AtkData.DefData.Move = FALSE;

		const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(AtkData.DefData.Buff);
		if (pBuffData)
		{
			EffectData EData;
			EData.effectFileName = pBuffData->Buff_SparkFile;
			EData.rotation = m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()->GetAnimationRotation();
			EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView();
			EData.attach = FALSE;
			m_EffectDataLst.insert(std::make_pair(AtkData.StartTime + AtkData.TimeToHit, EData));
			m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
		}

		m_EntityBeingAttackLst.insert(std::make_pair(m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar ,AtkData));

		if (k == QueneData.DefenserLst.size() - 1 && (AtkData.DefData.Attri.HPChange != 0 ||
			AtkData.DefData.Attri.SPChange != 0))
			m_NextAttackQueueTime += 900;
	}

	return TRUE;
}
VOID CBattleControl::PerformGetHurt(BattleEntData * pEntData, Int HitAnimationIndex, Int HitTimeIndex, BYTE	DefReaction,
									BOOL Move, BOOL Back, BOOL petActive, BOOL HurtBack)
{

	if (pEntData == NULL)
		return;

	CCharacterControl * pChar = pEntData->pChar;

	Int StopShake = gConfigure.uStopShake;
	Int ShakeTime = gConfigure.uShakeTime;
	Int OrgRotation = pChar->GetCharMainView()->GetAnimationRotation();//
	UInt32 HurtTime = pChar->GetCharMainView()->GetTotalTimeOfGivenAnimation(
			HitTimeIndex, OrgRotation);
	Int DevFactor = 1;
	if (DefReaction == BATCMD_DEFEND_HIT ||
		DefReaction == BATCMD_COUNTER_ATTACK_DEF_HIT ||
		DefReaction == BATCMD_DEFEND_CRIT_HIT)
		DevFactor = 2;

	if (petActive)
	{
		if (pEntData->pPet)
			pEntData->pPet->GetCharMainView()->SetShakeInfo(0, 0, 200,0, 2, 0, HurtTime / 2);
	}
	if (DefReaction != BATCMD_PROTECT_HIT)
	{
		
		//ResetPlayerState(pChar);
		pChar->GetCharMainView()->m_animationset.clear();

		Int ExtraTime = 0;
		
		Int OrgIndex = 0;
		
		
		if (HitAnimationIndex == ANIM_DEF)
			OrgIndex = pChar->GetCharMainView()->GetBattleAnimation();

		OrgIndex = pChar->GetCharMainView()->GetBattleAnimation();
			
		pChar->GetCharMainView()->SetAnimationIndexByIndex(HitAnimationIndex, FALSE, OrgRotation, TRUE);

		UInt32 CurrTime = ::timeGetTime();

		pChar->GetCharMainView()->SetShakeInfo(0, 0, 200,0, 2, 0, HurtTime / 2);

		if (Move)
		{
			int upX = pEntData->BattlePoxX;
			int upY = pEntData->BattlePoxY;

			Int MoveHurtTime1 = gConfigure.uMoveHurtTime1;
			Int MoveHurtTime2 = gConfigure.uMoveHurtTime2;
			Int MoveHurtTime3 = gConfigure.uMoveHurtTime3;
			Int HoldHurtTime = gConfigure.uHouldHurtTime;
			Int Rotation = QGLMath::GetOppRotation(pEntData->OrgRotation);
			QGLMath::MoveToBlock(upX, upY, Rotation,  
				gConfigure.uHurtDistance / DevFactor);

			Int DesPixelX = 0;
			Int DesPixelY = 0;
			QGLMath::BlockPosToPixel(upX, upY, DesPixelX, DesPixelY);

			Int StartPixelX = 0;
			Int StartPixelY = 0;
			QGLMath::BlockPosToPixel(pEntData->BattlePoxX, pEntData->BattlePoxY, StartPixelX, StartPixelY);

			upX = DesPixelX - StartPixelX;
			upY = DesPixelY - StartPixelY;	
			
			pChar->GetCharMainView()->AppendAnimationSet(HitAnimationIndex, upX, upY, 0, 0, FALSE, OrgRotation, CurrTime, 0,
				pChar->GetCharMainView()->GetStopAnimation(), TRUE,  gConfigure.uShakeX,
				 gConfigure.uShakeY,
				  gConfigure.uShakeSpeedX,
				   gConfigure.uShakeSpeedY,
				    gConfigure.uShakeWidth,
					gConfigure.uShakeHeight,
					HurtTime/2);				

				pChar->GetCharMainView()->AppendAnimationSet(pChar->GetCharMainView()->GetBattleAnimation(), 
					-1 * upX, -1 * upY, 0, MoveHurtTime3, FALSE, OrgRotation, CurrTime + HurtTime,0,
				FALSE,TRUE,  0, 0, 0, 0, 0, 0, 0);
			

			ExtraTime = HurtTime + HoldHurtTime;// MoveHurtTime3 + HoldHurtTime + HurtTime / 2;

		}

		UInt32 FinalTime = HurtTime;
		if (ExtraTime > (Int)HurtTime)
			FinalTime = ExtraTime;

		Int RunTime = 0;

		if (Back)
		{
			if (pChar->GetCharMainView()->GetEntOffsetX() != 0 ||
				pChar->GetCharMainView()->GetEntOffsetY() != 0)
			{
				Int RunSpeed = gConfigure.uRunSpeed;
				RunTime = (Int)sqrt(pow((double)pChar->GetCharMainView()->GetEntOffsetX() , (int)2) + 
					pow((double)pChar->GetCharMainView()->GetEntOffsetY(), (int)2)) * 1000 / RunSpeed;

				pChar->GetCharMainView()->AppendAnimationSet(ANIM_RUN, 
					-1 * pChar->GetCharMainView()->GetEntOffsetX(), 
					-1 * pChar->GetCharMainView()->GetEntOffsetY(), 0, 
					RunTime, TRUE, QGLMath::GetOppRotation(OrgRotation), CurrTime + FinalTime,0,
					FALSE, TRUE,  0, 0, 0, 0, 0, 0, 0);
			}
		}
		if (HurtBack)
		{
			pChar->GetCharMainView()->AppendAnimationSet(OrgIndex, 
				0, 0, 0, 0, TRUE, OrgRotation, CurrTime + FinalTime + RunTime,0,
				FALSE,FALSE,  0, 0, 0, 0, 0, 0, 0);
		}
	}
	else
	{
		pChar->GetCharMainView()->SetShakeInfo(0, 0, 200,0, 2, 0, HurtTime / 2);
	}
}

VOID CBattleControl::PerformMiss(CCharacterControl * pChar, Int Rotation)
{

	if (pChar == NULL)
		return;

	ResetPlayerState(pChar);

	int upX = 0;
	int upY = 0;

	QGLMath::MoveToBlock(upX, upY, Rotation,  gConfigure.uMissDistance);

	Int MissRunTime = gConfigure.uMissRunTime;

	UInt32 CurrTime = ::timeGetTime();


	pChar->GetCharMainView()->SetJumpData(0,upX,upY, MissRunTime);

	pChar->GetCharMainView()->AppendAnimationSet(ERROR_INDEX, -upX, -upY, 0, MissRunTime, TRUE, ERROR_INDEX, CurrTime + MissRunTime, 0,
		FALSE, FALSE, 0,0,0,0,0,0,0);

	//add disposl effect
	DisposalEffectData Data;
	Data.PreUpdateTime = CurrTime;
	Data.EndTime = CurrTime + MissRunTime * 2;
	Data.LifeTime = gConfigure.uMissLifeTime;
	Data.StartAlpha = gConfigure.uMissAlpha;
	Data.TimeToProduce = gConfigure.uMissTimeToProduce;
	Data.pEnt = pChar->GetCharMainView();

	QGLApplication::GetInstance().InsertDisposalEffect(Data);

}


VOID CBattleControl::RemoveShieldEffect()
{
	for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i ++)
		m_BattlePlayerLst[i].pChar->GetCharMainView()->RemovePart(EQUIP_OTHERS);
}

VOID CBattleControl::AddShieldEffect()
{
	if (QGLApplication::GetInstance().GetRenderClass())
	{
		for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i ++)
		{
			m_BattlePlayerLst[i].pChar->GetCharMainView()->RemovePart(EQUIP_OTHERS);
			Char Effect[50];
			GetPrivateProfileString(_T("LOADFILE"), _T("EFFECT"), _T(""), Effect, 50, CONFIGURATION_INI_FILE);
			m_BattlePlayerLst[i].pChar->GetCharMainView()->LoadEntityFile(Effect, gGlobal.pD3DDevice, EQUIP_OTHERS);
		}
	}
}

VOID CBattleControl::UpdateBeingAttackQueue(UInt32 CurrTime)
{
	StlMultiMap<CCharacterControl *, BeingAttackData>::iterator EntityBeingAttackItr = m_EntityBeingAttackLst.begin();

	StlVector<StlMultiMap<CCharacterControl *, BeingAttackData>::iterator> EntityBeingAttackItrRmLst;

	while (EntityBeingAttackItr != m_EntityBeingAttackLst.end())
	{
		if (CurrTime > EntityBeingAttackItr->second.TimeToHit + EntityBeingAttackItr->second.StartTime)
		{
			EntityBeingAttackItrRmLst.push_back(EntityBeingAttackItr);

			BOOL Miss = FALSE;
			if (gConfigure.uNetwork)
			{
				Miss = EntityBeingAttackItr->second.DefData.DefReaction == BATCMD_MISS;
			}
			else
			{
				Int MissRate = gConfigure.uMissRate;
				Miss = MissRate > 0 && rand() % MissRate == 0;
			}

			{
				CCharacterControl * pDefenseEntity = m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].pChar;

				//add number
				{
					if (!gConfigure.uNetwork)
					{
						if (EntityBeingAttackItr->second.DefData.ShowNum)
						{
						QGLApplication::GetInstance().AddNumberEnt(pDefenseEntity->GetCharMainView(), PixelDirection(), 
							gConfigure.uLiveTimeBase, 
							gConfigure.uMoveSpeed,
							gConfigure.uDisposalTime, 
							gConfigure.uWaitTime, 
							TRUE, NumberExtraZ, 1, EntityBeingAttackItr->second.DefData.Attri.HPChange,
							0, m_BattleParticle ,
							1, 
							hurtNum,
							gConfigure.uNumJumpHeight,
							gConfigure.uNumShake > 0, 0,
							fontChiEnt);
							
						}
					}
					else
					{
						CCharacterControl * pCharCtrl = NULL;
						AttriChange Attri;
						BYTE action = 0;
						CBatActor * pBatActor = NULL;
						Int EntIndex = 0;
						Int HitAnim = 0;

						BOOL Shake = FALSE;
						BOOL ShowNum = FALSE;
						PCSTRINGW FileName = NULL;
						Int Change = 0;
						Int symbol = 0;
						Int numCount = 0;
						BOOL Move = false;
						BOOL Hurt = false;


						for (Int k = 0; k < 2; k ++)
						{
							Shake = FALSE;
							Change = 0;
							symbol = 0;
							numCount = 0;
							Hurt = false;

							if (k == 0)
							{
								pBatActor = m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].pChar->GetBatActorObject();
								pCharCtrl = m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].pChar;
								EntIndex = EntityBeingAttackItr->second.DefData.DefenserIndex;
								Attri = EntityBeingAttackItr->second.DefData.Attri;
								action = EntityBeingAttackItr->second.DefData.DefReaction;
								Move = EntityBeingAttackItr->second.DefData.Move;
								HitAnim = EntityBeingAttackItr->second.HitAnimationIndex;

								ShowNum = EntityBeingAttackItr->second.DefData.ShowNum;
								Hurt = Attri.HPChange < 0 ||
										Attri.SPChange < 0 ||
										Attri.VPChange < 0
										;
							}
							else
							{
								ShowNum = EntityBeingAttackItr->second.AtkData.ShowNum;
								if (EntityBeingAttackItr->second.AtkData.IsValid() && EntityBeingAttackItr->second.AtkData.ShowNum)
								{
									pBatActor = m_BattlePlayerLst[EntityBeingAttackItr->second.AtkData.AttackerIndex].pChar->GetBatActorObject();
									pCharCtrl = m_BattlePlayerLst[EntityBeingAttackItr->second.AtkData.AttackerIndex].pChar;
									Attri = EntityBeingAttackItr->second.AtkData.Attri;
									action = EntityBeingAttackItr->second.AtkData.AtkAction;
									EntIndex = EntityBeingAttackItr->second.AtkData.AttackerIndex;
									Move = true;
									HitAnim = ANIM_HURT;
									Hurt = FALSE;
								}
								else
									break;
							}

							BOOL AfterFound = FALSE;
							StlMultiMap<CCharacterControl *, BeingAttackData>::iterator Itr = EntityBeingAttackItr;
							++ Itr;
							for(; Itr != m_EntityBeingAttackLst.end(); ++ Itr)
							{
								if (Itr->second.DefData.DefenserIndex == EntityBeingAttackItr->second.DefData.DefenserIndex)
								{
									AfterFound = TRUE;
									break;
								}
							}

							if (!AfterFound)
							{
								if (m_AttackQueneData.size() > 0)
								{
									for (Int i = 0; i < (Int)m_AttackQueneData[0].DefenserLst.size(); i ++)
									{
										if (m_AttackQueneData[0].DefenserLst[i].DefenserIndex 
											== EntityBeingAttackItr->second.DefData.DefenserIndex)
										{
											AfterFound = TRUE;
											break;
										}
									}
								}
							}

							BOOL AtkAfterFound = FALSE;
							UInt8 count = 0;
							for (Itr = EntityBeingAttackItr; Itr != m_EntityBeingAttackLst.end() && count < 2;  Itr ++, count ++)
							{
							}

							
							for(; Itr != m_EntityBeingAttackLst.end(); ++ Itr)
							{
								if (Itr->second.AtkData.AttackerIndex == EntityBeingAttackItr->second.DefData.DefenserIndex)
								{
									AtkAfterFound = TRUE;
									break;
								}
							}

							if (!AtkAfterFound)
							{
								if (m_AttackQueneData.size() > 0)
								{
									for (Int i = 0; i < (Int)m_AttackQueneData[0].AttackerLst.size(); i ++)
									{
										if (m_AttackQueneData[0].AttackerLst[i].AttackerIndex 
											== EntityBeingAttackItr->second.DefData.DefenserIndex)
										{
											AtkAfterFound = TRUE;
											break;
										}
									}
								}
							}

							if (ShowNum && Attri.HPOrg <= 0  &&
								Attri.state == CHAR_ST_DEAD)
								//pBatActor && pBatActor->GetState() == CHAR_ST_DEAD)
							{
								pCharCtrl->GetCharMainView()->m_animationset.clear();
								pCharCtrl->GetCharMainView()->StopShake();
								CEntityBase * pEntBase = m_BattlePlayerLst[EntIndex].pEntHpBar;
								if (pEntBase)
									pEntBase->SetScaleX(0);
								pEntBase = m_BattlePlayerLst[EntIndex].pEntHpDBar;
								if (pEntBase)
									pEntBase->SetScaleX(0);
								
								if (pCharCtrl->GetCharMainView()->GetAnimationIndex() != ANIM_DIE)
								{
									pCharCtrl->GetCharMainView()->m_animationset.clear();

									int upX = m_BattlePlayerLst[EntIndex].BattlePoxX;
									int upY = m_BattlePlayerLst[EntIndex].BattlePoxY;

									Int MoveHurtTime1 = gConfigure.uMoveHurtTime1;
									Int MoveHurtTime2 = gConfigure.uMoveHurtTime2;
									Int MoveHurtTime3 = gConfigure.uMoveHurtTime3;
									Int HoldHurtTime = gConfigure.uHouldHurtTime;
									Int Rotation = QGLMath::GetOppRotation(m_BattlePlayerLst[EntIndex].OrgRotation);
									QGLMath::MoveToBlock(upX, upY, Rotation,  
										gConfigure.uHurtDistance);

									Int DesPixelX = 0;
									Int DesPixelY = 0;
									QGLMath::BlockPosToPixel(upX, upY, DesPixelX, DesPixelY);

									Int StartPixelX = 0;
									Int StartPixelY = 0;
									QGLMath::BlockPosToPixel(m_BattlePlayerLst[EntIndex].BattlePoxX, m_BattlePlayerLst[EntIndex].BattlePoxY, StartPixelX, StartPixelY);

									upX = DesPixelX - StartPixelX;
									upY = DesPixelY - StartPixelY;	
									
									UInt32 time = 500;

									pCharCtrl->GetCharMainView()->SetShakeInfo(0, 0, 200,0, 2, 0, time);
									pCharCtrl->GetCharMainView()->SetEntOffsetX(upX);
									pCharCtrl->GetCharMainView()->SetEntOffsetY(upY);

									pCharCtrl->GetCharMainView()->SetAnimationIndexByIndex(ANIM_HURT);
									pCharCtrl->GetCharMainView()->SetStopAnimation(TRUE);
									pCharCtrl->GetCharMainView()->AppendAnimationSet(ANIM_DIE, 0, 0, 0, 0, false, 
										pCharCtrl->GetCharMainView()->GetAnimationRotation(), ::timeGetTime() + time, 
										0, false, false,	0, 0, 0, 0, 0, 0, 0);
								}

								// show bubble msg on die
								ShowBattleBubbleMsg(EntityBeingAttackItr->second.DefData.DefenserIndex, BATBUBBLE_DIE);

							}
							else
							{
								if (pBatActor->GetHP() > 0 && pCharCtrl->GetCharMainView()->GetAlphaOffset() == 255)
								{
									pCharCtrl->GetCharMainView()->SetAlphaOffset(0);
								}
								

								if (!Hurt && action == BATCMD_MISS)
								{
									PerformMiss(m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].pChar,
										QGLMath::GetOppRotation(m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].OrgRotation));
								
									// show miss attack  bubble msg
									ShowBattleBubbleMsg(EntityBeingAttackItr->second.DefData.DefenserIndex, BATBUBBLE_MISS);

									String Temp;
									FSound* sfx = NULL;
									Temp.Format(_T("%sSoundB0014_eva.wav"), WAV_FILE_PATH);
									sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
									if (sfx)
									{
										sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
										sfx->Play();
									}
								}
								else if (Hurt)
								{
									// show bubble msg on hurt
									ShowBattleBubbleMsg(EntityBeingAttackItr->second.DefData.DefenserIndex, BATBUBBLE_HURT);

									PerformGetHurt(&m_BattlePlayerLst[EntIndex], 
										HitAnim, ANIM_HURT, 
										action, 
										Move, !AtkAfterFound || (EntityBeingAttackItr->second.DefData.Back && !EntityBeingAttackItr->second.DefData.hitTogether),
										EntityBeingAttackItr->second.DefData.petActive);

									if (action == BATCMD_DEFEND_HIT ||
										action == BATCMD_COUNTER_ATTACK_DEF_HIT ||
										action == BATCMD_DEFEND_CRIT_HIT ||
										action == BATCMD_PROTECT_HIT)
									{
										String Temp;
										Temp.Format(_T("%sSoundB0015_def.wav"), WAV_FILE_PATH);
										FSound* sfx = NULL;
										sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
										if (sfx)
										{
											sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
											sfx->Play();
										}
									}
								}
	
								if (pCharCtrl->GetCharMainView()->GetAnimationIndex() == ANIM_DIE && Attri.HPOrg > 0)
								{
									pCharCtrl->GetCharMainView()->SetAnimationIndexByIndex(pCharCtrl->GetCharMainView()->GetBattleAnimation(), TRUE);
									ResetPlayerState(pCharCtrl);
									m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].DeadTime = 0;
								}

								if (pCharCtrl->GetCharMainView()->GetAlphaOffset() > 200  && Attri.HPOrg > 0)
								{
									pCharCtrl->GetCharMainView()->SetAlphaOffset(0, TRUE);
									m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].DeadTime = 0;
								}
								if (!pCharCtrl->GetCharMainView()->GetVisible()   && Attri.HPOrg > 0)
								{
									pCharCtrl->GetCharMainView()->SetVisible(TRUE, TRUE);
									m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].DeadTime = 0;
								}
								
							}

							//if (ShowNum)
							{
								numCount = 0;
								for (Int i = 0; i < 5 ; i ++)
								{
									Shake = FALSE;
									Change = 0;
									symbol = 0;
									

									if (i == 0)
									{
										Change = Attri.HPChange;
										if (Change < 0)
										{
											if (action == BATCMD_CRIT_HIT ||
												EntityBeingAttackItr->second.AtkData.AtkAction == BATCMD_CRIT_ATTACK || 
												EntityBeingAttackItr->second.AtkData.AtkAction == BATCMD_CRIT_SKILL ||
												action == BATCMD_DEFEND_CRIT_HIT)
											{
												//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("CRTHURTNUM"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
												FileName = criHurtNum;
												Shake = TRUE;
												String Temp;
												Temp.Format(_T("%sSoundB0012_attack_critcal.wav"), WAV_FILE_PATH);
												FSound* sfx = NULL;
												sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
												if (sfx)
												{
													sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
													sfx->Play();
												}
											}
											else
												FileName = hurtNum;
												//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("HURTNUM"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
										}
										else
											FileName = healNum;
											//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("HEALNUM"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);

										//if (Change != 0)
										{
											
											if (pCharCtrl->GetMapActorID() == gGlobal.g_pCharacter->GetMapActorID() &&
												pBatActor)
											{
												gGlobal.SetDisplayHP(Attri.HPOrg, pBatActor->GetMHP(), pBatActor->GetMaxHP());
											}
											else if (pControlPartner != NULL && pControlPartner->GetBatActorObject())
											{
												CBatMob* pmob = CastNetObject<CBatMob>(pBatActor);
												if (pmob && pmob->GetOwner_id() == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id() )
												{
													CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(pmob->GetPartner_id());
													if (partner)
													{
														partner->SetBattleHP(Attri.HPOrg);
														partner->SetBattleMaxHP(pBatActor->GetMaxHP());
														gGlobal.g_pPartnerProcess->UpdateSmallHP(partner->GetPartner_id());
													}
												}
											}
											
										}
										Hurt = Change < 0;
									}
									else if (i == 1)
									{
										Change = Attri.SPChange;
										if (Change < 0)
										{
											FileName = fontSubEnt;
											symbol = NUM_ENTITY_INDEX_SP_SUB;

										}
										else
										{
											FileName = fontAddEnt;
											
											symbol = NUM_ENTITY_INDEX_SP_ADD;
										}


										//if (Change != 0)
										{
											if (pCharCtrl->GetMapActorID() == gGlobal.g_pCharacter->GetMapActorID() &&
												pBatActor)
											{
												gGlobal.SetDisplaySP(Attri.SPOrg, pBatActor->GetMSP(), pBatActor->GetMaxSP());
											}
											else if (pControlPartner != NULL && pControlPartner->GetBatActorObject())
											{
												CBatMob* pmob = CastNetObject<CBatMob>(pBatActor);
												if (pmob && pmob->GetOwner_id() == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id() )
												{
													CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(pmob->GetPartner_id());
													if (partner)
													{
														partner->SetBattleSP(Attri.SPOrg);
														gGlobal.g_pPartnerProcess->UpdateSmallSP(partner->GetPartner_id());
													}
												}
											}
										}
										Hurt = Change < 0;
									}
									else if (i == 2)
									{
										Change = Attri.DPChange;
										if (Change < 0)
										{
											//_tcscpy(FileName, fontSubEnt);
											FileName= fontSubEnt;
											symbol = NUM_ENTITY_INDEX_DP_SUB;
										}
										else
										{
											//_tcscpy(FileName, fontAddEnt);
											FileName= fontAddEnt;
											symbol = NUM_ENTITY_INDEX_DP_ADD;
										}

										//if (Change != 0)
										{
											if (pCharCtrl->GetMapActorID() == gGlobal.g_pCharacter->GetMapActorID() &&
												pBatActor)
											{
												CBatCharacter * pBatCharacter = CastNetObject<CBatCharacter>(pBatActor);
												if (pBatCharacter)
													gGlobal.SetDisplayDP(Attri.DPOrg, pBatCharacter->GetMaxDP());
											}
										}
										Hurt = Change < 0;
									}
									else if  (i == 3)
									{
										Change = Attri.WcChange;
										if (Change < 0)
										{
											FileName = fontSubEnt;
										}
											//_tcscpy(FileName, fontSubEnt);
										else
											FileName = fontAddEnt;
											//_tcscpy(FileName, fontAddEnt);

										//if (Change != 0)
										{
											if (pCharCtrl->GetMapActorID() == gGlobal.g_pCharacter->GetMapActorID() &&
												pBatActor)
											{
												CCharacter * pCharacter = CastNetObject<CCharacter>(gGlobal.g_pCharacter->GetCharNetObject());
												if (pCharacter)
													gGlobal.SetDisplayWC(pCharacter->GetWC(), pCharacter->GetMaxWC(),
													pCharacter->GetWcExp(), pCharacter->GetNextWCExp());
											}
										}
										Hurt = Change < 0;
									}
									else if  (i == 4)
									{
										Change = Attri.VPChange;
										if (Change < 0)
										{
											//_tcscpy(FileName, fontSubEnt);
											FileName = fontSubEnt;
											symbol = NUM_ENTITY_INDEX_VP_SUB;
										}
										else
										{
//											_tcscpy(FileName, fontAddEnt);
											FileName = fontAddEnt;
											symbol = NUM_ENTITY_INDEX_VP_ADD;
										}


										//if (Change != 0)
										{
											if (pCharCtrl->GetMapActorID() == gGlobal.g_pCharacter->GetMapActorID() &&
												pBatActor)
											{
												CBatCharacter * pBatCharacter = CastNetObject<CBatCharacter>(pBatActor);
												if (pBatCharacter)
													gGlobal.SetDisplayVP(pBatCharacter->GetVP(), pBatCharacter->GetMaxVP());
											}
										}
										Hurt = Change < 0;
									}

									if (Change == 0)
										continue;

									if (ShowNum)
									{
										Int LiveBase = gConfigure.uLiveTimeBase;
										Int DisPosalTime = gConfigure.uDisposalTime;
										Int WaitTime = gConfigure.uWaitTime;
										QGLApplication::GetInstance().AddNumberEnt(pCharCtrl->GetCharMainView(), PixelDirection(), 
											LiveBase, 
											gConfigure.uMoveSpeed,
											DisPosalTime, 
											WaitTime, 
											TRUE, NumberExtraZ, 1, Change,
											symbol, m_BattleParticle ,FALSE, FileName,
											gConfigure.uNumJumpHeight ,
											Shake,  numCount * (WaitTime + DisPosalTime + LiveBase), fontChiEnt);
											
										UInt NumWaitCount = ::timeGetTime() + LiveBase + DisPosalTime + WaitTime +  numCount * (WaitTime + DisPosalTime + LiveBase);

										if (!AfterFound &&NumWaitCount > m_NextAttackQueueTime)
											m_NextAttackQueueTime = NumWaitCount;
 
										numCount ++;
									}
								}


								CEntityBase * pEntBase = m_BattlePlayerLst[EntIndex].pEntHpBar;
								if (pEntBase)
								{
									Real32 ScaleX = (Real32)Attri.HPOrg / 
										(Real32)m_BattlePlayerLst[EntIndex].pChar->GetBattleMaxHp();

									pEntBase->SetScaleX(ScaleX);
								}

								pEntBase = m_BattlePlayerLst[EntIndex].pEntHpDBar;
								if (pEntBase)
								{
									Real32 ScaleX = (Real32)m_BattlePlayerLst[EntIndex].pChar->GetBattleMHp() / 
										(Real32)m_BattlePlayerLst[EntIndex].pChar->GetBattleMaxHp();

									pEntBase->SetScaleX(ScaleX);
								}
								
							}
							
						}
					}
				}
			}


			if (EntityBeingAttackItr->second.DefData.ShowNum)
			{
				for (Int i = 0; i < (Int)EntityBeingAttackItr->second.DefData.Attri.addBuffList.size(); i ++)
				{
					m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].pChar->GetCharMainView()->
						LoadEntityFile(EntityBeingAttackItr->second.DefData.Attri.addBuffList[i]->Buff_EffectFile, gGlobal.pD3DDevice, EQUIP_OTHERS);
					
					if (EntityBeingAttackItr->second.DefData.Attri.addBuffList[i]->Buff_HideEffect ||
						(m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].pChar ==
						pControlPlayer && EntityBeingAttackItr->second.DefData.Attri.addBuffList[i]->Buff_DetectBuffs.size() > 0))
						ShowHideBuffEffects();

					CallLuaFunc(gGlobal.g_pParentLua, "IsInvisibleBuff", String(), EntityBeingAttackItr->second.DefData.Attri.addBuffList[i]->id);
					if (gGlobal.g_TempResult > 0)
					{
						if (pControlPlayer == NULL || !pControlPlayer->DetectBuff(EntityBeingAttackItr->second.DefData.Attri.addBuffList[i]->id))
							m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].pChar->GetCharMainView()->SetAlphaOffset(InvisibleAlphaOffset, TRUE);
					}
					
				}

				for (Int i = 0; i < (Int)EntityBeingAttackItr->second.DefData.Attri.removeBuffList.size(); i ++)
				{
					m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].pChar->GetCharMainView()->
						RemovePartByName(EntityBeingAttackItr->second.DefData.Attri.removeBuffList[i]->Buff_EffectFile);

					if (m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].pChar ==
						pControlPlayer && EntityBeingAttackItr->second.DefData.Attri.removeBuffList[i]->Buff_DetectBuffs.size() > 0)
						HideBuffEffects();

					CallLuaFunc(gGlobal.g_pParentLua, "IsInvisibleBuff", String(), EntityBeingAttackItr->second.DefData.Attri.removeBuffList[i]->id);
					if (gGlobal.g_TempResult > 0)
					{
						m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].pChar->GetCharMainView()->SetAlphaOffset(0, TRUE);
					}
					/*if (invisibleBuff == EntityBeingAttackItr->second.DefData.Attri.removeBuffList[i]->id)
					{
						m_BattlePlayerLst[EntityBeingAttackItr->second.DefData.DefenserIndex].pChar->GetCharMainView()->SetAlphaOffset(0, TRUE);
					}*/
				}

			}
			//if (EntityBeingAttackItr->second.AtkData.ShowNum)
			{
				for (Int i = 0; i < (Int)EntityBeingAttackItr->second.AtkData.Attri.addBuffList.size(); i ++)
				{
					m_BattlePlayerLst[EntityBeingAttackItr->second.AtkData.AttackerIndex].pChar->GetCharMainView()->
						LoadEntityFile(EntityBeingAttackItr->second.AtkData.Attri.addBuffList[i]->Buff_EffectFile, gGlobal.pD3DDevice, EQUIP_OTHERS);

					if (EntityBeingAttackItr->second.AtkData.Attri.addBuffList[i]->Buff_HideEffect ||
						(m_BattlePlayerLst[EntityBeingAttackItr->second.AtkData.AttackerIndex].pChar ==
						pControlPlayer && EntityBeingAttackItr->second.AtkData.Attri.addBuffList[i]->Buff_DetectBuffs.size() > 0))
						ShowHideBuffEffects();

					CallLuaFunc(gGlobal.g_pParentLua, "IsInvisibleBuff", String(), EntityBeingAttackItr->second.AtkData.Attri.addBuffList[i]->id);
					if (gGlobal.g_TempResult > 0)
					{
						if (pControlPlayer == NULL || !pControlPlayer->DetectBuff(EntityBeingAttackItr->second.AtkData.Attri.addBuffList[i]->id))
						m_BattlePlayerLst[EntityBeingAttackItr->second.AtkData.AttackerIndex].pChar->GetCharMainView()->SetAlphaOffset(InvisibleAlphaOffset, TRUE);
					}
					/*if (EntityBeingAttackItr->second.AtkData.Attri.addBuffList[i]->Buff_type == 2) //debuff
					{
					PerformGetHurt(&m_BattlePlayerLst[EntityBeingAttackItr->second.AtkData.AttackerIndex], 
					ANIM_HURT, ANIM_HURT, 
					BATCMD_BUFF_EFFECT_HIT, 
					FALSE);
					}*/
				}

				for (Int i = 0; i < (Int)EntityBeingAttackItr->second.AtkData.Attri.removeBuffList.size(); i ++)
				{
					m_BattlePlayerLst[EntityBeingAttackItr->second.AtkData.AttackerIndex].pChar->GetCharMainView()->
						RemovePartByName(EntityBeingAttackItr->second.AtkData.Attri.removeBuffList[i]->Buff_EffectFile);

					if (m_BattlePlayerLst[EntityBeingAttackItr->second.AtkData.AttackerIndex].pChar ==
						pControlPlayer && EntityBeingAttackItr->second.AtkData.Attri.removeBuffList[i]->Buff_DetectBuffs.size() > 0)
						HideBuffEffects();

					CallLuaFunc(gGlobal.g_pParentLua, "IsInvisibleBuff", String(), EntityBeingAttackItr->second.AtkData.Attri.removeBuffList[i]->id);
					if (gGlobal.g_TempResult > 0)
					{
						m_BattlePlayerLst[EntityBeingAttackItr->second.AtkData.AttackerIndex].pChar->GetCharMainView()->SetAlphaOffset(0, TRUE);
					}
				}
			}
		}
		EntityBeingAttackItr ++;
	}

	for (Int i = 0; i < (Int)EntityBeingAttackItrRmLst.size(); i++)
	{
		m_EntityBeingAttackLst.erase(EntityBeingAttackItrRmLst[i]);
	}

}
BOOL CBattleControl::CheckBattleFinish()
{
	//if (m_BattleMapData.State != BATTLE_STATE_INWAR)
	//	return FALSE;

	if (m_AttackQueneData.size() >0 ||
		m_EntityBeingAttackLst.size() > 0)
		return FALSE;

	if (::timeGetTime() < m_NextAttackQueueTime)
		return FALSE;

	/*if (gConfigure.uNetwork &&
	gGlobal.g_pCharacter->GetAction() != CCharacter::ST_BATTLE &&
	gGlobal.g_pCharacter->GetAction() != CCharacter::ST_MONITOR &&
	gGlobal.g_pCharacter->GetAction() != CCharacter::ST_NEEDHELP)
	m_LeaveBattle = TRUE;
	*/

	if (!m_LeaveBattle || m_PlayingBattleAnim)
		return FALSE;

	if (pControlPlayer && pControlPlayer->GetCharMainView()->GetAnimationIndex() == ANIM_DIE)
	{
		if (!pControlPlayer->GetCharMainView()->FrameEndNoOthers(::timeGetTime()))
		{
			m_BattleFinishTime = 0;
			return FALSE;
		}
	}

	if (m_BattleFinishTime > 0)
	{
		if (::timeGetTime() > FinishWaitTime + m_BattleFinishTime)
		{
			m_BattleFinishTime = 0;
			m_CommandAvial = TRUE;
			m_BattleParticle->ClearLst();
			QGLApplication::GetInstance().RemoveParticleSystem(m_BattleParticle);
			return TRUE;
		}
	}
	else
	{
		m_BattleFinishTime = ::timeGetTime();
	}
	return FALSE;
}

VOID CBattleControl::SetBattleState(BattleState State) 
{ 
	m_BattleMapData.State = State;
	m_BattleMapData.StartBattleTime = ::timeGetTime();
}

VOID CBattleControl::UpdateEntityStatus(UInt32 CurrTime)
{
	for (BattleEntDataList::iterator Itr = m_BattlePlayerLst.begin();
		Itr != m_BattlePlayerLst.end(); ++Itr)
	{
		if ((*Itr).pChar->GetBattleHp() <= 0 && 
			(*Itr).pChar->GetCharMainView()->GetAnimationIndex() == ANIM_DIE)
		{

			if ((*Itr).pChar->GetBatActorObject() && !(*Itr).pChar->GetBatActorObject()->GetLeftBattle())
			{
				continue;
			}

			if ((*Itr).DeadTime == 0)
			{
				if ((*Itr).pChar->GetCharMainView()->FrameEndNoOthers(CurrTime))
				{
					(*Itr).DeadTime = ::timeGetTime();
					//if ((*Itr).DeadTime +  FadeOutTime > m_NextAttackQueueTime)
					//	m_NextAttackQueueTime = (*Itr).DeadTime +  FadeOutTime;
				}
			}
			else if (CurrTime - (*Itr).DeadTime < FadeOutTime) 
			{
				Real32 Alpha = (Real32)(CurrTime - (*Itr).DeadTime) /(Real32)FadeOutTime;
				if (Alpha > 1)
					Alpha = 1.0f;
				Byte AlphaOffset = (Byte)(255 * Alpha);
				(*Itr).pChar->GetCharMainView()->SetAlphaOffset(AlphaOffset, TRUE);
			}
			else
			{
				(*Itr).pChar->GetCharMainView()->SetVisible(FALSE, TRUE);
			}
		}
	}
}

VOID CBattleControl::UpdateBattle(UInt32 CurrTime)
{
	//if (m_BattleMapData.State == BATTLE_STATE_NO_BATTLE)
	//	return;

	//update the battle background
	UpdateBattleBgMap(CurrTime);

	//update being attack data
	UpdateBeingAttackQueue(CurrTime);

	//update the attack data
	UpdateAttackQueue(CurrTime);

	//update entityt status
	UpdateEntityStatus(CurrTime);

	//update effect
	UpdateEffectDataLst(CurrTime);

	//update sound
	UpdateSoundDataLst(CurrTime);

	//check whether the this ture was ended
	if (m_PlayingBattleAnim && IsEveryReady())
	{
		EndThisTurn();
		m_PlayingBattleAnim = FALSE;
	}

	//check the count down time
	if (CurrTime > m_PreUpdateTime)
	{
		if (m_CountDownTime > CurrTime - m_PreUpdateTime)
			m_CountDownTime -= CurrTime - m_PreUpdateTime;
		else
			m_CountDownTime = 0;
	}
	if (m_StartCountDown)
	{
		UInt32 Num = (UInt32)ceil((Real32)m_CountDownTime / 1000);
		if (Num > CountDownTotalTime / 1000)
			Num = CountDownTotalTime / 1000;

		m_CountDownEnt.SetNumber(Num);
	
	}
	m_PreUpdateTime = CurrTime;
}

VOID CBattleControl::AttackUsingSkill(int Attacker, int Defenser, int SkillIndex)
{
	if (m_BattlePlayerLst[Attacker].pChar->GetCharMainView()->m_animationset.size() > 0)
		return;

	m_BattlePlayerLst[Attacker].pChar->GetCharMainView()->SetAnimationIndexByIndex(ANIM_SKILL, TRUE);
}

VOID CBattleControl::PerformJumpProtect(BattleEntData * pProtecter, Int ProtectTargetIndex, Int JumpHeight, 
										Int JumpTime, UInt32 CurrTime, UInt32 LastTime, bool petActive)
{
	if (ProtectTargetIndex >= (Int) m_BattlePlayerLst.size() || pProtecter == NULL)
		return;

	ResetPlayerState(m_BattlePlayerLst[ProtectTargetIndex].pChar);

	CCharacterControl * pProCtrl = pProtecter->pChar;
	if (petActive)
	{
		pProCtrl = pProtecter->pPet;
		pProCtrl->GetCharMainView()->SetVisible(true, true);
		pProCtrl->GetCharMainView()->CopyPos(pProtecter->pChar->GetCharMainView());
		pProCtrl->GetCharMainView()->SetRotation(pProtecter->pChar->GetCharMainView()->GetAnimationRotation());
		if (pProCtrl->GetCharMainView()->GetAnimationRotation() == NORTHWEST)
		{
			pProCtrl->GetCharMainView()->SetEntPixelPos(pProCtrl->GetCharMainView()->GetEntPixelPos().PixelPosX +  BlockSizeX * 10,
				pProCtrl->GetCharMainView()->GetEntPixelPos().PixelPosY +  BlockSizeY * 10);
		}
		else
		{
			pProCtrl->GetCharMainView()->SetEntPixelPos(pProCtrl->GetCharMainView()->GetEntPixelPos().PixelPosX -  BlockSizeX * 10,
				pProCtrl->GetCharMainView()->GetEntPixelPos().PixelPosY -  BlockSizeY * 10);
		}
	}
	int upX = m_BattlePlayerLst[ProtectTargetIndex].pChar->GetCharMainView()->GetEntPixelPos().PixelPosX -
		pProCtrl->GetCharMainView()->GetEntPixelPos().PixelPosX;
	int upY = m_BattlePlayerLst[ProtectTargetIndex].pChar->GetCharMainView()->GetEntPixelPos().PixelPosY - 
		pProCtrl->GetCharMainView()->GetEntPixelPos().PixelPosY;

	upX /= BlockSizeX;
	upY /= BlockSizeY / 2;

	if(m_BattlePlayerLst[ProtectTargetIndex].OrgRotation == NORTHWEST)
		QGLMath::MoveToBlock(upX, upY, NORTHWEST, 2);
	else
		QGLMath::MoveToBlock(upX, upY, SOUTHEAST, 2);
	pProCtrl->GetCharMainView()->SetAnimationIndexByIndex(ANIM_JUMP_START, true);
	pProCtrl->GetCharMainView()->SetJumpData(JumpHeight,upX,upY, JumpTime);

	pProCtrl->GetCharMainView()->AppendAnimationSet(ANIM_JUMP, 0, 0, 0, 0, FALSE, m_BattlePlayerLst[ProtectTargetIndex].OrgRotation,
		CurrTime , 0, FALSE, FALSE, 0,0,0,0,0,0,0);

	pProCtrl->GetCharMainView()->AppendAnimationSet(ANIM_DEF, 0, 0, 0, 0, FALSE, m_BattlePlayerLst[ProtectTargetIndex].OrgRotation,
		CurrTime + JumpTime, 0, FALSE, FALSE, 0,0,0,0,0,0,0);

	pProCtrl->GetCharMainView()->AppendAnimationSet(ANIM_JUMP, -upX, -upY, JumpHeight, JumpTime, TRUE, m_BattlePlayerLst[ProtectTargetIndex].OrgRotation,
		CurrTime + JumpTime + LastTime, 0, FALSE, FALSE, 0,0,0,0,0,0,0);

	//pProCtrl->GetCharMainView()->AppendAnimationSet(ANIM_JUMP, 0, 0, 0, 0, TRUE, m_BattlePlayerLst[ProtectTargetIndex].OrgRotation,
	//CurrTime + JumpTime + LastTime , 0, FALSE, FALSE, 0,0,0,0,0,0,0);

	if (petActive)
	{
		pProCtrl->GetCharMainView()->AppendAnimationSet(pProCtrl->GetCharMainView()->GetBattleAnimation(), 
		-upX * 100, -upY * 100, 0, 0, TRUE, m_BattlePlayerLst[ProtectTargetIndex].OrgRotation,
		CurrTime + JumpTime + LastTime + JumpTime, 0, FALSE, FALSE, 0,0,0,0,0,0,0);
	}
	else
	{
	pProCtrl->GetCharMainView()->AppendAnimationSet(pProCtrl->GetCharMainView()->GetBattleAnimation(), 
		0, 0, 0, 0, TRUE, m_BattlePlayerLst[ProtectTargetIndex].OrgRotation,
		CurrTime + JumpTime + LastTime + JumpTime, 0, FALSE, FALSE, 0,0,0,0,0,0,0);
	}


}

BOOL CBattleControl::OnSkillComboAttack(AttackQuene QueneData)
{
	UInt32Vector HitLst;

	Int RunSpeed = gConfigure.uRunSpeed;
	Int JumpSpeed = gConfigure.uJumpSpeed;
	UInt32 RunTime = 0;
	UInt32 TotalWaitTime = 0; 
	Int JumpHeight = gConfigure.uJumpHeight;

	UInt32 CurrTime = ::timeGetTime();

	BOOL SkillUse = FALSE;
	BOOL AreaAttack = FALSE;
	UInt32 SkillAtkTime = 0;
	AttackerData  FirstAttcker;
	UInt32 PlayerSkillUseTime = 0;
	UInt32 AtkSkillTotalTime = 0;
	UInt32 StartStopTime = 0;
	Int SkillIndex = 0;
	Int ItemIndex = 0;
	UInt32 StartDelay = 0;
	UInt32Vector tempHitLstT;

	CAnimatedEntity EffectAtk;
	CAnimatedEntity EffectDef;

	Int MaxSkillTime = 0;

	for (Int i = 0; i < (Int)QueneData.AttackerLst.size(); i++)
	{
		Int AttackerIndex = QueneData.AttackerLst[i].AttackerIndex;
		CCharacterControl * pAttacker = m_BattlePlayerLst[AttackerIndex].pChar;
		QueneData.AttackerLst[i].Stop = TRUE;

		Int AttackIndex = ANIM_SKILL;

		int upX = 0;
		int upY = 0;


		SkillIndex = QueneData.AttackerLst[i].Skill;
		ItemIndex = QueneData.AttackerLst[i].Item;

		const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(SkillIndex);
		const ItemData * pItemData = gGlobal.m_resource->GetItemData(ItemIndex);

		String str;

		if(pSkillData)
		{
			if(m_S1==pAttacker->GetCharName()&&m_S2==pSkillData->name)
			{
				goto NEXTONE;
			}
			else
			{			
				m_S1=pAttacker->GetCharName();
				m_S2=pSkillData->name;
				str.Format(gGlobal.GetStringTable()->Get(_T("MSG_USED")), m_S1, m_S2);
			}
		}
		else if (pItemData)
		{
			m_S1=pAttacker->GetCharName();
			m_S2=pItemData->name;
			str.Format(gGlobal.GetStringTable()->Get(_T("MSG_USED")), m_S1, m_S2);
		}

		if(m_pStaticTextist&&m_pStaticTextsnd)
		{
			if (m_iShowState==0)
			{				
				m_pStaticTextist->SetOffsetXY(575,133);
				m_pStaticTextist->SetText(str);	
				m_pStaticTextist->Redraw();
				m_iShowState=1;
			}
			else if (m_iShowState==1)
			{	
				m_pStaticTextist->SetOffsetXY(575,113);
				m_pStaticTextsnd->SetOffsetXY(575,133);
				m_pStaticTextsnd->SetText(str);	
				m_pStaticTextsnd->Redraw();
				m_iShowState=2;
			}
			else if (m_iShowState==2)
			{	
				m_pStaticTextsnd->SetOffsetXY(575,113);
				m_pStaticTextist->SetOffsetXY(575,133);			
				m_pStaticTextist->SetText(str);	
				m_pStaticTextist->Redraw();
				m_iShowState=1;
			}	
		}

NEXTONE:	if (RunTime == 0)
			{
				if (QueneData.Jump)
					RunTime = (Int)sqrt(pow((double)upX , (int)2) + pow((double)upY, (int)2)) * 1000 / JumpSpeed;
				else
					RunTime = (Int)sqrt(pow((double)upX , (int)2) + pow((double)upY, (int)2)) * 1000 / RunSpeed;
			}

		if (i == 0)
			FirstAttcker = QueneData.AttackerLst[i];

		if (PlayerSkillUseTime == 0)
			PlayerSkillUseTime = m_BattlePlayerLst[FirstAttcker.AttackerIndex].pChar->GetCharMainView()->GetTotalTimeOfGivenAnimation(AttackIndex) / 2;

		Int TempTime = m_BattlePlayerLst[AttackerIndex].pChar->GetCharMainView()->GetTotalTimeOfGivenAnimation(AttackIndex);

		if (TempTime > MaxSkillTime)
			MaxSkillTime = TempTime;


		if (StartStopTime == 0)
		{
			tempHitLstT = m_BattlePlayerLst[FirstAttcker.AttackerIndex].pChar->GetCharMainView()->FindHitTime(AttackIndex, 
				m_BattlePlayerLst[FirstAttcker.AttackerIndex].pChar->GetCharMainView()->GetAnimationRotation());
			if (tempHitLstT.size() > 0)
				StartStopTime = tempHitLstT[i];
			else
				StartStopTime = PlayerSkillUseTime;
		}

		SkillUse = TRUE;
		
		String FileName;
		if (gConfigure.uNetwork)
		{
			if (pSkillData)
				FileName = pSkillData->effect_Atk;
		}
		else
		{
			if (pSkillData)
				FileName = pSkillData->effect_Atk;
			//else
			//	FileName = FileNameOffline;
		}

		//CAnimatedEntity Effect;
		EffectAtk.LoadEntityFile(FileName.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);
		tempHitLstT =  EffectAtk.FindHitTime(ANIM_IDLE1, 
			ERROR_INDEX);
		if (tempHitLstT.size() > 0)
			SkillAtkTime = tempHitLstT[0];
		else
			SkillAtkTime = EffectAtk.GetTotalTimeOfGivenAnimation() / 2;

		if (AtkSkillTotalTime == 0)
		{
			AtkSkillTotalTime = EffectAtk.GetTotalTimeOfGivenAnimation();
		}

		EffectData EData;
		EData.effectFileName = FileName;
		EData.targetEnt = m_BattlePlayerLst[AttackerIndex].pChar->GetCharMainView();
		EData.rotation = m_BattlePlayerLst[AttackerIndex].pChar->GetCharMainView()->GetAnimationRotation();
		m_EffectDataLst.insert(std::make_pair(CurrTime + StartDelay + RunTime + PlayerSkillUseTime, EData));
		m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
		m_SoundDataLst.insert(std::make_pair(CurrTime + StartDelay  + RunTime + PlayerSkillUseTime, pSkillData->sound_Atk));
		CBatCharacter * pBatChar = CastNetObject<CBatCharacter>(pAttacker->GetBatActorObject());
		if (pBatChar)
			m_SoundDataLst.insert(std::make_pair(CurrTime + RunTime, SoundNameArray[pBatChar->GetSexClass()][PLAYER_SOUND_SKILL]));

		if (i == 0)
		{
			//if(QueneData.AttackerLst[i].Skill)
			{

				String FileName;// = TestEffectHurtSuffix[SkillIndex].c_str();

				if (gConfigure.uNetwork)
				{
					if (pSkillData)
						FileName = pSkillData->effect_Hurt;
				}
				else
				{
					if (pSkillData)
						FileName = pSkillData->effect_Hurt;
					//else
					//	FileName = FileNameOffline;
				}

				EffectDef.LoadEntityFile(FileName.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);
				//Effect.WaitResReady();
				EffectDef.SetRotation(pAttacker->GetCharMainView()->GetAnimationRotation());
				HitLst = EffectDef.FindHitTime(ANIM_IDLE1, 
					ERROR_INDEX);

				if (HitLst.size() == 0)
				{
					HitLst.push_back(EffectDef.GetTotalTimeOfGivenAnimation(EffectDef.GetAnimationIndex(), 
					EffectDef.GetAnimationRotation())/2);
				}
				//AttackIndex = ANIM_SKILL;
				UInt32 WaitTime = SkillDelayTime + RunTime + EffectDef.GetTotalTimeOfGivenAnimation() + RunTime;
				
				if (WaitTime > TotalWaitTime)
					TotalWaitTime = WaitTime;
				if (EffectDef.GetOffsetType() == ENTITYOFFSETTYPE_FULLSCREEN)
				{
					//AddBattleParticle(FileName, 0, TRUE, 1, NULL, TRUE, ParticleExtraZ, Effect.GetAnimationRotation());
					AreaAttack = TRUE;
				}
				else
					AreaAttack = FALSE;
			}
		}

		
		if(QueneData.Jump)
			PerformJumpAttack(pAttacker, upX, upY, JumpHeight, RunTime, CurrTime + StartDelay , AttackIndex, AtkSkillTotalTime, StartStopTime,
			QueneData.AttackerLst[i].Back);
		else
			PerformRunAttack(pAttacker, upX, upY, RunTime, CurrTime + StartDelay , AttackIndex, AtkSkillTotalTime, StartStopTime,
			QueneData.AttackerLst[i].Back);

		EffectData EData2;
		EData2.effectFileName = testComEffect;
		EData2.DeleteTime = TotalWaitTime;
		EData2.targetEnt = m_BattlePlayerLst[AttackerIndex].pChar->GetCharMainView();
		EData2.rotation = m_BattlePlayerLst[AttackerIndex].pChar->GetCharMainView()->GetAnimationRotation();
		m_EffectDataLst.insert(std::make_pair(CurrTime + StartDelay + RunTime + PlayerSkillUseTime, EData2));
		m_PlayedEffect.insert(std::make_pair(EData2.effectFileName, EData2));
	}

	if ((UInt32)MaxSkillTime >= (UInt32)StartStopTime + (UInt32)AtkSkillTotalTime)
		m_NextAttackQueueTime = CurrTime +  MaxSkillTime;
	else
		m_NextAttackQueueTime = CurrTime +  StartStopTime + AtkSkillTotalTime;

	return TRUE;
}
BOOL CBattleControl::OnSkillAttack(AttackQuene QueneData)
{
	//VMProtectBegin; VM_START;

	UInt32Vector HitLst;
	
	Int RunSpeed = gConfigure.uRunSpeed;
	UInt32 RunTime = 0;
	UInt32 TotalWaitTime = 0; 
	Int JumpHeight = gConfigure.uJumpHeight;
	Int JumpSpeed = gConfigure.uJumpSpeed;

	UInt32 CurrTime = ::timeGetTime();

	BOOL SkillUse = FALSE;
	BOOL AreaAttack = FALSE;
	UInt32 SkillAtkTime = 0;
	AttackerData  FirstAttcker;
	UInt32 PlayerSkillUseTime = 0;
	UInt32 AtkSkillTotalTime = 0;
	UInt32 StartStopTime = 0;
	Int SkillIndex = 0;
	Int ItemIndex = 0;
	UInt32 StartDelay = 0;
	UInt32 PlayerAnimTime = 0;
	BOOL OneByOne = FALSE;
	UInt32Vector tempHitLstT;
	Int atkAction = 0;

	/*if (QueneData.AttackerLst.size() > 1)
	{
		OnSkillComboAttack(QueneData);
		if (m_NextAttackQueueTime > CurrTime)
			StartDelay = m_NextAttackQueueTime - CurrTime;
	}
	*/

	CAnimatedEntity EffectAtk;
	CAnimatedEntity EffectDef;

	Int TotalWidth = (Int)(QueneData.AttackerLst.size() - 1) * BlockSizeX * 2;
	Int TotalHeight = (Int)(QueneData.AttackerLst.size() - 1) * BlockSizeY * 2;

	//only allow one person skill
	for (Int i = 0; i < (Int)QueneData.AttackerLst.size() && i < 1; i++)
	{
		Int AttackerIndex = QueneData.AttackerLst[i].AttackerIndex;
		CCharacterControl * pAttacker = m_BattlePlayerLst[AttackerIndex].pChar;

		if (i == 0)
			atkAction = QueneData.AttackerLst[i].AtkAction;
		if (QueneData.AttackerLst[i].OneByOne)
			OneByOne = QueneData.AttackerLst[i].OneByOne;

		Int AttackIndex = QueneData.AttackerLst[i].AttackIndex;

		int upX = QueneData.AttackerLst[i].AttackDes.PixelPosX - (pAttacker->GetCharMainView()->GetEntPixelPos().PixelPosX
			+ pAttacker->GetCharMainView()->GetEntOffsetX());
		int upY = QueneData.AttackerLst[i].AttackDes.PixelPosY - (pAttacker->GetCharMainView()->GetEntPixelPos().PixelPosY
			+ pAttacker->GetCharMainView()->GetEntOffsetY());

		if ((upX != 0 || upY != 0) && QueneData.AttackerLst.size() > 1 )
		{
			upX += (Int)((i)* TotalWidth / (Int)(QueneData.AttackerLst.size() - 1) - TotalWidth / 2);
			upY += (Int)(TotalHeight / 2 - (i)* TotalHeight / (Int)(QueneData.AttackerLst.size() - 1));
		}

		SkillIndex = QueneData.AttackerLst[i].Skill;
		ItemIndex = QueneData.AttackerLst[i].Item;

		const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(SkillIndex);
		const ItemData * pItemData = gGlobal.m_resource->GetItemData(ItemIndex);
		
		String str;
	
		if(pSkillData)
		{
			if(m_S1==pAttacker->GetCharName()&&m_S2==pSkillData->name)
			{
				goto NEXT;
			}
			else
			{			
				m_S1=pAttacker->GetCharName();
				m_S2=pSkillData->name;
				str.Format(gGlobal.GetStringTable()->Get(_T("MSG_USED")), m_S1, m_S2);
			}
		}
		else if (pItemData)
		{
			m_S1=pAttacker->GetCharName();
			m_S2=pItemData->name;
			str.Format(gGlobal.GetStringTable()->Get(_T("MSG_USED")), m_S1, m_S2);
		}
		
		if(m_pStaticTextist&&m_pStaticTextsnd)
		{
			if (m_iShowState==0)
			{				
				m_pStaticTextist->SetOffsetXY(575,133);
				m_pStaticTextist->SetText(str);	
				m_pStaticTextist->Redraw();
				m_iShowState=1;
			}
			else if (m_iShowState==1)
			{	
				m_pStaticTextist->SetOffsetXY(575,113);
				m_pStaticTextsnd->SetOffsetXY(575,133);
				m_pStaticTextsnd->SetText(str);	
				m_pStaticTextsnd->Redraw();
				m_iShowState=2;
			}
			else if (m_iShowState==2)
			{	
				m_pStaticTextsnd->SetOffsetXY(575,113);
				m_pStaticTextist->SetOffsetXY(575,133);			
				m_pStaticTextist->SetText(str);	
				m_pStaticTextist->Redraw();
				m_iShowState=1;
			}	
		}

NEXT:	if (RunTime == 0)
		{
			if (QueneData.Jump)
				RunTime = (Int)sqrt(pow((double)upX , (int)2) + pow((double)upY, (int)2)) * 1000 / JumpSpeed;
			else
				RunTime = (Int)sqrt(pow((double)upX , (int)2) + pow((double)upY, (int)2)) * 1000 / RunSpeed;
		}

		if (i == 0)
			FirstAttcker = QueneData.AttackerLst[i];
		//upX /= BlockSizeX;
		//upY /= BlockSizeY / 2;

			if (PlayerSkillUseTime == 0)
				PlayerSkillUseTime = m_BattlePlayerLst[FirstAttcker.AttackerIndex].pChar->GetCharMainView()->GetTotalTimeOfGivenAnimation(AttackIndex) / 2;

			if (PlayerAnimTime == 0)
				PlayerAnimTime = m_BattlePlayerLst[FirstAttcker.AttackerIndex].pChar->GetCharMainView()->GetTotalTimeOfGivenAnimation(AttackIndex);

			if (QueneData.AttackerLst[i].Stop)
			{
				if (StartStopTime == 0)
				{
					tempHitLstT = m_BattlePlayerLst[FirstAttcker.AttackerIndex].pChar->GetCharMainView()->FindHitTime(AttackIndex, 
						m_BattlePlayerLst[FirstAttcker.AttackerIndex].pChar->GetCharMainView()->GetAnimationRotation());
					if (tempHitLstT.size() > 0)
						StartStopTime = tempHitLstT[i];
					else
						StartStopTime = PlayerSkillUseTime;
				}
			}
			else
				PlayerSkillUseTime = RunTime;
			SkillUse = TRUE;
			
			String FileName;
			if (gConfigure.uNetwork)
			{
				if (pSkillData)
					FileName = pSkillData->effect_Atk;
				else if (pItemData)
					FileName = pItemData->effect_Atk;
			}
			else
			{
				if (pSkillData)
					FileName = pSkillData->effect_Atk;
				else if (pItemData)
					FileName = pItemData->effect_Atk;
//				else
//					FileName = FileNameOffline;
			}
			
			//CAnimatedEntity Effect;
			EffectAtk.LoadEntityFile(FileName.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);
			tempHitLstT =  EffectAtk.FindHitTime(ANIM_IDLE1, 
					ERROR_INDEX);
			if (tempHitLstT.size() > 0)
				SkillAtkTime = tempHitLstT[0];
			else
				SkillAtkTime = EffectAtk.GetTotalTimeOfGivenAnimation() / 2;
			
			if (AtkSkillTotalTime == 0)
			{
				if (!QueneData.AttackerLst[i].Stop)
				{
					

					String FileName;// = TestEffectHurtSuffix[SkillIndex].c_str();

					if (gConfigure.uNetwork)
					{
						if (pSkillData)
							FileName = pSkillData->effect_Hurt;
						else if (pItemData)
							FileName = pItemData->effect_Hurt;
					}
					else
					{
						if (pSkillData)
							FileName = pSkillData->effect_Hurt;
						else if (pItemData)
							FileName = pItemData->effect_Hurt;
//						else
//							FileName = FileNameOffline;
					}
					
					EffectDef.LoadEntityFile(FileName.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);

					UInt32 HurtDelay = 0;
					Int Count = 0;
					for (Int k = 0; k < (Int)QueneData.DefenserLst.size(); k++)
					{
						if ((Int)QueneData.DefenserLst[k].HurtDelay > Count)
							Count = QueneData.DefenserLst[k].HurtDelay;
					}

					HurtDelay = Count * EffectDef.GetTotalTimeOfGivenAnimation();
					if (Count > 0)
						HurtDelay += 100;

					UInt32 Time = m_BattlePlayerLst[FirstAttcker.AttackerIndex].pChar->GetCharMainView()->GetTotalTimeOfGivenAnimation(AttackIndex);
					UInt32 HurtTime = EffectDef.GetTotalTimeOfGivenAnimation() + HurtDelay;
					Int factor = 0;
					/*if (Time != 0)
					{
						factor = HurtTime / Time;

						if (HurtTime % Time > 2 * Time / 3)
							factor ++;
					}*/

					if (factor > 0)
						factor --;

					PlayerAnimTime = (1 + factor) * m_BattlePlayerLst[FirstAttcker.AttackerIndex].pChar->GetCharMainView()->GetTotalTimeOfGivenAnimation(AttackIndex);

					//if (factor == 0)
					//	factor = 1;
					AtkSkillTotalTime = Time * factor;
					//AtkSkillTotalTime =  EffectHurt.GetTotalTimeOfGivenAnimation();
				}
				else
				{
					if (StartStopTime != 0)
						PlayerAnimTime = m_BattlePlayerLst[FirstAttcker.AttackerIndex].pChar->GetCharMainView()->GetTotalTimeOfGivenAnimation(AttackIndex) +
						StartStopTime + SkillAtkTime;
					AtkSkillTotalTime = EffectAtk.GetTotalTimeOfGivenAnimation();
				}
			}

			EffectData EData;
			EData.effectFileName = FileName;
			EData.targetEnt = m_BattlePlayerLst[AttackerIndex].pChar->GetCharMainView();
//			EData.pos = EFFECT_POS_TYPE_ATTACH;
			EData.rotation = m_BattlePlayerLst[AttackerIndex].pChar->GetCharMainView()->GetAnimationRotation();
			m_EffectDataLst.insert(std::make_pair(CurrTime + StartDelay + RunTime + PlayerSkillUseTime, EData));
			m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
			if (pSkillData)
			{
				CBatCharacter * pBatChar = CastNetObject<CBatCharacter>(pAttacker->GetBatActorObject());
				if (pBatChar)
					m_SoundDataLst.insert(std::make_pair(CurrTime + RunTime, SoundNameArray[pBatChar->GetSexClass()][PLAYER_SOUND_ATK]));
				m_SoundDataLst.insert(std::make_pair(CurrTime + StartDelay  + RunTime + PlayerSkillUseTime, pSkillData->sound_Atk));
			}
			else if (pItemData)
				m_SoundDataLst.insert(std::make_pair(CurrTime + StartDelay  + RunTime + PlayerSkillUseTime, pItemData->sound_Atk));
			else
				m_SoundDataLst.insert(std::make_pair(CurrTime + StartDelay  + RunTime + PlayerSkillUseTime,
				_T("SoundB0103_NoWeaponAtk.wav")));

		

		if (i == 0)
		{
			//if(QueneData.AttackerLst[i].Skill)
			{
				

				String FileName;// = TestEffectHurtSuffix[SkillIndex].c_str();

				if (gConfigure.uNetwork)
				{
					if (pSkillData)
						FileName = pSkillData->effect_Hurt;
					else if (pItemData)
						FileName = pItemData->effect_Hurt;
				}
				else
				{
					if (pSkillData)
						FileName = pSkillData->effect_Hurt;
					else if (pItemData)
						FileName = pItemData->effect_Hurt;
//					else
//						FileName = FileNameOffline;
				}

				EffectDef.LoadEntityFile(FileName.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);
				//Effect.WaitResReady();
				EffectDef.SetRotation(pAttacker->GetCharMainView()->GetAnimationRotation());
				HitLst = EffectDef.FindHitTime(ANIM_IDLE1, 
					ERROR_INDEX);

				if (HitLst.size() == 0)
				{
					HitLst.push_back(EffectDef.GetTotalTimeOfGivenAnimation(EffectDef.GetAnimationIndex(), 
					EffectDef.GetAnimationRotation())/2);
				}
				//AttackIndex = ANIM_SKILL;
				UInt32 WaitTime = SkillDelayTime + RunTime + SkillAtkTime + EffectDef.GetTotalTimeOfGivenAnimation() + RunTime;
				
				if (OneByOne)
					WaitTime = SkillDelayTime + RunTime + PlayerAnimTime;

				if (WaitTime > TotalWaitTime)
					TotalWaitTime = WaitTime;
					
				if (EffectDef.GetOffsetType() == ENTITYOFFSETTYPE_FULLSCREEN)
				{
					//AddBattleParticle(FileName, 0, TRUE, 1, NULL, TRUE, ParticleExtraZ, Effect.GetAnimationRotation());
					AreaAttack = TRUE;
				}
				else
					AreaAttack = FALSE;
			}
		}

		
		if(QueneData.Jump)
			PerformJumpAttack(pAttacker, upX, upY, JumpHeight, RunTime, CurrTime + StartDelay , AttackIndex, AtkSkillTotalTime, StartStopTime,
			QueneData.AttackerLst[i].Back);
		else
			PerformRunAttack(pAttacker, upX, upY, RunTime, CurrTime + StartDelay , AttackIndex, AtkSkillTotalTime, StartStopTime,
			QueneData.AttackerLst[i].Back);
	}

	Int ProtectDelay = 0;

	for (Int k = 0; k < (Int)QueneData.DefenserLst.size(); k++)
	{
		SkillIndex = QueneData.DefenserLst[k].Skill;
		const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(SkillIndex);
		const ItemData * pItemData = gGlobal.m_resource->GetItemData(ItemIndex);
		if (QueneData.DefenserLst[k].ProtectIndex >= 0 && QueneData.DefenserLst[k].ProtectIndex < (Int)m_BattlePlayerLst.size())
		{
			ProtectDelay = RunTime + PlayerSkillUseTime + SkillAtkTime + m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()
				->GetTotalTimeOfGivenAnimation(ANIM_HURT)
				+ HitLst[HitLst.size() - 1];
			PerformJumpProtect(&m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex], QueneData.DefenserLst[k].ProtectIndex, JumpHeight, 
				RunTime / 2, CurrTime + StartDelay , ProtectDelay, false);
		}
		else if (QueneData.DefenserLst[k].petActive)
		{
			ProtectDelay = RunTime + PlayerSkillUseTime + SkillAtkTime + m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()
				->GetTotalTimeOfGivenAnimation(ANIM_HURT)
				+ HitLst[HitLst.size() - 1];
			PerformJumpProtect(&m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex], QueneData.DefenserLst[k].DefenserIndex, JumpHeight, 
				RunTime / 2, CurrTime + StartDelay , ProtectDelay, true);
		}

		for (Int j = 0; j < (Int)HitLst.size(); j ++)
		{

			BeingAttackData AtkData;
			if (k == QueneData.DefenserLst.size() - 1
				&& j == HitLst.size() -1)
				AtkData.AtkData = FirstAttcker;
			{
				AtkData.AtkSkillIndex = SkillIndex;
				//AtkData.DefSkillIndex = SkillIndex;
				if (k == 0 && j == 0)
				{

					//AtkData.AtkTimeToHitSkill = RunTime + PlayerSkillUseTime;
					//AtkData.AtkSkillPlayed = FALSE;
				}
				if (AreaAttack )
				{
					if (k == 0 && j == 0)
					{
						//AtkData.DefTimeToHitSkill = RunTime + PlayerSkillUseTime + SkillAtkTime;
						//AtkData.DefSkillPlayed = FALSE;
						//Char FileNameOffline[50];
						//GetPrivateProfileString(_T("BATTLESETTING"), _T("GETHIT"), _T(""), FileNameOffline, 50, CONFIGURATION_INI_FILE);

						String FileName;// = TestEffectHurtSuffix[SkillIndex].c_str();

						if (gConfigure.uNetwork)
						{
							if (pSkillData)
								FileName = pSkillData->effect_Hurt;
							else if (pItemData)
								FileName = pItemData->effect_Hurt;
						}
						else
						{
							if (pSkillData)
								FileName = pSkillData->effect_Hurt;
							else if (pItemData)
								FileName = pItemData->effect_Hurt;
//							else
//								FileName = FileNameOffline;
						}
						EffectData EData;
						EData.effectFileName = FileName;
						EData.rotation = m_BattlePlayerLst[FirstAttcker.AttackerIndex].pChar->GetCharMainView()->GetAnimationRotation();
						EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView();
						
						EData.attach = QueneData.DefenserLst[k].DefReaction != BATCMD_MISS;
//						EData.pos = EFFECT_POS_TYPE_ATTACH;

						UInt32 HurtDelay = 0;
						if (QueneData.DefenserLst[k].HurtDelay > 0)
							HurtDelay = EffectDef.GetTotalTimeOfGivenAnimation() * QueneData.DefenserLst[k].HurtDelay + 100;

						StartDelay += HurtDelay;
						
						if (QueneData.DefenserLst[k].ProtectIndex == -1)
						{
							m_EffectDataLst.insert(std::make_pair(CurrTime + StartDelay  + RunTime + PlayerSkillUseTime + SkillAtkTime, EData));
							m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
						}
						if (pSkillData)
						m_SoundDataLst.insert(std::make_pair(CurrTime + StartDelay  + RunTime + PlayerSkillUseTime + SkillAtkTime, pSkillData->sound_Hurt));
						else if (pItemData)
								m_SoundDataLst.insert(std::make_pair(CurrTime + StartDelay  + RunTime + PlayerSkillUseTime + SkillAtkTime, pItemData->sound_Hurt));

						//UInt32 WaitTime =  StartDelay  + RunTime + PlayerSkillUseTime + SkillAtkTime + EffectDef.GetTotalTimeOfGivenAnimation();
						UInt32 WaitTime = RunTime + SkillAtkTime + StartStopTime + EffectDef.GetTotalTimeOfGivenAnimation();
						if (OneByOne)
							WaitTime = RunTime + PlayerAnimTime;

						if (WaitTime > TotalWaitTime)
							TotalWaitTime = WaitTime;
							
							
					}
				}
				else
				{
					if (j == 0)
					{
						//Char FileNameOffline[50];
						//GetPrivateProfileString(_T("BATTLESETTING"), _T("GETHIT"), _T(""), FileNameOffline, 50, CONFIGURATION_INI_FILE);

						String FileName;// = TestEffectHurtSuffix[SkillIndex].c_str();

						if (gConfigure.uNetwork)
						{
							if (pSkillData)
								FileName = pSkillData->effect_Hurt;
							else if (pItemData)
								FileName = pItemData->effect_Hurt;
						}
						else
						{
							if (pSkillData)
								FileName = pSkillData->effect_Hurt;
							else if (pItemData)
								FileName = pItemData->effect_Hurt;
//							else
//								FileName = FileNameOffline;
						}
						EffectData EData;
						EData.effectFileName = FileName;
						EData.rotation = m_BattlePlayerLst[FirstAttcker.AttackerIndex].pChar->GetCharMainView()->GetAnimationRotation();
						EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView();
						EData.attach = QueneData.DefenserLst[k].DefReaction != BATCMD_MISS;
//						EData.pos = EFFECT_POS_TYPE_ATTACH;

						UInt32 HurtDelay = 0;
						if (QueneData.DefenserLst[k].HurtDelay > 0)
							HurtDelay = EffectDef.GetTotalTimeOfGivenAnimation() * QueneData.DefenserLst[k].HurtDelay + 100;

						StartDelay += HurtDelay;
						
						if (QueneData.DefenserLst[k].ProtectIndex == -1)
						{
							m_EffectDataLst.insert(std::make_pair(CurrTime + StartDelay  + RunTime + PlayerSkillUseTime + SkillAtkTime, EData));
							m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
						}
						if (pSkillData)
						m_SoundDataLst.insert(std::make_pair(CurrTime + StartDelay  + RunTime + PlayerSkillUseTime + SkillAtkTime, pSkillData->sound_Hurt));
						else if (pItemData)
								m_SoundDataLst.insert(std::make_pair(CurrTime + StartDelay  + RunTime + PlayerSkillUseTime + SkillAtkTime, pItemData->sound_Hurt));

						//UInt32 WaitTime =  StartDelay  + RunTime + PlayerSkillUseTime + SkillAtkTime + EffectDef.GetTotalTimeOfGivenAnimation();
						UInt32 WaitTime = RunTime + StartStopTime + SkillAtkTime + EffectDef.GetTotalTimeOfGivenAnimation();

						if (OneByOne)
							WaitTime = RunTime + PlayerAnimTime;
						if (WaitTime > TotalWaitTime)
							TotalWaitTime = WaitTime;
							
						//AtkData.DefTimeToHitSkill = RunTime + PlayerSkillUseTime + SkillAtkTime;
						//AtkData.DefSkillPlayed = FALSE;
					}
				}
			}

			if (QueneData.DefenserLst[k].DefReaction == BATCMD_DEFEND_HIT ||
				QueneData.DefenserLst[k].DefReaction == BATCMD_COUNTER_ATTACK_DEF_HIT ||
				QueneData.DefenserLst[k].DefReaction == BATCMD_DEFEND_CRIT_HIT)
			{
				m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()->SetAnimationIndexByIndex(ANIM_DEF, FALSE);
				AtkData.HitAnimationIndex = ANIM_DEF;
			}
			else if (QueneData.DefenserLst[k].DefReaction == BATCMD_PROTECT_HIT)
			{
				AtkData.HitAnimationIndex = ANIM_DEF;
			}
			else if (QueneData.DefenserLst[k].DefReaction == BATCMD_USED_ITEM)
				AtkData.HitAnimationIndex = -1;
			else
				AtkData.HitAnimationIndex = ANIM_HURT;
			AtkData.StartTime = CurrTime + StartDelay  + QueneData.DefenserLst[k].HurtDelay;
			if (OneByOne)
				AtkData.StartTime -= 600;

			AtkData.DefData = QueneData.DefenserLst[k];


			if (ProtectDelay > 0 && QueneData.DefenserLst[k].DefReaction != BATCMD_PROTECT_HIT)
			{
				AtkData.TimeToHit = RunTime + PlayerSkillUseTime + SkillAtkTime +  HitLst[j];//ProtectDelay + DelatAdjust;
				AtkData.DefData.Attri.HPChange = QueneData.DefenserLst[k].Attri.HPChange;
				if (j == HitLst.size() - 1)
					AtkData.DefData.ShowNum = TRUE;
				else
					AtkData.DefData.ShowNum = FALSE;
				m_EntityBeingAttackLst.insert(std::make_pair(m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar ,AtkData));
				if (!QueneData.DefenserLst[k].petActive)
					continue;
				
			}
			else
				AtkData.TimeToHit = RunTime + PlayerSkillUseTime + SkillAtkTime +  HitLst[j];

			//Char FileNameOffline[50];
			//GetPrivateProfileString(_T("BATTLESETTING"), _T("SKILLSPARK"), _T(""), FileNameOffline, 50, CONFIGURATION_INI_FILE);

			String FileName;// = TestEffectHurtSuffix[SkillIndex].c_str();

			if (gConfigure.uNetwork)
			{
				if (pSkillData)
					FileName = pSkillData->effect_Spark;
				else if (pItemData)
					FileName = pItemData->effect_Spark;
			}
			else
			{
				if (pSkillData)
					FileName = pSkillData->effect_Spark;
				else if (pItemData)
					FileName = pItemData->effect_Spark;
//				else
//					FileName = FileNameOffline;
			}

			if (QueneData.DefenserLst[k].DefReaction != BATCMD_MISS)
			{
				EffectData EData;
				EData.effectFileName = FileName;
				EData.rotation = m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()->GetAnimationRotation();
				EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView();
				EData.attach = FALSE;


				
				if (QueneData.DefenserLst[k].ProtectIndex == -1)
				{
					m_EffectDataLst.insert(std::make_pair(CurrTime + StartDelay  + AtkData.TimeToHit, EData));
					m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
				}
				if (pSkillData)
				m_SoundDataLst.insert(std::make_pair(CurrTime + StartDelay  + AtkData.TimeToHit, pSkillData->sound_Spark));
				else if (pItemData)
					m_SoundDataLst.insert(std::make_pair(CurrTime + StartDelay  + AtkData.TimeToHit, pItemData->sound_Spark));

				if (QueneData.DefenserLst[k].petActive)
				{
					EData.rotation = m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pPet->GetCharMainView()->GetAnimationRotation();
					EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pPet->GetCharMainView();
					//GetPrivateProfileString(_T("BATTLESETTING"), _T("DEFSPARK"), _T(""), FileNameOffline, 50, CONFIGURATION_INI_FILE);
					EData.effectFileName = defSpark;
					EData.attach = TRUE;
					m_EffectDataLst.insert(std::make_pair(CurrTime + AtkData.TimeToHit, EData));
					m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
				}

				if (QueneData.DefenserLst[k].DefReaction == BATCMD_CRIT_HIT)
				{
					EffectData EData;
					FileName = CrtHitEnt;
					EData.effectFileName = FileName;
					EData.rotation = m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()->GetAnimationRotation();
					EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView();
					EData.attach = FALSE;
					m_EffectDataLst.insert(std::make_pair(CurrTime + StartDelay  + AtkData.TimeToHit, EData));
					m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
				}
				else if (QueneData.DefenserLst[k].DefReaction == BATCMD_DEFEND_HIT ||
					QueneData.DefenserLst[k].DefReaction == BATCMD_PROTECT_HIT)
				{
					FileName = gConfigure.config.GetString(_T("BATTLESETTING"), _T("DEFSPARK"), _T(""));
					
					EffectData EData;
					EData.effectFileName = FileName;
					EData.rotation = m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()->GetAnimationRotation();
					EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView();
					EData.attach = TRUE;
					m_EffectDataLst.insert(std::make_pair(CurrTime + StartDelay  + AtkData.TimeToHit, EData));
					m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
					
				}
				else if (QueneData.DefenserLst[k].DefReaction == BATCMD_DEFEND_CRIT_HIT)
				{
					FileName = gConfigure.config.GetString(_T("BATTLESETTING"), _T("DEFSPARK"), _T(""));
					
					EffectData EData;
					EData.effectFileName = FileName;
					EData.rotation = m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()->GetAnimationRotation();
					EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView();
					EData.attach = TRUE;
					m_EffectDataLst.insert(std::make_pair(CurrTime + StartDelay  + AtkData.TimeToHit, EData));
					m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));

					FileName = CrtHitEnt;
					EData.effectFileName = FileName;
					EData.rotation = m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()->GetAnimationRotation();
					EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView();
					EData.attach = FALSE;
					m_EffectDataLst.insert(std::make_pair(CurrTime + StartDelay  + AtkData.TimeToHit, EData));
					m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
					
				}
				if (QueneData.DefenserLst[k].DefReaction != BATCMD_DEFEND_CRIT_HIT &&
					(atkAction == BATCMD_CRIT_SKILL || atkAction == BATCMD_CRIT_ATTACK) )
				{
					FileName = CrtHitEnt;
					EffectData EData;
					EData.effectFileName = FileName;
					EData.rotation = m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()->GetAnimationRotation();
					EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView();
					EData.attach = FALSE;
					m_EffectDataLst.insert(std::make_pair(CurrTime + StartDelay  + AtkData.TimeToHit, EData));
					m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
					
				}
			}
			

			if (j == HitLst.size() - 1)
				AtkData.DefData.Attri.HPChange = QueneData.DefenserLst[k].Attri.HPChange;
			else
				AtkData.DefData.ShowNum = FALSE;

			AtkData.DefData.Move = QueneData.DefenserLst[k].Move && AtkData.DefData.ShowNum;

			m_EntityBeingAttackLst.insert(std::make_pair(m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar ,AtkData));
		}

	}

	m_NextAttackQueueTime = CurrTime + StartDelay  + TotalWaitTime;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("AnimTime: Skill time %d"), 
		StartDelay  + TotalWaitTime);

	return TRUE;

	//VMProtectEnd; VM_END;
}

BOOL CBattleControl::OnAttack(AttackQuene QueneData)
{
	//VMProtectBegin; VM_START;

	UInt32Vector HitLst;
	
	Int RunSpeed = gConfigure.uRunSpeed;
	Int JumpSpeed = gConfigure.uJumpSpeed;
	UInt32 RunTime = 0;
	UInt32 TotalWaitTime = 0; 
	Int JumpHeight = gConfigure.uJumpHeight;

	UInt32 CurrTime = ::timeGetTime();

	BOOL SkillUse = FALSE;
	BOOL AreaAttack = FALSE;
	UInt32 SkillAtkTime = 0;
	AttackerData FirstAttcker;
	UInt32 PlayerSkillUseTime = 0;
	UInt32 AtkSkillTotalTime = 0;
	UInt32 StartStopTime = 0;
	Int SkillIndex = 0;

	CAnimatedEntity EffectAtk;
	CAnimatedEntity EffectDef;

	if (QueneData.AttackerLst.size() == 0)
		HitLst.push_back(0);

	for (Int i = 0; i < (Int)QueneData.AttackerLst.size(); i++)
	{
		CCharacterControl * pAttacker = m_BattlePlayerLst[QueneData.AttackerLst[i].AttackerIndex].pChar;

		Int AttackIndex = QueneData.AttackerLst[i].AttackIndex;

		int upX = QueneData.AttackerLst[i].AttackDes.PixelPosX - (pAttacker->GetCharMainView()->GetEntPixelPos().PixelPosX
			+ pAttacker->GetCharMainView()->GetEntOffsetX());
		int upY = QueneData.AttackerLst[i].AttackDes.PixelPosY - (pAttacker->GetCharMainView()->GetEntPixelPos().PixelPosY
			+ + pAttacker->GetCharMainView()->GetEntOffsetY());

		SkillIndex = QueneData.AttackerLst[i].Skill;
		
		if (RunTime == 0)
		{
			if (QueneData.Jump)
				RunTime = (Int)sqrt(pow((double)upX , (int)2) + pow((double)upY, (int)2)) * 1000 / JumpSpeed;
			else
				RunTime = (Int)sqrt(pow((double)upX , (int)2) + pow((double)upY, (int)2)) * 1000 / RunSpeed;
		}

		if (i == 0)
			FirstAttcker = QueneData.AttackerLst[i];
		//upX /= BlockSizeX;
		//upY /= BlockSizeY / 2;

		//if(QueneData.AttackerLst[i].Skill == 0)
		{
			if (upX != 0 || upY != 0)
			{
				if(pAttacker->GetCharMainView()->GetAnimationRotation() == NORTHWEST)
				{
					//QGLMath::MoveToBlock(upX, upY, SOUTHEAST, skipY);
					 upX += BlockSizeX * 2;
					 upY += BlockSizeY * 2;
				}
				else
				{
					upX -= BlockSizeX * 2;
					upY -= BlockSizeY * 2;
					//QGLMath::MoveToBlock(upX, upY, NORTHWEST, skipY);
				}
			}
		}
		if (i == 0)
		{
			//else
			{
				HitLst = pAttacker->GetCharMainView()->FindHitTime(AttackIndex, 
					pAttacker->GetCharMainView()->GetAnimationRotation());

				if (HitLst.size() == 0)
				{
					HitLst.push_back(pAttacker->GetCharMainView()->GetTotalTimeOfGivenAnimation(AttackIndex, 
					pAttacker->GetCharMainView()->GetAnimationRotation())/2);
				}

				UInt32 WaitTime = RunTime + pAttacker->GetCharMainView()->GetTotalTimeOfGivenAnimation(AttackIndex, 
					pAttacker->GetCharMainView()->GetAnimationRotation()) + RunTime;
				if (WaitTime > TotalWaitTime)
					TotalWaitTime = WaitTime;
			}
		}

		if (!pAttacker->GetCharMainView()->IsAnimExist(ANIM_JUMP))
			QueneData.Jump = false;

		if(QueneData.Jump)
			PerformJumpAttack(pAttacker, upX, upY, JumpHeight, RunTime, CurrTime, AttackIndex, AtkSkillTotalTime, StartStopTime,
			QueneData.AttackerLst[i].Back);
		else
			PerformRunAttack(pAttacker, upX, upY, RunTime, CurrTime, AttackIndex, AtkSkillTotalTime, StartStopTime,
			QueneData.AttackerLst[i].Back);

		//play sound
		CBatActor * pBatActor = pAttacker->GetBatActorObject();
		if (pBatActor)
		{
			if (pBatActor->GetClassID() == BatMob_CLASSID)
			{
				CBatMob * pBatMob = CastNetObject<CBatMob>(pBatActor);
				if (pBatMob)
				{
					MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(pBatMob->GetMob_id());
					if( mobTemplate )
					{
						if (QueneData.AttackerLst[i].AtkAction == BATCMD_ATTACK)
						{
							m_SoundDataLst.insert(std::make_pair(CurrTime + RunTime, String(mobTemplate->m_basicinfo->mob_sound_atk1)));
						}
						else
						{
							m_SoundDataLst.insert(std::make_pair(CurrTime + RunTime, String(mobTemplate->m_basicinfo->mob_sound_atk2)));
						}
					}
				}
			}
			else if (pBatActor->GetClassID() == BatCharacter_CLASSID)
			{
				CBatCharacter * pBatChar = CastNetObject<CBatCharacter>(pBatActor);
				if (pBatChar)
				{
					//add attack sound
					m_SoundDataLst.insert(std::make_pair(CurrTime + RunTime, SoundNameArray[pBatChar->GetSexClass()][PLAYER_SOUND_ATK]));
					
					const ItemData * pItemData = gGlobal.m_resource->GetItemData(pBatChar->GetEq_weapon());

					if (pItemData)
					{
						if (pItemData->reqSexClass1 == pBatChar->GetSexClass())
						{
							if (QueneData.AttackerLst[i].AtkAction == BATCMD_ATTACK)
							{
								m_SoundDataLst.insert(std::make_pair(CurrTime + RunTime, String(pItemData->eqo1_atk1_sound)));

							}
							else
							{
								m_SoundDataLst.insert(std::make_pair(CurrTime + RunTime, String(pItemData->eqo1_atk2_sound)));
							}
						}
						else
						{
							if (QueneData.AttackerLst[i].AtkAction == BATCMD_ATTACK)
							{
								m_SoundDataLst.insert(std::make_pair(CurrTime + RunTime, String(pItemData->eqo2_atk1_sound)));
							}
							else
							{
								m_SoundDataLst.insert(std::make_pair(CurrTime + RunTime, String(pItemData->eqo2_atk2_sound)));
							}
						}
					}
					else
					{
						if (QueneData.AttackerLst[i].AtkAction == BATCMD_ATTACK)
						m_SoundDataLst.insert(std::make_pair(CurrTime +  RunTime ,
							_T("SoundB0103_NoWeaponAtk.wav")));
						else
							m_SoundDataLst.insert(std::make_pair(CurrTime + RunTime,
							_T("SoundB0102_NoWeaponCritAtk.wav")));
					}
				}
			}
		}
	}

	Int ProtectDelay = 0;

	for (Int k = 0; k < (Int)QueneData.DefenserLst.size(); k++)
	{

		if (QueneData.DefenserLst[k].ProtectIndex >= 0 && QueneData.DefenserLst[k].ProtectIndex < (Int)m_BattlePlayerLst.size())
		{
			//Int JumpProtect = GetPrivateProfileInt(_T("BATTLESETTING"), _T("JUMPPROTECT"), 1, CONFIGURATION_INI_FILE);
			ProtectDelay = RunTime + PlayerSkillUseTime + SkillAtkTime + m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()
				->GetTotalTimeOfGivenAnimation(ANIM_HURT)
				+ HitLst[HitLst.size() - 1];
			PerformJumpProtect(&m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex], QueneData.DefenserLst[k].ProtectIndex, JumpHeight, 
				RunTime / 2, CurrTime, ProtectDelay, false);
		}
		else if (QueneData.DefenserLst[k].petActive)
		{
			//Int JumpProtect = GetPrivateProfileInt(_T("BATTLESETTING"), _T("JUMPPROTECT"), 1, CONFIGURATION_INI_FILE);
			ProtectDelay = RunTime + PlayerSkillUseTime + SkillAtkTime + m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()
				->GetTotalTimeOfGivenAnimation(ANIM_HURT)
				+ HitLst[HitLst.size() - 1];
			PerformJumpProtect(&m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex], QueneData.DefenserLst[k].DefenserIndex, JumpHeight, 
				RunTime / 2, CurrTime , ProtectDelay, true);
		}

		for (Int j = 0; j < (Int)HitLst.size(); j ++)
		{

			BeingAttackData AtkData;

			if (k == QueneData.DefenserLst.size() - 1
				&& j == HitLst.size() -1)
				AtkData.AtkData = FirstAttcker;
			PCSTRINGW FileName = NULL;
			
			if (QueneData.DefenserLst[k].DefReaction == BATCMD_DEFEND_HIT ||
				QueneData.DefenserLst[k].DefReaction == BATCMD_COUNTER_ATTACK_DEF_HIT ||
				QueneData.DefenserLst[k].DefReaction == BATCMD_DEFEND_CRIT_HIT)
			{
				m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()->SetAnimationIndexByIndex(ANIM_DEF, FALSE);
				AtkData.HitAnimationIndex = ANIM_DEF;
				//GetPrivateProfileString(_T("BATTLESETTING"), _T("DEFSPARK"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
				FileName = defSpark;
			}
			else if (QueneData.DefenserLst[k].DefReaction == BATCMD_PROTECT_HIT)
			{
				AtkData.HitAnimationIndex = ANIM_DEF;
				//GetPrivateProfileString(_T("BATTLESETTING"), _T("DEFSPARK"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
				FileName = defSpark;
			}
			else if (QueneData.DefenserLst[k].DefReaction == BATCMD_USED_ITEM)
				AtkData.HitAnimationIndex = -1;
			else if (QueneData.DefenserLst[k].DefReaction == BATCMD_CRIT_HIT)
			{
				AtkData.HitAnimationIndex = ANIM_HURT;
//				wcscpy(FileName, CrtHitEnt);
				FileName = CrtHitEnt;
			}
			else
			{
				AtkData.HitAnimationIndex = ANIM_HURT;
				//GetPrivateProfileString(_T("BATTLESETTING"), _T("SPARK"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
				FileName = Spark;
			}
			AtkData.StartTime = CurrTime;

			AtkData.DefData = QueneData.DefenserLst[k];

			if (j != HitLst.size() -1)
				AtkData.DefData.Back = FALSE;


			if (ProtectDelay > 0 && QueneData.DefenserLst[k].DefReaction != BATCMD_PROTECT_HIT)
			{
					//Int DelatAdjust = GetPrivateProfileInt(_T("BATTLESETTING"), _T("PROTECTDELAY"), -113, CONFIGURATION_INI_FILE);
					AtkData.TimeToHit = RunTime + PlayerSkillUseTime + SkillAtkTime +  HitLst[j];//ProtectDelay + DelatAdjust;
					AtkData.DefData.Attri.HPChange = QueneData.DefenserLst[k].Attri.HPChange;
					if (j == HitLst.size() - 1)
						AtkData.DefData.ShowNum = TRUE;
					else
						AtkData.DefData.ShowNum = FALSE;

					m_EntityBeingAttackLst.insert(std::make_pair(m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar ,AtkData));
					/*if (!QueneData.DefenserLst[k].petActive)
					{
						break;
					}*/
				
			}
			else
				AtkData.TimeToHit = RunTime + PlayerSkillUseTime + SkillAtkTime +  HitLst[j];

			if (j == HitLst.size() - 1)
				AtkData.DefData.Attri.HPChange = QueneData.DefenserLst[k].Attri.HPChange;
			else
				AtkData.DefData.ShowNum = FALSE;

			AtkData.DefData.Move = QueneData.DefenserLst[k].Move && AtkData.DefData.ShowNum;

			if (QueneData.DefenserLst[k].DefReaction != BATCMD_MISS &&
				QueneData.DefenserLst[k].DefReaction != BATCMD_BUFF_EFFECT_HIT)
			{
				EffectData EData;
				EData.effectFileName = FileName;
				
				EData.rotation = m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()->GetAnimationRotation();
				EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView();
				
				EData.attach = FALSE;

				if (AtkData.HitAnimationIndex == ANIM_DEF)
					EData.attach = TRUE;
				m_EffectDataLst.insert(std::make_pair(CurrTime + AtkData.TimeToHit, EData));
				m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));

				if (QueneData.DefenserLst[k].petActive)
				{
					EData.rotation = m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pPet->GetCharMainView()->GetAnimationRotation();
					EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pPet->GetCharMainView();
					//GetPrivateProfileString(_T("BATTLESETTING"), _T("DEFSPARK"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
					EData.effectFileName = defSpark;
					m_EffectDataLst.insert(std::make_pair(CurrTime + AtkData.TimeToHit, EData));
					m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
				}
				if (FirstAttcker.AtkAction == BATCMD_CRIT_ATTACK &&
					QueneData.DefenserLst[k].DefReaction != BATCMD_CRIT_HIT )
				{
					EffectData EData;
					//wcscpy(FileName, CrtHitEnt);
					FileName = CrtHitEnt;
					EData.effectFileName = FileName;
					EData.rotation = m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView()->GetAnimationRotation();
					EData.targetEnt =  m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar->GetCharMainView();
					EData.attach = FALSE;
					m_EffectDataLst.insert(std::make_pair(CurrTime + AtkData.TimeToHit, EData));
					m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));
				}
			}

			m_EntityBeingAttackLst.insert(std::make_pair(m_BattlePlayerLst[QueneData.DefenserLst[k].DefenserIndex].pChar ,AtkData));
		}
	}

	m_NextAttackQueueTime = CurrTime + TotalWaitTime;
	return TRUE;

	//VMProtectEnd; VM_END;
}


VOID CBattleControl::ResetBattlePos(PixelPos LeftUpPos, Int Width, Int Height)
{	
	m_BattleLeftUpPos = LeftUpPos;
	RECT EffectRect;
	EffectRect.left = -Width;
	EffectRect.top = -Height;
	EffectRect.right = Width;
	EffectRect.bottom = Height;

	//m_CountDownTime = CountDownTotalTime;
	//m_CountDownEnt.SetNumber(CountDownTotalTime / 1000);
	//m_StartCountDown = FALSE;
	//m_CountDownEnt.SetVisible(TRUE);
	//if (!gConfigure.uNetwork)
	//	m_CountDownEnt.SetVisible(FALSE);

	SetRect(&m_CurrWorldRect, LeftUpPos.PixelPosX, LeftUpPos.PixelPosY, LeftUpPos.PixelPosX + Width, LeftUpPos.PixelPosY + Height);

	//m_EntityBeingAttackLst.clear();
	//m_AttackQueneData.clear();

	m_BattleParticle->SetRelativeEffectBounding(EffectRect);

	for (int i = 0 ; i < (int)m_BattlePlayerLst.size(); i++)
	{
		/*if (m_BattlePlayerLst[i].pChar->GetBatActorObject() &&
		m_BattlePlayerLst[i].pChar->GetBatActorObject()->GetLeftBattle())
		//||		m_BattlePlayerLst[i].pChar->GetBattleHp() == 0))
		continue;
		*/

		//m_BattlePlayerLst[i].pChar->GetCharMainView()->SetAlphaOffset(0, TRUE);

		if (m_BattlePlayerLst[i].pChar->GetBatActorObject())
		{
			if (m_BattlePlayerLst[i].pChar->GetCharMainView()->GetAnimationIndex() != ANIM_DIE &&
				!m_BattlePlayerLst[i].pChar->GetBatActorObject()->GetLeftBattle())
			{
				m_BattlePlayerLst[i].DeadTime = 0;
				m_BattlePlayerLst[i].pChar->GetCharMainView()->SetEntOffsetX(0);
				m_BattlePlayerLst[i].pChar->GetCharMainView()->SetEntOffsetY(0);
				m_BattlePlayerLst[i].pChar->GetCharMainView()->SetEntOffsetJumpY(0);
				m_BattlePlayerLst[i].pChar->GetCharMainView()->m_animationset.clear();
				m_BattlePlayerLst[i].pChar->GetCharMainView()->UpDate(::timeGetTime());

				//m_BattlePlayerLst[i].Hp = TempHp;
				Int PosX = BaseX;
				Int PosY = BaseY;

				m_BattlePlayerLst[i].pChar->GetCharMainView()->SetCharPos(0,0);

				QGLMath::MoveToBlock(PosX, PosY, NORTHEAST, m_BattlePlayerLst[i].BattlePoxX);
				QGLMath::MoveToBlock(PosX, PosY, SOUTHEAST, ChessboardMaxY - m_BattlePlayerLst[i].BattlePoxY);

				if (m_BattlePlayerLst[i].BattlePoxY > ChessboardMaxY / 2)
					m_BattlePlayerLst[i].pChar->GetCharMainView()->SetAnimationIndexByIndex(
					m_BattlePlayerLst[i].pChar->GetCharMainView()->GetBattleAnimation(), true, SOUTHEAST);			
				else
					m_BattlePlayerLst[i].pChar->GetCharMainView()->SetAnimationIndexByIndex(
					m_BattlePlayerLst[i].pChar->GetCharMainView()->GetBattleAnimation(), true, NORTHWEST);

				m_BattlePlayerLst[i].OrgRotation = m_BattlePlayerLst[i].pChar->GetCharMainView()->GetAnimationRotation();
				m_BattlePlayerLst[i].pChar->GetCharMainView()->SetEntPixelPos(PosX * BlockSizeX + 
					LeftUpPos.PixelPosX,PosY * (BlockSizeY / 2)+ LeftUpPos.PixelPosY);
				m_BattlePlayerLst[i].OrgEntPos = m_BattlePlayerLst[i].pChar->GetCharMainView()->GetEntPixelPos();
				//m_BattlePlayerLst[i].pChar->GetCharMainView()->SetCharPos(PosX ,PosY);

				m_BattlePlayerLst[i].pChar->GetCharMainView()->RandomCurrFrIndex();
			}

			if (m_BattlePlayerLst[i].pChar->GetBatActorObject()->GetState() == CHAR_ST_DEAD  && m_BattlePlayerLst[i].pChar->GetCharMainView()->GetAnimationIndex() != ANIM_DIE)
			{
				m_BattlePlayerLst[i].pChar->GetCharMainView()->SetAnimationIndexByIndex(ANIM_DIE, FALSE);
				m_BattlePlayerLst[i].pChar->GetCharMainView()->ToFrameEnd();
				if (m_BattlePlayerLst[i].pChar->GetBatActorObject() && m_BattlePlayerLst[i].pChar->GetBatActorObject()->GetLeftBattle())
				{
					m_BattlePlayerLst[i].pChar->GetCharMainView()->SetAlphaOffset(255, TRUE);
					m_BattlePlayerLst[i].pChar->GetCharMainView()->SetVisible(FALSE, TRUE);
				}
			}
		}
	}

	PixelPos Center = LeftUpPos;
	Center.PixelPosY += 64;
	Center.PixelPosX += Width/2;
	//Center.PixelPosY += 50;
	m_CountDownEnt.SetEntPixelPos(Center);

}

CCharacterControl * CBattleControl::FindCharByBodyEnt(CEntityBase * pEntBase)
{
	for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].pChar->GetCharMainView() == pEntBase)
		{
			return m_BattlePlayerLst[i].pChar;
		}
	}
	return NULL;
}
VOID CBattleControl::TestJumpAttack(CAnimatedEntity * pTarget)
{
	if (m_AttackQueneData.size() > 0)
		return;

	if (pControlPlayer == NULL)
		return;

	CCharacterControl * pTargetChar = FindCharByBodyEnt(pTarget);

	if (pTargetChar == NULL)
		return;

	BattleEntData * pBattleData = FindBattleData(pTargetChar);
	if (pBattleData == NULL ||
		pBattleData->pChar->GetBattleHp() <= 0)
		return;
	
	BattleEntData * pCtrlBattleEntData = FindBattleData(pControlPlayer);
	AttackQuene QueueData;

	QueueData.Jump = TRUE;
	AttackerData AtkData;
	AtkData.AttackerIndex = pCtrlBattleEntData->Index;
	AtkData.AttackIndex = pCtrlBattleEntData->pChar->GetCharMainView()->GetBattleAttack2Animation();
	AtkData.AttackDes = pTarget->GetEntPixelPos();

	QueueData.AttackerLst.push_back(AtkData);
	
	DefenserData DefData;
	DefData.DefenserIndex = pBattleData->Index;
	DefData.DefReaction = BATCMD_CRIT_HIT;
	DefData.Attri.HPOrg = 99999;
	DefData.Attri.HPChange = gConfigure.sHurtCount;
	QueueData.DefenserLst.push_back(DefData);
	
	m_AttackQueneData.push_back(QueueData);
	for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].pChar == pControlPlayer)
			continue;
		if (m_BattlePlayerLst[i].pChar->GetBattleHp() <= 0)
			continue;
		if (m_BattlePlayerLst[i].pChar->GetCharMainView()->GetAnimationRotation() 
			== pControlPlayer->GetCharMainView()->GetAnimationRotation())
			continue;
		
		AttackQuene QueueData;

		QueueData.Jump = TRUE;

		AttackerData AtkData;
		AtkData.AttackerIndex = i;
		AtkData.AttackIndex = m_BattlePlayerLst[i].pChar->GetCharMainView()->GetBattleAttack2Animation();
		AtkData.AttackDes = pControlPlayer->GetCharMainView()->GetEntPixelPos();

		QueueData.AttackerLst.push_back(AtkData);

		DefenserData DefData;
		DefData.DefenserIndex = pCtrlBattleEntData->Index;
		DefData.DefReaction = BATCMD_CRIT_HIT;
		DefData.Attri.HPOrg = 99999;
		DefData.Attri.HPChange = gConfigure.sHurtCount;
		QueueData.DefenserLst.push_back(DefData);
		
		m_AttackQueneData.push_back(QueueData);
	}
}

VOID CBattleControl::TestAttack(CAnimatedEntity * pTarget)
{
	if (m_AttackQueneData.size() > 0)
		return;

	if (pControlPlayer == NULL)
		return;

	CCharacterControl * pTargetChar = FindCharByBodyEnt(pTarget);

	if (pTargetChar == NULL)
		return;

	BattleEntData * pBattleData = FindBattleData(pTargetChar);
	if (pBattleData == NULL ||
		pBattleData->pChar->GetBattleHp() <= 0)
		return;
	
	BattleEntData * pCtrlBattleEntData = FindBattleData(pControlPlayer);
	AttackQuene QueueData;

	QueueData.Jump = FALSE;
	AttackerData AtkData;
	AtkData.AttackerIndex = pCtrlBattleEntData->Index;
	AtkData.AttackIndex = pCtrlBattleEntData->pChar->GetCharMainView()->GetBattleAttack1Animation();
	AtkData.AttackDes = pTarget->GetEntPixelPos();

	QueueData.AttackerLst.push_back(AtkData);
	
	DefenserData DefData;
	DefData.DefenserIndex = pBattleData->Index;
	DefData.DefReaction = BATCMD_CRIT_HIT;
	DefData.Attri.HPOrg = 999999999;
	DefData.Attri.HPChange = gConfigure.sHurtCount;
	
	QueueData.DefenserLst.push_back(DefData);
	
	m_AttackQueneData.push_back(QueueData);
	for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].pChar == pControlPlayer)
			continue;
		if (m_BattlePlayerLst[i].pChar->GetBattleHp() <= 0)
			continue;
		if (m_BattlePlayerLst[i].pChar->GetCharMainView()->GetAnimationRotation() 
			== pControlPlayer->GetCharMainView()->GetAnimationRotation())
			continue;
		
		AttackQuene QueueData;

		AttackerData AtkData;
		AtkData.AttackerIndex = i;
		AtkData.AttackIndex = m_BattlePlayerLst[i].pChar->GetCharMainView()->GetBattleAttack1Animation();
		AtkData.AttackDes = pControlPlayer->GetCharMainView()->GetEntPixelPos();

		QueueData.AttackerLst.push_back(AtkData);

		DefenserData DefData;
		DefData.DefenserIndex = pCtrlBattleEntData->Index;
		DefData.DefReaction = BATCMD_CRIT_HIT;
		DefData.Attri.HPOrg = 99999;
		DefData.Attri.HPChange = gConfigure.sHurtCount;
		QueueData.DefenserLst.push_back(DefData);
		
		m_AttackQueneData.push_back(QueueData);
	}
}

VOID CBattleControl::SetAnimation(Int Index)
{
	if (QGLApplication::GetInstance().GetMapStateID() != ST_APP_BATTLE)
		return;

	for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].pChar)
			m_BattlePlayerLst[i].pChar->GetCharMainView()->SetAnimationIndexByIndex(Index);
	}
}

VOID CBattleControl::ResetAnimation()
{
	for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].pChar)
			m_BattlePlayerLst[i].pChar->GetCharMainView()->SetAnimationIndexByIndex(
			m_BattlePlayerLst[i].pChar->GetCharMainView()->GetBattleAnimation());
	}
}

BattleEntData * CBattleControl::FindBattleData(CCharacterControl * pChar)
{
	for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].pChar == pChar)
			return &m_BattlePlayerLst[i];
	}
	return NULL;
}

VOID CBattleControl::ProtectSeletedChar(CCharacterControl * pTarget)
{
	VMProtectBegin; VM_START;

	if (pControlPlayer && !pControlPlayer->IsSentCommand() && !pControlPlayer->m_ActionCmdAva[BATCMD_PROTECT])
		return;

	if (pControlPartner && (!pControlPartner->IsSentCommand() && pControlPlayer->IsSentCommand()) && !pControlPartner->m_ActionCmdAva[BATCMD_PROTECT])
		return;

	if (pTarget && pTarget->GetBatActorObject() && pControlPlayer && pControlPlayer->GetBatActorObject() &&
		pTarget->GetBatActorObject()->GetTeam() == pControlPlayer->GetBatActorObject()->GetTeam())
	{
		if (!pControlPlayer->IsSentCommand())
		{
			pControlPlayer->SendBattleCommand(BATCMD_PROTECT, 
				pTarget->GetBatActorObject()->GetIndex(), 0, 0);
		}
		else
		{
			if (pControlPartner && !pControlPartner->IsSentCommand())
			{
				pControlPartner->SendBattleCommand(BATCMD_PROTECT, 
					pTarget->GetBatActorObject()->GetIndex(), 0, 0);

			
			}
		}

	}
	VMProtectEnd; VM_END;
}
VOID CBattleControl::AttackSeletedChar(CCharacterControl * pTarget)
{
	VMProtectBegin; VM_START;

	if (!pControlPlayer)
		return;

	if (!pControlPlayer->IsSentCommand() && !pControlPlayer->m_ActionCmdAva[BATCMD_ATTACK] )
		return;

	if (pControlPartner && (!pControlPartner->IsSentCommand() && pControlPlayer->IsSentCommand()) && !pControlPartner->m_ActionCmdAva[BATCMD_ATTACK] )
		return;

	if (gGlobal.g_bCtrlButtonUp && pTarget->GetBatActorObject()->GetTeam() == pControlPlayer->GetBatActorObject()->GetTeam())
		return;

	if ( (!pControlPlayer->IsSentCommand() && pTarget == pControlPlayer) || (pControlPartner && pControlPlayer->IsSentCommand() && !pControlPartner->IsSentCommand() && pTarget == pControlPartner))
		return;
	

	if (pTarget && pTarget->GetBatActorObject() && pControlPlayer && pControlPlayer->GetBatActorObject() )
	{
		if (!pControlPlayer->IsSentCommand())
		{
			pControlPlayer->SendBattleCommand(BATCMD_ATTACK, 
				pTarget->GetBatActorObject()->GetIndex(), 0, 0);
			
		}
		else
		{
			if (pControlPartner && !pControlPartner->IsSentCommand())
			{
				pControlPartner->SendBattleCommand(BATCMD_ATTACK, 
					pTarget->GetBatActorObject()->GetIndex(), 0, 0);
			}
		}
	}

	VMProtectEnd; VM_END;
}

VOID CBattleControl::RepeatRound()
{
	if (pControlPlayer && !pControlPlayer->IsSentCommand() && !pControlPlayer->m_ActionCmdAva[BATCMD_ATTACK] )
		return;

	if (pControlPartner && (!pControlPartner->IsSentCommand() && pControlPlayer->IsSentCommand()) && !pControlPartner->m_ActionCmdAva[BATCMD_ATTACK] )
		return;

	if (!pControlPlayer->IsSentCommand())
	{
		pControlPlayer->SendBattleCommand(BATCMD_REPEAT, 
			0, 0, 0);
		
	}
	else
	{
		if (pControlPartner && !pControlPartner->IsSentCommand())
		{
			pControlPartner->SendBattleCommand(BATCMD_REPEAT, 
				0, 0, 0);
		}
	}
}

VOID CBattleControl::CatchSelectedChar(CCharacterControl* pTarget)
{
	VMProtectBegin; VM_START;

	if (pControlPlayer && !pControlPlayer->IsSentCommand() && !pControlPlayer->m_ActionCmdAva[BATCMD_CATCH])
		return;

	if (pControlPartner && (!pControlPartner->IsSentCommand() && pControlPlayer->IsSentCommand()) && !pControlPartner->m_ActionCmdAva[BATCMD_CATCH])
		return;
	
	if (pTarget && pTarget->GetBatActorObject() && pControlPlayer && pControlPlayer->GetBatActorObject() &&
		pTarget->GetBatActorObject()->GetTeam() != pControlPlayer->GetBatActorObject()->GetTeam())
	{
		CBatMob* pmob = CastNetObject<CBatMob>(pTarget->GetBatActorObject());

		if (pmob && pmob->GetPartner_type() != CANNOT_BE_PARTNER )
		{
			pControlPlayer->SendBattleCommand(BATCMD_CATCH, 
				pTarget->GetBatActorObject()->GetIndex(), 0, 0);
		}
		else
		{
			if ( (pmob && pmob->GetPartner_type() == CANNOT_BE_PARTNER) || (pmob == NULL) )
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARNTER_CANT_CATCH")), CHANNELTYPE_SYSTEM, String());
			
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePanel", String());
		}
	}

	VMProtectEnd; VM_END;
}

VOID CBattleControl::SetBattleNetObject(CBattle* pGivBattle)
{ 
	pBattle = pGivBattle;
	if (pBattle)
		pBattle->SetBattleView(this);
}

UInt16 CBattleControl::GetCountInLst(const AttackQuene & Quene, BattleEntData * pTarget, BOOL DefTeam)
{
	Int count = 0;
	if (DefTeam)
	{
		for (Int i = 0; i < (Int)Quene.DefenserLst.size(); i++)
		{
			if (m_BattlePlayerLst[Quene.DefenserLst[i].DefenserIndex].pChar == pTarget->pChar)
				count ++;
		}
	}
	else
	{
		for (Int i = 0; i < (Int)Quene.AttackerLst.size(); i++)
		{
			if (m_BattlePlayerLst[Quene.AttackerLst[i].AttackerIndex].pChar == pTarget->pChar)
				count ++;
		}
	}
	return count;
}
VOID CBattleControl::NotifyPlayTurn(LPCPEER pPeer, RPCContext &context, Buffer &buffer, UInt32 totalSize)
{
	//m_BattlePlayerLst[0].pChar->GetCharMainView()->SetVisible(FALSE);
	// 
	counterTime = ::timeGetTime();
	if ( gGlobal.g_CursorState->IsBlocked() )
		gGlobal.g_CursorState->Unblock();

	//if (gGlobal.g_CursorState->GetStateID() != ST_APP_MAP_NORMAL)
	//	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	if (gGlobal.g_pBatScriptControl)
		gGlobal.g_pBatScriptControl->DisappearDialogHandle(); // Close battle dialog by turn
	
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseNewGuide", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseNewArrow", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseBattleItemMenu", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ClosePartnerBattleSkill", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseSkillPanel", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideWaitPanel", String());
	//

	if (m_CurrAutoCount > 0)
	{
		ShowAutoCountBox(m_CurrAutoCount - 1);
	}
	PlayAllMember();


	m_CountDownEnt.SetVisible(FALSE);
	m_CommandAvial = TRUE;

	//Buffer buf = buffer;
	Size size = (Size) totalSize;
	UInt8 index = 0;
	CBatActor *pActor = NULL;
	CBatActor *pTarget = NULL;
	Char srcName[81];
	Char targetName[81];
	UInt8 curStep = 0;
	buffer.ResetCursor();

	AttackQuene QData;
	AttackQuene QData2;
	AttackerData AtkData;
	DefenserData DefData;

	BOOL Break = FALSE;
	CBatActor * pPreActor = NULL;
	while (buffer.GetCursor() < size)
	{
		if (Break)
			break;

		buffer.Read<UInt8>(index);		// get actor index
		if (index >= m_BattlePlayerLst.size())
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("index >= m_BattlePlayerLst.size() index: %d size: %d"), index, m_BattlePlayerLst.size());
			break;
		}
		pActor = m_BattlePlayerLst[index].pChar->GetBatActorObject();
		TRACE_ENSURE(pActor != NULL);
		Int OldState = pActor->GetState();
		pActor->ReadAttr(buffer);	// read attributes for that actors
		pActor->OnUpdate();
		BOOL preFound = FALSE;
		QData2 = QData;
		for (Int i = 0; i < (Int)QData.AttackerLst.size(); i ++)
		{
			
			if (QData.AttackerLst[i].AttackerIndex == pActor->GetIndex() &&
				//!pActor->IsUpdated(CBatActor::reactionMask) &&  
				//!pActor->IsUpdated(CBatActor::actionMask) &&  
				pActor->IsUpdated())
			{
				QData.AttackerLst[i].Attri.OldState = OldState;
				QData.AttackerLst[i].Attri.HPOrg = pActor->GetHP();
				QData.AttackerLst[i].Attri.state = pActor->GetState();
				if (pActor->IsUpdated(CBatActor::HPMask))
				{
					QData.AttackerLst[i].Attri.HPChange += pActor->GetDShowHP();	
				}
				
				if (pActor->IsUpdated(CBatActor::SPMask))
				{
					QData.AttackerLst[i].Attri.SPChange += pActor->GetDShowSP();

				}

				/*if (!QData.AttackerLst[i].OneByOne && 
					QData.AttackerLst[i].Attri.HPChange < 0)
				{
					QData2.AttackerLst[i].Back = FALSE;
					QData2.AttackerLst[i].Attri.HPChange = 0;
					QData2.AttackerLst[i].Attri.SPChange = 0;
					preFound = TRUE;
					QData.AttackerLst[i].OneByOne = TRUE;
					QData.DefenserLst.clear();
				}*/
				
				QData.AttackerLst[i].AtkAction = pActor->GetAction();	
				
				QData.AttackerLst[i].Attri.SPOrg = pActor->GetSP();
				

				if (pActor->IsUpdated(CBatActor::pet_activeMask))
				{
					QData.AttackerLst[i].petActive = pActor->GetPet_active();

				}

				CBatCharacter * pBatChar = CastNetObject<CBatCharacter>(pActor);
				if (pBatChar)
				{
					QData.AttackerLst[i].Attri.DPOrg = pBatChar->GetDP();
					QData.AttackerLst[i].Attri.WcOrg = pBatChar->GetWC();
					QData.AttackerLst[i].Attri.VPOrg = pBatChar->GetVP();
					if (pBatChar->IsUpdated(CBatCharacter::DPMask))
					{
						QData.AttackerLst[i].Attri.DPChange += pBatChar->GetDShowDP();

					}
					if (pBatChar->IsUpdated(CBatCharacter::WCMask))
					{
						QData.AttackerLst[i].Attri.WcChange += pBatChar->GetDShowWC();
					}
					if (pBatChar->IsUpdated(CBatCharacter::VPMask))
					{
						QData.AttackerLst[i].Attri.VPChange += pBatChar->GetDShowVP();
					}
				}
				preFound = TRUE;
			}
		}
		//if (preFound)
		//	continue;
		//{
		//	m_AttackQueneData.push_back(QData2);
		//}
		if (pActor->GetClassID() == BatMob_CLASSID)
			_stprintf(srcName, _T("Mob %d"), index);
		else if (pActor->GetClassID() == BatCharacter_CLASSID)
			_stprintf(srcName, _T("Character %d"), index);
		if (pActor->IsUpdated(CBatActor::targetMask))
		{
			if (pActor->GetTarget() < (UInt8) m_BattlePlayerLst.size())
			{
				pTarget = m_BattlePlayerLst[pActor->GetTarget()].pChar->GetBatActorObject();
				if (pTarget->GetClassID() == BatMob_CLASSID)
					_stprintf(targetName, _T("Mob %d"), pActor->GetTarget());
				else if (pTarget->GetClassID() == BatCharacter_CLASSID)
					_stprintf(targetName, _T("Character %d"), pActor->GetTarget());
			}
			else if (pActor->GetTarget() == BATIDX_ALL)
			{
				_stprintf(targetName, _T("ALL actors"));
			}
			else if (pActor->GetTarget() == BATIDX_TEAM0 || pActor->GetTarget() == BATIDX_TEAM1)
			{
				_stprintf(targetName, _T("ALL in team %d"), pActor->GetTarget() - BATIDX_TEAM0);
			}
			else
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("PlayTurn Error: invalid target %d"), pActor->GetTarget());
				continue;
			}
		}
		//pActor->GetState()
		// play action
		if (pActor->IsUpdated(CBatActor::actionMask))
		{
			if (pActor->GetLinkNext() == BOOLEAN_FALSE)
			{
				curStep++;
			}

			if (!preFound)
			{

				AtkData.Init();
				AtkData.Attri.addBuffList = m_BattlePlayerLst[index].pChar->addBuffList;
				AtkData.Attri.removeBuffList = m_BattlePlayerLst[index].pChar->removeBuffList;
				m_BattlePlayerLst[index].pChar->ClearBuffChange();
				AtkData.AttackerIndex = index;
				AtkData.AttackDes = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetEntPixelPos();
				AtkData.AtkAction = pActor->GetAction();


				AtkData.Attri.OldState = OldState;
				AtkData.Attri.HPOrg = pActor->GetHP();
				AtkData.Attri.SPOrg = pActor->GetSP();
				AtkData.Attri.state = pActor->GetState();
				if (pActor->IsUpdated(CBatActor::HPMask))
				{
					AtkData.Attri.HPChange = pActor->GetDShowHP();	
				}
		

				if (pActor->IsUpdated(CBatActor::SPMask))
				{
					AtkData.Attri.SPChange = pActor->GetDShowSP();

				}

				if (pActor->IsUpdated(CBatActor::pet_activeMask))
				{
					AtkData.petActive = pActor->GetPet_active();

				}

				CBatCharacter * pBatChar = CastNetObject<CBatCharacter>(pActor);
				if (pBatChar)
				{
					AtkData.Attri.DPOrg = pBatChar->GetDP();
					AtkData.Attri.WcOrg = pBatChar->GetWC();
					AtkData.Attri.VPOrg = pBatChar->GetVP();
					if (pBatChar->IsUpdated(CBatCharacter::DPMask))
					{
						AtkData.Attri.DPChange = pBatChar->GetDShowDP();

					}
					if (pBatChar->IsUpdated(CBatCharacter::WCMask))
					{
						AtkData.Attri.WcChange = pBatChar->GetDShowWC();
					}
					if (pBatChar->IsUpdated(CBatCharacter::VPMask))
					{
						AtkData.Attri.VPChange = pBatChar->GetDShowVP();
					}
				}

				if (AtkData.Attri.HPChange < 0 || 
					AtkData.Attri.SPChange < 0)
				{
					AtkData.Back = FALSE;
					AtkData.ShowNum = FALSE;
				}

				if (!preFound)
				{
				switch (pActor->GetAction())
				{


				case BATCMD_IDLE:
					TRACE_INFODTL_2(GLOBAL_LOGGER, _F("%d: %s has no action"), curStep, srcName);
					break;

				case BATCMD_ATTACK:
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("%d: %s attack %s"), curStep, srcName, targetName);
					AtkData.AttackIndex = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetBattleAttack1Animation();
					AtkData.AttackDes = m_BattlePlayerLst[pActor->GetTarget()].OrgEntPos;
					AtkData.AtkAction = pActor->GetAction();
					QData.Jump = FALSE;

					if (AtkData.IsValid())
						QData.AttackerLst.push_back(AtkData);
					break;

				case BATCMD_CRIT_ATTACK:
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("%d: %s critical attack %s"), curStep, srcName, targetName);

					AtkData.AttackIndex = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetBattleAttack2Animation();
					AtkData.AttackDes = m_BattlePlayerLst[pActor->GetTarget()].OrgEntPos;
					QData.Jump = TRUE;
					if (AtkData.IsValid())
						QData.AttackerLst.push_back(AtkData);
					break;

				case BATCMD_ESCAPE:
					{
						if (AtkData.IsValid())
							QData.AttackerLst.push_back(AtkData);

						m_AttackQueneData.push_back(QData);

						if (pControlPartner && m_BattlePlayerLst[index].pChar == pControlPartner)
							pControlPartner = NULL;
						else if (pControlPlayer && m_BattlePlayerLst[index].pChar == pControlPlayer)
						{
							pControlPlayer = NULL;
							gGlobal.g_pPartnerProcess->UpdateSmallIcon(false);
						}
						// stop processing buffer when character and partner escaped(or partner dead)
						if (pControlPlayer == NULL && (pControlPartner == NULL || 
							pControlPartner->GetBatActorObject()->GetState() == CHAR_ST_DEAD))
							Break = TRUE;
						QData.AttackerLst.clear();
					}
					break;
				case BATCMD_ITEM:
				case BATCMD_WINE:
					{

						AtkData.AttackDes = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetEntPixelPos();
						AtkData.AttackIndex = ANIM_USE;
						AtkData.Item = pActor->GetSubType();
						AtkData.AtkAction = pActor->GetAction();
						QData.Jump = FALSE;
						if (AtkData.IsValid())
							QData.AttackerLst.push_back(AtkData);
					}
					break;
				case BATCMD_ESCAPE_FAIL:

					if (AtkData.IsValid())
						QData.AttackerLst.push_back(AtkData);

					m_AttackQueneData.push_back(QData);

					QData.AttackerLst.clear();

					TRACE_INFODTL_2(GLOBAL_LOGGER, _F("%d: %s escaped failed"), curStep, srcName);
					break;
				case BATCMD_SKILL:
				case BATCMD_CRIT_SKILL:
					{
						AtkData.AttackIndex = ANIM_SKILL;
						AtkData.Skill = pActor->GetSubType();
						const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(AtkData.Skill);
						
						if (pSkillData)
						{
							if (pSkillData->animation_Motion == SKILL_ANIM_MOTION_SKILL)
								AtkData.AttackIndex = ANIM_SKILL;
							else if (pSkillData->animation_Motion == SKILL_ANIM_MOTION_ATK1)
								AtkData.AttackIndex = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetBattleAttack1Animation();
							else if (pSkillData->animation_Motion == SKILL_ANIM_MOTION_ATK2)
								AtkData.AttackIndex = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetBattleAttack2Animation();

							if (pSkillData->animation_Type == SKILL_ANIM_TYPE_ORG_POS ||
								pSkillData->animation_Type == SKILL_ANIM_TYPE_ORG_POS_1BY1 )
							{
								AtkData.AttackDes = m_BattlePlayerLst[index].OrgEntPos;
							}
							else if (pSkillData->animation_Type == SKILL_ANIM_TYPE_CENTER_POS ||
								pSkillData->animation_Type == SKILL_ANIM_TYPE_CENTER_POS_1BY1)
							{
								AtkData.AttackDes.PixelPosX = m_CurrWorldRect.left + (m_CurrWorldRect.right - m_CurrWorldRect.left) / 2;
								AtkData.AttackDes.PixelPosY = m_CurrWorldRect.top + (m_CurrWorldRect.bottom - m_CurrWorldRect.top) / 2;
								/*if (pActor->GetPet() > 0)
								{
									AtkData.AttackIndex = ANIM_IDLE1;
								}*/
							}
							else if (pSkillData->animation_Type == SKILL_ANIM_TYPE_TARGET_POS ||
								pSkillData->animation_Type == SKILL_ANIM_TYPE_TARGET_POS_1BY1)
							{
								if(m_BattlePlayerLst[index].pChar->GetCharMainView()->GetAnimationRotation() == NORTHWEST)
								{
									AtkData.AttackDes.PixelPosX = m_BattlePlayerLst[pActor->GetTarget()].OrgEntPos.PixelPosX + BlockSizeX * 2;
									AtkData.AttackDes.PixelPosY = m_BattlePlayerLst[pActor->GetTarget()].OrgEntPos.PixelPosY + BlockSizeY * 2;
								}
								else
								{
									AtkData.AttackDes.PixelPosX = m_BattlePlayerLst[pActor->GetTarget()].OrgEntPos.PixelPosX - BlockSizeX * 2;
									AtkData.AttackDes.PixelPosY = m_BattlePlayerLst[pActor->GetTarget()].OrgEntPos.PixelPosY - BlockSizeY * 2;
								}
							}

							AtkData.Stop = pSkillData->animation_Motion == SKILL_ANIM_MOTION_SKILL;
							AtkData.OneByOne = pSkillData->animation_Type == SKILL_ANIM_TYPE_ORG_POS_1BY1 ||
								pSkillData->animation_Type == SKILL_ANIM_TYPE_CENTER_POS_1BY1 ||
								pSkillData->animation_Type == SKILL_ANIM_TYPE_TARGET_POS_1BY1 ;
							AtkData.animationType = pSkillData->animation_Type;
						}
						if (AtkData.IsValid())
							QData.AttackerLst.push_back(AtkData);
					}
					break;
				case BATCMD_CATCH:

					AtkData.AtkAction = pActor->GetAction();

					if (AtkData.IsValid())
						QData.AttackerLst.push_back(AtkData);

					break;

				}
				}
			}
        }

		// play reaction
		
		if (pActor->IsUpdated(CBatActor::reactionMask))
		{
			switch (pActor->GetReaction())
			{
			case BATCMD_DEFEND_HIT:
			case BATCMD_MISS:
			case BATCMD_NORM_HIT:
			case BATCMD_CRIT_HIT:
			case BATCMD_BREAK_HIT:
			case BATCMD_KNOCKBACK:
			case BATCMD_PROTECT_HIT:
			case BATCMD_SKILL_HIT:
			case BATCMD_USED_ITEM:
			case BATCMD_BUFF_EFFECT_HIT:
			case BATCMD_COUNTER_ATTACK_HIT:
			case BATCMD_COUNTER_ATTACK_DEF_HIT:
			case BATCMD_DEFEND_CRIT_HIT:
				{
					DefData.Init();
					DefData.Attri.addBuffList = m_BattlePlayerLst[index].pChar->addBuffList;
					DefData.Attri.removeBuffList = m_BattlePlayerLst[index].pChar->removeBuffList;
					m_BattlePlayerLst[index].pChar->ClearBuffChange();
					DefData.DefenserIndex = index;
					DefData.DefReaction = (BatCmdEnum)pActor->GetReaction();
					DefData.Attri.OldState = OldState;
					DefData.Attri.HPOrg = pActor->GetHP();
					DefData.Attri.SPOrg = pActor->GetSP();
					DefData.Attri.state = pActor->GetState();
					DefData.die = (pActor->GetState() == CHAR_ST_DEAD);
					if (pActor->IsUpdated(CBatActor::HPMask))
					{
						DefData.Attri.HPChange = pActor->GetDShowHP();	
					}
					
					if (pActor->IsUpdated(CBatActor::SPMask))
					{
						DefData.Attri.SPChange = pActor->GetDShowSP();
						
					}

					if (pActor->IsUpdated(CBatActor::pet_activeMask))
					{
						DefData.petActive = pActor->GetPet_active();
					}

					if (pActor->IsUpdated(CBatActor::hitTogetherMask))
						DefData.hitTogether = pActor->GetHitTogether();					 

					CBatCharacter * pBatChar = CastNetObject<CBatCharacter>(pActor);
					if (pBatChar)
					{
						DefData.Attri.DPOrg = pBatChar->GetDP();
						DefData.Attri.WcOrg = pBatChar->GetWC();
						DefData.Attri.VPOrg = pBatChar->GetVP();
						if (pBatChar->IsUpdated(CBatCharacter::DPMask))
						{
							DefData.Attri.DPChange = pBatChar->GetDShowDP();
							
						}
						if (pBatChar->IsUpdated(CBatCharacter::WCMask))
						{
							DefData.Attri.WcChange = pBatChar->GetDShowWC();
						}
						if (pBatChar->IsUpdated(CBatCharacter::VPMask))
						{
							DefData.Attri.VPChange = pBatChar->GetDShowVP();
						}
					}

					if (pActor->GetReaction() == BATCMD_PROTECT_HIT)
						DefData.ProtectIndex = pActor->GetTarget();
					else
						DefData.ProtectIndex = -1;

					if (AtkData.IsValid() && AtkData.Skill)//pActor->GetReaction() == BATCMD_SKILL_HIT)
					{
						DefData.Skill = AtkData.Skill;
						DefData.Move = FALSE;
					}
					else if (pActor->GetReaction() == BATCMD_BUFF_EFFECT_HIT &&
						QData.AttackerLst.size() > 0)
					{
						DefData.Buff = pActor->GetSubType();
						QData.AttackerLst[0].ShowNum = FALSE;
						DefData.Move = FALSE;
					}
					else
						DefData.Move = TRUE;

					// check whether it is one by one attack



					if (DefData.IsValid())
					{
						//check defenser exist

						if (QData.AttackerLst.size() > 0)
						{
							if (!QData.AttackerLst[0].OneByOne)
							{
								for (Int i = 0; i < (Int)QData.DefenserLst.size(); i++)
								{
									if (QData.DefenserLst[i].DefenserIndex == DefData.DefenserIndex)
									{
										QData.AttackerLst[0].OneByOne = TRUE;
										QData.AttackerLst[0].Back = FALSE;
										if (QData.AttackerLst[0].animationType == SKILL_ANIM_TYPE_TARGET_POS_1BY1)
										{
											if(m_BattlePlayerLst[QData.AttackerLst[0].AttackerIndex].pChar->GetCharMainView()->GetAnimationRotation() == NORTHWEST)
											{
												QData.AttackerLst[0].AttackDes.PixelPosX = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetEntPixelPos().PixelPosX + BlockSizeX * 2;
												QData.AttackerLst[0].AttackDes.PixelPosY = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetEntPixelPos().PixelPosY + BlockSizeY * 2;
											}
											else
											{
												QData.AttackerLst[0].AttackDes.PixelPosX = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetEntPixelPos().PixelPosX - BlockSizeX * 2;
												QData.AttackerLst[0].AttackDes.PixelPosY = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetEntPixelPos().PixelPosY - BlockSizeY * 2;
											}
										}
										QData.Jump = TRUE;

										QData.AttackerLst[0].ShowNum = FALSE;
										if ((pActor->GetReaction() == BATCMD_COUNTER_ATTACK_HIT ||
											pActor->GetReaction() == BATCMD_COUNTER_ATTACK_DEF_HIT)&&
											QData.AttackerLst.size() > 0)
										{
											QData.AttackerLst[0].Back = FALSE;
											QData.AttackerLst[0].ShowNum = FALSE;
										}

										m_AttackQueneData.push_back(QData);
										if (QData.AttackerLst.size() > 0)
										{
										
											QData.AttackerLst[0].Attri.HPChange = 0;
											QData.AttackerLst[0].Attri.SPChange = 0;
										}
										QData.DefenserLst.clear();	


										break;
									}
								}
							}
							//check attacker
							for (UInt i = 0; i < QData.AttackerLst.size(); i++)
							{
								if (QData.AttackerLst[i].AttackerIndex == DefData.DefenserIndex)
								{
									DefData.Attri.HPChange += QData.AttackerLst[i].Attri.HPChange;
									DefData.Attri.SPChange += QData.AttackerLst[i].Attri.SPChange;
									DefData.Attri.DPChange += QData.AttackerLst[i].Attri.DPChange;
									DefData.Attri.WcChange += QData.AttackerLst[i].Attri.WcChange;
									DefData.Attri.VPChange += QData.AttackerLst[i].Attri.VPChange;
									QData.AttackerLst[i].Attri.cleanChange();
								}
							}

						}
						QData.DefenserLst.push_back(DefData);
					}

					if (QData.AttackerLst.size() > 0 && QData.AttackerLst[0].OneByOne && DefData.ProtectIndex == -1 && !DefData.hitTogether)
					{
						//QData.AttackerLst[0].Back = !pActor->GetLinkNext();
						if (QData.AttackerLst[0].animationType == SKILL_ANIM_TYPE_TARGET_POS_1BY1)
						{
							if(m_BattlePlayerLst[QData.AttackerLst[0].AttackerIndex].pChar->GetCharMainView()->GetAnimationRotation() == NORTHWEST)
							{
								QData.AttackerLst[0].AttackDes.PixelPosX = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetEntPixelPos().PixelPosX + BlockSizeX * 2;
								QData.AttackerLst[0].AttackDes.PixelPosY = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetEntPixelPos().PixelPosY + BlockSizeY * 2;
							}
							else
							{
								QData.AttackerLst[0].AttackDes.PixelPosX = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetEntPixelPos().PixelPosX - BlockSizeX * 2;
								QData.AttackerLst[0].AttackDes.PixelPosY = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetEntPixelPos().PixelPosY - BlockSizeY * 2;
							}
						}
						QData.Jump = TRUE;
						if (!pActor->GetLinkNext())
						{
							if (QData.AttackerLst[0].Attri.HPChange >= 0 &&
								QData.AttackerLst[0].Attri.SPChange >= 0)
							{
								QData.AttackerLst[0].ShowNum = TRUE;
								QData.AttackerLst[0].Back = TRUE;
							}
							else
								QData.AttackerLst[0].Back = FALSE;
							QData.AttackerLst[0].OneByOne = FALSE;
						}
						else
						{
							QData.AttackerLst[0].ShowNum = FALSE;
							QData.AttackerLst[0].Back = FALSE;
						}
						if ((pActor->GetReaction() == BATCMD_COUNTER_ATTACK_HIT ||
							pActor->GetReaction() == BATCMD_COUNTER_ATTACK_DEF_HIT)&&
							QData.AttackerLst.size() > 0)
						{
							QData.AttackerLst[0].Back = FALSE;
							QData.AttackerLst[0].ShowNum = FALSE;
						}

						
						{
							m_AttackQueneData.push_back(QData);
							if (QData.AttackerLst.size() > 0)
							{
								QData.AttackerLst[0].Attri.HPChange = 0;
								QData.AttackerLst[0].Attri.SPChange = 0;
							}
							QData.DefenserLst.clear();	

							if (!pActor->GetLinkNext())
							{
								QData.AttackerLst.clear();
								QData.Jump = FALSE;
							}
						}
					}
					else
					{
						if (!pActor->GetLinkNext() && DefData.IsValid())// && AtkData.IsValid())
						{
							if ((pActor->GetReaction() == BATCMD_COUNTER_ATTACK_HIT ||
								pActor->GetReaction() == BATCMD_COUNTER_ATTACK_DEF_HIT)&&
								QData.AttackerLst.size() > 0)
							{
								QData.AttackerLst[0].Back = FALSE;
								QData.AttackerLst[0].ShowNum = FALSE;
							}
							m_AttackQueneData.push_back(QData);
							QData.AttackerLst.clear();
							QData.DefenserLst.clear();
							QData.Jump = FALSE;
						}
					}
					break;
				}
			case BATCMD_CATCHED:
			case BATCMD_CATCHED_FAIL:
				DefData.Init();
				DefData.Attri.addBuffList = m_BattlePlayerLst[index].pChar->addBuffList;
				DefData.Attri.removeBuffList = m_BattlePlayerLst[index].pChar->removeBuffList;
				m_BattlePlayerLst[index].pChar->ClearBuffChange();
				DefData.DefenserIndex = index;
				DefData.DefReaction = (BatCmdEnum)pActor->GetReaction();
							
				
				if (DefData.IsValid())
					QData.DefenserLst.push_back(DefData);

				if (!pActor->GetLinkNext() && DefData.IsValid() && AtkData.IsValid())
				{
					m_AttackQueneData.push_back(QData);
					QData.AttackerLst.clear();
					QData.DefenserLst.clear();
					QData.Jump = FALSE;
				}
				break;
			case BATCMD_INVISIBLE:
			case BATCMD_SHOW_FRIEND:
				{
					DefData.Init();
					DefData.Attri.addBuffList = m_BattlePlayerLst[index].pChar->addBuffList;
					DefData.Attri.removeBuffList = m_BattlePlayerLst[index].pChar->removeBuffList;
					m_BattlePlayerLst[index].pChar->ClearBuffChange();
					DefData.DefenserIndex = index;
					DefData.DefReaction = (BatCmdEnum)pActor->GetReaction();

					if (DefData.IsValid())
						QData.DefenserLst.push_back(DefData);
					if (!pActor->GetLinkNext() && DefData.IsValid())
					{
						m_AttackQueneData.push_back(QData);
						QData.AttackerLst.clear();
						QData.DefenserLst.clear();
						QData.Jump = FALSE;
					}
					/*
					*/
				}
				break;
			
			}

		}
		
		pPreActor = pActor;
	}


	//for testing only
	
	///// do battle bubble handle  //////////////
	{
		if ( m_pBatBubbleCtrl && m_NpcId != 0 )
			m_pBatBubbleCtrl->RunUserBubbleLua();
	}

	if (m_AttackQueneData.size() > 0)
	{
		m_PlayingBattleAnim = TRUE;
	}
	else
	{
		m_PlayingBattleAnim = FALSE;
		if (pBattle)
		{
			pBattle->EndTurn(gGlobal.pPeerProxy, RPCContext());
		}
	}

	
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("AnimTime: %d"), 
		::timeGetTime() - counterTime);

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePartnerPanel", String());
	//reconstruct the list
	ReConstructAttackQueue();
	
}

VOID CBattleControl::ReConstructAttackQueue()
{
	StlDeque<AttackQuene> tempAttackQueneData = m_AttackQueneData;

	StlDeque<AttackQuene>::iterator ItrPre = m_AttackQueneData.end();
	for (StlDeque<AttackQuene>::iterator Itr = m_AttackQueneData.begin(); 
		Itr != m_AttackQueneData.end(); ++Itr)
	{
		if (ItrPre != m_AttackQueneData.end())
		{
			//check attacker index
			for (UInt i = 0; i < ItrPre->AttackerLst.size(); i ++)
			{
				for (UInt j = 0; j < Itr->AttackerLst.size(); j ++)
				{
					if (ItrPre->AttackerLst[i].AttackerIndex == Itr->AttackerLst[j].AttackerIndex)
					{
						ItrPre->AttackerLst[i].Back = false;
					}
				}
			}
		}

		ItrPre = Itr;
	}

}
VOID CBattleControl::NotifyWaitTalk(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const UInt32 &timeLeft)
{

	m_PreUpdateTime = ::timeGetTime();
	m_CountDownTime = CountDownTotalTime;
	m_CountDownEnt.SetNumber(CountDownTotalTime / 1000);
	m_StartCountDown = TRUE;
	m_CountDownEnt.SetVisible(TRUE);

	StopAllMember();
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideWaitPanel", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePartnerPanel", String());
}

VOID CBattleControl::AddAndRemoveBuffEffect()
{
//clear and add buff effect
	for (StlDeque<AttackQuene>::iterator Itr = m_AttackQueneData.begin(); 
		Itr != m_AttackQueneData.end(); ++Itr)
	{
		for (Int index = 0; index < (Int)Itr->AttackerLst.size(); index++)
		{
			if (Itr->AttackerLst[index].AttackerIndex >= (Int)m_BattlePlayerLst.size())
			{
				continue;
			}					
			{
				for (Int i = 0; i < (Int)Itr->AttackerLst[index].Attri.addBuffList.size(); i ++)
				{
					m_BattlePlayerLst[Itr->AttackerLst[index].AttackerIndex].pChar->GetCharMainView()->
						LoadEntityFile(Itr->AttackerLst[index].Attri.addBuffList[i]->Buff_EffectFile, gGlobal.pD3DDevice, EQUIP_OTHERS);

					if (Itr->AttackerLst[index].Attri.addBuffList[i]->Buff_HideEffect ||
						(m_BattlePlayerLst[Itr->AttackerLst[index].AttackerIndex].pChar ==
						pControlPlayer && Itr->AttackerLst[index].Attri.addBuffList[i]->Buff_DetectBuffs.size() > 0))
						ShowHideBuffEffects();

					CallLuaFunc(gGlobal.g_pParentLua, "IsInvisibleBuff", String(), Itr->AttackerLst[index].Attri.addBuffList[i]->id);
					if (gGlobal.g_TempResult > 0)
					{
						if (pControlPlayer == NULL || !pControlPlayer->DetectBuff(Itr->AttackerLst[index].Attri.addBuffList[i]->id))
						m_BattlePlayerLst[Itr->AttackerLst[index].AttackerIndex].pChar->GetCharMainView()->SetAlphaOffset(InvisibleAlphaOffset, TRUE);
					}
				}

				for (Int i = 0; i < (Int)Itr->AttackerLst[index].Attri.removeBuffList.size(); i ++)
				{
					m_BattlePlayerLst[Itr->AttackerLst[index].AttackerIndex].pChar->GetCharMainView()->
						RemovePartByName(Itr->AttackerLst[index].Attri.removeBuffList[i]->Buff_EffectFile);

					if (m_BattlePlayerLst[Itr->AttackerLst[index].AttackerIndex].pChar ==
						pControlPlayer && Itr->AttackerLst[index].Attri.removeBuffList[i]->Buff_DetectBuffs.size() > 0)
						HideBuffEffects();

					CallLuaFunc(gGlobal.g_pParentLua, "IsInvisibleBuff", String(), Itr->AttackerLst[index].Attri.removeBuffList[i]->id);
					if (gGlobal.g_TempResult > 0)
					{
						m_BattlePlayerLst[Itr->AttackerLst[index].AttackerIndex].pChar->GetCharMainView()->SetAlphaOffset(0, TRUE);
					}
				}
			}
				

		}

		for (Int index = 0; index < (Int)Itr->DefenserLst.size(); index++)
		{
			if (Itr->DefenserLst[index].DefenserIndex >= (Int)m_BattlePlayerLst.size())
			{
				continue;
			}

			{
				for (Int i = 0; i < (Int)Itr->DefenserLst[index].Attri.addBuffList.size(); i ++)
				{
					m_BattlePlayerLst[Itr->DefenserLst[index].DefenserIndex].pChar->GetCharMainView()->
						LoadEntityFile(Itr->DefenserLst[index].Attri.addBuffList[i]->Buff_EffectFile, gGlobal.pD3DDevice, EQUIP_OTHERS);
					
					if (Itr->DefenserLst[index].Attri.addBuffList[i]->Buff_HideEffect ||
						(m_BattlePlayerLst[Itr->DefenserLst[index].DefenserIndex].pChar ==
						pControlPlayer && Itr->DefenserLst[index].Attri.addBuffList[i]->Buff_DetectBuffs.size() > 0))
						ShowHideBuffEffects();

					CallLuaFunc(gGlobal.g_pParentLua, "IsInvisibleBuff", String(), Itr->DefenserLst[index].Attri.addBuffList[i]->id);
					if (gGlobal.g_TempResult > 0)
					{
						if (pControlPlayer == NULL || !pControlPlayer->DetectBuff(Itr->DefenserLst[index].Attri.addBuffList[i]->id))
							m_BattlePlayerLst[Itr->DefenserLst[index].DefenserIndex].pChar->GetCharMainView()->SetAlphaOffset(InvisibleAlphaOffset, TRUE);
					}
					
				}

				for (Int i = 0; i < (Int)Itr->DefenserLst[index].Attri.removeBuffList.size(); i ++)
				{
					m_BattlePlayerLst[Itr->DefenserLst[index].DefenserIndex].pChar->GetCharMainView()->
						RemovePartByName(Itr->DefenserLst[index].Attri.removeBuffList[i]->Buff_EffectFile);

					if (m_BattlePlayerLst[Itr->DefenserLst[index].DefenserIndex].pChar ==
						pControlPlayer && Itr->DefenserLst[index].Attri.removeBuffList[i]->Buff_DetectBuffs.size() > 0)
						HideBuffEffects();

					CallLuaFunc(gGlobal.g_pParentLua, "IsInvisibleBuff", String(), Itr->DefenserLst[index].Attri.removeBuffList[i]->id);
					if (gGlobal.g_TempResult > 0)
					{
						m_BattlePlayerLst[Itr->DefenserLst[index].DefenserIndex].pChar->GetCharMainView()->SetAlphaOffset(0, TRUE);
					}
				}

			}
			
		}
	}
}

VOID CBattleControl::NotifyNextTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const UInt32 &timeLeft, const UInt16 &autoCount,
									const Boolean &bInput, const Boolean &bPartnerInput)
{

	m_CommandAvial = TRUE;
	//show turn number?
	m_PreUpdateTime = ::timeGetTime();
	m_CountDownTime = CountDownTotalTime;
	m_CountDownEnt.SetNumber(CountDownTotalTime / 1000);
	m_StartCountDown = TRUE;
	m_CountDownEnt.SetVisible(TRUE);

	if (autoCount > 0 )
	{
		if (pControlPartner && pControlPartner->GetBattleHp() > 0)
			pControlPartner->SetIsSentCommand(true);
		if (pControlPlayer)
			pControlPlayer->SetIsSentCommand(true);
	}
	else
	{
		if (pControlPartner && pControlPartner->GetBattleHp() > 0 )
			pControlPartner->SetIsSentCommand(false);
		if (pControlPlayer)// && pControlPlayer->GetBattleHp() > 0)
			pControlPlayer->SetIsSentCommand(false);
	}

	if (pControlPartner && bPartnerInput)
		pControlPartner->SetIsSentCommand(true);

	if (pControlPlayer && bInput)
		pControlPlayer->SetIsSentCommand(true);

	StopAllMember();
	// 
	if ( gGlobal.g_CursorState->IsBlocked() )
		gGlobal.g_CursorState->Unblock();

	//if (gGlobal.g_CursorState->GetStateID() != ST_APP_MAP_NORMAL)
	//	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	//

	ShowAutoCountBox(autoCount);

	/*if (pControlPlayer && pControlPlayer->GetBatActorObject()) &&
		(pControlPlayer->GetBatActorObject()->GetHP() <= 0 ))
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());

		if (pControlPartner && pControlPartner->GetBatActorObject() && pControlPartner->GetBattleHp() > 0  )
		{
			if (autoCount == 0)
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePartnerPanel", String());
		}
		else
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowWaitPanel", String());
		}
	}
	else
	*/
	{
		if (m_CurrAutoCount == 0 &&
			gGlobal.g_pCharacter->GetAction() != CCharacter::ST_MONITOR)
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePanel", String());
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideWaitPanel", String());
	}

	if (bInput)
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());
		if (bPartnerInput)
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePartnerPanel", String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowWaitPanel", String());
		}
	}

	AddAndRemoveBuffEffect();
	//force to end all animation
	ResetBattlePos(m_BattleLeftUpPos, m_CurrWorldRect.right - m_CurrWorldRect.left, m_CurrWorldRect.bottom - m_CurrWorldRect.top); 
	m_AttackQueneData.clear();
	m_EntityBeingAttackLst.clear();
	m_EffectDataLst.clear();
	m_SoundDataLst.clear();
	

}

VOID CBattleControl::NotifyDrawBattle(LPCPEER pPeer, RPCContext &context)
{
	m_LeaveBattle = TRUE;
}

VOID CBattleControl::NotifyEscapeBattle(LPCPEER pPeer, RPCContext &context)
{
	m_LeaveBattle = TRUE;
	//perform escape animation

}

VOID CBattleControl::NotifyWinBattle(LPCPEER pPeer, RPCContext &context)
{
	m_LeaveBattle = TRUE;
	//show message of getting exp
	FSound* sfx = NULL;
	String Temp;
	if (pBattle && (pBattle->GetBattleType() == PK_BATTLE || pBattle->GetBattleType() == FORCE_PK_BATTLE))
		Temp.Format(_T("%sSoundB0019_battle_win_pvp.wav"), WAV_FILE_PATH);
	else
		Temp.Format(_T("%sSoundB0018_battle_win_pve.wav"), WAV_FILE_PATH);
	sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
	if (sfx)
	{
		sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
		sfx->Play();
	}
}

VOID CBattleControl::NotifyLoseBattle(LPCPEER pPeer, RPCContext &context)
{
	m_LeaveBattle = TRUE;
	gGlobal.g_pMiddleMap->ResetCharDes();
	CCrossMapPathFinder::GetInstance()->DestroyPathData();
	gGlobal.m_u32DestNpcId = 0;
	FSound* sfx = NULL;
	String Temp;
	Temp.Format(_T("%sSoundB0020_battle_fail.wav"), WAV_FILE_PATH);
	sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
	if (sfx)
	{
		if (gGlobal.g_pSystemSetting->IsSoundEffectModeEnable())
			sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
		else
			sfx->SetVolume(0);
		sfx->Play();
	}
	//show message you lose, jump to reborn point
}

VOID CBattleControl::NotifyMonitorEnd(LPCPEER pPeer, RPCContext &context)
{
	//leave battle imm
	m_LeaveBattle = TRUE;
	m_AttackQueneData.clear();
	m_EntityBeingAttackLst.clear();
	m_NextAttackQueueTime = ::timeGetTime() + 1000;
	m_PlayingBattleAnim = FALSE;
	QGLApplication::GetInstance().LeaveBattle();
}

BOOL CBattleControl::IsSendCommandAvial()
{
	return m_CommandAvial && 
		m_AttackQueneData.size() == 0 && 
		m_EntityBeingAttackLst.size() == 0;
}
CCharacterControl * CBattleControl::FindCharCtrlByBatActor(CBatActor * pBatActor)
{
	BattleEntDataList::iterator Itr = m_BattlePlayerLst.begin();
	while (Itr != m_BattlePlayerLst.end())
	{
		if ((*Itr).pChar->GetBatActorObject() == pBatActor)
			return (*Itr).pChar;
		Itr++;
	}
	return NULL;
}
CAnimatedEntity * CBattleControl::FindCharHpBar(CCharacterControl * pAttacker)
{
	BattleEntDataList::iterator Itr = m_BattlePlayerLst.begin();
	while (Itr != m_BattlePlayerLst.end())
	{
		if ((*Itr).pChar == pAttacker)
			return (*Itr).pEntHpBar;
		Itr++;
	}
	return NULL;
}
VOID CBattleControl::PerformRunAttack(CCharacterControl * pAttacker, Int upX, Int upY, Int RunTime,
										UInt32 CurrTime, Int AttackIndex, UInt32 HoldTime, UInt32 StartStopTime, BOOL Back)
{
	
	if (pAttacker == NULL)
		return;

	//ResetPlayerState(pAttacker);
	UInt32 AttackIndexTime = pAttacker->GetCharMainView()->GetTotalTimeOfGivenAnimation(
		AttackIndex,  pAttacker->GetCharMainView()->GetAnimationRotation());
	if (upX != 0 || upY != 0)
	{
		//pAttacker->GetCharMainView()->SetAnimationIndexByIndex(ANIM_RUN, true);
		//pAttacker->GetCharMainView()->SetJumpData(0,upX,upY, RunTime);
		pAttacker->GetCharMainView()->AppendAnimationSet(ANIM_RUN, upX, upY, 0, RunTime, TRUE, ERROR_INDEX,
			CurrTime, 0, FALSE, TRUE, 0,0,0,0,0,0,0);
	}

	Int OrgRot = pAttacker->GetCharMainView()->GetAnimationRotation();
	Int OppRot = pAttacker->GetCharMainView()->GetOppositeRotation();

	pAttacker->GetCharMainView()->AppendAnimationSet(AttackIndex, 0, 0, 0, 0, TRUE, ERROR_INDEX,
	CurrTime + RunTime, 0, FALSE, FALSE, 0,0,0,0,0,0,0);

	if (StartStopTime > 0 && HoldTime > 0)
	{
		if (AttackIndexTime > StartStopTime)
			AttackIndexTime -= StartStopTime;
		else
			AttackIndexTime = 0;


		pAttacker->GetCharMainView()->AppendAnimationSet(ERROR_INDEX, 0, 0, 0, 0, FALSE, ERROR_INDEX,
		CurrTime + RunTime + StartStopTime, 0, TRUE, FALSE, 0,0,0,0,0,0,0);
		
		pAttacker->GetCharMainView()->AppendAnimationSet(ERROR_INDEX, 0, 0, 0, 0, FALSE, ERROR_INDEX,
		CurrTime + RunTime + HoldTime, 0, FALSE, FALSE, 0,0,0,0,0,0,0);
	}

	Int runTimeBack = 0;
	if (Back)
	{
		Int RunSpeed = gConfigure.uRunSpeed;
		Int upOffsetX = -1 * pAttacker->GetCharMainView()->GetEntOffsetX() -1 * upX;
		Int upOffsetY = -1 * pAttacker->GetCharMainView()->GetEntOffsetY() -1 * upY;

		runTimeBack = (Int)((Real32)sqrt(pow((double)upOffsetX, (int)2) + pow((double)upOffsetY, (int)2)) * 1000 / RunSpeed);
		if (upOffsetX != 0 || upOffsetX != 0)
		{

			pAttacker->GetCharMainView()->AppendAnimationSet(ANIM_RUN, upOffsetX, upOffsetY, 0, runTimeBack, TRUE, OppRot,
				CurrTime + runTimeBack + HoldTime + AttackIndexTime, 0, FALSE, TRUE, 0,0,0,0,0,0,0);
		}

		pAttacker->GetCharMainView()->AppendAnimationSet(pAttacker->GetCharMainView()->GetBattleAnimation(), 0, 0, 0, RunTime, TRUE, 
			OrgRot, CurrTime + runTimeBack + HoldTime + AttackIndexTime + RunTime, 0, FALSE, FALSE, 0,0,0,0,0,0,0);
	}
	else
	{
		pAttacker->GetCharMainView()->AppendAnimationSet(pAttacker->GetCharMainView()->GetBattleAnimation(), 0, 0, 0, RunTime, TRUE, 
			OrgRot, CurrTime + RunTime + HoldTime + AttackIndexTime, 0, FALSE, FALSE, 0,0,0,0,0,0,0);
	}

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("AnimTime: RunAttack time %d, runTime %d, attackTime %d"), 
		runTimeBack + HoldTime + AttackIndexTime + RunTime, runTimeBack + RunTime,
		HoldTime + AttackIndexTime);
}

VOID CBattleControl::PerformJumpAttack(CCharacterControl * pAttacker, Int upX, Int upY, Int JumpHeight,
									Int JumpTime, UInt32 CurrTime, Int AttackIndex, UInt32 HoldTime, UInt32 StartStopTime,
									BOOL Back)
{
	
	if (pAttacker == NULL)
		return;

	
	//ResetPlayerState(pAttacker);
	UInt32 AttackIndexTime = pAttacker->GetCharMainView()->GetTotalTimeOfGivenAnimation(
		AttackIndex,  pAttacker->GetCharMainView()->GetAnimationRotation());
	//pAttacker->GetCharMainView()->SetAnimationIndexByIndex(ANIM_JUMP_START, true);
	//pAttacker->GetCharMainView()->SetJumpData(JumpHeight,upX,upY, JumpTime);
	Int OrgRot = pAttacker->GetCharMainView()->GetAnimationRotation();
	Int OppRot = pAttacker->GetCharMainView()->GetOppositeRotation();

	if (upX != 0 || upY != 0)
	{
		//pAttacker->GetCharMainView()->SetAnimationIndexByIndex(ANIM_RUN, true);
		//pAttacker->GetCharMainView()->SetJumpData(0,upX,upY, RunTime);
		pAttacker->GetCharMainView()->AppendAnimationSet(ANIM_JUMP_START, upX, upY, JumpHeight, JumpTime, TRUE, ERROR_INDEX,
			CurrTime, 0, FALSE, TRUE, 0,0,0,0,0,0,0);



		pAttacker->GetCharMainView()->AppendAnimationSet(ANIM_JUMP, 0, 0, 0, 0, TRUE, 
			OrgRot, CurrTime , 0, FALSE, FALSE, 0,0,0,0,0,0,0);

	}
	pAttacker->GetCharMainView()->AppendAnimationSet(AttackIndex, 0, 0, 0, 0, TRUE, 
		OrgRot, CurrTime + JumpTime, 0, FALSE, FALSE, 0,0,0,0,0,0,0);
	

	if (StartStopTime > 0)
	{
		if (AttackIndexTime > StartStopTime)
			AttackIndexTime -= StartStopTime;
		else
			AttackIndexTime = 0;

		pAttacker->GetCharMainView()->AppendAnimationSet(ERROR_INDEX, 0, 0, 0, 0, FALSE, ERROR_INDEX,
		CurrTime + JumpTime + StartStopTime, 0, TRUE, FALSE, 0,0,0,0,0,0,0);
		
		pAttacker->GetCharMainView()->AppendAnimationSet(ERROR_INDEX, 0, 0, 0, 0, FALSE, ERROR_INDEX,
		CurrTime + JumpTime + HoldTime, 0, FALSE, FALSE, 0,0,0,0,0,0,0);
	}

	Int jumpTimeBack = 0;
	if (Back)
	{
		Int JumpSpeed = gConfigure.uJumpSpeed;
		Int upOffsetX = -1 * pAttacker->GetCharMainView()->GetEntOffsetX() -1 * upX;
		Int upOffsetY = -1 * pAttacker->GetCharMainView()->GetEntOffsetY() -1 * upY;

		jumpTimeBack = (Int)((Real32)sqrt(pow((double)upOffsetX, (int)2) + pow((double)upOffsetY, (int)2))  * 1000/ JumpSpeed);

		pAttacker->GetCharMainView()->AppendAnimationSet(ANIM_JUMP, upOffsetX
			, upOffsetY, JumpHeight, jumpTimeBack, FALSE, OppRot,
			CurrTime + JumpTime + HoldTime + AttackIndexTime, 0, FALSE, TRUE, 0,0,0,0,0,0,0);

		//pAttacker->GetCharMainView()->AppendAnimationSet(ANIM_JUMP, 0, 0, 0, 0, FALSE, OppRot,
		//	CurrTime + JumpTime + HoldTime + AttackIndexTime , 0, FALSE, FALSE, 0,0,0,0,0,0,0);

		pAttacker->GetCharMainView()->AppendAnimationSet(pAttacker->GetCharMainView()->GetBattleAnimation(), 0, 0, 0, 0, TRUE, OrgRot,
			CurrTime + JumpTime + HoldTime + AttackIndexTime + jumpTimeBack, 0, FALSE, FALSE, 0,0,0,0,0,0,0);
	}
	else
	{
		pAttacker->GetCharMainView()->AppendAnimationSet(pAttacker->GetCharMainView()->GetBattleAnimation(), 0, 0, 0, 0, TRUE, OrgRot,
			CurrTime + JumpTime + HoldTime + AttackIndexTime, 0, FALSE, FALSE, 0,0,0,0,0,0,0);
	}

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("AnimTime: JumpAttack time %d, jumpTime %d, attackTime %d"), 
		jumpTimeBack + HoldTime + AttackIndexTime + JumpTime, jumpTimeBack + JumpTime,
		HoldTime + AttackIndexTime);
}

BOOL CBattleControl::IsEveryReady()
{
	if (m_AttackQueneData.size() > 0 ||
		m_EntityBeingAttackLst.size() > 0)
		return FALSE;

	for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].pChar->GetCharMainView()->m_animationset.size() > 0 ||
			m_BattlePlayerLst[i].pChar->GetCharMainView()->IsMoving())
			return FALSE;
	}
	return TRUE;
}

VOID CBattleControl::TestCombinedAttack(CAnimatedEntity * pTarget)
{
	if (m_AttackQueneData.size() > 0)
		return;

	if (pControlPlayer == NULL)
		return;

	CCharacterControl * pTargetChar = FindCharByBodyEnt(pTarget);

	if (pTargetChar == NULL)
		return;

	if (pTargetChar == NULL ||
		pTargetChar->GetBattleHp() <= 0)
		return;
	
	AttackQuene QueueData;

	for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].pChar == pControlPlayer)
			continue;
		if (m_BattlePlayerLst[i].pChar->GetBattleHp() <= 0)
			continue;
		if (m_BattlePlayerLst[i].pChar->GetCharMainView()->GetAnimationRotation() 
			!= pControlPlayer->GetCharMainView()->GetAnimationRotation())
			continue;

		AttackerData AtkData;
		AtkData.AttackerIndex = i;
		AtkData.AttackIndex = m_BattlePlayerLst[i].pChar->GetCharMainView()->GetBattleAttack2Animation();
		AtkData.AttackDes = pTargetChar->GetCharMainView()->GetEntPixelPos();

		QueueData.AttackerLst.push_back(AtkData);
		
	}

	DefenserData DefData;
	DefData.DefenserIndex = FindBattleData(pTargetChar)->Index;
	DefData.DefReaction = BATCMD_CRIT_HIT;
	DefData.Attri.HPOrg = 1;
	QueueData.DefenserLst.push_back(DefData);
	m_AttackQueneData.push_back(QueueData);
	
}


VOID CBattleControl::TestSkillAttack(CAnimatedEntity * pTarget, Int SkillIndex, BYTE Pos, BOOL All, Int AnimIndex, BOOL Stop)
{
	if (m_AttackQueneData.size() > 0)
		return;

	if (pControlPlayer == NULL)
		return;

	CCharacterControl * pTargetChar = FindCharByBodyEnt(pTarget);

	if (pTargetChar == NULL)
		return;

	if (pTargetChar == NULL ||
		pTargetChar->GetBattleHp() <= 0)
		return;
	AttackQuene QueueData;

	AttackerData AtkData;
	AtkData.Stop = FALSE;
	AtkData.AttackerIndex = FindBattleData(pControlPlayer)->Index;
	AtkData.Stop = Stop;

	if (AnimIndex == SKILL_ANIM_MOTION_SKILL)
		AtkData.AttackIndex = ANIM_SKILL;
	else if (AnimIndex == SKILL_ANIM_MOTION_ATK1)
		AtkData.AttackIndex = pControlPlayer->GetCharMainView()->GetBattleAttack1Animation();
	else if (AnimIndex == SKILL_ANIM_MOTION_ATK2)
		AtkData.AttackIndex = pControlPlayer->GetCharMainView()->GetBattleAttack2Animation();

	if (Pos == SKILL_ANIM_TYPE_ORG_POS)
	{
		AtkData.AttackDes.PixelPosX = pControlPlayer->GetCharMainView()->GetEntPixelPos().PixelPosX;
		AtkData.AttackDes.PixelPosY = pControlPlayer->GetCharMainView()->GetEntPixelPos().PixelPosY;
	}
	else if (Pos == SKILL_ANIM_TYPE_CENTER_POS)
	{
		AtkData.AttackDes.PixelPosX = m_CurrWorldRect.left + (m_CurrWorldRect.right - m_CurrWorldRect.left) / 2;
		AtkData.AttackDes.PixelPosY = m_CurrWorldRect.top + (m_CurrWorldRect.bottom - m_CurrWorldRect.top) / 2;
	}
	else if (Pos == SKILL_ANIM_TYPE_TARGET_POS)
	{
		if(pControlPlayer->GetCharMainView()->GetAnimationRotation() == NORTHWEST)
		{
			AtkData.AttackDes.PixelPosX = pTargetChar->GetCharMainView()->GetEntPixelPos().PixelPosX + BlockSizeX * 2;
			AtkData.AttackDes.PixelPosY = pTargetChar->GetCharMainView()->GetEntPixelPos().PixelPosY + BlockSizeY * 2;
		}
		else
		{
			AtkData.AttackDes.PixelPosX = pTargetChar->GetCharMainView()->GetEntPixelPos().PixelPosX - BlockSizeX * 2;
			AtkData.AttackDes.PixelPosY = pTargetChar->GetCharMainView()->GetEntPixelPos().PixelPosY - BlockSizeY * 2;
		}
	}

	AtkData.Skill = SkillIndex;
	//AtkData.AreaAttack = TRUE;

	QueueData.AttackerLst.push_back(AtkData);

	if (All)
	{
		for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i++)
		{
			if (m_BattlePlayerLst[i].pChar == pControlPlayer)
				continue;
			if (m_BattlePlayerLst[i].pChar->GetBattleHp() <= 0)
				continue;
			if (m_BattlePlayerLst[i].pChar->GetCharMainView()->GetAnimationRotation() 
				== pControlPlayer->GetCharMainView()->GetAnimationRotation())
				continue;

			DefenserData DefData;
			DefData.Skill = SkillIndex;
			DefData.DefenserIndex = i;
			DefData.DefReaction = BATCMD_CRIT_HIT;
			DefData.Attri.HPOrg = 99999;
			DefData.Attri.HPChange = gConfigure.sHurtCount;
			QueueData.DefenserLst.push_back(DefData);
		}
	}
	else
	{
		DefenserData DefData;
		DefData.Skill = SkillIndex;
		DefData.DefenserIndex = FindBattleData(pTargetChar)->Index;
		DefData.DefReaction = BATCMD_CRIT_HIT;
		DefData.Attri.HPOrg = 99999;
		DefData.Attri.HPChange = gConfigure.sHurtCount;
		QueueData.DefenserLst.push_back(DefData);
	}
	
	m_AttackQueneData.push_back(QueueData);

	//another side
	AttackQuene QueueData2;

	AttackerData AtkData2;
	AtkData2.AttackerIndex = FindBattleData(pTargetChar)->Index;
	//AtkData2.AttackIndex = pTargetChar->GetCharMainView()->GetBattleAttack2Animation();
	AtkData2.Stop = Stop;

	if (AnimIndex == SKILL_ANIM_MOTION_SKILL)
		AtkData2.AttackIndex = ANIM_SKILL;
	else if (AnimIndex == SKILL_ANIM_MOTION_ATK1)
		AtkData2.AttackIndex = pTargetChar->GetCharMainView()->GetBattleAttack1Animation();
	else if (AnimIndex == SKILL_ANIM_MOTION_ATK2)
		AtkData2.AttackIndex = pTargetChar->GetCharMainView()->GetBattleAttack2Animation();

	if (Pos == SKILL_ANIM_TYPE_ORG_POS)
	{
		AtkData2.AttackDes.PixelPosX = pTargetChar->GetCharMainView()->GetEntPixelPos().PixelPosX;
		AtkData2.AttackDes.PixelPosY = pTargetChar->GetCharMainView()->GetEntPixelPos().PixelPosY;
	}
	else if (Pos == SKILL_ANIM_TYPE_CENTER_POS)
	{
		AtkData2.AttackDes.PixelPosX = m_CurrWorldRect.left + (m_CurrWorldRect.right - m_CurrWorldRect.left) / 2;
		AtkData2.AttackDes.PixelPosY = m_CurrWorldRect.top + (m_CurrWorldRect.bottom - m_CurrWorldRect.top) / 2;
	}
	else if (Pos == SKILL_ANIM_TYPE_TARGET_POS)
	{
		if(pTargetChar->GetCharMainView()->GetAnimationRotation() == NORTHWEST)
		{
			AtkData2.AttackDes.PixelPosX = pControlPlayer->GetCharMainView()->GetEntPixelPos().PixelPosX + BlockSizeX * 2;
			AtkData2.AttackDes.PixelPosY = pControlPlayer->GetCharMainView()->GetEntPixelPos().PixelPosY + BlockSizeY * 2;
		}
		else
		{
			AtkData2.AttackDes.PixelPosX = pControlPlayer->GetCharMainView()->GetEntPixelPos().PixelPosX - BlockSizeX * 2;
			AtkData2.AttackDes.PixelPosY = pControlPlayer->GetCharMainView()->GetEntPixelPos().PixelPosY - BlockSizeY * 2;
		}
	}

	AtkData2.Skill = SkillIndex;
	//AtkData2.AreaAttack = TRUE;

	QueueData2.AttackerLst.push_back(AtkData2);
	if (All)
	{
		for (Int i = 0; i < (Int)m_BattlePlayerLst.size(); i++)
		{
			if (m_BattlePlayerLst[i].pChar->GetBattleHp() <= 0)
				continue;
			if (m_BattlePlayerLst[i].pChar->GetCharMainView()->GetAnimationRotation() 
				!= pControlPlayer->GetCharMainView()->GetAnimationRotation())
				continue;

			DefenserData DefData2;
			DefData2.Skill = SkillIndex;
			DefData2.DefenserIndex = i;
			DefData2.DefReaction = BATCMD_CRIT_HIT;
			DefData2.Attri.HPOrg = 999999999;
			DefData2.Attri.HPChange = gConfigure.sHurtCount;
			QueueData2.DefenserLst.push_back(DefData2);
		}
	}
	else
	{
		DefenserData DefData2;
		DefData2.Skill = SkillIndex;
		DefData2.DefenserIndex = FindBattleData(pControlPlayer)->Index;
		DefData2.DefReaction = BATCMD_CRIT_HIT;
		DefData2.Attri.HPOrg = 99999;
		DefData2.Attri.HPChange = gConfigure.sHurtCount;
		QueueData2.DefenserLst.push_back(DefData2);
	}
	
	m_AttackQueneData.push_back(QueueData2);
}

VOID CBattleControl::AddBattleParticle(LPCTSTR filename, Int LiveBase, BOOL Destory, Int NoOfParticle,
									   CEntityBase * pTargetEntity, BOOL SetPartent, Int64 ZValue, Int Rotation, BOOL Free)
{
	ParticleStructSetting ParSetting;
	ParSetting.Destory = Destory;
	ParSetting.LiveTimeBase = LiveBase;
	ParSetting.ExtraZValue = ZValue;
	ParSetting.NoOfParticle = NoOfParticle;
	ParSetting.Rotation = Rotation;
	ParSetting.Loop = FALSE;
	ParSetting.SubOffsetY = 0;
	ParSetting.HaveEffect = FALSE;
	ParSetting.FreeAfterFinish = Free;
	CAnimatedEntity TempEnt;
	TempEnt.LoadEntityFile(filename, gGlobal.pD3DDevice, EQUIP_OTHERS);
	m_BattleParticle->AddAnimatedEntityToSystem(&TempEnt, ParSetting, 
		pTargetEntity, SetPartent);
}

VOID CBattleControl::NotifyControlCharacter(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx)
{
	if (charIdx < m_BattlePlayerLst.size())
	{
		pControlPlayer = m_BattlePlayerLst[charIdx].pChar;

		if (pControlPlayer->GetBatActorObject())
		{
			gGlobal.SetDisplayHP(pControlPlayer->GetBatActorObject()->GetHP(), 
			pControlPlayer->GetBatActorObject()->GetMHP(), pControlPlayer->GetBatActorObject()->GetMaxHP());
			gGlobal.SetDisplaySP(pControlPlayer->GetBatActorObject()->GetSP(), 
				pControlPlayer->GetBatActorObject()->GetMSP(), pControlPlayer->GetBatActorObject()->GetMaxSP());
			gGlobal.SetDisplayDP(pControlPlayer->GetBatActorObject()->GetDP(), 
				pControlPlayer->GetBatActorObject()->GetMaxDP());
		}
		StopAllMember();
	
	}
}

VOID CBattleControl::NotifyControlPartner(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx, const UInt8 &mobIdx)
{
	if (mobIdx < m_BattlePlayerLst.size())
	{
		pControlPartner = m_BattlePlayerLst[mobIdx].pChar;
	
		if (pControlPartner->GetBatActorObject())
		{
			CBatMob * pMob = CastNetObject<CBatMob>(pControlPartner->GetBatActorObject());
			if (pMob)
			{
				CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(pMob->GetPartner_id());
				if (partner)
				{
					partner->SetBattleHP(pMob->GetHP());
					partner->SetBattleMaxHP(pMob->GetMaxHP());
					partner->SetBattleSP(pMob->GetSP());

					gGlobal.g_pPartnerProcess->UpdateSmallHP(partner->GetPartner_id());
					gGlobal.g_pPartnerProcess->UpdateSmallSP(partner->GetPartner_id());
				}		
			}		
		}
	}
	
}

VOID CBattleControl::EndThisTurn()
{
	//reset player animation
	for (size_t i = 0; i < m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].pChar->GetCharMainView()->GetAnimationIndex() == ANIM_RUN)
		{
			m_BattlePlayerLst[i].pChar->GetCharMainView()->SetAnimationIndexByIndex(
			m_BattlePlayerLst[i].pChar->GetCharMainView()->GetBattleAnimation(), TRUE, m_BattlePlayerLst[i].OrgRotation);
			m_BattlePlayerLst[i].pChar->GetCharMainView()->RemovePartByName(escapeEffect);
		}
	}

	if (pBattle)
	{
		pBattle->EndTurn(gGlobal.pPeerProxy, RPCContext());
		if(m_pStaticTextist)
		{
			m_pStaticTextist->SetTextA("");
			m_pStaticTextist->Redraw();
		}
		if (m_pStaticTextsnd)
		{
			m_pStaticTextsnd->SetTextA("");
			m_pStaticTextsnd->Redraw();
		}	
		m_iShowState=0;
		m_S1=m_S2="";
	}
}

VOID CBattleControl::PerformEscape(CCharacterControl * pActorCtrl, BOOL petActive)
{
	

	if (pActorCtrl->GetBatActorObject()->GetState() != CHAR_ST_DEAD)
	{
		ResetPlayerState(pActorCtrl);
		BattleEntData * pBattleData = FindBattleData(pActorCtrl);
		if (pBattleData)
		{
			int upX = 0;
			int upY = 0;

			QGLMath::MoveToBlock(upX, upY, QGLMath::GetOppRotation(pBattleData->OrgRotation) ,  30);

			ResetPlayerState(pActorCtrl);

			if (petActive)
			{
				pActorCtrl->OnUpdatePet(pActorCtrl->GetBatActorObject()->GetPet_uid(), pActorCtrl->GetBatActorObject()->GetPet_id(), 
					pActorCtrl->GetBatActorObject()->GetPetName(), pActorCtrl->GetBatActorObject()->GetPetColorIndex());
			}
			BYTE OrgRot = pActorCtrl->GetCharMainView()->GetAnimationRotation();
			BYTE OppRot = QGLMath::GetOppRotation(pActorCtrl->GetCharMainView()->GetAnimationRotation());
			UInt32 CurrTime = ::timeGetTime();
			UInt32 EscapeTime = gConfigure.uEscapeFailTime;
			BYTE Anim = gConfigure.uEscapeFailAnim;
			UInt32 AnimTime = pActorCtrl->GetCharMainView()->GetTotalTimeOfGivenAnimation(Anim, OppRot);

			pActorCtrl->GetCharMainView()->SetAnimationIndexByIndex(ANIM_RUN, TRUE, OppRot, TRUE);

			//pActorCtrl->GetCharMainView()->AppendAnimationSet(ANIM_RUN, 4, 4, 0, 0, TRUE, OrgRot, ::timegettime(), 0, 0, FALSE, FALSE, 0, 0,0 0,0,0,0,0);
			//pActorCtrl->GetCharMainView()->AppendAnimationSet(ANIM_RUN, -4, -4, 0, 0, TRUE, OppRot, ::timegettime(), 2000, 0, FALSE, FALSE, 0, 0,0 0,0,0,0,0);
			pActorCtrl->GetCharMainView()->AppendAnimationSet(ANIM_RUN, upX, upY, 0, 1000, true, 
				OppRot, CurrTime + EscapeTime, 0, false, false,
				0, 0, 0, 0, 0, 0, 0);
			m_SoundDataLst.insert(std::make_pair(CurrTime + EscapeTime, _T("SoundB0016_escape.wav")));
			//pCharCtrl->GetCharMainView()->SetAnimationIndexByIndex(ANIM_RUN, TRUE, QGLMath::GetOppRotation(pBattleData->OrgRotation));
			//pCharCtrl->GetCharMainView()->SetJumpData(0,upX,upY, 1000);

			
			pActorCtrl->GetCharMainView()->LoadEntityFile(escapeEffect, gGlobal.pD3DDevice, EQUIP_OTHERS);

			m_NextAttackQueueTime += 2000;

			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("AnimTime: Escape time %d, escapeTime %d"), 
				EscapeTime + 1000, EscapeTime);
		}
	}
	else
	{
		UInt32 CurrTime = ::timeGetTime();
		BYTE OppRot = QGLMath::GetOppRotation(pActorCtrl->GetCharMainView()->GetAnimationRotation());
		pActorCtrl->GetCharMainView()->AppendAnimationSet(ANIM_DIE, 50, 50, 0, 0, true, 
				OppRot, CurrTime , 0, false, false,	0, 0, 0, 0, 0, 0, 0);
	}
}

VOID CBattleControl::NotifyMartialResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &money, const UInt8 &point)
{
	if (result == END_WIN)
	{
		String buf;
		buf.Format(gGlobal.GetStringTable()->Get(_T("MSG_PK_MARTIAL_WIN_MONEY")), money);
		gGlobal.PrintMessage(buf, CHANNELTYPE_SYSTEM);
		if (point > 0)
		{
			buf.Format(gGlobal.GetStringTable()->Get(_T("MSG_PK_MARTIAL_WIN_POINT")), point);
			gGlobal.PrintMessage(buf, CHANNELTYPE_SYSTEM);
		}
	}
	else if (END_DRAW)
	{
		String buf;
		buf.Format(gGlobal.GetStringTable()->Get(_T("MSG_PK_MARTIAL_WIN_MONEY")), money);
		gGlobal.PrintMessage(buf, CHANNELTYPE_SYSTEM);
	}
}

VOID CBattleControl::NotifyReward(LPCPEER pPeer, RPCContext &context, const Boolean &levelUp, const UInt32 &exp, 
		const UInt32Vector &dropItems, const UInt32Vector &discardItems, const UInt32 &partnerExp, const Boolean &partnerLvlUp
		, const UInt32 &boundsExp, const UInt32 &boundsPartnerExp)
{
	String sbuf;
	String middleBuf;
	if (exp)
	{
		sbuf.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_WON")), exp);
		sbuf.Format(_T("%s\\r"), sbuf.c_str());
	}
	if (boundsExp)
	{
		String message;
		message.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_BOUNDS_EXP")), boundsExp);
		sbuf.AppendFormat(_T("%s\\r"), message.c_str());
	}
	
	if (levelUp)
	{
		middleBuf.AppendFormat(_T("%s\\r"),gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_ANDLEVELUP")));
	}

	CPartner* partner = gGlobal.g_pPartnerGroup->GetCurrBattlePartner();
	if (partner)
	{
		if (partnerExp)
		{
			String message;
			message.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_PARTNER_EXP")), partnerExp);
			sbuf.AppendFormat(_T("%s\\r"), message.c_str());
		}

		if (boundsPartnerExp)
		{
			String message;
			message.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_BOUNDS_EXP")), boundsPartnerExp);
			sbuf.AppendFormat(_T("%s\\r"), message.c_str());
		}

		if (partner->GetFavor() < 60.0f)
		{
			String message =gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_FAVOUR_LOW"));
			sbuf.AppendFormat(_T("%s\\r"), message.c_str());
			middleBuf.AppendFormat(_T("%s\\r"), message.c_str());
		}
	}

	if (partnerLvlUp)
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayPartnerLevelUpEffect", String());
		middleBuf.AppendFormat(_T("%s\\r"),gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_AND_PARTNER_LEVELUP")));
	}

	if (!dropItems.empty())
	{
		ItemCountMap itemcount;
		ItemCountMap::iterator itr;
		itemcount.clear();
		for (UInt32Vector::const_iterator it = dropItems.begin(); it != dropItems.end(); it++)
		{
			itr = itemcount.find(*it);
			if( itr!=itemcount.end() )
				itr->second += 1;
			else
				itemcount.insert(std::make_pair(*it, 1));
		}
		for( itr = itemcount.begin(); itr!=itemcount.end(); ++itr )
		{
			const ItemData* itemdata = gGlobal.m_resource->GetItemData(itr->first);
			if( itemdata )
			{
				if( itr->second > 1 )
				{
					middleBuf.AppendFormat(_T("%s%sx%i\\r"),gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_ANDITEMGET")), itemdata->name, itr->second );
					sbuf.AppendFormat(_T("%s%sx%i\\r"),gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_ANDITEMGET")), itemdata->name, itr->second );
				}
				else
				{
					middleBuf.AppendFormat(_T("%s%s\\r"),gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_ANDITEMGET")), itemdata->name );
					sbuf.AppendFormat(_T("%s%s\\r"),gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_ANDITEMGET")), itemdata->name );
				}
			}
		}

		FSound* sfx = NULL;
		String Temp;
		Temp.Format(_T("%sSoundE0023_obtain_item_2.wav"), WAV_FILE_PATH);
		sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
		if (sfx)
		{
			sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
			sfx->Play();
		}
	}
	if (discardItems.empty())
	{
		// bag full warning check
		if (gGlobal.m_pBag)
		{
			bool curBagFull = true;
			for (Index i = C_INDEX(0); i < C_INDEX(24); ++i )
			{
				if (!gGlobal.m_pBag->HasItem((UInt8)i))
				{
					curBagFull = false;
					break;
				}
			}
			if( curBagFull )
			{
				middleBuf.AppendFormat(_T("%s\\r"),gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_BAGFULLNOTICE")));
				sbuf.AppendFormat(_T("%s\\r"),gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_BAGFULLNOTICE")));
			}
		}
	}
	else
	{
		ItemCountMap itemcount;
		ItemCountMap::iterator itr;
		itemcount.clear();
		for (UInt32Vector::const_iterator it = discardItems.begin(); it != discardItems.end(); it++)
		{
			itr = itemcount.find(*it);
			if( itr!=itemcount.end() )
				itr->second += 1;
			else
				itemcount.insert(std::make_pair(*it, 1));
		}
		for( itr = itemcount.begin(); itr!=itemcount.end(); ++itr )
		{
			const ItemData* itemdata = gGlobal.m_resource->GetItemData(itr->first);
			String buf;
			if( itemdata )
			{
				if( itr->second > 1 )
				{
					middleBuf.AppendFormat(_T("%s%sx%i\\r"),gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_BAGFULLDROP")), itemdata->name, itr->second );
					sbuf.AppendFormat(_T("%s%sx%i\\r"),gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_BAGFULLDROP")), itemdata->name, itr->second );
				}
				else
				{
					middleBuf.AppendFormat(_T("%s%s\\r"),gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_BAGFULLDROP")), itemdata->name);
					sbuf.AppendFormat(_T("%s%s\\r"),gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_BAGFULLDROP")), itemdata->name);
				}
			}
		}
	}
	if (middleBuf.GetLength())
		gGlobal.PrintPlayerMessage(middleBuf);

	if( sbuf.GetLength() )
	{		
		gGlobal.PrintMessage(sbuf, CHANNELTYPE_NOTICE);
		
	}
	m_LeaveBattle = TRUE;
}
VOID CBattleControl::NotifyPenalty(LPCPEER pPeer, RPCContext &context, const UInt32 &exp, const UInt32 &money, const UInt32 &honor)
{
	//TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Battle Lose: You have dead, and lose got %d exp, %d money, %d honor"), exp, money, honor);

	if (exp > 0 || money > 0 || honor > 0)
	{
		TCHAR buf[256];
		String message;
		_stprintf(buf,gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_LOST")), exp, money, honor);
		//gGlobal.PrintMessage(buf, CHANNELTYPE_SYSTEM);
		message.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_BATTLE_LOST")), exp, money, honor);
		gGlobal.PrintPlayerMessage(message);
	}
	m_LeaveBattle = TRUE;
}

UInt8 CBattleControl::GetControlPlayerTeam()
{
	if (pControlPlayer && pControlPlayer->GetBatActorObject())
		return pControlPlayer->GetBatActorObject()->GetTeam();

	return m_CurrTeam;
}

VOID CBattleControl::PlayAllMember()
{
	for (size_t i = 0; i < m_BattlePlayerLst.size(); i++)
	{
		m_BattlePlayerLst[i].pChar->GetCharMainView()->SetStopAnimation(FALSE);
	}
}
VOID CBattleControl::StopAllMember()
{
	for (size_t i = 0; i < m_BattlePlayerLst.size(); i++)
	{
		if (!m_BattlePlayerLst[i].pChar->GetCharMainView()->IsMoving() &&
			m_BattlePlayerLst[i].pChar->GetBatActorObject() &&
			m_BattlePlayerLst[i].pChar->GetBatActorObject()->GetHP() > 0 &&
			!m_BattlePlayerLst[i].pChar->GetBatActorObject()->GetLeftBattle() && 
			m_BattlePlayerLst[i].pChar->GetCharMainView()->GetAnimationIndex() != ANIM_DIE)
		{
			ResetPlayerState(m_BattlePlayerLst[i].pChar);
			if (m_BattlePlayerLst[i].pChar->GetBatActorObject()->IsPlayer() ||
				m_BattlePlayerLst[i].pChar->GetBatActorObject()->IsParnter())
			m_BattlePlayerLst[i].pChar->GetCharMainView()->SetStopAnimation(TRUE);
		}
	}
}

VOID CBattleControl::ResetPlayerState(CCharacterControl * pActorCtrl)
{
	if (pActorCtrl->GetBatActorObject() &&
		pActorCtrl->GetBatActorObject()->GetLeftBattle())
		return;

	BattleEntData * pBatData = FindBattleData(pActorCtrl);
	if (pBatData == NULL)
		return;

	pActorCtrl->GetCharMainView()->m_animationset.clear();
	pActorCtrl->GetCharMainView()->StopShake();
	pActorCtrl->GetCharMainView()->SetEntOffsetX(0);
	pActorCtrl->GetCharMainView()->SetEntOffsetY(0);
	pActorCtrl->GetCharMainView()->SetEntOffsetJumpY(0);
	pActorCtrl->GetCharMainView()->SetAnimationIndexByIndex(
		pActorCtrl->GetCharMainView()->GetBattleAnimation(), TRUE, pBatData->OrgRotation);
}

VOID CBattleControl::NotifyEndBattle()
{
	// Close battle dialog after battle
	if (gGlobal.g_pBatScriptControl)
		gGlobal.g_pBatScriptControl->DisappearDialogHandle();

	// fix battle end crash
	DeleteNetObject(pBattle);
	pBattle = NULL;
	SetLeaveBattle(TRUE);
	
	if (gGlobal.g_pPartnerGroup)
		gGlobal.g_pPartnerGroup->ResetBattleAllHP();
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(),"onClosePanel",String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(),"onCloseShowButton",String());
	m_pStaticTextist=NULL;
	m_pStaticTextsnd=NULL;
}

VOID CBattleControl::OnCancelUseItemInBattle()
{
	m_BattleItemUseIndex = -1;
	if ( m_CountDownTime > 0 )
	{
		m_CommandAvial = TRUE;
		if (gGlobal.g_pCharacter->GetAction() != CCharacter::ST_MONITOR)
		{
			if (IsBattlePartnerTurn())
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePartnerPanel", String());
			else
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePanel", String());
		}
		
	}
}

VOID CBattleControl::UseWine()
{
	if (pControlPlayer && pControlPlayer->GetBatActorObject() &&
		m_BattleItemUseIndex != -1)
	{
		UInt8 bagindex = m_BattleItemUseIndex;
		//gGlobal.m_pBag->GetItem(bagindex);

		if (pControlPlayer && !pControlPlayer->IsSentCommand() && !pControlPlayer->m_ActionCmdAva[BATCMD_WINE])
			return;

		if (pControlPartner && (!pControlPartner->IsSentCommand() && pControlPlayer->IsSentCommand()) && !pControlPartner->m_ActionCmdAva[BATCMD_WINE])
			return;

		if (IsBattlePartnerTurn())
		{
			if (pControlPartner && !pControlPartner->m_ActionCmdAva[BATCMD_WINE])
				return;
			pControlPartner->SendBattleCommand(BATCMD_WINE, 
				pControlPartner->GetBatActorObject()->GetIndex(), bagindex, 0);
		}
		else
		{
			if (pControlPlayer && !pControlPlayer->m_ActionCmdAva[BATCMD_WINE])
				return;
			pControlPlayer->SendBattleCommand(BATCMD_WINE, 
				pControlPartner->GetBatActorObject()->GetIndex(), bagindex, 0);
		}
	}
}

VOID CBattleControl::UseBattleItemForSelectedChar(CCharacterControl * pTarget)
{
	VMProtectBegin; VM_START;

	if (pTarget && pTarget->GetBatActorObject() && pControlPlayer && pControlPlayer->GetBatActorObject() &&
		m_BattleItemUseIndex != -1)
	{
		UInt8 bagindex = m_BattleItemUseIndex;
		//gGlobal.m_pBag->GetItem(bagindex);

		if (pControlPlayer && !pControlPlayer->IsSentCommand() && !pControlPlayer->m_ActionCmdAva[BATCMD_ITEM])
			return;

		if (pControlPartner && (!pControlPartner->IsSentCommand() && pControlPlayer->IsSentCommand()) && !pControlPartner->m_ActionCmdAva[BATCMD_ITEM])
			return;

		if (IsBattlePartnerTurn())
		{
			if (pControlPartner && !pControlPartner->m_ActionCmdAva[BATCMD_ITEM])
				return;
			pControlPartner->SendBattleCommand(BATCMD_ITEM, 
				pTarget->GetBatActorObject()->GetIndex(), bagindex, 0);
		}
		else
		{
			if (pControlPlayer && !pControlPlayer->m_ActionCmdAva[BATCMD_ITEM])
				return;
			pControlPlayer->SendBattleCommand(BATCMD_ITEM, 
				pTarget->GetBatActorObject()->GetIndex(), bagindex, 0);
		}
	}

	VMProtectEnd; VM_END;
}

VOID CBattleControl::NotifyCommandSuccess(const UInt8 &Index, const UInt8 &cmd)
{
	if (Index >= m_BattlePlayerLst.size())// || m_PlayingBattleAnim)
		return;

	m_BattlePlayerLst[Index].pChar->GetCharMainView()->SetStopAnimation(FALSE);

	if (pControlPlayer == m_BattlePlayerLst[Index].pChar)
	{
		if (pControlPartner && pControlPartner->GetBatActorObject() && pControlPartner->GetBattleHp() > 0)
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());

			//if (cmd != BATCMD_FRIEND)
			if (!pControlPartner->IsSentCommand() && !pControlPartner->AllCmdBlock())
			{
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePartnerPanel", String());
				gGlobal.g_pBattleControl->SetCommandAvial(TRUE);
			}
			else
			{
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowWaitPanel", String());
				gGlobal.g_pBattleControl->SetCommandAvial(TRUE);
			}
		}
		else
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowWaitPanel", String());
		}

		pControlPlayer->SetIsSentCommand(true);
		
	}
	else if (pControlPartner == m_BattlePlayerLst[Index].pChar)
	{
		//if (cmd == BATCMD_ESCAPE)
		//	pControlPartner = NULL;
		//	PerformPreEscape(m_BattlePlayerLst[Index].pChar);
		if (pControlPlayer && pControlPlayer->IsSentCommand())
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePartnerPanel", String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowWaitPanel", String());			
		}
		pControlPartner->SetIsSentCommand(true);
	}
}

VOID CBattleControl::PerformPreEscape(CCharacterControl * pActorCtrl)
{
	ResetPlayerState(pActorCtrl);
	BattleEntData * pBatData = FindBattleData(pActorCtrl);
	if (pBatData == NULL)
		return;

	pActorCtrl->GetCharMainView()->SetAnimationIndexByIndex(ANIM_RUN, TRUE,
		QGLMath::GetOppRotation(pBatData->OrgRotation));
	
	pActorCtrl->GetCharMainView()->LoadEntityFile(escapeEffect, gGlobal.pD3DDevice, EQUIP_OTHERS);
}

VOID CBattleControl::NotifyAddActors(LPCPEER pPeer, RPCContext &context, NetGroup* actors, const UInt8 &Team)
{
	CBatActor* pActor;

	StlVector<CCharacterControl *> NewCharLst;
	for (UInt16 i = 0; i < actors->GetCount(); ++i) {
		pActor = actors->GetChild<CBatActor>(i);
		if (pActor == NULL) continue;

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CB_AddActor: add new actor (%d,%d), index %d"), 
			pActor->GetNetId().srvId, pActor->GetNetId().objId, pActor->GetIndex());

		CCharacterControl * pCharCtrl = NULL;
		CCharacterControl * pPetCtrl = NULL;

		UInt8 index = pActor->GetIndex();
		BOOL New = FALSE;
		if (index < m_objects->GetCount())
		{
			// replace actor that escaped
			CBatActor *pOldActor = m_objects->GetChild<CBatActor>(index);
			m_objects->Replace(index, pActor);
			pCharCtrl = m_BattlePlayerLst[index].pChar;
			if (pOldActor != pActor)
				DeleteNetObject(pOldActor);
		}
		else
		{
			New = TRUE;
			// append actor to actor list
			pCharCtrl = SafeCreateObject(CCharacterControl);
			//m_objects->Add(pActor);
		}

		if (pActor->GetClassID() == BatCharacter_CLASSID)
		{
			CBatCharacter * pBatChar = CastNetObject<CBatCharacter>(pActor);
			if (pBatChar)
			{
				pCharCtrl->LoadDefaultPart(pBatChar->GetSexClass());
				pCharCtrl->InitShadow();
			}
		}
		else
		{
			CBatMob * pBatMob = CastNetObject<CBatMob>(pActor);
			if (pBatMob)
			{
				// find in file for MOB model and color
				MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(pBatMob->GetMob_id());
				if( mobTemplate )
				{
					// load model
					pCharCtrl->LoadDefaultMob(String(mobTemplate->m_basicinfo->model), String(mobTemplate->m_basicinfo->color));
				}
				else
					pCharCtrl->LoadDefaultPart();
				pCharCtrl->InitShadow();
			}
		}
		pCharCtrl->SetBatActor(pActor);

		if (pActor->GetPet_uid() > 0)
		{
			const PetData* pPetData = gGlobal.m_resource->GetPetData(pActor->GetPet_id());
			if (pPetData)
			{
				pPetCtrl = SafeCreateObject(CCharacterControl);
				String mobName = pPetData->pet_model;
				pPetCtrl->GetCharMainView()->SetEntityType(ENTITY_TYPE_MAP_PLAYER);
				pPetCtrl->GetCharMainView()->LoadEntityFile(mobName.c_str(), gGlobal.pD3DDevice);

				String colorFileName = mobName.substr(0, mobName.length() - 4);
				char abc = pPetData->pet_color + 96;
				colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
				pPetCtrl->GetCharMainView()->ChangeColorP(mobName.c_str(), colorFileName.c_str());

				pPetCtrl->GetCharMainView()->SetExtraZBuffer(-1);



				pPetCtrl->GetCharShadowView()->InitEntPicInfo();
				pPetCtrl->GetCharShadowView()->LoadEntityFile(gConfigure.strShadowName.c_str(), gGlobal.pD3DDevice);
				pPetCtrl->GetCharShadowView()->SetExtraZBuffer(-2);
				pPetCtrl->GetCharShadowView()->SetEntParent(pPetCtrl->GetCharMainView());

				StringA strTemp = gConfigure.strFontStyle.c_str();
				pPetCtrl->GetCharNameView()->LoadText(pActor->GetPetName().c_str(), 
					strTemp.c_str(), gConfigure.uNameFontSize, D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
					gConfigure.uNameFontColorG, gConfigure.uNameFontColorB), EDGE_SHADOW);
				pPetCtrl->GetCharNameView()->SetEntParent(pPetCtrl->GetCharMainView());
				pPetCtrl->GetCharNameView()->SetExtraZBuffer(NameExtraZ - 1);

				pPetCtrl->GetCharNameView()->SetFontColor(pCharCtrl->GetFontColor());

				pPetCtrl->GetCharMainView()->SetVisible(false ,true);
			}
		}
		if (New)
		{
			State::ChangeState(pCharCtrl->m_mapState, SafeCreateObject(CBattleState));

			if (pActor->GetTeam() != Team)
				{
					AddCharacterToBattleLst(pCharCtrl, gGlobal.pD3DDevice, 
						BattlePos[pActor->GetPosition() + 20][0], BattlePos[pActor->GetPosition() + 20][1], FALSE, pPetCtrl);

					m_EnemyTeamIndex ++;
				}
				else
				{
					AddCharacterToBattleLst(pCharCtrl, gGlobal.pD3DDevice, 
						BattlePos[pActor->GetPosition()][0], BattlePos[pActor->GetPosition()][1], gGlobal.g_pCharacter->GetAction() != CCharacter::ST_MONITOR, pPetCtrl);
					m_PlayerTeamIndex ++;
				}
		}
		else
		{
			
			CEntityBase * pEntBase = m_BattlePlayerLst[index].pEntHpBar;//DefData.pDefenser->pEntHpBar;
			if (pEntBase)
			{
				Real32 ScaleX = (Real32)pActor->GetHP() / 
					(Real32)pActor->GetMaxHP();
				pEntBase->SetScaleX(ScaleX);
			}

			pEntBase = m_BattlePlayerLst[index].pEntHpDBar;//DefData.pDefenser->pEntHpBar;
			if (pEntBase)
			{
				Real32 ScaleX = (Real32)pActor->GetMHP() / 
					(Real32)pActor->GetMaxHP();
				pEntBase->SetScaleX(ScaleX);
			}
			
			if (pActor->GetTeam() != Team)
			{
				m_BattlePlayerLst[index].BattlePoxX = BattlePos[pActor->GetPosition() + 20][0];
				m_BattlePlayerLst[index].BattlePoxY = BattlePos[pActor->GetPosition() + 20][1];
			}
			else
			{
				m_BattlePlayerLst[index].BattlePoxX = BattlePos[pActor->GetPosition()][0];
				m_BattlePlayerLst[index].BattlePoxY = BattlePos[pActor->GetPosition()][1];
			}
			
		}

		NewCharLst.push_back(pCharCtrl);

		Int PosX = BaseX;
	Int PosY = BaseY;

	m_BattlePlayerLst[i].pChar->GetCharMainView()->SetCharPos(0,0);

	QGLMath::MoveToBlock(PosX, PosY, NORTHEAST, m_BattlePlayerLst[index].BattlePoxX);
	QGLMath::MoveToBlock(PosX, PosY, SOUTHEAST, ChessboardMaxY - m_BattlePlayerLst[index].BattlePoxY);
	if (m_BattlePlayerLst[index].BattlePoxY > ChessboardMaxY / 2)
		m_BattlePlayerLst[index].pChar->GetCharMainView()->SetAnimationIndexByIndex(
		m_BattlePlayerLst[index].pChar->GetCharMainView()->GetBattleAnimation(), true, SOUTHEAST);			
	else
		m_BattlePlayerLst[index].pChar->GetCharMainView()->SetAnimationIndexByIndex(
		m_BattlePlayerLst[index].pChar->GetCharMainView()->GetBattleAnimation(), true, NORTHWEST);

	m_BattlePlayerLst[index].OrgRotation = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetAnimationRotation();
	m_BattlePlayerLst[index].pChar->GetCharMainView()->SetEntPixelPos(PosX * BlockSizeX + 
		m_BattleLeftUpPos.PixelPosX,PosY * (BlockSizeY / 2)+ m_BattleLeftUpPos.PixelPosY);
	m_BattlePlayerLst[index].OrgEntPos = m_BattlePlayerLst[index].pChar->GetCharMainView()->GetEntPixelPos();
	}

	//ResetBattlePos(m_BattleLeftUpPos, m_CurrWorldRect.right - m_CurrWorldRect.left, m_CurrWorldRect.bottom - m_CurrWorldRect.top); 
	
	//jump the character from outside

	for (Int i = 0; i < (Int)NewCharLst.size(); i++)
		PerformJumpEnterBattle(NewCharLst[i]);
}

VOID CBattleControl::PerformJumpEnterBattle(CCharacterControl * pChar)
{
	ResetPlayerState(pChar);
	BYTE OrgRot = pChar->GetCharMainView()->GetAnimationRotation();
	BYTE OppRot = QGLMath::GetOppRotation(OrgRot);
	int upX = 0;//pChar->GetCharMainView()->GetEntPixelPos().PixelPosX;
	int upY = 0;//pChar->GetCharMainView()->GetEntPixelPos().PixelPosY;
	upX /= BlockSizeX;
	upY /= BlockSizeY / 2;

	QGLMath::MoveToBlock(upX, upY, OppRot ,  30);

	pChar->GetCharMainView()->SetJumpData(0,upX,upY, 0);
	UInt32 JumpEnterTime = gConfigure.uJumpEnterTime;
	UInt32 JumpEnterHeight = gConfigure.uJumpEnterHeight;
	//pChar->GetCharMainView()->SetJumpData(100,-upX,- upY, 1000);
	pChar->GetCharMainView()->AppendAnimationSet(ANIM_JUMP_START,-upX, -upY, JumpEnterHeight, JumpEnterTime, FALSE, OrgRot, ::timeGetTime()
		, 0, FALSE, FALSE, 0,0,0,0,0,0,0);

	//pChar->GetCharMainView()->SetJumpData(100, upX, upY,0);

	//pChar->GetCharMainView()->AppendAnimationSet(ANIM_JUMP_ENDT,0, 0, 0, 0, FALSE, OrgRot, ::timeGetTime(), 
	//	1000, 0, TRUE, FALSE, 0,0,0,0,0,0,0);

	pChar->GetCharMainView()->AppendAnimationSet(pChar->GetCharMainView()->GetBattleAnimation(),0, 0, 0, 0, TRUE, OrgRot, ::timeGetTime() + 
		JumpEnterTime, 0, TRUE, FALSE, 0,0,0,0,0,0,0);

	m_NextAttackQueueTime = ::timeGetTime() + 
		JumpEnterTime;

}
VOID CBattleControl::PerformEscapeFail(CCharacterControl * pActorCtrl)
{
	if (pActorCtrl == NULL)
		return;

	ResetPlayerState(pActorCtrl);

	BYTE OrgRot = pActorCtrl->GetCharMainView()->GetAnimationRotation();
	BYTE OppRot = QGLMath::GetOppRotation(pActorCtrl->GetCharMainView()->GetAnimationRotation());
	UInt32 CurrTime = ::timeGetTime();
	UInt32 EscapeTime = gConfigure.uEscapeFailTime;
	BYTE Anim = gConfigure.uEscapeFailAnim;
	UInt32 AnimTime = pActorCtrl->GetCharMainView()->GetTotalTimeOfGivenAnimation(Anim, OppRot);

	pActorCtrl->GetCharMainView()->SetAnimationIndexByIndex(ANIM_RUN, TRUE, OppRot, TRUE);

	pActorCtrl->GetCharMainView()->AppendAnimationSet(Anim, 0, 0, 0, 0, false, 
		OppRot, CurrTime + EscapeTime, 0, false, false,
	0, 0, 0, 0, 0, 0, 0);

	m_SoundDataLst.insert(std::make_pair(CurrTime + EscapeTime, _T("SoundB0017_escape_fail.wav")));
	
	pActorCtrl->GetCharMainView()->AppendAnimationSet(pActorCtrl->GetCharMainView()->GetBattleAnimation(), 0, 0, 0, 0, true, 
		OrgRot, CurrTime + EscapeTime + AnimTime, 0, false, false,
	0, 0, 0, 0, 0, 0, 0);

	m_NextAttackQueueTime += AnimTime + EscapeTime;
	
	pActorCtrl->GetCharMainView()->LoadEntityFile(escapeEffect, gGlobal.pD3DDevice, EQUIP_OTHERS, TRUE, CurrTime + EscapeTime);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("AnimTime: EscapeFail time %d, animTime %d"), 
		EscapeTime + AnimTime, AnimTime);
}

VOID CBattleControl::PerformCatch(CCharacterControl * pCharCtrl, CCharacterControl * pTarget, BOOL canCatch, BOOL petActive)
{
	if (pCharCtrl == NULL || pTarget == NULL)
		return;

	ResetPlayerState(pCharCtrl);
	ResetPlayerState(pTarget);

	CAnimatedEntity * pCatchEnt = pCharCtrl->GetCharMainView();


	if (petActive)
	{
		BattleEntData * pEntData = FindBattleData(pCharCtrl);
		if (pEntData)
		{
			pCatchEnt = pEntData->pPet->GetCharMainView();
			pCatchEnt->SetVisible(true, true);
			pCatchEnt->CopyPos(pCharCtrl->GetCharMainView());
			pCatchEnt->SetRotation(pCharCtrl->GetCharMainView()->GetAnimationRotation());
		}
	}
	
	BYTE charOrgRot = pCharCtrl->GetCharMainView()->GetAnimationRotation();


	BYTE charOppRot = QGLMath::GetOppRotation(pCatchEnt->GetAnimationRotation());

	
	BYTE targetOrgRot = pTarget->GetCharMainView()->GetAnimationRotation();
	BYTE targetOppRot = QGLMath::GetOppRotation(pTarget->GetCharMainView()->GetAnimationRotation());


	UInt32 CurrTime = ::timeGetTime();
	UInt32 moveTime = gConfigure.uMovingTime;
	
	UInt32 animTime = pCatchEnt->GetTotalTimeOfGivenAnimation(ANIM_SKILL, charOrgRot);

	int upX = pTarget->GetCharMainView()->GetEntPixelPos().PixelPosX - pCatchEnt->GetEntPixelPos().PixelPosX + BlockSizeX * 2;
	int upY = pTarget->GetCharMainView()->GetEntPixelPos().PixelPosY - pCatchEnt->GetEntPixelPos().PixelPosY + BlockSizeY * 2;


	//upX /= BlockSizeX;
	
	//upY /= BlockSizeY / 2;

	//QGLMath::MoveToBlock(upX, upY, SOUTHEAST, skipY);

	pCatchEnt->AppendAnimationSet(ANIM_RUN, upX, upY, 0, moveTime, true, charOrgRot, CurrTime, 0, false, true, 
			0, 0, 0, 0, 0, 0, 0);

	pCatchEnt->AppendAnimationSet(ANIM_SKILL, 0, 0, 0, 0, true, charOrgRot, CurrTime + moveTime, 0, false, true, 
			0, 0, 0, 0, 0, 0, 0);

	m_SoundDataLst.insert(std::make_pair(CurrTime +  moveTime, _T("SoundB0080_Catch.wav")));

	pCatchEnt->AppendAnimationSet(ANIM_SKILL, 0, 0, 0, 0, true, charOrgRot, CurrTime + moveTime + animTime / 2, 0, true, true, 
			0, 0, 0, 0, 0, 0, 0);

	EffectData EData;
	EData.effectFileName = _T("effCatch_atk.ent");
	EData.targetEnt = pCatchEnt;
//	EData.pos = EFFECT_POS_TYPE_ATTACH;
	EData.rotation = charOrgRot;
	m_EffectDataLst.insert(std::make_pair(CurrTime + moveTime , EData));
	m_PlayedEffect.insert(std::make_pair(EData.effectFileName, EData));

	CAnimatedEntity pEffectEnt;
	pEffectEnt.InitEntPicInfo();
	pEffectEnt.LoadEntityFile(EData.effectFileName.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);
	UInt32 effectTime = pEffectEnt.GetTotalTimeOfGivenAnimation(pEffectEnt.GetAnimationIndex(), pEffectEnt.GetAnimationRotation());

	pCatchEnt->AppendAnimationSet(ANIM_RUN, -upX, -upY, 0, moveTime, true, charOppRot, CurrTime +  moveTime + effectTime, 0, false, true, 
			0, 0, 0, 0, 0, 0, 0);

	if (petActive)
	{
		pCatchEnt->AppendAnimationSet(pCatchEnt->GetBattleAnimation(), 1000, 1000, 0, 0, true, charOrgRot, CurrTime + 2 * moveTime + effectTime, 0, false, false, 
			0, 0, 0, 0, 0, 0, 0);
	}
	else
	{
		pCatchEnt->AppendAnimationSet(pCatchEnt->GetBattleAnimation(), 0, 0, 0, 0, true, charOrgRot, CurrTime + 2 * moveTime + effectTime, 0, false, false, 
			0, 0, 0, 0, 0, 0, 0);
	}

	EffectData EData2;
	EData2.effectFileName = _T("effCatch_hurt.ent");
	EData2.targetEnt = pTarget->GetCharMainView();
//	EData2.pos = EFFECT_POS_TYPE_ATTACH;
	EData2.rotation = targetOrgRot;
	m_EffectDataLst.insert(std::make_pair(CurrTime + moveTime , EData2));
	m_PlayedEffect.insert(std::make_pair(EData2.effectFileName, EData2));


	if (canCatch)
	{
		pTarget->GetCharMainView()->AppendAnimationSet(ANIM_RUN, -upX, -upY, 0, moveTime, true, targetOrgRot, CurrTime + moveTime + effectTime, 0, false, true, 
			0, 0, 0, 0, 0, 0, 0);

		pTarget->GetCharMainView()->AppendAnimationSet(ANIM_RUN, -upX * 2 , -upY * 2, 0, 0, true, targetOrgRot, CurrTime + 2 * moveTime + effectTime, 0, false, true, 
			0, 0, 0, 0, 0, 0, 0);
		m_SoundDataLst.insert(std::make_pair(CurrTime + 2 * moveTime + effectTime, _T("SoundE0019_join_partner.wav")));
	}
	else
	{
		float randRatio = GetRand(0.5f) + 0.3f;

		UInt32 nowMoveTime = (UInt32)(moveTime * randRatio);
		int nowUpX = (int)(upX * randRatio);
		int nowUpY = (int)(upY * randRatio);


		pTarget->GetCharMainView()->AppendAnimationSet(ANIM_RUN, -nowUpX, -nowUpY, 0, nowMoveTime, true, targetOrgRot, CurrTime + moveTime + effectTime, 0, false, true, 
			0, 0, 0, 0, 0, 0, 0);

		//nowUpX = upX - nowUpX;
		//nowUpY = upY - nowUpY;
		UInt32 nowMoveTime2 = moveTime - nowMoveTime;

		//pTarget->GetCharMainView()->AppendAnimationSet(ANIM_RUN, 0, 0, 0, 0, true, targetOppRot, CurrTime, moveTime + animTime +  effectTime + nowMoveTime , 0, false, false, 
		//	0, 0, 0, 0, 0, 0, 0);
		pTarget->GetCharMainView()->AppendAnimationSet(ANIM_RUN, nowUpX, nowUpY, 0, nowMoveTime2, true, targetOppRot, CurrTime + moveTime +  effectTime + nowMoveTime , 0, false, true, 
			0, 0, 0, 0, 0, 0, 0);
		m_SoundDataLst.insert(std::make_pair(CurrTime + moveTime +  effectTime + nowMoveTime, _T("SoundI0031_catchfail.wav")));

		pTarget->GetCharMainView()->AppendAnimationSet(pTarget->GetCharMainView()->GetBattleAnimation(), 0, 0, 0, 0, true, targetOrgRot, CurrTime + 2 * moveTime + effectTime, 0, false, true, 
			0, 0, 0, 0, 0, 0, 0);
			
	}

	m_NextAttackQueueTime = CurrTime + 2 * moveTime + effectTime;

	if (pCharCtrl->GetMapActorID() == gGlobal.g_pCharacter->GetMapActorID())
		gGlobal.SetDisplaySP(pCharCtrl->GetBatActorObject()->GetSP(), pCharCtrl->GetBatActorObject()->GetMSP(), pCharCtrl->GetBatActorObject()->GetMaxSP());

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("AnimTime: Catch time %d, moveTime %d, animTime %d, effectTime %d"), 
		2 * moveTime + animTime + effectTime, 2 * moveTime, animTime, effectTime);

}

VOID CBattleControl::ShowAutoCountBox(UInt16 count)
{	
	if (m_CurrAutoCount == count)
		return;

	if (count == 0)
	{
		m_AutoCountBox.HideMessageBox();
		
		if (!m_PlayingBattleAnim)
		{
			if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE &&
				gGlobal.g_pCharacter->GetAction() != CCharacter::ST_MONITOR)
			{
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideWaitPanel", String());
				if (pControlPlayer)
				{
					pControlPlayer->SetIsSentCommand(false);
					CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePanel", String());

					if (pControlPartner)
						pControlPartner->SetIsSentCommand(false);
				}
				else if (pControlPartner && pControlPartner->GetBattleHp() > 0)
				{
					pControlPartner->SetIsSentCommand(false);
					CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePartnerPanel", String());
				}
			}
		}
	}
	else
	{
		
		//format the string 
		gGlobal.GetStringTable()->Get(_T("MSE_BATTLE_AUTO_COUNT_MSG"));
		String msg;
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSE_BATTLE_AUTO_COUNT_MSG")), count);
		m_AutoCountBox.ShowMessageBox(2, msg, "AutoCount");

		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePartnerPanel", String());
		//CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowWaitPanel", String());
	}

	m_CurrAutoCount = count;
}

VOID CBattleControl::SkillUseOnSeletedChar(CCharacterControl * pTarget)
{
	VMProtectBegin; VM_START;
	UInt8 cmd = BATCMD_SKILL;
	if (gGlobal.g_pSkillProcess && gGlobal.g_pSkillProcess->m_pTempSkillData 
		&& SKILL_TYPE_SPECIAL == gGlobal.g_pSkillProcess->m_pTempSkillData->type)
	{
		if (pControlPlayer && !pControlPlayer->IsSentCommand())
		{
			if (!pControlPlayer->m_ActionCmdAva[BATCMD_SPECIAL])
				return;
			cmd = BATCMD_SPECIAL;
		}
	}
	else if (pControlPlayer && !pControlPlayer->IsSentCommand() && !pControlPlayer->m_ActionCmdAva[BATCMD_SKILL])
		return;

		if (pControlPartner && (!pControlPartner->IsSentCommand() && pControlPlayer->IsSentCommand()) && !pControlPartner->m_ActionCmdAva[BATCMD_SKILL])
			return;
	if (!gConfigure.uNetwork)
	{
		if (gGlobal.g_pSkillProcess->m_pTempSkillData == NULL)
			return;
		
		TestSkillAttack(pTarget->GetCharMainView(), gGlobal.g_pSkillProcess->m_pTempSkillData->id,
			gGlobal.g_pSkillProcess->m_pTempSkillData->animation_Type, 1,
			gGlobal.g_pSkillProcess->m_pTempSkillData->animation_Motion, 
			gGlobal.g_pSkillProcess->m_pTempSkillData->animation_Motion == SKILL_ANIM_MOTION_SKILL);
		return;
	}
	if (pTarget && pTarget->GetBatActorObject() )
	{
		if (pControlPlayer && pControlPlayer->GetBatActorObject() && !pControlPlayer->IsSentCommand())
		{
				
			if (gGlobal.g_pSkillProcess->m_pTempSkillData == NULL)
				return;

			if (m_BattleItemUseIndex != -1 && gGlobal.g_pSkillProcess->m_pTempSkillData->Skill_ThrowItemType != 255)
				pControlPlayer->SendBattleCommand(cmd, 
				pTarget->GetBatActorObject()->GetIndex(), gGlobal.g_pSkillProcess->m_pTempSkillData->id, m_BattleItemUseIndex);
			else
				pControlPlayer->SendBattleCommand(cmd, 
				pTarget->GetBatActorObject()->GetIndex(), gGlobal.g_pSkillProcess->m_pTempSkillData->id, 0);
				
		}
		else if (pControlPartner && pControlPartner->GetBatActorObject() && pControlPlayer->IsSentCommand() && !pControlPartner->IsSentCommand())
		{
			if (gGlobal.g_pPartnerProcess->GetSelSkillData() == NULL)
				return;

			if (m_BattleItemUseIndex != -1 && gGlobal.g_pPartnerProcess->GetSelSkillData()->Skill_ThrowItemType != 255)
				pControlPartner->SendBattleCommand(cmd, 
				pTarget->GetBatActorObject()->GetIndex(), gGlobal.g_pPartnerProcess->GetSelSkillData()->id, m_BattleItemUseIndex);
			else
				pControlPartner->SendBattleCommand(cmd, 
				pTarget->GetBatActorObject()->GetIndex(), gGlobal.g_pPartnerProcess->GetSelSkillData()->id, 0);
			

		}
	}
	VMProtectEnd; VM_END;
}

CCharacterControl * CBattleControl::FindCharCtrlByChar_id(UInt32 Char_id)
{
	
	for (size_t i = 0; i < m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].pChar->GetMapActorID() == Char_id)
			return m_BattlePlayerLst[i].pChar;
	}
	return NULL;
}

VOID CBattleControl::NotifyNeedHelp()
{
	UIButton * pBu = gGlobal.pUIManager->FindUIButton("BattleJoinB");
	if (pBu)
	{
		pBu->SetNormalState();
		gGlobal.pUIManager->AddDirtyUIItem(pBu);
	}
}

VOID CBattleControl::UpdateEffectDataLst(UInt32 CurrTime)
{
	
	UInt PlayTime = 0;
	StlMultiMap<UInt32, EffectData>::iterator it;

	for (it = m_EffectDataLst.begin(); it != m_EffectDataLst.end();
		++ it)
	{
		if (CurrTime > it->first)
		{
			//String FileName = TestEffectAtkSuffix[EntityBeingAttackItr->second.AtkSkillIndex].c_str();
			//FileName.Format(_T("effAtk%s.ent"), TestEffectSuffix[EntityBeingAttackItr->second.AtkSkillIndex].c_str());
			CAnimatedEntity Effect;
			Effect.LoadEntityFile(it->second.effectFileName.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);
			Effect.SetRotation(it->second.rotation);
			if (Effect.GetOffsetType() == ENTITYOFFSETTYPE_FULLSCREEN ||
				!it->second.attach)
			{
				AddBattleParticle(it->second.effectFileName.c_str(), 0, TRUE, 1, it->second.targetEnt, TRUE, ParticleExtraZ, Effect.GetAnimationRotation(), TRUE);
			}
			else
			{
				if (it->second.DeleteTime > 0)
					it->second.targetEnt->LoadEntityFile(it->second.effectFileName.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS, TRUE, CurrTime + it->second.DeleteTime, TRUE);
				else
					it->second.targetEnt->LoadEntityFile(it->second.effectFileName.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS, TRUE, CurrTime + Effect.GetTotalTimeOfGivenAnimation(), TRUE);
			}
			//EntityBeingAttackItr->second.AtkTimeToHitSkill = 0;
			if (Effect.GetTotalTimeOfGivenAnimation() > PlayTime)
				PlayTime = Effect.GetTotalTimeOfGivenAnimation();
		}
		else
			break;
	}
	if (PlayTime > 0)
	{
		//m_NextAttackQueueTime = ::timeGetTime() + PlayTime;
		m_EffectDataLst.erase(m_EffectDataLst.begin(), it);
	}
	
}

//before exchange the partner, the new partner will store in temp
VOID CBattleControl::StoreWaitingPartner(NetGroup* grp)
{
	//if (m_tempPartnerObject)
	//	DeleteNetGroup(m_tempPartnerObject);

	m_tempPartnerObject = grp;

	for (UInt i = 0; i < m_tempPartnerObject->GetCount(); ++i)
	{
		CBatMob* partner = m_tempPartnerObject->GetChild<CBatMob>(i);
		if (partner)
		{
			
			{
				NetGroup temp;
				temp.Add(m_tempPartnerObject->GetChild(i));							
				NotifyAddActors(gGlobal.pPeerProxy, gGlobal.g_context, &temp, m_CurrTeam);

				m_BattlePlayerLst[partner->GetIndex()].pChar->GetCharMainView()->m_animationset.clear();
			}
		}
	}

}

VOID CBattleControl::UpdateSoundDataLst(UInt32 CurrTime)
{
	UInt PlayTime = 0;
	StlMultiMap<UInt32, String>::iterator it;

	for (it = m_SoundDataLst.begin(); it != m_SoundDataLst.end();
		++ it)
	{
		if (CurrTime > it->first)
		{
			if (!it->second.IsEmpty())
			{
				FSound* sfx = NULL;
				String Temp;
				Temp.Format(_T("%s%s"), WAV_FILE_PATH, it->second.c_str());
				sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
				if (sfx)
				{
					sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
					sfx->Play();
				}
				
			}
			PlayTime = 1;
		}
		else
			break;
	}
	if (PlayTime > 0)
	{
		//m_NextAttackQueueTime = ::timeGetTime() + PlayTime;
		m_SoundDataLst.erase(m_SoundDataLst.begin(), it);
	}
}

VOID CBattleControl::Init()
{
	m_CountDownTime = CountDownTotalTime;
	m_CountDownEnt.SetNumber(CountDownTotalTime / 1000);

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(),"OpenShowButton",String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(),"OpenShowPanel",String());
	m_pStaticTextist = gGlobal.pUIManager->FindUIStaticText("ShowActorNameist");
	m_pStaticTextsnd = gGlobal.pUIManager->FindUIStaticText("ShowActorNamesnd");
	m_EntityBeingAttackLst.clear();
	m_AttackQueneData.clear();
	m_EffectDataLst.clear();
	m_SoundDataLst.clear();
}

Int CBattleControl::FindBattleEntByPosition(UInt16 BkX, UInt16 BkY)
{
	for (UInt16 i = 0; i < m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].BattlePoxX == BkX &&
			m_BattlePlayerLst[i].BattlePoxY == BkY)
		{
			return i;
		}
	}

	return -1;
}

void CBattleControl::ShowBattlePanel()
{

}

VOID CBattleControl::ClearBatBubbleCtrlData()
{
	if ( m_pBatBubbleCtrl )
		m_pBatBubbleCtrl->ResetParam();
}

UInt32 CBattleControl::GetBatMobIdByIndex(Int Index)
{
	if ( !pControlPlayer || !(m_BattlePlayerLst[Index].pChar) ) return 0;
	if ( m_BattlePlayerLst[Index].pChar->GetBatTeam() != pControlPlayer->GetBatTeam() )
	{
		CBatActor* pBatActor = m_BattlePlayerLst[Index].pChar->GetBatActorObject();
		if ( pBatActor )
		{
			CBatMob * pBatMob = CastNetObject<CBatMob>(pBatActor);
			if ( pBatMob )
				return pBatMob->GetMob_id();
		}
	}
	return 0;
}

VOID CBattleControl::AddBatBubbleMobId(UInt32 mobId)
{
	if ( m_pBatBubbleCtrl )
	{
		m_pBatBubbleCtrl->AddMobId(mobId);
	}
}

VOID CBattleControl::LoadNpcDialogueLua(UInt32 npcId)
{
	if ( m_pBatBubbleCtrl )
		m_pBatBubbleCtrl->StartUp(npcId);
}


VOID CBattleControl::ShowBattleBubbleMsg(Int posIndex, Int Type)
{
	if ( m_pBatBubbleCtrl )
	{
		m_pBatBubbleCtrl->ShowEnemyBubble(posIndex, Type);
		m_pBatBubbleCtrl->ShowUserBubble(posIndex, Type);
	}
}

Int CBattleControl::CalcFinalBatPosition(CCharacterControl * pCharCtrl)
{
	if ( !pCharCtrl ) return -1;

	Int batPos = pCharCtrl->GetBatPosition();
	UInt8 playerteam = pControlPlayer->GetBatTeam();
	UInt8 selfteam = pCharCtrl->GetBatTeam();
	if ( selfteam != playerteam )
	{// enemy
		batPos += 20;
	}

	return batPos;
}

CCharacterControl * CBattleControl::FindCharCtrlByBatPos(Int Pos, Boolean bEnemy)
{
	if (pControlPlayer == NULL)
		return NULL;
	UInt8 playerteamid = pControlPlayer->GetBatTeam();
	UInt8 tempid = 0;
	for (size_t i = 0; i < m_BattlePlayerLst.size(); i++)
	{
		tempid = m_BattlePlayerLst[i].pChar->GetBatTeam();
		if ( ((tempid == playerteamid) && !bEnemy) || ((tempid != playerteamid) && bEnemy) )
		{
			return m_BattlePlayerLst[i].pChar;
		}
	}
	return NULL;				
}

VOID CBattleControl::BatSayHande(UInt32 Npcid, const String & SayMsg, Int ImageType, Boolean bLeft, Boolean finalSay)
{
	if ( gGlobal.g_pBatScriptControl )
		gGlobal.g_pBatScriptControl->OnBatSayHandle(Npcid, SayMsg, ImageType, bLeft, finalSay);
}

VOID CBattleControl::BatChooseHandle(UInt8 askerType, UInt32 askerId, Int Opt, Boolean bLeft, const StringVector &OptMsg)
{
	if ( gGlobal.g_pBatScriptControl )
		gGlobal.g_pBatScriptControl->OnBatChooseHandle(askerType, askerId, Opt, bLeft, OptMsg);
}

VOID CBattleControl::BatPushEndCmdMsg()
{
	if ( gGlobal.g_pBatScriptControl ) {
		gGlobal.g_pBatScriptControl->OnBatPushEndCmdMsgHandle();
		gGlobal.g_pBatScriptControl->CloseBatDialog();
	}
}

VOID CBattleControl::BatSetDisEnableCmd(UInt32 disEnableCmd, Boolean bUpdate)
{
	if ( gGlobal.g_pBatScriptControl )
	{
		gGlobal.g_pBatScriptControl->OnBatSetDisEnableCmd(disEnableCmd);
		if ( bUpdate && pControlPlayer)
		{
			pControlPlayer->UpdateBlockCmd();
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePanel", String());
		}
	}
}

VOID CBattleControl::BatHideBatCmdMenu(UInt8 index, Boolean bHide, Boolean bUpdateBlock)
{
	if ( bUpdateBlock )
	{
		if ( pControlPlayer )
		{
			pControlPlayer->UpdateBlockCmd();
			
		}
		return;
	}

	if ( bHide ) 
	{
		if ( pControlPlayer )
			pControlPlayer->BlockAllCmd();

		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePartnerPanel", String());
	}
	else
	{
		if ( pControlPlayer )
			pControlPlayer->UpdateBlockCmd();
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePanel", String());

		/*
		if (pControlPartner && pControlPartner->GetBatActorObject() && pControlPartner->GetBattleHp() > 0)
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePartnerPanel", String());
		}
		else if (pControlPartner == m_BattlePlayerLst[index].pChar)
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePanel", String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePartnerPanel", String());
		}
		*/
	}
	/*
		if (pControlPlayer == m_BattlePlayerLst[Index].pChar)
	{
		if (pControlPartner && pControlPartner->GetBatActorObject() && pControlPartner->GetBattleHp() > 0)
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());

			//if (cmd != BATCMD_FRIEND)
			//{
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePartnerPanel", String());
				gGlobal.g_pBattleControl->SetCommandAvial(TRUE);
			//}
		}
		else
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowWaitPanel", String());
		}

		pControlPlayer->SetIsSentCommand(true);
		
	}
	else if (pControlPartner == m_BattlePlayerLst[Index].pChar)
	{
		//if (cmd == BATCMD_ESCAPE)
		//	pControlPartner = NULL;
		//	PerformPreEscape(m_BattlePlayerLst[Index].pChar);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePartnerPanel", String());
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowWaitPanel", String());
		pControlPartner->SetIsSentCommand(true);
	}
	*/
}

VOID CBattleControl::BatNextTalk()
{
	if (pBattle && (gGlobal.g_pParty == NULL || gGlobal.g_pParty->IsLeader()))
		pBattle->NextTalk(gGlobal.pPeerProxy, RPCContext());
}

VOID CBattleControl::BatChooseFinish(Int Opt)
{
	if (pBattle && (gGlobal.g_pParty == NULL || gGlobal.g_pParty->IsLeader()))
		pBattle->ChooseResult(gGlobal.pPeerProxy, gGlobal.g_context, (UInt8) Opt);
}

VOID CBattleControl::RandAttack()
{
	if (pControlPlayer)
	{
		UInt8 count = (UInt8)m_BattlePlayerLst.size();
		UInt8 rand = GetRand(count);
		for (int i = rand; i < rand + count; i++)
		{
			CBatActor* actor = m_BattlePlayerLst[i % count].pChar->GetBatActorObject();
			if (actor && actor->GetTeam() != pControlPlayer->GetBatActorObject()->GetTeam() )
			{
				AttackSeletedChar(m_BattlePlayerLst[i % count].pChar);
				break;
			}
		}
	}
}

VOID CBattleControl::ShowHideBuffEffects()
{
	if (pControlPlayer)
	{
		for (UInt i = 0; i < m_BattlePlayerLst.size(); i ++)
		{
			if (m_BattlePlayerLst[i].pChar->GetTeam() != pControlPlayer->GetTeam())
			{
				for (BuffMap::iterator Itr = m_BattlePlayerLst[i].pChar->m_BuffMap.begin(); Itr != 
					m_BattlePlayerLst[i].pChar->m_BuffMap.end(); Itr ++)
				{
					if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_HideEffect)
					{
						if (pControlPlayer->DetectBuff(Itr->second.pBuffData->id) &&
							!m_BattlePlayerLst[i].pChar->GetCharMainView()->IsPartExist(Itr->second.pBuffData->Buff_EffectFile))
							m_BattlePlayerLst[i].pChar->GetCharMainView()->LoadEntityFile(Itr->second.pBuffData->Buff_EffectFile,
							gGlobal.pD3DDevice, EQUIP_OTHERS);
					}
				}

				for (BuffMap::iterator Itr = m_BattlePlayerLst[i].pChar->m_BuffCacheMap.begin(); 
					Itr != m_BattlePlayerLst[i].pChar->m_BuffCacheMap.end(); Itr ++)
				{
					
					const BuffData * pBuffData = Itr->second.pBuffData;
						if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_HideEffect)
						{
							if (pControlPlayer->DetectBuff(Itr->second.pBuffData->id) &&
								!m_BattlePlayerLst[i].pChar->GetCharMainView()->IsPartExist(Itr->second.pBuffData->Buff_EffectFile))
								m_BattlePlayerLst[i].pChar->GetCharMainView()->LoadEntityFile(Itr->second.pBuffData->Buff_EffectFile,
								gGlobal.pD3DDevice, EQUIP_OTHERS);
						}
					
				}
				
			}
		}
	}
}
VOID CBattleControl::HideBuffEffects()
{
	if (pControlPlayer)
	{
		for (UInt i = 0; i < m_BattlePlayerLst.size(); i ++)
		{
			if (m_BattlePlayerLst[i].pChar->GetTeam() != pControlPlayer->GetTeam())
			{
				for (BuffMap::iterator Itr = m_BattlePlayerLst[i].pChar->m_BuffMap.begin(); Itr != 
					m_BattlePlayerLst[i].pChar->m_BuffMap.end(); Itr ++)
				{
					if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_HideEffect &&
						!pControlPlayer->DetectBuff(Itr->second.pBuffData->id))
					{
						m_BattlePlayerLst[i].pChar->GetCharMainView()->RemovePartByName(Itr->second.pBuffData->Buff_EffectFile);
					}
				}

				for (BuffMap::iterator Itr = m_BattlePlayerLst[i].pChar->m_BuffCacheMap.begin(); 
					Itr != m_BattlePlayerLst[i].pChar->m_BuffCacheMap.end(); Itr ++)
				{
					if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_HideEffect &&
						!pControlPlayer->DetectBuff(Itr->second.pBuffData->id))
					{
						m_BattlePlayerLst[i].pChar->GetCharMainView()->RemovePartByName(Itr->second.pBuffData->Buff_EffectFile);
					}
				}
				
			}
		}
	}
}

BOOL CBattleControl::IsPKBattle()
{
	BOOL TeamAHasPlayer = FALSE;
	BOOL TeamBHasPlayer = FALSE;

	for (UInt16 i = 0; i < m_BattlePlayerLst.size(); i++)
	{
		if (m_BattlePlayerLst[i].pChar && m_BattlePlayerLst[i].pChar->GetBatActorObject() &&
			m_BattlePlayerLst[i].pChar->GetBatActorObject()->GetClassID() == BatCharacter_CLASSID)
		{
			if( m_BattlePlayerLst[i].pChar->GetTeam()==0 )
				TeamAHasPlayer = TRUE;
			else if( m_BattlePlayerLst[i].pChar->GetTeam()==1 )
				TeamBHasPlayer = TRUE;
			
			if( TeamAHasPlayer && TeamBHasPlayer )
				break;
		}
	}
	return (TeamAHasPlayer && TeamBHasPlayer);
}

void CBattleControl::SetBattleCountDownTime(UInt countTime)
{
	m_PreUpdateTime = ::timeGetTime();
	m_CountDownTime = countTime;
	m_CountDownEnt.SetNumber(countTime / 1000);
	m_StartCountDown = TRUE;
	m_CountDownEnt.SetVisible(TRUE);
}

UInt8 CBattleControl::GetCurTurn()
{
	return (pBattle ? pBattle->GetCurTurn() : 0);
}

CCharacterControl* CBattleControl::GetActor(UInt8 index, UInt8 team, bool bChar, bool bMob)
{
	BattleEntDataList::iterator it;
	UInt8 i = 0;
	CCharacterControl* pChar;
	CBatActor* pActor;

	if (index >= (UInt8) m_BattlePlayerLst.size())
		return NULL;

	for (it = m_BattlePlayerLst.begin(); it != m_BattlePlayerLst.end(); ++it)
	{
		pChar = it->pChar;
		if (pChar == NULL || pChar->GetBatActorObject() == NULL)
			continue;
		pActor = pChar->GetBatActorObject();
		if (pActor->GetLeftBattle() ||
			bMob && pActor->GetClassID() == BatCharacter_CLASSID ||
			bChar && pActor->GetClassID() == BatMob_CLASSID ||
			team != 255 && pActor->GetTeam() != team)
			continue;
		if (i == index)
			return pChar;
		++i;
	}
	return NULL;
}

CCharacterControl* CBattleControl::GetCharacter(UInt8 index, UInt8 team)
{
	return GetActor(index, true, false, (bool)team);
}

CCharacterControl* CBattleControl::GetMob(UInt8 index, UInt8 team)
{
	return GetActor(index, false, true, (bool)team);
}

UInt16 CBattleControl::GetActorCount(UInt8 team)
{
	BattleEntDataList::iterator it;
	UInt16 count = 0;
	CCharacterControl* pChar;
	CBatActor* pActor;

	for (it = m_BattlePlayerLst.begin(); it != m_BattlePlayerLst.end(); ++it)
	{
		pChar = it->pChar;
		if (pChar == NULL || pChar->GetBatActorObject() == NULL)
			continue;
		pActor = pChar->GetBatActorObject();
		if (!pActor->GetLeftBattle() && (team == 255 || pActor->GetTeam() == team))
			++count;
	}

	return count;
}

Boolean CBattleControl::IsNeedHelp()
{
	UIButton * pBu = gGlobal.pUIManager->FindUIButton("BattleJoinB");
	return (pBu && pBu->GetState() == _ButtonNormal);
}