//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "AnimatedEntity.h"
//-- Library
#include "Math.h"
#include "Resources.h"
#include "Render.h"
#include "QGLApplication.h"
#include "Global.h"
#include "Configure.h"

#define MaxWaitTime 5000

CAnimatedEntity::CAnimatedEntity()
{
	m_CurrentEntityIndex = 0;
	m_AnimationRotation = 0;
	m_CharPos.PosX = 0;
	m_CharPos.PosY = 0;
	m_CurrTexture = NULL;
	m_Updated = FALSE;
	m_EntOrgWidth = 0;
	m_EntOrgHeight = 0;
	m_InitialMoveYSpeed = 0;
	m_OffsetJumpY = 0;
	m_OffsetY = 0;
	m_OffsetX = 0;
	m_StartTime = 0;
	m_CurrPartBlitType = 0;
	m_GravityAcc = -800;
	m_AnimationLoop = TRUE;
	m_OverlappedRegion.clear();
	m_TotalTime = 0;
	m_AlphaOffset = 0;
	m_pParentEnt = NULL;
	m_ExtraZBuffer = 0;
	m_StopAnim = FALSE;
	m_OrgPosX = 0;
	m_OrgPosY = 0;
//	m_HitCount = 0;
	m_EntityType = ENTITY_TYPE_MAP_PLAYER;
	m_AnimationIndex = ANIM_IDLE1;
	m_ScaleX = 1.0f;
	m_ScaleY = 1.0f;

	m_CanBeSelected = FALSE;

	m_PlaySpeed = 1.0f;

	m_WaitRes = FALSE;

	m_OffsetType = 0;
	m_CurrColorPalette = NULL;
	m_MountState = 0;
	m_MountExtendY = 0;

	m_Mounting = FALSE;

	m_OthersOnly = TRUE;
}


CAnimatedEntity::~CAnimatedEntity()
{	
	m_RenderLst.clear();
	m_animationset.clear();
	m_EnData.clear();
	
}

void CAnimatedEntity::SetStopAnimation(BOOL Value)
{
	m_StopAnim = Value;
	InitTime();
}
BOOL CAnimatedEntity::Updateanimationset(UInt32 CurrentTime)
{
	if (m_animationset.size() > 0)
	{
		StlMultiMap<UInt32, animationsetData>::iterator itrAniChain = m_animationset.begin();
		if (CurrentTime >= itrAniChain->second.StartTime)
		{
			//if (Stop)
			//	DebugBreak();
			SetAnimationIndexByIndex(itrAniChain->second.AnimationIndex, itrAniChain->second.Loop, itrAniChain->second.Rotation);
			SetStopAnimation(itrAniChain->second.StopAnim);
			if (itrAniChain->second.UpX != 0 || itrAniChain->second.UpY != 0 || itrAniChain->second.height != 0)
			{
				SetJumpData(itrAniChain->second.height,
					itrAniChain->second.UpX, 
					itrAniChain->second.UpY, 
					itrAniChain->second.TimeInMS,
					itrAniChain->second.JumpDelay,
					itrAniChain->second.IsPixel);
			}

			SetShakeInfo(itrAniChain->second.ShakeX, itrAniChain->second.ShakeY, itrAniChain->second.ShakeSpeedX, itrAniChain->second.ShakeSpeedY,
				itrAniChain->second.ShakeWidth, itrAniChain->second.ShakeHeight, itrAniChain->second.ShakeTime);

			m_animationset.erase(itrAniChain);
			m_Updated = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}
VOID CAnimatedEntity::UpDateCurrRect()
{
	StlMultiMap<UInt32, RenderLstData>::iterator itrRenderLst;

	BOOL firstPic = TRUE;
	for (itrRenderLst = m_RenderLst.begin(); itrRenderLst != m_RenderLst.end(); itrRenderLst++)
	{
		RenderLstData * pRData = &itrRenderLst->second;
		if (pRData->EffectType == AdditiveBody ||
			pRData->EffectType == SubtractiveBody)
			continue;

		//check offset type
		Int OffsetY = 0;
		if (GetOffsetType() != pRData->OffsetType)
		{
			if (pRData->OffsetType == ENTITYOFFSETTYPE_FOOT)
			{
				pRData->OffsetY =  GetEntPixelPos().PixelPosY
					+ (-1 * m_OffsetJumpY + GetEntOffsetY()) - m_EntOrgHeight / 2 - AboveHeadOffset - m_EntLeftUpPos.PixelPosY;

				OffsetY = pRData->OffsetY;
			}
			if (pRData->OffsetType == ENTITYOFFSETTYPE_CENTER)
			{
				pRData->OffsetY =  GetEntPixelPos().PixelPosY
						+ (-1 * m_OffsetJumpY + GetEntOffsetY()) - m_EntOrgHeight / 2
						- (m_BattleTotalRect.bottom - m_BattleTotalRect.top) / 2 - m_EntLeftUpPos.PixelPosY;
				OffsetY = pRData->OffsetY;
			}
			else if (pRData->OffsetType == ENTITYOFFSETTYPE_HEAD)
			{
				pRData->OffsetY =  GetEntPixelPos().PixelPosY
						+ (-1 * m_OffsetJumpY + GetEntOffsetY()) - m_EntOrgHeight / 2
						- ((m_EntOrgHeight - CharPicOffsetY)  - m_BattleTotalRect.top) - AboveHeadOffset - m_EntLeftUpPos.PixelPosY;
				OffsetY = pRData->OffsetY;
			}
		}


		if (pRData->PicData.OrgWidth > 0 &&//PicPosMaxX - pRData->ShadowPicData.PicPosMinX > 0 &&
			pRData->PicData.OrgHeight > 0) //PicPosMaxY - pRData->ShadowPicData.PicPosMinY > 0)
		{
			LONG right = 0;
			LONG bottom = 0;
			LONG left = 0;
			LONG top = 0;
			if (pRData->Flip)
			{
				left = m_EntOrgWidth - pRData->PicData.PicOffX  - 
					(pRData->PicData.OrgWidth);//.PicPosMaxX - pRData->PicData.PicPosMinX);
				left -= pRData->ExtendX;
				right = m_EntOrgWidth - pRData->PicData.PicOffX ;
				//right += pRData->ExtendX;
			}
			else
			{
				left = pRData->PicData.PicOffX;
				left += pRData->ExtendX;
				right = (LONG)pRData->PicData.PicOffX  
					+ pRData->PicData.OrgWidth;//PicData.PicPosMaxX -
				right += pRData->ExtendX;
				//pRData->PicData.PicPosMinX;
			}
			top = pRData->PicData.PicOffY + pRData->OffsetY;
			if (pRData->EquipType == EQUIP_MOUNT || !m_Mounting || pRData->EquipType == EQUIP_OTHERS)
				top += pRData->ExtendY;
			else
				top += pRData->ExtendY + m_MountExtendY;

			bottom = (LONG)pRData->PicData.PicOffY + pRData->OffsetY + pRData->PicData.OrgHeight;
			if (pRData->EquipType == EQUIP_MOUNT || !m_Mounting || pRData->EquipType == EQUIP_OTHERS)
				bottom += pRData->ExtendY;
			else
				bottom += pRData->ExtendY + m_MountExtendY;
			//bottom += pRData->ExtendY;

			//pRData->PicData.PicPosMaxY - pRData->PicData.PicPosMinY;

			//int i = 0;
			//i ++;
			if (left < m_CurrRect.left)
				m_CurrRect.left = left;

			if (top < m_CurrRect.top)
				m_CurrRect.top = top;

			if (right > m_CurrRect.right)
				m_CurrRect.right = right;

			if (bottom > m_CurrRect.bottom)
				m_CurrRect.bottom = bottom;

		}

		//continue;

		//update shadow data
		if (pRData->ShadowPicData.OrgWidth > 0 &&//PicPosMaxX - pRData->ShadowPicData.PicPosMinX > 0 &&
			pRData->ShadowPicData.OrgHeight > 0) //PicPosMaxY - pRData->ShadowPicData.PicPosMinY > 0)
		{
			LONG right = 0;
			LONG bottom = 0;
			if (pRData->Flip)
			{
				if (m_EntOrgWidth - pRData->ShadowPicData.PicOffX - pRData->ShadowPicData.OrgWidth //(pRData->ShadowPicData.PicPosMaxX - 
					//pRData->ShadowPicData.PicPosMinX) 
					< m_CurrRect.left)
				{
					m_CurrRect.left = (m_EntOrgWidth - pRData->ShadowPicData.PicOffX - pRData->ShadowPicData.OrgWidth);
						//(pRData->ShadowPicData.PicPosMaxX - pRData->ShadowPicData.PicPosMinX));
				}
				right = m_EntOrgWidth - pRData->ShadowPicData.PicOffX;
			}
			else
			{
				if (pRData->ShadowPicData.PicOffX < m_CurrRect.left)
				{
					m_CurrRect.left = pRData->ShadowPicData.PicOffX;
				}
				right = (LONG)pRData->ShadowPicData.PicOffX + pRData->ShadowPicData.OrgWidth;//pRData->ShadowPicData.PicPosMaxX - pRData->ShadowPicData.PicPosMinX;
			}
			if (pRData->ShadowPicData.PicOffY < m_CurrRect.top)
			{
				m_CurrRect.top = pRData->ShadowPicData.PicOffY;
			}
			if (right > m_CurrRect.right)
				m_CurrRect.right = right;
			bottom = (LONG)pRData->ShadowPicData.PicOffY + pRData->ShadowPicData.OrgHeight;//pRData->ShadowPicData.PicPosMaxY - pRData->ShadowPicData.PicPosMinY;
			if (bottom > m_CurrRect.bottom)
				m_CurrRect.bottom = bottom;
		}
	}
	m_EntityTotalRect = m_CurrRect;

	//scale the rectangle of the entity
	Int Width = (Int)((m_EntityTotalRect.right - m_EntityTotalRect.left) * m_ScaleX);
	Int Height = (Int)((m_EntityTotalRect.bottom - m_EntityTotalRect.top) * m_ScaleY);

	if (Width == 0 && m_ScaleX > 0 && m_EntityTotalRect.right - m_EntityTotalRect.left > 0)
		Width = 1;

	if (Height == 0 && m_ScaleY > 0 && m_EntityTotalRect.bottom - m_EntityTotalRect.top > 0)
		Height = 1;

	m_EntityTotalRect.right = m_EntityTotalRect.left + Width;
	m_EntityTotalRect.bottom = m_EntityTotalRect.top + Height;

}

BOOL CAnimatedEntity::GetEntityPart(size_t index, Int ImageType, RECT * LockRegion)
{
	if (index >= m_RenderLst.size())
		return FALSE;
	if (m_CurrTexture == NULL)
		return FALSE;

	//return FALSE;

	RECT EntLockRegion = m_EntityTotalRect;

	if (LockRegion)
	{
		EntLockRegion = *LockRegion;
		EntLockRegion.left -= GetEntLeftUpPos().PixelPosX;
		EntLockRegion.right -= GetEntLeftUpPos().PixelPosX;
		EntLockRegion.top -= GetEntLeftUpPos().PixelPosY;
		EntLockRegion.bottom -= GetEntLeftUpPos().PixelPosY;
	}

	if (EntLockRegion.top < 0)
	{
		EntLockRegion.bottom -= EntLockRegion.top;
		EntLockRegion.top = 0;
	}
	StlMultiMap<UInt32, RenderLstData>::iterator itrRenderLst;

	itrRenderLst = m_RenderLst.begin();

	size_t tempIndex = 0;
	while (itrRenderLst != m_RenderLst.end())
	{
		if (tempIndex == index)
		{
			RenderLstData * pRenData = &itrRenderLst->second;
			if (pRenData->EffectType == AdditiveBody || 
				pRenData->EffectType == SubtractiveBody)
				return FALSE;

			EntityPicInfo * EntPic = CResourcesManager::GetInstance().LoadEntityResourcesFile(
				pRenData->EntFileName.c_str(), gGlobal.pD3DDevice);

			if (EntPic == NULL ||
				EntPic->State < DATA_FINISH_LOADING ||
				EntPic->State == DATA_DECOMPRESSING)
				return FALSE;

			EntPic->PreInUseTime = ::timeGetTime();

			m_CurrPartBlitType = pRenData->EffectType;

			PicPosData ImagePosData;
			if (ImageType == ImagePic)
				ImagePosData = pRenData->PicData;
			else if (ImageType == ShadowPic)
				ImagePosData = pRenData->ShadowPicData;

			if (ImagePosData.OrgWidth == 0 ||//== ImagePosData.PicPosMinX ||
				ImagePosData.OrgHeight == 0)// == ImagePosData.PicPosMinY)
				return FALSE;

			if (pRenData->Flip)
				m_CurrRect.left = (m_EntOrgWidth - ImagePosData.PicOffX - 
				(ImagePosData.OrgWidth))  - pRenData->ExtendX;
			else
				m_CurrRect.left = ImagePosData.PicOffX + pRenData->ExtendX;

			
			if (pRenData->EquipType == EQUIP_MOUNT || !m_Mounting || pRenData->EquipType == EQUIP_OTHERS)
				m_CurrRect.top = ImagePosData.PicOffY + pRenData->OffsetY + pRenData->ExtendY;
			else
				m_CurrRect.top = ImagePosData.PicOffY + pRenData->OffsetY + pRenData->ExtendY + m_MountExtendY;
			//m_CurrRect.top = ImagePosData.PicOffY + pRenData->OffsetY + pRenData->ExtendY;
			m_CurrRect.right = m_CurrRect.left + ImagePosData.OrgWidth;
			m_CurrRect.bottom = m_CurrRect.top + ImagePosData.OrgHeight;

			Int OffsetY = m_CurrRect.top;
			if (m_CurrRect.top < 0)
			{
				m_CurrRect.bottom -= m_CurrRect.top;
				m_CurrRect.top = 0;

			}

			RECT ResultRect;

			
			if (QGLMath::CalculateOverlappedRect(EntLockRegion, m_CurrRect, ResultRect))
			{
				if (pRenData->Flip)
				{
					Int width = ResultRect.right - ResultRect.left;
					ResultRect.right = (m_CurrRect.right - ResultRect.left) + m_CurrRect.left;
					ResultRect.left = ResultRect.right - width;
				}

				if (QGLMath::InvalidLock(&ResultRect, MaxEntSizeX, MaxEntSizeY))
				{
					return FALSE;
				}
				Byte * pTDataCurr = m_CurrTexture + (ResultRect.top * MaxEntSizeX + ResultRect.left) * sizeof(Color32);


				BYTE * pTData2 = EntPic->m_PictureData + ImagePosData.PicStartPos;

				DWORD Len = MaxEntSizeX * MaxEntSizeY * 4;
				m_CurrPicData = pTDataCurr;
				Byte * pTempData = NULL;
				//m_PicWidth = ImagePosData.OrgWidth;
				/*if (EntPic->State == DATA_DECOMPRESSED)
				{
					StlMap<UInt32, BYTE *>::iterator Itr = EntPic->m_UncompressedDataLst.find(ImagePosData.PicStartPos);
					if (Itr != EntPic->m_UncompressedDataLst.end())
					{
						pTempData = Itr->second;
					}
					else
						return FALSE;
				}
				else
				{
					pTempData = CResourcesManager::GetInstance().GetEntExtractCache();
					ZLib::uncompress(pTempData, &Len, 
						pTData2, ImagePosData.PicEndPos - ImagePosData.PicStartPos); 
				}*/

				StlMap<UInt32, BYTE *>::iterator Itr = EntPic->m_UncompressedDataLst.find(ImagePosData.PicStartPos);
				if (Itr != EntPic->m_UncompressedDataLst.end())
				{
					pTempData = Itr->second;
				}
				else
				{
					pTempData = SafeAllocate(Byte, ImagePosData.OrgSize);//CResourcesManager::GetInstance().GetEntExtractCache();
					ZLib::uncompress(pTempData, &ImagePosData.OrgSize, 
						pTData2, ImagePosData.PicEndPos - ImagePosData.PicStartPos); 
					EntPic->m_UncompressedDataLst.insert(std::make_pair(ImagePosData.PicStartPos, pTempData));
				}


			
				RECT rect;
					rect.top = ResultRect.top - m_CurrRect.top;
					rect.bottom = ImagePosData.OrgHeight + ResultRect.bottom - m_CurrRect.bottom;
					rect.left = ResultRect.left - m_CurrRect.left;
					rect.right = ImagePosData.OrgWidth + ResultRect.right - m_CurrRect.right;

					m_CurrPicType = itrRenderLst->second.PicType;
					if (itrRenderLst->second.PicType == HighQ32Bits ||
						itrRenderLst->second.PicType == Z_HighQ32Bits)
					{
						//m_CurrPicData = pTempData + (rect.top * ImagePosData.OrgWidth + rect.left) * 4;
						memcpy(pTDataCurr, pTempData + (rect.top * ImagePosData.OrgWidth + rect.left) * 4, 
							ImagePosData.OrgSize - (rect.top * ImagePosData.OrgWidth + rect.left) * 4);

						/*Byte * pTempSrcData = pTempData + (rect.top * ImagePosData.OrgWidth + rect.left) * 4;
						
						Byte * pTempDataCurr = pTDataCurr;
						for (Int i = 0; i < ImagePosData.OrgSize - (rect.top * ImagePosData.OrgWidth + rect.left) * 4; i+= 4)
						{
							memcpy(pTempDataCurr, pTempSrcData, 4);

							pTempDataCurr[0] = (pTempDataCurr[0] >> 4) << 4;
							pTempDataCurr[1] = (pTempDataCurr[1] >> 4) << 4;
							pTempDataCurr[2] = (pTempDataCurr[2] >> 4) << 4;
							pTempDataCurr[3] = (pTempDataCurr[3] >> 4) << 4;
							pTempDataCurr += 4;
							pTempSrcData += 4;
						}*/

						m_PicWidth = ImagePosData.OrgWidth;
					}
					else
						m_PicWidth = MaxEntSizeX;

					

					if (itrRenderLst->second.PicType == Color256 ||
						itrRenderLst->second.PicType == Z_Color256)
					{
						pTempData += (rect.top * ImagePosData.OrgWidth + rect.left) * 2;

						if (pRenData->pColorP == NULL)
						{
							CopyTextureFrom256Color(EntPic->ColorPalette,
								pTempData, ImagePosData.OrgWidth,
								ImagePosData.OrgHeight, MaxEntSizeX, pTDataCurr , rect,ResultRect);
						}
						else
						{
							pRenData->pColorP->PreInUseTime = ::timeGetTime();
							if (pRenData->pColorP->ColorPaletteData == NULL)
								return FALSE;
							CopyTextureFrom256Color(pRenData->pColorP->ColorPaletteData,
								pTempData, ImagePosData.OrgWidth,
								ImagePosData.OrgHeight, MaxEntSizeX, pTDataCurr , rect,ResultRect);
						}

					}
					else if (itrRenderLst->second.PicType == HighQ16Bits ||
						itrRenderLst->second.PicType == Z_HighQ16Bits)
					{
						pTempData += (rect.top * ImagePosData.OrgWidth + rect.left) * 2;
						D3DSURFACE_DESC  pDescDes;
						pDescDes.Width = MaxEntSizeX;
						pDescDes.Height = MaxEntSizeY;
						CopyTextureFrom16Bits(pTempData, ImagePosData.OrgWidth,
							ImagePosData.OrgHeight, pDescDes, pTDataCurr,  rect,ResultRect);
					}
				
/*
				//EntPic->m_BuffTextureLst[ImagePosData.TextureIndex]->UnlockRect(0);

				/*pTDataCurr = m_CurrTexture + (ResultRect.top * MaxEntSizeX + ResultRect.left) * sizeof(Color32);
				//find is there any additive or substractive image acting on the part		
				if (pRenData->EffectType != Additive &&
					pRenData->EffectType != Subtractive)
				{
					itrRenderLst++;
					while (itrRenderLst != m_RenderLst.end())
					{
						if (pRenData->EffectType == AdditiveBody || 
							pRenData->EffectType == SubtractiveBody)
						{
							RECT TempCurrRect;
							if (pRenData->Flip)
								TempCurrRect.left = (m_EntOrgWidth - pRenData->PicData.PicOffX - 
								(pRenData->PicData.PicPosMaxX - pRenData->PicData.PicPosMinX));
							else
								TempCurrRect.left = pRenData->PicData.PicOffX;

							TempCurrRect.top = pRenData->PicData.PicOffY;
							TempCurrRect.right = TempCurrRect.left + pRenData->PicData.PicPosMaxX -
								pRenData->PicData.PicPosMinX;
							TempCurrRect.bottom = TempCurrRect.top + pRenData->PicData.PicPosMaxY - 
								pRenData->PicData.PicPosMinY;

								Byte * pTDataSrc = (Byte *)EntPic->m_BuffTextureLst[ImagePosData.TextureIndex].pData;

								UInt32 SrcStartIndex = ((pRenData->PicData.PicPosMinY + ResultRect.top - 
									TempCurrRect.top )* pDescSrc.Width + (pRenData->PicData.PicPosMinX 
									+ ResultRect.left - TempCurrRect.left) ) ;

								if (pRenData->EffectType == AdditiveBody)
									CRender::BlitAdditivePic(pTDataSrc, SrcStartIndex, (UInt16)(ResultRect.right - ResultRect.left),
									(UInt16)(ResultRect.bottom - ResultRect.top), 
									pDescSrc.Width, pTDataCurr, 0, MaxEntSizeX, FALSE, sizeof(Color32));
								else
									CRender::BlitSubtractivePic(pTDataSrc, SrcStartIndex, 
									(UInt16)(ResultRect.right - ResultRect.left),
									(UInt16)(ResultRect.bottom - ResultRect.top), 
									pDescSrc.Width, pTDataCurr, 0, MaxEntSizeX,
									FALSE, FALSE, sizeof(Color32));

								//EntPic->m_BuffTextureLst[ImagePosData.TextureIndex]->UnlockRect(0);
							
						}
						itrRenderLst++;
					}
				}

				//m_CurrTexture->UnlockRect(0);
				//m_CurrRect = ResultRect;
				*/
				if (OffsetY < 0)
				{
					m_CurrRect.top = OffsetY;
					m_CurrRect.bottom += OffsetY;
				}
				return TRUE;
			}
			else
				return FALSE;
		}

		tempIndex ++;
		itrRenderLst++;
		}
		return FALSE;
}


UInt32Vector CAnimatedEntity::FindHitTime(Int GivenAnimationIndex, Int GivenRotation)
{
	
	UInt32Vector HitLst;

	if (GivenAnimationIndex < 0 ||
		GivenAnimationIndex >= MAX_ANIM_TYPE)
		return HitLst;

	//if (m_CurrentEntityIndex >= 0 && m_CurrentEntityIndex < (Int)m_EnData.size())
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		UInt32 TimeDelay = 0;
		Int Frame = -1;
		for (Int i = 0; i < (Int)Itr->second.m_EntityHeader[GivenAnimationIndex].size(); i++)
		{
			if ((GivenRotation == ERROR_INDEX && Itr->second.m_EntityHeader[GivenAnimationIndex][i].Rotation 
				== Itr->second.AnimationRotation) ||
				Itr->second.m_EntityHeader[GivenAnimationIndex][i].Rotation == GivenRotation)
			{
				if (Frame != Itr->second.m_EntityHeader[GivenAnimationIndex][i].Frame)
				{
					TimeDelay += Itr->second.m_EntityHeader[GivenAnimationIndex][i].Delay;

					if (Itr->second.m_EntityHeader[GivenAnimationIndex][i].hit > 0)
						HitLst.push_back(TimeDelay);
					
					Frame = Itr->second.m_EntityHeader[GivenAnimationIndex][i].Frame;
				}
			}
		}

		if (HitLst.size() > 0)
			break;
	}
	return HitLst;
}

Int CAnimatedEntity::GetCurrAnimationMaxFrame(EntityData * pEnData)
{

	Int Count = 0;

	Int PreFrameIndex = 0;
	Int SameIndexCount = 0;

	for (Int i = 0; i < (Int)pEnData->m_EntityHeader[pEnData->AnimationIndex].size(); i++)
	{
		if (PreFrameIndex == pEnData->m_EntityHeader[pEnData->AnimationIndex][i].Frame)
			SameIndexCount ++;
		else
		{
			PreFrameIndex = pEnData->m_EntityHeader[pEnData->AnimationIndex][i].Frame;
			SameIndexCount = 0;
		}

		if (pEnData->m_EntityHeader[pEnData->AnimationIndex][i].Rotation == pEnData->AnimationRotation )
			Count ++;
	}

	PreFrameIndex = 0;
	SameIndexCount = 0;

	if (Count == 0)
	{
		Int rotation = QGLMath::GetFlipRotation(pEnData->AnimationRotation);

		for (Int i = 0; i < (Int)pEnData->m_EntityHeader[pEnData->AnimationIndex].size(); i++)
		{
			if (PreFrameIndex == pEnData->m_EntityHeader[pEnData->AnimationIndex][i].Frame)
				SameIndexCount ++;
			else
			{
				PreFrameIndex = pEnData->m_EntityHeader[pEnData->AnimationIndex][i].Frame;
				SameIndexCount = 0;
			}

			if (pEnData->m_EntityHeader[pEnData->AnimationIndex][i].Rotation == rotation)
				Count ++;
		}
	}
	/*StlMultiMap<Int, AnimationFrameData>::iterator itrEntHeader = 
		pEnData->m_EntityHeader.find(pEnData->AnimationIndex);

	Int AllRotFrameCount = (Int)pEnData->m_EntityHeader.count(pEnData->AnimationIndex);

	Int PreFrameIndex = 0;
	Int SameIndexCount = 0;
	for (Int i = 0; i < AllRotFrameCount; i++)
	{
		if (PreFrameIndex == itrEntHeader->second.Frame)
			SameIndexCount ++;
		else
		{
			PreFrameIndex = itrEntHeader->second.Frame;
			SameIndexCount = 0;
		}

		if (itrEntHeader->second.Rotation == pEnData->AnimationRotation )
			Count ++;
		itrEntHeader ++;
	}

	if (Count == 0)
	{
		PreFrameIndex = 0;
		SameIndexCount = 0;
		Int rotation = QGLMath::GetFlipRotation(pEnData->AnimationRotation);
		itrEntHeader = pEnData->m_EntityHeader.find(pEnData->AnimationIndex);
		for (Int i = 0; i < AllRotFrameCount; i++)
		{
			if (PreFrameIndex == itrEntHeader->second.Frame)
				SameIndexCount ++;
			else
			{
				PreFrameIndex = itrEntHeader->second.Frame;
				SameIndexCount = 0;
			}

			if (itrEntHeader->second.Rotation == rotation )
				Count ++;
			itrEntHeader ++;
		}
	}
	*/
	if (SameIndexCount > 0)
		Count -= SameIndexCount ;
		

	return Count;

}

Int CAnimatedEntity::GetOffsetType()
{
	//StlMap<String, EntityData> m_EnData;
	if (m_EnData.size() == 1)
	{
		return m_OffsetType;
	}
	return ENTITYOFFSETTYPE_NORMAL;
}

VOID CAnimatedEntity::UpDate(UInt32 CurrentTime)
{
	CEntityBase::UpDate(CurrentTime);
	//reset the timer
	BOOL ALLFinish = TRUE;
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (m_WaitRes)
		{
			EntityPicInfo * EntPic = GetEntPic(&Itr->second);
			if (EntPic == NULL || EntPic->State == DATA_LOADING)
			{
				InitTime();
				m_WaitRes = TRUE;
				return;
			}
			if (EntPic->State == DATA_FINISH_LOADING)
			{
				EntPic->State = DATA_READED;
				m_Updated = TRUE;
			}
			m_WaitRes = FALSE;
		}
		
		if (CurrentTime - Itr->second.PreTime > MaxWaitTime)
		{
			InitTime();
			break;
		}
	}

	m_RenderLst.clear();

	m_CurrRect.right = m_CurrRect.bottom = 0;

	m_CurrRect.left = MaxEntSizeX;
	m_CurrRect.top = MaxEntSizeY;

	Updateanimationset(CurrentTime);

	CheckRemove(CurrentTime);



	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.CurrFrIndex == ERROR_INDEX || Itr->second.AnimationIndex == ERROR_INDEX)
			continue;

//		StlMultiMap<Int, AnimationFrameData>::iterator itrEntHeader = 
//			Itr->second.m_EntityHeader.find(Itr->second.AnimationIndex);

		Int frameNo = ERROR_INDEX;
		Int framNext = (Int)Itr->second.CurrFrIndex + 1;
		//UInt count = (UInt)Itr->second.m_EntityHeader.count(Itr->second.AnimationIndex);
		UInt count = (UInt)Itr->second.m_EntityHeader[Itr->second.AnimationIndex].size();
		Int Rotation = ERROR_INDEX;

		UInt16 Layer = 0;
		UInt32 CurrIndex = 0;

		BOOL LoopBack = FALSE;

		BOOL Found = FALSE;
		for (CurrIndex = 0;  CurrIndex < count; CurrIndex ++)
			{
				if (CurrIndex == Itr->second.CurrFrIndex)
				{
					//AnimationFrameData * itrEntHeaderCurr = Itr->second.m_EntityHeader[CurrIndex];
					frameNo = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Frame;
					Rotation = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Rotation;

					if (!m_StopAnim || Itr->second.Destory || Itr->second.EquipType == EQUIP_OTHERS)
					{
						while (CurrentTime > Itr->second.PreTime && 
							CurrentTime - Itr->second.PreTime >= 
							(UInt32)(Itr->second.PreFrData.Delay * m_PlaySpeed) &&
							CurrIndex < count)
						{
							frameNo = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Frame;
							Rotation = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Rotation;
							UInt32 PreTime = Itr->second.PreTime;
							Itr->second.PreTime += (UInt32)(Itr->second.PreFrData.Delay * m_PlaySpeed);
							Itr->second.PreFrData.Delay = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Delay;

							//find next frame

							//itrEntHeader ++;
							CurrIndex ++;

							if (CurrIndex < count
								//itrEntHeader != Itr->second.m_EntityHeader.end()
								)
							{
								Int CurrRotation = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Rotation;

								while (CurrIndex < count)
								{
									if (Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Frame != frameNo)
										break;
									//itrEntHeader ++;
									CurrIndex ++;
								}

							}
							//::GetSystemTime(
							if (CurrIndex >= count || Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Rotation != Rotation)
								LoopBack = TRUE;
							//check whether it need to loop
							if (LoopBack)
							{
								if ((Itr->second.Destory) || (!m_AnimationLoop && (Itr->second.EquipType != EQUIP_OTHERS ||
									(Itr->second.EquipType == EQUIP_OTHERS && m_OthersOnly))))
								{
									CurrIndex = Itr->second.CurrFrIndex;
									//itrEntHeader = itrEntHeaderCurr;
									//CheckRemove();
									Itr->second.FrameEnd = TRUE;
									break;
								}
								else
								{
									Itr->second.FrameEnd = FALSE;
									Int index = GetCurrAnimStartIndexByRotaion(&Itr->second, Itr->second.AnimationRotation, 
										Itr->second.AnimationIndex);
									if (index == ERROR_INDEX)
									{
										index = GetCurrAnimStartIndexByRotaion(&Itr->second , Rotation, Itr->second.AnimationIndex);
										Itr->second.AnimationRotation = Rotation;
									}
									CurrIndex = index;
									//itrEntHeader = Itr->second.m_EntityHeader.find(Itr->second.AnimationIndex);
									//for (Int i = 0; i < index; i++)
									//	itrEntHeader ++;
								}
							}

							if (CurrIndex == Itr->second.CurrFrIndex)
								break;

							if (Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Delay == 0)
								break;
						}
					}
					else
					{
						Itr->second.PreTime = CurrentTime - Itr->second.PreFrData.Delay;
					}
					
					if (!m_Updated)
					{
						m_Updated = CurrIndex != Itr->second.CurrFrIndex;
					}

					if (CurrIndex != Itr->second.CurrFrIndex)
						Itr->second.FrameEnd = FALSE;
					Itr->second.CurrFrIndex = CurrIndex;

					frameNo = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Frame;
					Rotation = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Rotation;
					while(//itrEntHeader != Itr->second.m_EntityHeader.end() && 
						CurrIndex < count)
					{
						if (frameNo != Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Frame || 
							Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Rotation != Rotation)
						{
							break;
						}
						//if (CurrIndex == Itr->second.CurrFrIndex
						RenderLstData RData;
						RData.AlphaBkIndex = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].AlphaIndex;
						RData.ShadowBkIndex = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].ShadowIndex;
						RData.BlockBkIndex = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].BlockingIndex;
						RData.PicData = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].ImagePicPos;
						RData.PicType = Itr->second.PicType;
						RData.OffsetType = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Offset;
						RData.pColorP = Itr->second.pColorP;
						RData.EntFileName = Itr->second.FileName;
						RData.EquipType = Itr->second.EquipType;
						RData.ExtendX = Itr->second.ExtendX;
						RData.ExtendY = Itr->second.ExtendY;

						Layer = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Layer;
						Itr->second.PreFrData = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex];

						/*if (RData.PicData.TextureIndex >= Itr->second.m_BuffTextureLst.size() || 
							RData.ShadowPicData.TextureIndex >= Itr->second.m_BuffTextureLst.size())
							break;

						RData.Image = Itr->second.m_BuffTextureLst[RData.PicData.TextureIndex];
						RData.ShadowImage = Itr->second.m_BuffTextureLst[RData.ShadowPicData.TextureIndex];
						*/

						RData.EffectType = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Disposal;

						RData.ShadowPicData = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].ShadowPicPos;

						frameNo = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Frame;
						Rotation = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][CurrIndex].Rotation;
						if (Rotation != Itr->second.AnimationRotation)
							Itr->second.flip = TRUE;
						else
							Itr->second.flip = FALSE;

						RData.Flip = Itr->second.flip;

						m_RenderLst.insert(std::make_pair(Layer, RData));
						//itrEntHeader ++;
						CurrIndex ++;
					}
				}
			}
		
	}
	UpDateCurrRect();
}


VOID CAnimatedEntity::CheckRemove(UInt32 CurrentTime)
{
	StlVector<StlMultiMap<String, EntityData>::iterator > RmLst;

	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.Destory && 
			CurrentTime >= Itr->second.DeleteTime 
			&& FrameEnd(&Itr->second, CurrentTime))
		{
			RmLst.push_back(Itr);
		}
	}

	for (size_t i = 0; i < RmLst.size(); i++)
	{
		if (RmLst[i]->second.Free)
			CResourcesManager::GetInstance().ReleaseEntityData(RmLst[i]->second.FileName);
		m_EnData.erase(RmLst[i]);
		m_Updated = TRUE;
	}

	if (RmLst.size() > 0)
	{
		m_BattleTotalRect = GetAnimationTotalRect(ANIM_IDLE1);
		
		if (m_BattleTotalRect.left == -1)
			m_BattleTotalRect = GetAnimationTotalRect(GetBattleAnimation());

		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
	}
}

BOOL CAnimatedEntity::RemovePartByName(String Name)
{
	BOOL Removed = FALSE;
	StlVector<StlMultiMap<String, EntityData>::iterator > RmLst;

	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); 
		Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.FileName == Name)
		{
			RmLst.push_back(Itr);
			Removed = TRUE;
		}
	}

	for (size_t i = 0; i < RmLst.size(); i++)
	{
		if (RmLst[i]->second.EquipType == EQUIP_MOUNT)
		{
			/*for (StlMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
			{
				if (Itr->second.EquipType != EQUIP_MOUNT)
				{
					Itr->second.ExtendY += gConfigure.uMountOffset;
				}
			}*/
			m_MountExtendY = 0;
			m_MountState = 0;
			SetAnimationIndexByIndex(ANIM_IDLE1);
		}
		m_EnData.erase(RmLst[i]);
		m_Updated = TRUE;
		
	}

	if (Removed)
	{
		m_BattleTotalRect = GetAnimationTotalRect(ANIM_IDLE1);
		
		if (m_BattleTotalRect.left == -1)
			m_BattleTotalRect = GetAnimationTotalRect(GetBattleAnimation());

		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
	}
	return Removed;
}

BOOL CAnimatedEntity::RemovePart(Equip_Type EQUIPTYPE)
{

	BOOL Removed = FALSE;
	StlVector<StlMultiMap<String, EntityData>::iterator > RmLst;

	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); 
		Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.EquipType == EQUIPTYPE &&
			!Itr->second.Destory)
		{
			RmLst.push_back(Itr);
			Removed = TRUE;
		}
	}

	for (size_t i = 0; i < RmLst.size(); i++)
	{
		if (RmLst[i]->second.EquipType == EQUIP_MOUNT)
		{
			/*for (StlMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
			{
				if (Itr->second.EquipType != EQUIP_MOUNT)
				{
					Itr->second.ExtendY += gConfigure.uMountOffset;
				}
			}*/
			m_MountExtendY = 0;
			m_MountState = 0;
			SetAnimationIndexByIndex(ANIM_IDLE1);
		}
	
		m_EnData.erase(RmLst[i]);
		m_Updated = TRUE;
		
	}

	if (Removed)
	{
		m_BattleTotalRect = GetAnimationTotalRect(ANIM_IDLE1);
		
		if (m_BattleTotalRect.left == -1)
			m_BattleTotalRect = GetAnimationTotalRect(GetBattleAnimation());

		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
	}

	return Removed;
}


BOOL CAnimatedEntity::GetFlip(Int Index)
 { 
	 StlMultiMap<UInt32, RenderLstData>::iterator itrRenderLst;
	 Int i = 0;
	 for (itrRenderLst = m_RenderLst.begin(); itrRenderLst != m_RenderLst.end(); i++, itrRenderLst++)
	 {
		 if (i == Index)
			 return itrRenderLst->second.Flip;
	 }
	 
	 return FALSE;
 }

 BYTE CAnimatedEntity::GetEquipType(Int Index)
 { 
	 StlMultiMap<UInt32, RenderLstData>::iterator itrRenderLst;
	 Int i = 0;
	 for (itrRenderLst = m_RenderLst.begin(); itrRenderLst != m_RenderLst.end(); i++, itrRenderLst++)
	 {
		 if (i == Index)
			 return itrRenderLst->second.EquipType;
	 }
	 
	 return FALSE;
 }


VOID CAnimatedEntity::SetRotation(Int Rotation)
{
	//if (m_AnimationRotation == Rotation)
	//	return;
	if (Rotation != ERROR_INDEX)
		m_AnimationRotation = Rotation;
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.AnimationRotation == m_AnimationRotation)
			continue;
		Int Index = GetCurrAnimStartIndexByRotaion(&Itr->second, m_AnimationRotation, Itr->second.AnimationIndex);
		if (Index >= 0 )
		{
			Itr->second.CurrFrIndex = Index;
			Itr->second.AnimationRotation = Rotation;
			m_Updated = TRUE;
		}
		else if (Itr->second.EquipType != EQUIP_OTHERS)
		{
			Itr->second.CurrFrIndex = ERROR_INDEX;
			Itr->second.AnimationRotation = ERROR_INDEX;
		}
	}
}

VOID CAnimatedEntity::FlipEntity() 
{
	switch(m_AnimationRotation)
	{
	case NORTHWEST:
		SetRotation(NORTHEAST);
		break;
	case WEST:
		SetRotation(EAST);
		break;
	case SOUTHWEST:
		SetRotation(SOUTHEAST);
		break;
	case EAST:
		SetRotation(WEST);
		break;
	case NORTHEAST:
		SetRotation(NORTHWEST);
		break;
	case SOUTHEAST:
		SetRotation(SOUTHWEST);
		break;
	}
}

VOID CAnimatedEntity::ResetUpdateTime(UInt32 CurrTime)
{
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		Itr->second.PreTime = CurrTime;
	}
}

UInt32 CAnimatedEntity::GetTotalTimeOfGivenAnimation(Int GivenAnimationIndex, Int GivenRotation)
{
	UInt32 TotalTime = 0;

	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		Int SubGivenRotation = GivenRotation;
		if (GivenRotation == ERROR_INDEX)
			SubGivenRotation = Itr->second.AnimationRotation;

		Int SubGivenAnimationIndex = GivenAnimationIndex;
		if (GivenAnimationIndex == ERROR_INDEX)
			SubGivenAnimationIndex = Itr->second.AnimationIndex;

		UInt32 CurrTotalTime = 0;
		//StlMultiMap<Int, AnimationFrameData>::iterator itrEntHeader;

		//Int count = (Int)Itr->second.m_EntityHeader.count(SubGivenAnimationIndex);
		Int count = (Int)Itr->second.m_EntityHeader[SubGivenAnimationIndex].size();

		Int j;
		UInt32 LastDelay = 0;
		Int PreFrameNo = -1;
		for (//itrEntHeader = Itr->second.m_EntityHeader.find(SubGivenAnimationIndex), 
			j = 0; j < count //&& itrEntHeader != Itr->second.m_EntityHeader.end() 
			; j ++)//, itrEntHeader++)
		{
			if (Itr->second.m_EntityHeader[SubGivenAnimationIndex][j].Rotation == SubGivenRotation &&
				PreFrameNo != Itr->second.m_EntityHeader[SubGivenAnimationIndex][j].Frame)
			{
				CurrTotalTime += (UInt32)(Itr->second.m_EntityHeader[SubGivenAnimationIndex][j].Delay * m_PlaySpeed);
				LastDelay = Itr->second.m_EntityHeader[SubGivenAnimationIndex][j].Delay;
				PreFrameNo = Itr->second.m_EntityHeader[SubGivenAnimationIndex][j].Frame;
			}
		}

		PreFrameNo = -1;
		if (CurrTotalTime == 0)
		{
			Int FlipRotation = QGLMath::GetFlipRotation(SubGivenRotation);
			for (//itrEntHeader = Itr->second.m_EntityHeader.find(SubGivenAnimationIndex), 
				j = 0; j < count //&& itrEntHeader != Itr->second.m_EntityHeader.end() 
				; j ++)//, itrEntHeader++)
			{
				if (Itr->second.m_EntityHeader[SubGivenAnimationIndex][j].Rotation == FlipRotation &&
					PreFrameNo != Itr->second.m_EntityHeader[SubGivenAnimationIndex][j].Frame)
				{
					CurrTotalTime += (UInt32)(Itr->second.m_EntityHeader[SubGivenAnimationIndex][j].Delay * m_PlaySpeed);
					LastDelay = Itr->second.m_EntityHeader[SubGivenAnimationIndex][j].Delay;
					PreFrameNo = Itr->second.m_EntityHeader[SubGivenAnimationIndex][j].Frame;
				}
			}
			/*for (itrEntHeader = Itr->second.m_EntityHeader.find(SubGivenAnimationIndex), j = 0; 
				j < count && itrEntHeader != Itr->second.m_EntityHeader.end() ; j ++, itrEntHeader++)
			{
				if (itrEntHeader->second.Rotation == FlipRotation &&
				PreFrameNo != itrEntHeader->second.Frame)
				{
					CurrTotalTime += (UInt32)(itrEntHeader->second.Delay * m_PlaySpeed);
					LastDelay = itrEntHeader->second.Delay;
					PreFrameNo = itrEntHeader->second.Frame;
				}
			}*/
		}

		//CurrTotalTime -= LastDelay;
		if (CurrTotalTime > TotalTime)
			TotalTime = CurrTotalTime;
	}

	return TotalTime;
}

VOID CAnimatedEntity::InitTime()
{
	UInt32 CurrTime = ::timeGetTime();
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		Itr->second.PreTime = CurrTime;
		Itr->second.PreFrData.Delay = 0;
	}
}

VOID CAnimatedEntity::FindAvailAnimation(EntityData * pEntData)
{
	//find available animation and rotation for this part

	BOOL FOUND = FALSE;

	//first find the same rotation with players
	for (Int AniIndex = ANIM_IDLE1; AniIndex < MAX_ANIM_TYPE; AniIndex++)
	{
		//StlMultiMap<Int, AnimationFrameData>::iterator itrEntHeaderAll = 
		// 	pEntData->m_EntityHeader.find(AniIndex);
		if (pEntData->m_EntityHeader[AniIndex].size() > 0)//itrEntHeaderAll != pEntData->m_EntityHeader.end())
		{
			Int CurrIndex = GetCurrAnimStartIndexByRotaion(pEntData, m_AnimationRotation, AniIndex);
			if (CurrIndex >= 0)
			{
				if (pEntData->AnimationIndex != AniIndex || 
					pEntData->AnimationRotation != m_AnimationRotation)
				{
					pEntData->CurrFrIndex = CurrIndex;
					pEntData->AnimationIndex = AniIndex;
					pEntData->AnimationRotation = m_AnimationRotation;
					m_Updated = TRUE;
				}
				break;
			}
			for (Int rot = SOUTH; rot <= SOUTHWEST; rot ++)
			{
				Int CurrIndex = GetCurrAnimStartIndexByRotaion(pEntData, rot, AniIndex);
				if (CurrIndex >= 0)
				{
					if (pEntData->AnimationIndex != AniIndex || 
						pEntData->AnimationRotation != rot)
					{
						pEntData->CurrFrIndex = CurrIndex;
						pEntData->AnimationIndex = AniIndex;
						pEntData->AnimationRotation = rot;
						m_Updated = TRUE;
					}
					FOUND = TRUE;
					break;
				}
			}
		}
		if (FOUND)
			break;
	}
}

Int CAnimatedEntity::ChangeMobAnim(Int Index)
{
	switch(Index)
	{
	case ANIM_MOUNT_IDLE1:
		return ANIM_IDLE1;
	case ANIM_MOUNT_IDLE2:
		return ANIM_IDLE2;
	case ANIM_MOUNT_RUN:
		return ANIM_RUN;
	}

	return Index;
}

Int CAnimatedEntity::ChangeNorAnim(Int Index)
{
	switch(Index)
	{
	case ANIM_IDLE1:
		return ANIM_MOUNT_IDLE1;
	case ANIM_IDLE2:
		return ANIM_MOUNT_IDLE2;
	case ANIM_RUN :
		return ANIM_MOUNT_RUN;
	}

	return Index;
}

VOID CAnimatedEntity::SetAnimationIndexByIndex(Int Index, BOOL Loop, Int GivenRotation, BOOL flush)
{

	BOOL tempMount = IsMount();
	m_AnimationLoop = Loop;

	UInt32 CurrTime = ::timeGetTime();

	if (IsMount())
	{
		Int temp = ChangeNorAnim(Index);
		Index = temp;
	}

	if (Index != ERROR_INDEX)
		m_AnimationIndex = Index;
	
	//Int Rotation =
	if (GivenRotation != ERROR_INDEX)
		m_AnimationRotation = GivenRotation;
	

	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		UInt32 TempCurrFrIndex = Itr->second.CurrFrIndex;

		Int tempIndex = m_AnimationIndex;
		if (Itr->second.EquipType == EQUIP_MOUNT)
			tempIndex = ChangeMobAnim(m_AnimationIndex);

		if (!flush)
		{
			if ((Itr->second.AnimationIndex == tempIndex && 
				Itr->second.AnimationRotation == m_AnimationRotation))
				continue;
		}

		Int CurrIndex = 0;
		
		
		CurrIndex = GetCurrAnimStartIndexByRotaion(&Itr->second, m_AnimationRotation, tempIndex);

		if (CurrIndex >= 0 && (Itr->second.EquipType != EQUIP_OTHERS || tempIndex != Itr->second.AnimationIndex))
		{
			Itr->second.CurrFrIndex = CurrIndex;
			
			Itr->second.AnimationIndex = tempIndex;
			Itr->second.AnimationRotation = m_AnimationRotation;
			Itr->second.PreTime = CurrTime;
			Itr->second.FrameEnd = FALSE;
			m_Updated = TRUE;
			if (Itr->second.EquipType == EQUIP_MOUNT)
				m_Mounting = TRUE;
		}
		else if (Itr->second.EquipType != EQUIP_OTHERS)
		{
			Itr->second.CurrFrIndex = ERROR_INDEX;
			Itr->second.AnimationIndex = ERROR_INDEX;
			Itr->second.AnimationRotation = ERROR_INDEX;
			Itr->second.FrameEnd = FALSE;
			m_Updated = TRUE;
			if (Itr->second.EquipType == EQUIP_MOUNT)
				m_Mounting = FALSE;
		}
	}

	if (tempMount != IsMount())
	{
		m_BattleTotalRect = GetAnimationTotalRect(ANIM_IDLE1);
		
		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
	}
	
}
BOOL CAnimatedEntity::FrameEndNoOthers( UInt32 CurrentTime)
{
	BOOL End = TRUE;
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.EquipType != EQUIP_OTHERS && !FrameEnd(&Itr->second, CurrentTime))
		{
			End = FALSE;
			break;
		}
	}
	return End;
}
BOOL CAnimatedEntity::FrameEnd(UInt32 CurrentTime)
{
	BOOL End = TRUE;
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (!FrameEnd(&Itr->second, CurrentTime))
		{
			End = FALSE;
			break;
		}
	}
	return End;
}

BOOL CAnimatedEntity::FrameEnd(EntityData * pEnData, UInt32 CurrentTime)
{
	if (pEnData->FrameEnd)
		return TRUE;

	Int StartIndex = GetCurrAnimStartIndexByRotaion(pEnData, pEnData->AnimationRotation, pEnData->AnimationIndex);
	
	if (StartIndex == ERROR_INDEX)
		return TRUE;

	UInt CurrIndex = pEnData->CurrFrIndex;
	if (pEnData->CurrFrIndex - StartIndex == 
		GetCurrAnimationMaxFrame(pEnData) - 1 &&
		CurrentTime > pEnData->PreTime + pEnData->PreFrData.Delay)
	{
		return TRUE;
	}

	return FALSE;
}
Int CAnimatedEntity::GetCurrAnimStartIndexByRotaion(EntityData * pEnData,  Int Rotation, Int AnimationIndex)
{
	if (pEnData == NULL || AnimationIndex < 0 || AnimationIndex > MAX_ANIM_TYPE)
		return ERROR_INDEX;

	//StlMultiMap<Int, AnimationFrameData>::iterator itrEntHeaderAll = pEnData->m_EntityHeader.find(AnimationIndex);

	Int index = 0;
	Int count = (Int)pEnData->m_EntityHeader[AnimationIndex].size();//m_EntityHeader.count(AnimationIndex);
	while(index < count) 
	{
		if (pEnData->m_EntityHeader[AnimationIndex][index].Rotation == Rotation)
		{
			return index;
		}
		index ++;
		//itrEntHeaderAll++;
	}

	//if Not Found, find the flipping picture rotation
	switch(Rotation)
	{
	case NORTHWEST:
		Rotation = NORTHEAST;
		break;
	case WEST:
		Rotation = EAST;
		break;
	case SOUTHWEST:
		Rotation = SOUTHEAST;
		break;
	case EAST:
		Rotation = WEST;
		break;
	case NORTHEAST:
		Rotation = NORTHWEST;
		break;
	case SOUTHEAST:
		Rotation = SOUTHWEST;
		break;
	}

	//itrEntHeaderAll = pEnData->m_EntityHeader.find(AnimationIndex);

	index = 0;
	index = 0;
	while(index < count) 
	{
		if (pEnData->m_EntityHeader[AnimationIndex][index].Rotation == Rotation)
		{
			return index;
		}
		index ++;
		//itrEntHeaderAll++;
	}
	/*while(itrEntHeaderAll != pEnData->m_EntityHeader.end() && index < count) 
	{
		if (itrEntHeaderAll->second.Rotation == Rotation)
		{
			return index;
		}
		index ++;
		itrEntHeaderAll++;
	}
	*/

	return ERROR_INDEX;
}

VOID CAnimatedEntity::CopyTextureFrom16Bits(Byte * PictureData, UInt16 ImageWidth, UInt16 ImageHeight,
									   D3DSURFACE_DESC  pDescDes, Byte * pDesTData, 
									    RECT SrcCopyRect, RECT DesCopyRect)
{
	UInt SrcLen = ImageWidth * 2;
	UInt DesLen = (pDescDes.Width) * sizeof(Color32);

	UInt UpdateSrcLen = (SrcCopyRect.right - SrcCopyRect.left ) * 2;
	UInt UpdateDesLen = (SrcCopyRect.right - SrcCopyRect.left ) * sizeof(Color32);

	for (Int y = 0 ; y < SrcCopyRect.bottom - SrcCopyRect.top; y ++)
	{
		for (Int x = 0 ; x < SrcCopyRect.right - SrcCopyRect.left; x ++)
		{		
			pDesTData[0] = PictureData[0] & 0xF0;
			pDesTData[ 1] =(PictureData[0] & 0x0F) << 4;
			pDesTData[ 2] = PictureData[ 1] & 0xF0;
			pDesTData[ 3] = (PictureData[ 1] & 0x0F) << 4;
			if (pDesTData[ 3] > 0)
				pDesTData[ 3] += 0x0F;

			pDesTData += sizeof(Color32);
			PictureData += 2;
		}
		PictureData += SrcLen - UpdateSrcLen;
		pDesTData += DesLen - UpdateDesLen;
	}
}

VOID CAnimatedEntity::CopyTextureFrom256Color(Byte * ColorPalette, Byte * ColorPaletteIndex, UInt16 ImageWidth, 
		UInt16 ImageHeight, Int  DescWidth, Byte * pDesTData,  RECT SrcCopyRect, RECT DesCopyRect)
{
	UInt SrcLen = ImageWidth * 2;
	UInt DesLen = (DescWidth) * sizeof(Color32);

	UInt UpdateSrcLen = SrcLen - (SrcCopyRect.right - SrcCopyRect.left ) * 2;
	UInt UpdateDesLen = DesLen - (SrcCopyRect.right - SrcCopyRect.left ) * sizeof(Color32);

	
		for (Int y = 0 ; y < SrcCopyRect.bottom - SrcCopyRect.top; y ++)
		{
			for (Int x = 0 ; x < SrcCopyRect.right - SrcCopyRect.left; x ++)
			{
				memcpy(pDesTData, ColorPalette + ColorPaletteIndex[0] * 3, 3);
				pDesTData[3] = ColorPaletteIndex[1];

				/*pDesTData[0] = (pDesTData[0] >> 4) << 4;
				pDesTData[1] = (pDesTData[1] >> 4) << 4;
				pDesTData[2] = (pDesTData[2] >> 4) << 4;
				pDesTData[3] = (pDesTData[3] >> 4) << 4;
				*/

				pDesTData += sizeof(Color32);
				ColorPaletteIndex += 2;
			}

			ColorPaletteIndex += UpdateSrcLen;
			pDesTData += UpdateDesLen;
		}
	
}

BOOL CAnimatedEntity::LoadEntityFile(LPCTSTR filename, LPDIRECT3DDEVICE9	pD3DDevice, Equip_Type EquipType, BOOL Destory,
									 UInt32 DeleteTime, BOOL Free)
{	
	if (m_EnData.find(String(filename)) != m_EnData.end() && EquipType != EQUIP_OTHERS)
		return FALSE;

	EntityData m_EntityPicInfo;
	m_EntityPicInfo.PreTime = ::timeGetTime();
	m_EntityPicInfo.EquipType = EquipType;
	m_EntityPicInfo.FileName = filename;
	m_EntityPicInfo.Destory = Destory;
	m_EntityPicInfo.DeleteTime = DeleteTime;
	m_EntityPicInfo.Free = Free;

	EntityPicInfo * EntPic = CResourcesManager::GetInstance().LoadEntityResourcesFile(filename, pD3DDevice);
	//m_EntityPicInfo.pEntPic = EntPic;
	if (EntPic)
	{
		if (EquipType != EQUIP_OTHERS)
			m_OthersOnly = FALSE;
		//m_EntOrgWidth = 0;
		if (EntPic->SrcWidth > m_EntOrgWidth)
			m_EntOrgWidth = EntPic->SrcWidth;

		if (EntPic->SrcWidth == 256)
		{
			m_EntityPicInfo.ExtendX = (CommonEntSizX - EntPic->SrcWidth) >> 1;
			m_EntOrgWidth = CommonEntSizX;
		}

		if (EntPic->SrcHeight > m_EntOrgHeight)
			m_EntOrgHeight = EntPic->SrcHeight;

		if (EntPic->SrcHeight == 256)
		{
			m_EntityPicInfo.ExtendY = (CommonEntSizY - EntPic->SrcHeight) >> 1;
			m_EntOrgHeight = CommonEntSizY;
		}

		//m_EntityPicInfo.m_EntityHeader = EntPic->m_EntityHeader;
		//for (Int i = 0; i < MAX_ANIM_TYPE; i++)
		//	m_EntityPicInfo.AnimationFrameDataCount[i] = EntPic->AnimationFrameDataCount[i];
		//memcpy(m_EntityPicInfo.AnimationFrameDataCount, EntPic->AnimationFrameDataCount, MAX_ANIM_TYPE * sizeof(UInt16));

		for (Int i = 0; i < MAX_ANIM_TYPE; i++)
		{
			m_EntityPicInfo.m_EntityHeader[i] = EntPic->m_EntityHeader[i];
				//m_EntityPicInfo.m_EntityHeader[i] = SafeAllocate(AnimationFrameData, 
				//	m_EntityPicInfo.AnimationFrameDataCount[i] * sizeof(AnimationFrameData));
				//memcpy(m_EntityPicInfo.m_EntityHeader[i], EntPic->m_EntityHeader[i], 
				//	m_EntityPicInfo.AnimationFrameDataCount[i] * sizeof(AnimationFrameData));
		}
		m_EntityPicInfo.PicType = EntPic->PicType;


		if (GetIsMapObject())
			m_CurrTexture = CResourcesManager::GetInstance().GetObjEntPicCacheTexture();
		else
			m_CurrTexture = CResourcesManager::GetInstance().GetEntPicCacheTexture();
		

		if (EquipType == EQUIP_OTHERS)
			FindAvailAnimation(&m_EntityPicInfo);

		m_EnData.insert(std::make_pair(String(filename) ,m_EntityPicInfo));

		if (EquipType != EQUIP_OTHERS)
		{
			if (EquipType == EQUIP_WEAPON && (m_AnimationIndex == ANIM_W1BAT || m_AnimationIndex == ANIM_W2BAT))
				SetAnimationIndexByIndex(GetBattleAnimation(), m_AnimationLoop, m_AnimationRotation, TRUE);
			else
				SetAnimationIndexByIndex(m_AnimationIndex, m_AnimationLoop, m_AnimationRotation, TRUE);
		}
	
		m_Updated = TRUE;
		m_WaitRes = TRUE;
		if (EquipType == EQUIP_MOUNT)
		{
			//m_MountExtendY = -1 * gConfigure.uMountOffset;
			m_MountState = 1;
		}

		m_BattleTotalRect = GetAnimationTotalRect(ANIM_IDLE1);
		
		if (m_BattleTotalRect.left == -1)
			m_BattleTotalRect = GetAnimationTotalRect(GetBattleAnimation());
			

		for (Int i = 0; i < MAX_ANIM_TYPE; i++)
		{
			if (EntPic->m_EntityHeader[i].size() > 0)
				m_OffsetType = EntPic->m_EntityHeader[i][0].Offset;
		}
		//if (EntPic->AnimationFrameDataCount.size() > 0)
		//	m_OffsetType = EntPic->m_EntityHeader.begin()->second.Offset;
		
		UpDate(::timeGetTime());

		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
		return TRUE;
	}
	return FALSE;
}

//remove child from this entity

RECT CAnimatedEntity::GetAnimationTotalRect(Int GivenAnimationIndex)
{
	RECT ResultRect;
	ResultRect.left = ResultRect.top = ResultRect.right = ResultRect.bottom = -1;

	if (GivenAnimationIndex < 0 || GivenAnimationIndex >= MAX_ANIM_TYPE)
		return ResultRect;

	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.EquipType != EQUIP_BODY && Itr->second.EquipType != EQUIP_HEAD &&
			Itr->second.EquipType != EQUIP_HAIR && Itr->second.EquipType != EQUIP_MOUNT)
			continue;

		Int tempIndex = GivenAnimationIndex;
		if (IsMount())
		{
			if (Itr->second.EquipType == EQUIP_MOUNT)
				tempIndex = ChangeMobAnim(GivenAnimationIndex);
			else
				tempIndex = ChangeNorAnim(GivenAnimationIndex);
		}
		//StlMultiMap<Int, AnimationFrameData>::iterator itrEntHeader = 
		//	Itr->second.m_EntityHeader.find(GivenAnimationIndex);
		Int Count = (Int)Itr->second.m_EntityHeader[tempIndex].size();

		for (Int i = 0; i < Count; i++)
		{
			RECT TempRect;
			if (Itr->second.m_EntityHeader[tempIndex][i].ImagePicPos.OrgSize == 0)
				continue;

			TempRect.left = Itr->second.m_EntityHeader[tempIndex][i].ImagePicPos.PicOffX + Itr->second.ExtendX;
			if (Itr->second.EquipType == EQUIP_MOUNT || !m_Mounting || Itr->second.EquipType == EQUIP_OTHERS)
				TempRect.top = Itr->second.m_EntityHeader[tempIndex][i].ImagePicPos.PicOffY + Itr->second.ExtendY;
			else
				TempRect.top = Itr->second.m_EntityHeader[tempIndex][i].ImagePicPos.PicOffY + Itr->second.ExtendY + m_MountExtendY;
			TempRect.right = TempRect.left + Itr->second.m_EntityHeader[tempIndex][i].ImagePicPos.OrgWidth;//.PicPosMaxX -
				//Itr->second.m_EntityHeader[tempIndex][i].ImagePicPos.PicPosMinX;
			TempRect.bottom = TempRect.top + Itr->second.m_EntityHeader[tempIndex][i].ImagePicPos.OrgHeight;//PicPosMaxY -
				//Itr->second.m_EntityHeader[tempIndex][i].ImagePicPos.PicPosMinY;

			if (ResultRect.left == -1)
			{
				ResultRect = TempRect;
			}
			else
				QGLMath::CalculateBiggestRect(ResultRect, TempRect, ResultRect);
		}
	}

	return ResultRect;
}

RECT CAnimatedEntity::GetMapReferenceTotalRect()
{
	RECT RefRect;

	RefRect = m_EntityTotalRect;

	RefRect.left += GetEntLeftUpPos().PixelPosX;
	RefRect.right += GetEntLeftUpPos().PixelPosX;
	RefRect.top += GetEntLeftUpPos().PixelPosY;
	RefRect.bottom += GetEntLeftUpPos().PixelPosY;

	return RefRect;
}

VOID CAnimatedEntity::RandomCurrFrIndex()
{
	//return;

	UInt32 Num = GetTotalTimeOfGivenAnimation(m_AnimationIndex, m_AnimationRotation);
	if (Num > 0)
	{
		UInt32 TimeVar = rand() % GetTotalTimeOfGivenAnimation(m_AnimationIndex, m_AnimationRotation);

		for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
		{
			Itr->second.PreTime -= TimeVar;
		}
	}
}

int CAnimatedEntity::GetWeaponAttackType()
{
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.EquipType == EQUIP_WEAPON)
		{
			//StlMultiMap<Int, AnimationFrameData>::iterator itrEntHeader = 
			//Itr->second.m_EntityHeader.find(ANIM_W1BAT);
			//if (itrEntHeader != Itr->second.m_EntityHeader.end())
			if (Itr->second.m_EntityHeader[ANIM_W1BAT].size() > 0)
				return ANIM_W1BAT;
			//itrEntHeader = Itr->second.m_EntityHeader.find(ANIM_W2BAT);
			//if (itrEntHeader != Itr->second.m_EntityHeader.end())
			if (Itr->second.m_EntityHeader[ANIM_W2BAT].size() > 0)
				return ANIM_W2BAT;
			break;
		}
	}
	return ERROR_INDEX;
}

Int CAnimatedEntity::GetBattleAnimation()
{
	BOOL ALLFOUND = TRUE;
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.EquipType == EQUIP_OTHERS ||
			Itr->second.EquipType == EQUIP_MOUNT)
			continue;

		//wait header finish if empty
		/*if (Itr->second.m_EntityHeader.size() == 0)
		{
			EntityPicInfo * pEntPic = GetEntPic(&Itr->second);
			if (pEntPic)
			{
				while (pEntPic->State == DATA_LOADING)
				{
					Sleep(1);
				}
				Itr->second.m_EntityHeader = pEntPic->m_EntityHeader;
			}
		}*/
		//StlMultiMap<Int, AnimationFrameData>::iterator itrEntHeader = 
		//	Itr->second.m_EntityHeader.find(ANIM_W1BAT);
		if (//itrEntHeader == Itr->second.m_EntityHeader.end()
			Itr->second.m_EntityHeader[ANIM_W1BAT].size() == 0)
		{
			ALLFOUND = FALSE;
			break;
		}
	}

	if (ALLFOUND)
		return ANIM_W1BAT;
	else
		return ANIM_W2BAT;

}

Int CAnimatedEntity::GetBattleAttack2Animation()
{
	BOOL ALLFOUND = TRUE;
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.EquipType == EQUIP_OTHERS ||
			Itr->second.EquipType == EQUIP_MOUNT)
			continue;

		/*StlMultiMap<Int, AnimationFrameData>::iterator itrEntHeader = 
			Itr->second.m_EntityHeader.find(ANIM_W1BAT);
		if (itrEntHeader == Itr->second.m_EntityHeader.end())
		{
			ALLFOUND = FALSE;
			break;
		}
		*/
		if (//itrEntHeader == Itr->second.m_EntityHeader.end()
			Itr->second.m_EntityHeader[ANIM_W1BAT].size() == 0)
		{
			ALLFOUND = FALSE;
			break;
		}
	}

	if (ALLFOUND)
		return ANIM_W1ATK2;
	else
		return ANIM_W2ATK2;
	

}
Int CAnimatedEntity::GetBattleAttack1Animation()
{
	BOOL ALLFOUND = TRUE;
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.EquipType == EQUIP_OTHERS ||
			Itr->second.EquipType == EQUIP_MOUNT)
			continue;

		/*StlMultiMap<Int, AnimationFrameData>::iterator itrEntHeader = 
			Itr->second.m_EntityHeader.find(ANIM_W1BAT);
		if (itrEntHeader == Itr->second.m_EntityHeader.end())
		{
			ALLFOUND = FALSE;
			break;
		}*/
		if (//itrEntHeader == Itr->second.m_EntityHeader.end()
			Itr->second.m_EntityHeader[ANIM_W1BAT].size() == 0)
		{
			ALLFOUND = FALSE;
			break;
		}
	}

	if (ALLFOUND)
		return ANIM_W1ATK1;
	else
		return ANIM_W2ATK1;
}

StlVector<BlockPos> CAnimatedEntity::GetCurrBlockPositionLst(Int index)
{
	StlVector<BlockPos> Copy;

	if (m_EnData.size() > 0)
	{
		EntityPicInfo * EntPic = GetEntPic(&m_EnData.begin()->second);

		if (EntPic != NULL && EntPic->State >= DATA_FINISH_LOADING)
		{
			if (EntPic->m_BlockingLst.size() > index)
				Copy = EntPic->m_BlockingLst[index];
		}
	}
	return Copy;
}

EntityPicInfo * CAnimatedEntity::GetEntPic(EntityData * pEnData)
{
	//return EnData.pEntPic;
	return CResourcesManager::GetInstance().LoadEntityResourcesFile(pEnData->FileName.c_str(), 
		gGlobal.pD3DDevice);
}

VOID CAnimatedEntity::ChangeColorP(String EntPartName, String ColorPName)
{
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.FileName == EntPartName)
		{
			Itr->second.pColorP = CResourcesManager::GetInstance().CreateColorPalette(ColorPName.c_str());
			return;
		}
	}
}

VOID CAnimatedEntity::WaitResReady()
{
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		while (TRUE)
		{
			EntityPicInfo * EntPic = CResourcesManager::GetInstance().LoadEntityResourcesFile(Itr->second.FileName.c_str(), 
				gGlobal.pD3DDevice);
			if (EntPic == NULL)
				break;
			else if (EntPic->State == DATA_LOADING)
				Sleep(1);
			else
				break;
		}
	}
}

EntityPicInfo * CAnimatedEntity::GetEntPic(UInt Index)
{
	if (Index >= m_EnData.size())
		return NULL;

	Int i = 0;
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (i == Index)
			return CResourcesManager::GetInstance().LoadEntityResourcesFile(Itr->second.FileName.c_str(), 
			gGlobal.pD3DDevice);
		i ++;
	}

	return NULL;

}

VOID CAnimatedEntity::SetCurrFrameIndex(Int Index)
{
	UInt32 CurrTime = ::timeGetTime();
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		Itr->second.PreTime = CurrTime;
		Itr->second.CurrFrIndex = Index;
		Itr->second.PreFrData.Delay = 100;
	}

	UpDate(CurrTime);

	m_Updated = TRUE;
}

BOOL CAnimatedEntity::IsMoving()
{
	if (m_CharPos != m_DesPos)
		return TRUE;

	if (m_StartTime > 0)
		return TRUE;

	for (StlMultiMap<UInt32, animationsetData>::iterator Itr = m_animationset.begin();
		Itr != m_animationset.end(); ++Itr)
	{
		if (Itr->second.TimeInMS > 0)
			return TRUE;
	}

	return FALSE;
}

BOOL CAnimatedEntity::GetCanBeSelected()
{
	return m_CanBeSelected;
}

VOID CAnimatedEntity::SetCanBeSelected(BOOL Value)
{
	m_CanBeSelected = Value;
}

VOID CAnimatedEntity::SetPlaySpeed(Real32 Value)
{
	m_PlaySpeed = Value;
}

VOID CAnimatedEntity::FreeEntityData()
{
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		CResourcesManager::GetInstance().ReleaseEntityData(Itr->second.FileName);
	}
	
	m_EnData.clear();
}

VOID CAnimatedEntity::AnimToMiddle()
{
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		Itr->second.CurrFrIndex = GetCurrAnimStartIndexByRotaion(&Itr->second, m_AnimationRotation, m_AnimationIndex) +
			GetCurrAnimationMaxFrame(&Itr->second) / 2;
	}
}

UInt32 CAnimatedEntity::LastAnimSetStartTime()
{
	UInt time = 0;
	for (StlMultiMap<UInt32, animationsetData>::iterator Itr = m_animationset.begin();
		Itr != m_animationset.end(); ++Itr)
	{
		time = Itr->first;
	}
	return time;
}
VOID CAnimatedEntity::AppendAnimationSet(Int AnimationIndex, Int UpX, Int UpY, Int height, Real64 TimeInMS,
	BOOL Loop, Int Rotation, UInt32 StartTime, UInt32 JumpDelay, BOOL StopAnim, 
	BOOL IsPixel,//sharking info
	Int ShakeX, Int ShakeY, Int ShakeSpeedX, Int ShakeSpeedY, UInt ShakeWidth, UInt ShakeHeight,UInt ShakeTime)
{
	animationsetData AData(AnimationIndex,  UpX,  UpY,  height,  TimeInMS,
	 Loop,  Rotation,  StartTime,  JumpDelay,  StopAnim, 
	 IsPixel,//sharking info
	 ShakeX,  ShakeY,  ShakeSpeedX,  ShakeSpeedY,  ShakeWidth,  ShakeHeight, ShakeTime);

	
	m_animationset.insert(std::make_pair(StartTime, AData));

}

VOID CAnimatedEntity::ToFrameEnd()
{
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.CurrFrIndex == ERROR_INDEX || Itr->second.AnimationIndex == ERROR_INDEX)
			continue;

		UInt count = (UInt)Itr->second.m_EntityHeader[Itr->second.AnimationIndex].size();

		if (Itr->second.CurrFrIndex < (Int)count)
		{
			Int OrgRot = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][Itr->second.CurrFrIndex].Rotation;

			while (Itr->second.CurrFrIndex + 1 < (Int)count)
			{
				Int Rot = Itr->second.m_EntityHeader[Itr->second.AnimationIndex][Itr->second.CurrFrIndex + 1].Rotation;
				if (Rot != OrgRot)
					break;
				Itr->second.CurrFrIndex ++;
			}

		}
		else
			Itr->second.CurrFrIndex = count;
	}
}

VOID CAnimatedEntity::InitEntPicInfo()
{
	m_EnData.clear(); 
	m_Updated = TRUE;
	m_EntOrgWidth = 0;
	m_EntOrgHeight = 0;
}

VOID CAnimatedEntity::LockResources()
{
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		EntityPicInfo * EntPic = CResourcesManager::GetInstance().LoadEntityResourcesFile(
			Itr->second.FileName.c_str(), gGlobal.pD3DDevice);
		if (EntPic)
		{
			EntPic->Lock = TRUE;
		}
	}
}

VOID CAnimatedEntity::UnLockResources()
{
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		EntityPicInfo * EntPic = CResourcesManager::GetInstance().LoadEntityResourcesFile(
				Itr->second.FileName.c_str(), gGlobal.pD3DDevice);
		if (EntPic)
		{
			EntPic->Lock = FALSE;
		}
	}
}

VOID CAnimatedEntity::InitChainData()
{
	m_tempAnimSet.Init();
}
VOID CAnimatedEntity::PushChainData()
{
	m_animationset.insert(std::make_pair(m_tempAnimSet.StartTime, m_tempAnimSet));
}
VOID CAnimatedEntity::SetAnimIndexChain(Int AnimationIndex, Int Rotation, BOOL Loop, BOOL StopAnim)
{
	m_tempAnimSet.AnimationIndex = AnimationIndex;
	m_tempAnimSet.Rotation = Rotation;
	m_tempAnimSet.Loop = Loop;
	m_tempAnimSet.StopAnim = StopAnim;
}
VOID CAnimatedEntity::SetSharkChain(Int ShakeX, Int ShakeY, Int ShakeSpeedX, Int ShakeSpeedY, 
									UInt ShakeWidth, UInt ShakeHeight,UInt ShakeTime)
{
	m_tempAnimSet.ShakeX = ShakeX;
	m_tempAnimSet.ShakeY = ShakeY;
	m_tempAnimSet.ShakeSpeedX = ShakeSpeedX;
	m_tempAnimSet.ShakeSpeedY = ShakeSpeedY;
	m_tempAnimSet.ShakeWidth = ShakeWidth;
	m_tempAnimSet.ShakeHeight = ShakeHeight;
	m_tempAnimSet.ShakeTime = ShakeTime;
}
VOID CAnimatedEntity::SetJumpDataChain(Int height, Int BkPosX, Int BkPosY, Real64 timeInMS, UInt32 StartDelay, BOOL IsPixel)
{
	m_tempAnimSet.height = height;
	m_tempAnimSet.UpX = BkPosX;
	m_tempAnimSet.UpY = BkPosY;
	m_tempAnimSet.TimeInMS = timeInMS;
	m_tempAnimSet.JumpDelay = StartDelay;
	m_tempAnimSet.IsPixel = IsPixel;
}
VOID CAnimatedEntity::SetChainStartTime(UInt32 Time)
{
	m_tempAnimSet.StartTime = ::timeGetTime() + Time;
}

BOOL CAnimatedEntity::IsAnimExist(Int AnimIndex)
{
	if (AnimIndex >= MAX_ANIM_TYPE)
		return FALSE;

	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.EquipType == EQUIP_HEAD ||
			Itr->second.EquipType == EQUIP_BODY ||
			Itr->second.EquipType == EQUIP_HAIR)
		{
			if (Itr->second.m_EntityHeader[AnimIndex].size() == 0)
				return FALSE;
		}
	}

	return TRUE;
}

String CAnimatedEntity::GetPartByEQType(Equip_Type EQUIPTYPE)
{
	String filename = _T("");
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); 
		Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.EquipType == EQUIPTYPE &&
			!Itr->second.Destory)
		{
			return  Itr->first;
		}
	}
	return filename;
}

ColorPaletteStruct* CAnimatedEntity::GetPartClourByEQType(Equip_Type EQUIPTYPE)
{
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); 
		Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.EquipType == EQUIPTYPE &&
			!Itr->second.Destory)
		{
			return  Itr->second.pColorP;
		}
	}
	return NULL;
}

void CAnimatedEntity::ChangePartClourByEQType(Equip_Type EQUIPTYPE,ColorPaletteStruct* clourP)
{
	for (StlMultiMap<String, EntityData>::iterator Itr = m_EnData.begin(); 
		Itr != m_EnData.end(); ++Itr)
	{
		if (Itr->second.EquipType == EQUIPTYPE &&
			!Itr->second.Destory)
		{
			Itr->second.pColorP = clourP;
		}
	}
}
