//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "Resources.h"
//-- Library
#include "Math.h"
#include "Configure.h"
#include "Global.h"


const int ColorPaletteSize = 768;
const int MaxColorPaletteBox = 256;

const UInt32 ExtractSize = 256 * 256 * 4;
//release the resources if no people use, 5 mins
CONST UInt32 ReleaseTime = 300000;
//CONST UInt32 ReleaseTime = 10000;

typedef unsigned (WINAPI *PBEGINTHREADEX_THREADFUNC)(LPVOID lpThreadParameter);
typedef unsigned *PBEGINTHREADEX_THREADID;

DWORD WINAPI ThreadFunc(LPVOID pData)
{
	CResourcesManager * pResMan = (CResourcesManager*)pData;

	while (TRUE)
	{
		BOOL Break = FALSE;
		EnterCriticalSection(&pResMan->ThreadLock);
		Break = pResMan->m_BreakThreadLoop;
		LeaveCriticalSection(&pResMan->ThreadLock);

		pResMan->ResProcess();

		if (Break)
			break;

		Sleep(10);
	}

	return (0);
}


//construct
CResourcesManager::CResourcesManager() 
{
	m_EntPicCacheTexture = SafeAllocate(Reuben::System::Byte, MaxEntSizeX * MaxEntSizeY * sizeof(Color32));
	m_ObjEntPicCacheTexture = SafeAllocate(Reuben::System::Byte, MaxEntSizeX * MaxEntSizeY * sizeof(Color32));
	m_EntExtractCache = NULL;//SafeAllocate(Byte, ExtractSize);
	//m_WorldImage = SafeAllocate(Byte, MaxEntSizeX * MaxEntSizeY * sizeof(Color32));
	InitializeCriticalSection(&ThreadLock);
	InitializeCriticalSection(&EntityFileReadThreadLock);
	InitializeCriticalSection(&MapFileReadDataThreadLock);
	InitializeCriticalSection(&ResetMapRequestThreadLock);
	InitializeCriticalSection(&MapFileReleaseThreadLock);
	InitializeCriticalSection(&EntityFileReleaseThreadLock);
	InitializeCriticalSection(&GarbageCollectorThreadLock);
	InitializeCriticalSection(&ReleaseQueueThreadLock);
	InitializeCriticalSection(&MapPartRequestThreadLock);
	m_BreakThreadLoop = FALSE;
	LoaderThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		(PBEGINTHREADEX_THREADFUNC)ThreadFunc,
		(LPVOID)this,
		0,
		(PBEGINTHREADEX_THREADID)&ThreadID);

	//LoaderThread = CreateThread(NULL,0, ThreadFunc, (LPVOID)(this), 0, &ThreadID);
}
//distruct
CResourcesManager::~CResourcesManager()
{
	StopThread();

	while (true)
	{
		DWORD ExitCode;
		if (GetExitCodeThread(LoaderThread, &ExitCode))
		{
			if (ExitCode != STILL_ACTIVE) 
			{
				CloseHandle(LoaderThread);
				break;
			}
		}
		Sleep(1);
	}

	DeleteCriticalSection(&ThreadLock);
	DeleteCriticalSection(&EntityFileReadThreadLock);
	DeleteCriticalSection(&MapFileReadDataThreadLock);
	DeleteCriticalSection(&MapFileReleaseThreadLock);
	DeleteCriticalSection(&ResetMapRequestThreadLock);
	DeleteCriticalSection(&EntityFileReleaseThreadLock);
	DeleteCriticalSection(&GarbageCollectorThreadLock);
	DeleteCriticalSection(&ReleaseQueueThreadLock);
	DeleteCriticalSection(&MapPartRequestThreadLock);

	StlMap<String, EntityPicInfo *>::iterator FileItr = AllLoadedEntityFile.begin();

	size_t i = 0;
	while(FileItr != AllLoadedEntityFile.end())
	{
		OnReleaseEntityResources(FileItr->second);
		FileItr++;
		i ++;
	}

	AllLoadedEntityFile.clear();

	StlMap<String, MapData *>::iterator MapFileItr = AllLoadedMapFile.begin();

	while(MapFileItr != AllLoadedMapFile.end())
	{
		OnReleaseMapResources(MapFileItr->second);
		MapFileItr++;
	}

	AllLoadedMapFile.clear();

	OnReleaseAllTexture();

	StlVector<EntityLoaderData *>::iterator Itr = m_EntityFileReadDataLst.begin();

	while(Itr != m_EntityFileReadDataLst.end())
	{
		SafeDeleteObject((*Itr));
		Itr ++;
	}

	m_EntityFileReadDataLst.clear();

	for (Int i = 0; i < (Int)m_GarbageCollector.size(); i++)
	{
		if (m_GarbageCollector[i].sizeOfData == 1)
			SafeDeleteObject(m_GarbageCollector[i].pData);
		else
			SafeDeallocate(m_GarbageCollector[i].pData);
	}

	m_GarbageCollector.clear();


	StlMap<String, ColorPaletteStruct>::iterator ColorItr = AllColorPalette.begin();
	while (ColorItr != AllColorPalette.end())
	{
		SafeDeallocate(ColorItr->second.ColorPaletteData);
		
		ColorItr++;
	}

	AllColorPalette.clear();

	for (Int i = 0; i < (Int)m_MapFileReadDataLst.size(); i++)
	{
		SafeDeleteObject(m_MapFileReadDataLst[i]);
	}

	for (Int i = 0; i < (Int)m_ResetMapRequestLst.size(); i++)
	{
		SafeDeleteObject(m_ResetMapRequestLst[i]);
	}

	for (Int i = 0; i < (Int)m_MapFileReleaseLst.size(); i++)
	{
		OnReleaseMapResources(m_MapFileReleaseLst[i]);
	}

	for (Int i = 0; i < (Int)m_EntityFileReleaseLst.size(); i++)
	{
		OnReleaseEntityResources(m_EntityFileReleaseLst[i]);
	}

	for (Int i = 0; i < (Int)m_MapPartRequestLst.size(); i++)
	{
		SafeDeleteObject(m_MapPartRequestLst[i]);
	}

	for (Int i = 0; i < (Int)m_ReleaseQueueLst.size(); i++)
		SafeRelease(m_ReleaseQueueLst[i]);

	SafeDeallocate(m_EntPicCacheTexture);
	SafeDeallocate(m_ObjEntPicCacheTexture);
	SafeDeallocate(m_EntExtractCache);
	//SafeDeallocate(m_WorldImage);
}

VOID CResourcesManager::OnResetMapResources(ResetMapRequestData * pResetMapRequestData)
{
	if (pResetMapRequestData == NULL)
		return;

	MapData * ResultData = pResetMapRequestData->pMapData;

	if (ResultData->State != DATA_FINISH_LOADING)
		return;

	Int MaxXPic = (Int)ceil((Real32)(ResultData->Header.dwMapWidth) / ResultData->Header.wSubMapWidth);
	Int MaxYPic = (Int)ceil((Real32)(ResultData->Header.dwMapHeight) / ResultData->Header.wSubMapHeight);

	RECT EnlargeEffectArea = pResetMapRequestData->EffectArea;

	Int MinXIndex = (EnlargeEffectArea.left) / ResultData->Header.wSubMapWidth;// - 1;
	if (MinXIndex < 0)
		MinXIndex = 0;

	Int MinYIndex = (EnlargeEffectArea.top)/ ResultData->Header.wSubMapHeight;// - 1;
	if (MinYIndex < 0)
		MinYIndex = 0;

	Int MaxXIndex = (EnlargeEffectArea.right)/ ResultData->Header.wSubMapWidth;// + 1;
	if (MaxXIndex > MaxXPic)
		MaxXIndex = MaxXPic;

	Int MaxYIndex = (EnlargeEffectArea.bottom)/ ResultData->Header.wSubMapHeight;// + 1;
	if (MaxYIndex > MaxYPic)
		MaxYIndex = MaxYPic;

	Reuben::System::Size TotalSize = ResultData->Header.wSubMapWidth * ResultData->Header.wSubMapHeight * sizeof(Color32);
	Byte * pDataPre = SafeAllocate(Byte, TotalSize);
	Byte * pExtractedData = SafeAllocate(Byte, TotalSize);

	ULONGLONG SizeOfSubPic = 0;

	Int count = 0;
	Int countX = 0;
	Int countY = 0;
	Int ReleaseCount = 0;

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Begin reset map total pic:%d total sub pic: %d total night sub: %d"), ResultData->TotalPic,
		ResultData->totalSubFrame, ResultData->totalNightFrame );
	Int PreY = -1;
	for (Int i = 0; i < ResultData->TotalPic; i++)
	{
		Int IndexX = i % (MaxXPic);
		Int IndexY = i / (MaxXPic); 
		BOOL Inside = IndexX >= MinXIndex && IndexX <= MaxXIndex && 
			IndexY >= MinYIndex && IndexY <= MaxYIndex;

		if (Inside)
		{
			count ++;
		}

		for (Int j = 0; j < ResultData->totalSubFrame; j++)
		{
			Int BgLstIndex = i * ResultData->totalSubFrame + j;

			if (BgLstIndex >= ResultData->totalSubFrame * ResultData->TotalPic)
				break;

			//TRACE_LOG(gConfigure.logFile, LOG_LV_ERROR, _T("Cant connect to proxy\n"));
			//TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Reset map BgLstIndex:%d "),BgLstIndex);

			if (Inside)
			{
				if (ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData == NULL)
				{
					Byte ImageType = 0;
					Byte * pFileData = ResultData->m_pTextureBGLst[BgLstIndex].pSrcData;
					memcpy(&ImageType, pFileData, sizeof(Byte));
					pFileData += sizeof(Byte);
					if (ImageType == IPic || ImageType == DiffPic)
					{
						ULONGLONG iFileSize = 0;

						memcpy(&iFileSize, pFileData, sizeof(ULONGLONG));
						pFileData += sizeof(ULONGLONG);

						Byte * pData = pFileData;
						pFileData += iFileSize;
						if (ImageType == DiffPic)
						{
							ZLib::uLongf TempSize = (ZLib::uLongf)SizeOfSubPic;
							ZLib::uncompress(pExtractedData, &TempSize, pData, (ZLib::uLongf)iFileSize);
							SizeOfSubPic = TempSize;
							//ReadRLEFile(pExtractedData, (UInt32)SizeOfSubPic, pData, (UInt32)iFileSize, 1);

							for (Int i = 0; i < SizeOfSubPic; i++)
							{
								pExtractedData[i] +=  pDataPre[i];
							}

							pData = pExtractedData;
						}
						else
						{
							ZLib::uLongf TempSize = (ZLib::uLongf)TotalSize;
							ZLib::uncompress(pExtractedData, &TempSize, pData, (ZLib::uLongf)iFileSize);
							SizeOfSubPic = TempSize;
							pData = pExtractedData;
							//SizeOfSubPic = iFileSize;
						}

						memcpy(pDataPre, pData, (size_t)SizeOfSubPic);



						Byte * pTempMapData = NULL;

						if (gConfigure.uCompressedMap)
						{
							pTempMapData = SafeAllocate(BYTE, (Reuben::System::Size)SizeOfSubPic);
							memcpy(pTempMapData, pData, (Reuben::System::Size)SizeOfSubPic);
						}
						else
						{
							pTempMapData = SafeAllocate(Byte, 
								ResultData->Header.wSubMapWidth * ResultData->Header.wSubMapHeight * sizeof(Color32));

							//divide into 4 parts
							RECT CopyRect;
							SetRect(&CopyRect, 0, 0, ResultData->Header.wSubMapWidth, ResultData->Header.wSubMapHeight);
							//SetRect(&CopyRect,33, 33, 199, 199);
							CRender::DXT1To32Bits(pData, pTempMapData, CopyRect, 
								ResultData->Header.wSubMapWidth);
						}

						ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData = pTempMapData;
						//create texture 
						//ResultData->m_pTextureBGLst[BgLstIndex].BgImage = 
						//	CreateTexture(pData, pResetMapRequestData->pD3DDevice, (UInt32)SizeOfSubPic, D3DX_DEFAULT, 
						//	D3DX_DEFAULT, D3DPOOL_SCRATCH, 0, 0, D3DFMT_R5G6B5) ;

						//ResultData->m_pTextureBGLst[BgLstIndex].BgImage = 
						//	CreateTexture(pData, pResetMapRequestData->pD3DDevice, (UInt32)SizeOfSubPic, D3DX_DEFAULT, 
						//	D3DX_DEFAULT, D3DPOOL_SCRATCH, 0, 0, D3DFMT_A8R8G8B8) ;
					}
					else if (ImageType == PPic)
					{
						Int reference = 0;
						//pMapLoaderData->file->Read((Byte*)(&reference), sizeof(Int));
						memcpy(&reference, pFileData, sizeof(Int));
						pFileData += sizeof(Int);
						ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData = 
							ResultData->m_pTextureBGLst[BgLstIndex - (j - reference)].pDDSPicData;
					}

				}
			}
			else
			{
				if (ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData != NULL)
				{
					ReleaseCount ++;
					//ReleaseTexture(ResultData->m_pTextureBGLst[BgLstIndex].BgImage);
					Byte * pData = ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData;
					ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData = NULL;
					if (*ResultData->m_pTextureBGLst[BgLstIndex].pSrcData != PPic)
						SafeDeallocate(pData);
					
				}
			}
		}


		for (Int k = 0; k < ResultData->totalNightFrame; k ++)
		{
			Int BgLstIndex = i * ResultData->totalNightFrame + k;

			if (BgLstIndex >=  ResultData->totalNightFrame * ResultData->TotalPic)
				break;

			//TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Reset night map BgLstIndex:%d "),BgLstIndex);

			if (Inside)
			{
				if (ResultData->m_pTextureNightLst[BgLstIndex].pDDSPicData == NULL)
				{
					ULONGLONG iFileSize = 0;

					Byte * pFileData = ResultData->m_pTextureNightLst[BgLstIndex].pSrcData;

					memcpy(&iFileSize, pFileData, sizeof(ULONGLONG));
					pFileData += sizeof(ULONGLONG);

					//create texture 
					ZLib::uLongf TempSize = (ZLib::uLongf)TotalSize;
					ZLib::uncompress(pExtractedData, &TempSize, pFileData, (ZLib::uLongf)iFileSize);
					SizeOfSubPic = TempSize;

					//ResultData->m_pTextureNightLst[BgLstIndex].pDDSPicData = SafeAllocate(Byte, (Reuben::System::Size)SizeOfSubPic);
					//memcpy(ResultData->m_pTextureNightLst[BgLstIndex].pDDSPicData, pExtractedData, (Reuben::System::Size)SizeOfSubPic);
					Byte * pTempMapData = NULL;
					if (gConfigure.uCompressedMap)
					{
						pTempMapData = SafeAllocate(BYTE, (Reuben::System::Size)SizeOfSubPic);
						memcpy(pTempMapData, pExtractedData, (Reuben::System::Size)SizeOfSubPic);
					}
					else
					{
						pTempMapData = SafeAllocate(Byte, 
							ResultData->Header.wSubMapWidth * ResultData->Header.wSubMapHeight * sizeof(Color32));

						//divide into 4 parts
						RECT CopyRect;
						SetRect(&CopyRect, 0, 0, ResultData->Header.wSubMapWidth, ResultData->Header.wSubMapHeight);
						//SetRect(&CopyRect,33, 33, 199, 199);
						CRender::DXT3To32Bits(pExtractedData, pTempMapData, CopyRect, 
							ResultData->Header.wSubMapWidth);
					}

					ResultData->m_pTextureNightLst[BgLstIndex].pDDSPicData = pTempMapData;
					
					
				}
			}
			else
			{
				if (ResultData->m_pTextureNightLst[BgLstIndex].pDDSPicData != NULL)
				{
					//ReleaseTexture(ResultData->m_pTextureNightLst[BgLstIndex].BgImage);
					Byte * pData = ResultData->m_pTextureNightLst[BgLstIndex].pDDSPicData;
					ResultData->m_pTextureNightLst[BgLstIndex].pDDSPicData = NULL;
					SafeDeallocate(pData);
					
				}
			}
		}
	}

	//return;

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Total PicInside %d MapX Inside %d MapY Inside %d"), count, MaxXIndex - MinXIndex + 1, MaxYIndex - MinYIndex + 1);
	RECT MaskRect;
	MaskRect.left = MinXIndex * ResultData->Header.wSubMapWidth;
	MaskRect.right = (MaxXIndex  + 1) * ResultData->Header.wSubMapWidth;
	MaskRect.top = MinYIndex * ResultData->Header.wSubMapHeight;
	MaskRect.bottom = (MaxYIndex + 1) * ResultData->Header.wSubMapHeight;

	RECT ResultRect;

	for (Int i = 0; i < (Int)ResultData->AlphaMaskLst.size(); i++)
	{
		if (QGLMath::CalculateOverlappedRect(ResultData->AlphaMaskLst[i].Bound , MaskRect, ResultRect))
		{
			if (ResultData->AlphaMaskLst[i].pData == NULL)
			{
				Byte * pFileData = ResultData->AlphaMaskLst[i].pSrcData;
				ULONGLONG BoundSize  = (ResultData->AlphaMaskLst[i].Bound.bottom - ResultData->AlphaMaskLst[i].Bound.top + 1)
					* (ResultData->AlphaMaskLst[i].Bound.right - ResultData->AlphaMaskLst[i].Bound.left + 1);
				UInt32 RLESize = 0;
				memcpy(&RLESize, pFileData, sizeof(UInt32));
				pFileData += sizeof(UInt32);

				Byte * pData = SafeAllocate(Byte, (Reuben::System::Size)BoundSize);
				//ReadRLEFile(pData, (UInt32)BoundSize, pFileData, RLESize, 1);
				ZLib::uLongf TempSize = (ZLib::uLongf)BoundSize;
				ZLib::uncompress(pData, &TempSize, pFileData, RLESize);
				ResultData->AlphaMaskLst[i].pData = pData;
			}
		}
		else
		{
			if (ResultData->AlphaMaskLst[i].pData != NULL)
			{
				Byte * pData = ResultData->AlphaMaskLst[i].pData;
				ResultData->AlphaMaskLst[i].pData = NULL;
				SafeDeallocate(pData);
			}
		}
	}

	for (Int i = 0; i < (Int)ResultData->ShadowMaskLst.size(); i++)
	{
		if (QGLMath::CalculateOverlappedRect(ResultData->ShadowMaskLst[i].Bound , MaskRect, ResultRect))
		{
			if (ResultData->ShadowMaskLst[i].pData == NULL)
			{
				Byte * pFileData = ResultData->ShadowMaskLst[i].pSrcData;
				ULONGLONG BoundSize  = (ResultData->ShadowMaskLst[i].Bound.bottom - ResultData->ShadowMaskLst[i].Bound.top + 1)
					* (ResultData->ShadowMaskLst[i].Bound.right - ResultData->ShadowMaskLst[i].Bound.left + 1);
				UInt32 RLESize = 0;

				memcpy(&RLESize, pFileData, sizeof(UInt32));
				pFileData += sizeof(UInt32);

				Byte * pData = SafeAllocate(Byte, (Reuben::System::Size)BoundSize);
				//ReadRLEFile(pData, (UInt32)BoundSize, pFileData, RLESize, 1);
				ZLib::uLongf TempSize = (ZLib::uLongf)BoundSize;
				ZLib::uncompress(pData, &TempSize, pFileData, RLESize);
				ResultData->ShadowMaskLst[i].pData = pData;
			}
		}
		else
		{
			if (ResultData->ShadowMaskLst[i].pData != NULL)
			{
				Byte * pData = ResultData->ShadowMaskLst[i].pData;
				ResultData->ShadowMaskLst[i].pData = NULL;
				SafeDeallocate(pData);

			}
		}
	}


	SafeDeallocate(pDataPre);
	SafeDeallocate(pExtractedData);
}
VOID CResourcesManager::OnLoadEntityFile(EntityLoaderData * pEntityLoaderData)
{
	UInt16 noOfTexture = 0;
	UInt16 CurrnoOfTexture = 1;


	{
		if (pEntityLoaderData->EntData->PicType == Color256 ||
			pEntityLoaderData->EntData->PicType == Z_Color256)
		{
			//pEntityLoaderData->EntData->ColorPalette = SafeAllocate(Byte, ColorPaletteSize);
			//pEntityLoaderData->file->Read((Byte*)(pEntityLoaderData->EntData->ColorPalette), ColorPaletteSize);
			pEntityLoaderData->EntData->ColorPalette = pEntityLoaderData->EntData->pInFileTemp;
			pEntityLoaderData->EntData->pInFileTemp += ColorPaletteSize;

		}
		UInt32 TotalSize = 0;
		//pEntityLoaderData->file->Read((Byte*)(&TotalSize), sizeof(UInt32));
		TotalSize = *(UInt32 *)pEntityLoaderData->EntData->pInFileTemp;
		pEntityLoaderData->EntData->pInFileTemp += sizeof(UInt32);
		if (gConfigure.uFileDataCopy)
		{
			pEntityLoaderData->EntData->m_PictureData = SafeAllocate(Byte, TotalSize);
			//pEntityLoaderData->file->Read((Byte*)pEntityLoaderData->EntData->m_PictureData, TotalSize);
			memcpy(pEntityLoaderData->EntData->m_PictureData, pEntityLoaderData->EntData->pInFileTemp, TotalSize);
			pEntityLoaderData->EntData->pInFileTemp += TotalSize;
		}
		else
		{
			pEntityLoaderData->EntData->m_PictureData = pEntityLoaderData->EntData->pInFileTemp;
			pEntityLoaderData->EntData->pInFileTemp += TotalSize;
		}
	}

	UInt16 noOfBkLst = 0;
	UInt16 CurrnoOfBkLst = 1;

	
	{
		//pEntityLoaderData->file->Read((Byte*)&noOfBkLst, sizeof(UInt16));
		noOfBkLst = *(UInt16 *)pEntityLoaderData->EntData->pInFileTemp;
		pEntityLoaderData->EntData->pInFileTemp += sizeof(UInt16);

		while (CurrnoOfBkLst <= noOfBkLst)
		{
			StlVector<BlockPos> BkPosLst;
			UInt16 noOfBk = 0;
			UInt16 CurrnoOfBk = 1;
			//pEntityLoaderData->file->Read((Byte*)&noOfBk, sizeof(UInt16));
			noOfBk = *(UInt16 *)pEntityLoaderData->EntData->pInFileTemp;
			pEntityLoaderData->EntData->pInFileTemp += sizeof(UInt16);

			while (CurrnoOfBk <= noOfBk)
			{
				BlockPos pos;
				//pEntityLoaderData->file->Read((Byte*)&pos, sizeof(BlockPos));
				pos = *(BlockPos *)pEntityLoaderData->EntData->pInFileTemp;
				pEntityLoaderData->EntData->pInFileTemp += sizeof(BlockPos);	

				BkPosLst.push_back(pos);
				CurrnoOfBk++;
			}
			pEntityLoaderData->EntData->m_BlockingLst.push_back(BkPosLst);
			CurrnoOfBkLst++;
		}
	}


	pEntityLoaderData->EntData->PreInUseTime = ::timeGetTime();

	//check whether it is too large, need to decompress
	/*if (pEntityLoaderData->EntData->MaxSize > ExtractSize)
	{
	pEntityLoaderData->EntData->State = DATA_DECOMPRESSING;

	for (Int i = 0; i < MAX_ANIM_TYPE; i++)
	{
	for (Int j = 0; j < (Int) pEntityLoaderData->EntData->m_EntityHeader[i].size(); j++)
	{
	AnimationFrameData FData = pEntityLoaderData->EntData->m_EntityHeader[i][j];
	Byte * pData = SafeAllocate(Byte, FData.ImagePicPos.OrgSize);

	Byte * pTData = pEntityLoaderData->EntData->m_PictureData + FData.ImagePicPos.PicStartPos;
	DWORD Len = FData.ImagePicPos.OrgSize;
	ZLib::uncompress(pData, &Len, 
	pTData, FData.ImagePicPos.PicEndPos - FData.ImagePicPos.PicStartPos); 
	pEntityLoaderData->EntData->m_UncompressedDataLst.insert(std::make_pair(FData.ImagePicPos.PicStartPos, pData));

	if (FData.ShadowPicPos.OrgSize > 0)
	{
	Byte * pData = SafeAllocate(Byte, FData.ShadowPicPos.OrgSize);
	pTData = pEntityLoaderData->EntData->m_PictureData + FData.ShadowPicPos.PicStartPos;
	DWORD Len = FData.ShadowPicPos.OrgSize;
	ZLib::uncompress(pData, &Len, 
	pTData, FData.ShadowPicPos.PicEndPos - FData.ShadowPicPos.PicStartPos); 
	pEntityLoaderData->EntData->m_UncompressedDataLst.insert(std::make_pair(FData.ShadowPicPos.PicStartPos, pData));
	}
	}
	}
	//pEntityLoaderData->EntData->m_UncompressedDataLst.begin();

	if (g_GdfResource)
	SafeDeallocate(pEntityLoaderData->EntData->m_PictureData);
	pEntityLoaderData->EntData->m_PictureData = NULL;
	pEntityLoaderData->EntData->State = DATA_DECOMPRESSED;
	}
	else*/
	pEntityLoaderData->EntData->State = DATA_FINISH_LOADING;
}

template<class T> VOID CResourcesManager::AddToGarbageCollector(T * pData, size_t sizeOfData)
{\
GarbageData GData;
GData.pData = pData;
GData.sizeOfData = sizeOfData;
m_GarbageCollector.push_back(GData);
}

BOOL CResourcesManager::FreeGarbageRes(Byte * pData)
{
	StlVector<GarbageData>::iterator Itr = m_GarbageCollector.begin();
	while (Itr != m_GarbageCollector.end())
	{
		if ((*Itr).pData == pData)
		{
			if ((*Itr).sizeOfData == 1)
				SafeDeleteObject(pData);
			else
				SafeDeallocate(pData);
			m_GarbageCollector.erase(Itr);
			return TRUE;
		}
		Itr++;
	}
	return FALSE;
}

VOID CResourcesManager::CheckResources()
{
	UInt32 CurrTime = ::timeGetTime();

	//check entity resources
	StlVector<StlMap<String, EntityPicInfo *>::iterator > EntRmLst;

	StlMap<String, EntityPicInfo *>::iterator EntDataItr =  AllLoadedEntityFile.begin();

	size_t index = 0;
	for (StlMap<String, EntityPicInfo *>::iterator EntDataItr =  AllLoadedEntityFile.begin() ;
		EntDataItr != AllLoadedEntityFile.end() && index < AllLoadedEntityFile.size()
		;EntDataItr ++, index ++)
	{
		if (EntDataItr->second &&
			EntDataItr->second->State >= DATA_FINISH_LOADING &&
			!EntDataItr->second->Lock &&
			CurrTime >= ReleaseTime + EntDataItr->second->PreInUseTime)
		{
			EntRmLst.push_back(EntDataItr);
		}
	}


	StlVector<StlMap<String, EntityPicInfo *>::iterator >::iterator EntRmLstItr = EntRmLst.begin();

	while (EntRmLstItr != EntRmLst.end())
	{
		EntityPicInfo * pEntData = (*EntRmLstItr)->second;
		AllLoadedEntityFile.erase(*EntRmLstItr);
		EnterCriticalSection(&EntityFileReleaseThreadLock);
		m_EntityFileReleaseLst.push_back(pEntData);
		LeaveCriticalSection(&EntityFileReleaseThreadLock);
		EntRmLstItr++;
	}

	EntRmLst.clear();

	//check map resources
	StlVector<StlMap<String, MapData *>::iterator > MapRmLst;

	index = 0;
	for (StlMap<String, MapData *>::iterator MapDataItr =  AllLoadedMapFile.begin() ;
		MapDataItr != AllLoadedMapFile.end() && index < AllLoadedMapFile.size()
		;MapDataItr ++, index ++)
	{
		if (MapDataItr->second->State >= DATA_FINISH_LOADING && 
			!MapDataItr->second->Lock &&
			CurrTime  >= ReleaseTime + MapDataItr->second->PreInUseTime)
		{
			MapRmLst.push_back(MapDataItr);
		}
	}



	StlVector<StlMap<String, MapData *>::iterator >::iterator MapRmLstItr = MapRmLst.begin();

	while (MapRmLstItr != MapRmLst.end())
	{
		MapData * pMapData = (*MapRmLstItr)->second;
		AllLoadedMapFile.erase(*MapRmLstItr);
		EnterCriticalSection(&MapFileReleaseThreadLock);
		m_MapFileReleaseLst.push_back(pMapData);
		LeaveCriticalSection(&MapFileReleaseThreadLock);

		//find whether there is reset map or map part reques in other thread
		EnterCriticalSection(&ResetMapRequestThreadLock);
		for (Int i = 0; i < (Int)m_ResetMapRequestLst.size(); i++)
		{
			if (m_ResetMapRequestLst[i]->pMapData == pMapData)
				m_ResetMapRequestLst[i]->pMapData = NULL;
		}
		LeaveCriticalSection(&ResetMapRequestThreadLock);

		EnterCriticalSection(&MapPartRequestThreadLock);
		for (Int i = 0; i < (Int)m_MapPartRequestLst.size(); i++)
		{
			if (m_MapPartRequestLst[i]->pMapData == pMapData)
				m_MapPartRequestLst[i]->pMapData = NULL;
		}
		LeaveCriticalSection(&MapPartRequestThreadLock);

		MapRmLstItr++;
	}

	MapRmLst.clear();

	//check color Palette
	/*StlVector<StlMap<String, ColorPaletteStruct>::iterator > ColorPaletteRmLst;

	index = 0;
	for (StlMap<String, ColorPaletteStruct>::iterator ColorPaletteDataItr =  AllColorPalette.begin() ;
	ColorPaletteDataItr != AllColorPalette.end() && index < AllColorPalette.size()
	;ColorPaletteDataItr ++, index ++)
	{
	if (CurrTime - ColorPaletteDataItr->second.PreInUseTime >= ReleaseTime)
	{
	ColorPaletteRmLst.push_back(ColorPaletteDataItr);
	SafeDeallocate(ColorPaletteDataItr->second.ColorPaletteData);
	}
	}

	StlVector<StlMap<String, ColorPaletteStruct>::iterator >::iterator ColorPaletteRmLstItr = ColorPaletteRmLst.begin();

	while (ColorPaletteRmLstItr != ColorPaletteRmLst.end())
	{
	AllColorPalette.erase(*ColorPaletteRmLstItr);
	ColorPaletteRmLstItr++;
	}

	ColorPaletteRmLst.clear();
	*/


}

VOID CResourcesManager::OnLoadMapFile(MapLoaderData * pMapLoaderData)
{
	DWORD ReaderByte = 0;
	DWORD FileSizeHigh = 0;

	{
		DWORD FileSize = GetFileSize(pMapLoaderData->pMapData->hFile, &FileSizeHigh);

		MapData * ResultData = pMapLoaderData->pMapData;

		if (gConfigure.uFileDataCopy)
		{
			Byte * pFileData = ResultData->FileReadedData;
			ResultData->FileReadedData = SafeAllocate(Byte, (Reuben::System::Size)FileSize);
			memcpy(ResultData->FileReadedData, pFileData, FileSize);
		}
		Byte * pFileData = ResultData->FileReadedData;

		ULONGLONG SizeOfSubPic = 0;

		Reuben::System::Size TotalSize = ResultData->Header.wSubMapWidth * ResultData->Header.wSubMapHeight * sizeof(Color32);
		Byte * pDataPre = SafeAllocate(Byte, TotalSize);
		Byte * pExtractedData = SafeAllocate(Byte, TotalSize);
		BYTE * pRLEData = NULL;

		Int MaxXPic = (Int)ceil((Real32)(ResultData->Header.dwMapWidth) / ResultData->Header.wSubMapWidth);
		Int MaxYPic = (Int)ceil((Real32)(ResultData->Header.dwMapHeight) / ResultData->Header.wSubMapHeight);

		RECT EnlargeEffectArea = pMapLoaderData->EffectArea;


		Int MinXIndex = (EnlargeEffectArea.left) / ResultData->Header.wSubMapWidth;// - 1;
		if (MinXIndex < 0)
			MinXIndex = 0;

		Int MinYIndex = (EnlargeEffectArea.top)/ ResultData->Header.wSubMapHeight;// - 1;
		if (MinYIndex < 0)
			MinYIndex = 0;

		Int MaxXIndex = (EnlargeEffectArea.right)/ ResultData->Header.wSubMapWidth;// + 1;
		if (MaxXIndex > MaxXPic)
			MaxXIndex = MaxXPic;

		Int MaxYIndex = (EnlargeEffectArea.bottom)/ ResultData->Header.wSubMapHeight;// + 1;
		if (MaxYIndex > MaxYPic)
			MaxYIndex = MaxYPic;


		int count = 0;

		//load the first bg first
		for (Int i = 0; i < ResultData->TotalPic; i++)
		{
			Int IndexX = i % (MaxXPic);
			Int IndexY = i / (MaxXPic);
			BOOL Inside = IndexX >= MinXIndex && IndexX <= MaxXIndex && 
				IndexY >= MinYIndex && IndexY <= MaxYIndex;

			if (Inside)
				count ++;

			for (Int j = 0; j < ResultData->totalSubFrame; j++)
			{
				Int BgLstIndex = i * ResultData->totalSubFrame + j;

				Byte ImageType = 0;

				ResultData->m_pTextureBGLst[BgLstIndex].pSrcData = pFileData;
				memcpy(&ImageType, pFileData, sizeof(Byte));
				pFileData += sizeof(Byte);		

				if (ImageType == IPic || ImageType == DiffPic)
				{
					ULONGLONG iFileSize = 0;

					memcpy(&iFileSize, pFileData, sizeof(ULONGLONG));
					pFileData += sizeof(ULONGLONG);

					pRLEData = pFileData;
					pFileData += iFileSize;
					if (j != 0)
						continue;
					if (Inside)
					{
						Byte * pData = pRLEData;
						if (ImageType == DiffPic)
						{
							//ReadRLEFile(pExtractedData, (UInt32)SizeOfSubPic, pRLEData, (UInt32)iFileSize, 1);
							ZLib::uLongf TempSize = (ZLib::uLongf)TotalSize;
							ZLib::uncompress(pExtractedData, &TempSize, pRLEData, (ZLib::uLongf)iFileSize);
							SizeOfSubPic = TempSize;

							for (Int i = 0; i < SizeOfSubPic; i++)
							{
								pExtractedData[i] +=  pDataPre[i];
							}
							pData = pExtractedData;
						}
						else
						{
							ZLib::uLongf TempSize = (ZLib::uLongf)TotalSize;
							ZLib::uncompress(pExtractedData, &TempSize, pRLEData, (ZLib::uLongf)iFileSize);
							SizeOfSubPic = TempSize;
							pData = pExtractedData;
							//SizeOfSubPic = iFileSize;
						}
						//create texture 


						//ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData = SafeAllocate(Byte, (Reuben::System::Size)SizeOfSubPic);
						//memcpy(ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData, pData, (Reuben::System::Size)SizeOfSubPic);
						Byte * pTempMapData = NULL;
						if (gConfigure.uCompressedMap)
						{
							pTempMapData = SafeAllocate(BYTE, (Reuben::System::Size)SizeOfSubPic);
							memcpy(pTempMapData, pExtractedData, (Reuben::System::Size)SizeOfSubPic);
						}
						else
						{
							pTempMapData = SafeAllocate(Byte, 
								ResultData->Header.wSubMapWidth * ResultData->Header.wSubMapHeight * sizeof(Color32));

							//divide into 4 parts
							RECT CopyRect;
							SetRect(&CopyRect, 0, 0, ResultData->Header.wSubMapWidth, ResultData->Header.wSubMapHeight);
							//SetRect(&CopyRect,33, 33, 199, 199);
							CRender::DXT1To32Bits(pExtractedData, pTempMapData, CopyRect, 
								ResultData->Header.wSubMapWidth);
						}


						ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData = pTempMapData;
						
					}
				}
				else if (ImageType == PPic)
				{
					Int reference = 0;

					memcpy(&reference, pFileData, sizeof(Int));
					pFileData += sizeof(Int);
					if (j != 0)
						continue;
					if (Inside)
						ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData = 
						ResultData->m_pTextureBGLst[BgLstIndex - (j - reference)].pDDSPicData;
				}
			}

			for (Int k = 0; k < ResultData->totalNightFrame; k ++)
			{
				Int BgLstIndex = i * ResultData->totalNightFrame + k;

				ULONGLONG iFileSize = 0;

				ResultData->m_pTextureNightLst[BgLstIndex].pSrcData = pFileData;
				memcpy(&iFileSize, pFileData, sizeof(ULONGLONG));
				pFileData += sizeof(ULONGLONG);

				//create texture 
				if (Inside && ResultData->m_pTextureNightLst[BgLstIndex].pDDSPicData == NULL)
				{
					ZLib::uLongf TempSize = (ZLib::uLongf)TotalSize;
					ZLib::uncompress(pExtractedData, &TempSize, pFileData, (ZLib::uLongf)iFileSize);
					SizeOfSubPic = TempSize;

					Byte * pTempMapData = NULL;
					if (gConfigure.uCompressedMap)
					{
						pTempMapData = SafeAllocate(BYTE, (Reuben::System::Size)SizeOfSubPic);
						memcpy(pTempMapData, pExtractedData, (Reuben::System::Size)SizeOfSubPic);
					}
					else
					{
						pTempMapData = SafeAllocate(Byte, 
							ResultData->Header.wSubMapWidth * ResultData->Header.wSubMapHeight * sizeof(Color32));

						//divide into 4 parts
						RECT CopyRect;
						SetRect(&CopyRect, 0, 0, ResultData->Header.wSubMapWidth, ResultData->Header.wSubMapHeight);
						//SetRect(&CopyRect,33, 33, 199, 199);
						CRender::DXT3To32Bits(pExtractedData, pTempMapData, CopyRect, 
							ResultData->Header.wSubMapWidth);
					}


					ResultData->m_pTextureNightLst[BgLstIndex].pDDSPicData = pTempMapData;

				}

				pFileData += iFileSize;

				//SafeDeallocate(pData);
			}
		}
		ResultData->State = DATA_FINISH_LOADING_FIRST_BGMAP;

		//then load other frame
		for (Int i = 0; i < ResultData->TotalPic; i++)
		{
			Int IndexX = i % (MaxXPic);
			Int IndexY = i / (MaxXPic);
			BOOL Inside = IndexX >= MinXIndex && IndexX <= MaxXIndex && 
				IndexY >= MinYIndex && IndexY <= MaxYIndex;

			if (Inside)
			{
				//count ++;
				for (Int j = 0; j < ResultData->totalSubFrame; j++)
				{
					Int BgLstIndex = i * ResultData->totalSubFrame + j;

					Byte ImageType = 0;

					Byte * pFileData = ResultData->m_pTextureBGLst[BgLstIndex].pSrcData;
					memcpy(&ImageType, pFileData, sizeof(Byte));
					pFileData += sizeof(Byte);		

					if (ImageType == IPic || ImageType == DiffPic)
					{
						ULONGLONG iFileSize = 0;

						memcpy(&iFileSize, pFileData, sizeof(ULONGLONG));
						pFileData += sizeof(ULONGLONG);

						pRLEData = pFileData;
						pFileData += iFileSize;
						if (Inside)
						{
							Byte * pData = pRLEData;
							if (ImageType == DiffPic)
							{
								//ReadRLEFile(pExtractedData, (UInt32)SizeOfSubPic, pRLEData, (UInt32)iFileSize, 1);
								ZLib::uLongf TempSize = (ZLib::uLongf)TotalSize;
								ZLib::uncompress(pExtractedData, &TempSize, pRLEData, (ZLib::uLongf)iFileSize);
								SizeOfSubPic = TempSize;

								for (Int i = 0; i < SizeOfSubPic; i++)
								{
									pExtractedData[i] +=  pDataPre[i];
								}
								pData = pExtractedData;
							}
							else
							{
								ZLib::uLongf TempSize = (ZLib::uLongf)TotalSize;
								ZLib::uncompress(pExtractedData, &TempSize, pRLEData, (ZLib::uLongf)iFileSize);
								SizeOfSubPic = TempSize;
								pData = pExtractedData;
								//SizeOfSubPic = iFileSize;
							}
							//create texture 
							memcpy(pDataPre, pData, (size_t)SizeOfSubPic);
							if (ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData == NULL)
							{
								//ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData = SafeAllocate(Byte, (Reuben::System::Size)SizeOfSubPic);
								//memcpy(ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData, pData, (Reuben::System::Size)SizeOfSubPic);
								Byte * pTempMapData = NULL;
								if (gConfigure.uCompressedMap)
								{
									pTempMapData = SafeAllocate(BYTE, (Reuben::System::Size)SizeOfSubPic);
									memcpy(pTempMapData, pExtractedData, (Reuben::System::Size)SizeOfSubPic);
								}
								else
								{
									pTempMapData = SafeAllocate(Byte, 
										ResultData->Header.wSubMapWidth * ResultData->Header.wSubMapHeight * sizeof(Color32));

									//divide into 4 parts
									RECT CopyRect;
									SetRect(&CopyRect, 0, 0, ResultData->Header.wSubMapWidth, ResultData->Header.wSubMapHeight);
									//SetRect(&CopyRect,33, 33, 199, 199);
									CRender::DXT1To32Bits(pExtractedData, pTempMapData, CopyRect, 
										ResultData->Header.wSubMapWidth);
								}

								ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData = pTempMapData;
							}
							//ResultData->m_pTextureBGLst[BgLstIndex].BgImage = 
							//CreateTexture(pData, pMapLoaderData->pD3DDevice, (UInt32)SizeOfSubPic, D3DX_DEFAULT, 
							//D3DX_DEFAULT, D3DPOOL_SCRATCH, 0, 0, D3DFMT_R5G6B5) ;
							//ResultData->m_pTextureBGLst[BgLstIndex].BgImage = 
							//CrateTexture(pData, pMapLoaderData->pD3DDevice, (UInt32)SizeOfSubPic, D3DX_DEFAULT, 
							//D3DX_DEFAULT, D3DPOOL_SCRATCH, 0, 0, D3DFMT_A8R8G8B8) ;
						}
					}
					else if (ImageType == PPic)
					{
						Int reference = 0;

						memcpy(&reference, pFileData, sizeof(Int));
						pFileData += sizeof(Int);
						if (Inside)
							ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData = 
							ResultData->m_pTextureBGLst[BgLstIndex - (j - reference)].pDDSPicData;
					}
				}
			}
		}




		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("on loadmap Total PicInside %d MapX Inside %d MapY Inside %d"), count, MaxXIndex - MinXIndex + 1, MaxYIndex - MinYIndex + 1);

		for (Int i = 0; i < ResultData->totalSubFrame; i ++)
		{
			UInt32 NoOfRect;
			NoOfRect = *(UInt32 *)pFileData;
			pFileData += sizeof(UInt32);
			StlVector<RECT> temp;
			for (Int j = 0; j < (Int)NoOfRect; j ++)
			{
				RECT Rect = *(RECT *)pFileData;
				temp.push_back(Rect);
				pFileData += sizeof(RECT);
			}
			ResultData->dirtyRectLst.push_back(temp);
		}

		ResultData->State = DATA_FINISH_LOADING_BGMAP;

		ULONGLONG NumOfBounding = 0;
		//pMapLoaderData->file->Read((Byte*)(&NumOfBounding), sizeof(ULONGLONG));
		memcpy(&NumOfBounding, pFileData, sizeof(ULONGLONG));
		pFileData += sizeof(ULONGLONG);

		RECT ResultRect;

		for (Int i = 0; i < NumOfBounding; i++)
		{
			MaskEffect Mask;
			//pMapLoaderData->file->Read((Byte*)(&Mask.Bound), sizeof(Bounding));
			memcpy(&Mask.Bound, pFileData, sizeof(RECT));
			pFileData += sizeof(RECT);

			Mask.pSrcData = pFileData;

			BOOL Overlapped = QGLMath::CalculateOverlappedRect(EnlargeEffectArea, Mask.Bound, ResultRect);
			ULONGLONG BoundSize  = (Mask.Bound.bottom - Mask.Bound.top + 1) * (Mask.Bound.right - Mask.Bound.left + 1);
			UInt32 RLESize = 0;
			//pMapLoaderData->file->Read((Byte*)&RLESize, sizeof(UInt32));
			memcpy(&RLESize, pFileData, sizeof(UInt32));
			pFileData += sizeof(UInt32);
			//pMapLoaderData->file->Read(pRLEData, (std::streamsize)RLESize);
			//if (Overlapped)
			//memcpy(pRLEData, pFileData, (std::streamsize)(RLESize));

			if (Overlapped)
			{
				Mask.pData = SafeAllocate(Byte, (Reuben::System::Size)BoundSize);
				//ReadRLEFile(Mask.pData, (UInt32)BoundSize, pFileData, RLESize, 1);
				ZLib::uLongf TempSize = (ZLib::uLongf)BoundSize;
				ZLib::uncompress(Mask.pData, &TempSize, pFileData, RLESize);
				BoundSize = TempSize;
			}
			else
				Mask.pData = NULL;
			pFileData += RLESize;
			ResultData->AlphaMaskLst.push_back(Mask);
		}

		//load shadow info
		NumOfBounding = 0;
		//pMapLoaderData->file->Read((Byte*)(&NumOfBounding), sizeof(ULONGLONG));
		memcpy(&NumOfBounding, pFileData, sizeof(ULONGLONG));
		pFileData += sizeof(ULONGLONG);

		for (Int i = 0; i < NumOfBounding; i++)
		{
			MaskEffect Mask;
			//pMapLoaderData->file->Read((Byte*)(&Mask.Bound), sizeof(Bounding));
			memcpy(&Mask.Bound, pFileData, sizeof(RECT));
			pFileData += sizeof(RECT);

			Mask.pSrcData = pFileData;
			BOOL Overlapped = QGLMath::CalculateOverlappedRect(EnlargeEffectArea, Mask.Bound, ResultRect);

			ULONGLONG BoundSize  = (Mask.Bound.bottom - Mask.Bound.top + 1) * (Mask.Bound.right - Mask.Bound.left + 1);
			UInt32 RLESize = 0;
			//pMapLoaderData->file->Read((Byte*)&RLESize, sizeof(UInt32));
			memcpy(&RLESize, pFileData, sizeof(UInt32));
			pFileData += sizeof(UInt32);
			//pMapLoaderData->file->Read(pRLEData, (std::streamsize)RLESize);

			if (Overlapped)
			{
				Mask.pData = SafeAllocate(Byte, (Reuben::System::Size)BoundSize);
				//ReadRLEFile(Mask.pData, (UInt32)BoundSize, pFileData, RLESize, 1);
				ZLib::uLongf TempSize = (ZLib::uLongf)BoundSize;
				ZLib::uncompress(Mask.pData, &TempSize, pFileData, RLESize);
				BoundSize = TempSize;
			}
			else
				Mask.pData = NULL;

			pFileData += RLESize;

			ResultData->ShadowMaskLst.push_back(Mask);
		}

		//load block info
		UInt32 TotalBlock = 0;
		//pMapLoaderData->file->Read((Byte*)(&TotalBlock), sizeof(UInt32));
		memcpy(&TotalBlock, pFileData, sizeof(UInt32));
		pFileData += sizeof(UInt32);

		for (Int i = 0; i < (Int)TotalBlock; i++)
		{
			BlockType Type;
			//pMapLoaderData->file->Read((Byte*)&Type, sizeof(BlockType));
			memcpy(&Type, pFileData, sizeof(BlockType));
			pFileData += sizeof(BlockType);
			ResultData->BlockPosDataLst.push_back(Type);
		}

		//alpha Index
		ULONGLONG NumOfIndex = 0;

		StlVector<Byte *>  BkEmptyIndexLst;
		for (Int i = 0; i < (Int)TotalBlock; i++)
		{
			Byte * pData = NULL;
			BkEmptyIndexLst.push_back(pData);
		}
		//BkEmptyIndexLst.resize(TotalBlock);

		for (int j = 0; j < ResultData->totalSubFrame; j++)
		{
			StlVector< Byte * >  BkAlphaIndexLst = BkEmptyIndexLst;

			for (Int k = 0; k < (Int)TotalBlock; k++)
			{
				Byte NumOfAlphaIndex = 0;
				BkAlphaIndexLst[k] = pFileData;
				memcpy(&NumOfAlphaIndex, pFileData, sizeof(Byte));
				pFileData += sizeof(Byte);
				pFileData += sizeof(UInt16) * NumOfAlphaIndex;
			}
			ResultData->BkAlphaIndexLstVec.push_back(BkAlphaIndexLst);
		}

		//shadow Index
		for (int j = 0; j < ResultData->totalSubFrame; j++)
		{
			StlVector< Byte * >  BkShadowIndexLst = BkEmptyIndexLst;

			for (Int k = 0; k < (Int)TotalBlock; k++)
			{
				BkShadowIndexLst[k] = pFileData;
				Byte NumOfShadowIndex = 0;
				memcpy(&NumOfShadowIndex, pFileData, sizeof(Byte));
				pFileData += sizeof(Byte);
				pFileData += sizeof(UInt16) * NumOfShadowIndex;
			}

			ResultData->BkShadowIndexLstVec.push_back(BkShadowIndexLst);
		}

		ResultData->PreInUseTime = ::timeGetTime();
		ResultData->State = DATA_FINISH_LOADING;

		SafeDeallocate(pDataPre);
		SafeDeallocate(pExtractedData);
	}

}

VOID CResourcesManager::ReadRLEFile(Byte* PicInfoBase, UInt32 iFilesize, Byte* RLEPic, UInt32 RLEFileSize,  
									UInt32 PixelSize)
{
	UInt32 Count = 0;
	UInt32 BaseCount = 0;
	while(Count < RLEFileSize && BaseCount < iFilesize)
	{
		BYTE relID = 0;
		relID = RLEPic[Count];
		//memcpy(&relID, &RLEPic[Count], sizeof(BYTE));
		Count += sizeof(BYTE);

		if (relID > 127)
		{
			relID -= 127;
			/*Int Value = 0;
			memcpy(&Value, &RLEPic[Count], PixelSize);
			Value = Value << 16;
			memcpy(&Value, &RLEPic[Count], PixelSize);
			memset(&PicInfoBase[BaseCount], Value, PixelSize * relID );
			*/
			for (Int i= 0; i < relID; i++)
			{

				memcpy(&PicInfoBase[BaseCount] , &RLEPic[Count], PixelSize);
				BaseCount += PixelSize;
			}

			//BaseCount += PixelSize * relID;
			Count += PixelSize;
		}
		else
		{
			/*for (Int i = 0; i < relID; i++)
			{
			memcpy(&PicInfoBase[BaseCount] , &RLEPic[Count], PixelSize);
			BaseCount += PixelSize;
			Count += PixelSize;
			}
			*/

			memcpy(&PicInfoBase[BaseCount] , &RLEPic[Count], PixelSize * relID);
			BaseCount += PixelSize * relID;
			Count += PixelSize * relID;
		}
	}
}

//loading the entity or StlMap file
EntityPicInfo * CResourcesManager::LoadEntityResourcesFile(LPCTSTR filename, LPDIRECT3DDEVICE9	pD3DDevice)
{
	//check whether the data was created before

	//return NULL;

	String FileNameString;
	FileNameString = filename;

	if (FileNameString.IsEmpty())
		return NULL;

	StlMap<String, EntityPicInfo *>::iterator FileItr = AllLoadedEntityFile.find(FileNameString);
	if (FileItr != AllLoadedEntityFile.end())
	{
		FileItr->second->PreInUseTime = ::timeGetTime();
		return FileItr->second;
	}

	String FullFilePath;
	FullFilePath = gConfigure.strWorkingDir + FileNameString;

	//otherwise load the data
	EntityPicInfo * ResultData = SafeCreateObject(EntityPicInfo);
	if( g_GdfResource || g_GdfResourceEx )
	{
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(FullFilePath, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(FullFilePath, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;

			ResultData->pInFile = fbuf;
			ResultData->pInFileTemp = buf;
			ResultData->PreInUseTime = ::timeGetTime();
			ResultData->State = DATA_LOADING;


		}
	}
	else
	{
		if (!Reuben::Platform::File::IsExists(FullFilePath.c_str()))
		{
			SafeDeleteObject(ResultData);
			return NULL;
		}

		//Reuben::Platform::File * file = SafeCreateObject(Reuben::Platform::File);
		try
		{
			//load the working directory
			//file->Open(FullFilePath.c_str(), Reuben::Platform::File::MODE_READ_EXISTING);
			//if (file->IsOpened())
			HANDLE hFile = CreateFile(FullFilePath.c_str(), FILE_READ_DATA, FILE_SHARE_READ, 
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

			String fileNameStr = filename;
			String::size_type pos = fileNameStr.find_last_of(_T("\\"));

			if (pos != String::npos)
			{
				fileNameStr = fileNameStr.substr(pos + 1);
			}

			HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 
				0, 0, fileNameStr.c_str());

			Byte * pInFile = (Byte *)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);

			if (hFile && hMapFile && pInFile)
			{
				ResultData->hFile = hFile;
				ResultData->hMapFile = hMapFile;
				ResultData->pInFile = pInFile;
				ResultData->pInFileTemp = pInFile;
				ResultData->PreInUseTime = ::timeGetTime();
				ResultData->State = DATA_LOADING;
			}
			//else
			//	SafeDeleteObject(file);
		}
		catch(...)
		{
			//SafeDeleteObject(file);
			SafeDeleteObject(ResultData);
			return NULL;
		}
	}
	if (ResultData->pInFile)
	{
		WORD Version = 0;

		Version = *(WORD *)ResultData->pInFileTemp;
		ResultData->pInFileTemp += sizeof(WORD);

		if (Version != ENTITY_VERSION_NUMBER)
		{
			if (ResultData->pInFile)
				UnmapViewOfFile(ResultData->pInFile);
			SafeDeleteObject(ResultData);
			return NULL;
		}

		EntityLoaderData * LoadData = SafeCreateObject(EntityLoaderData);
		LoadData->EntData = ResultData;
		LoadData->pD3DDevice = pD3DDevice;

		//read the size of the entity first
		ResultData->SrcWidth = *(UInt16 *)ResultData->pInFileTemp;
		ResultData->pInFileTemp += sizeof(UInt16);
		ResultData->SrcHeight = *(UInt16 *)ResultData->pInFileTemp;
		ResultData->pInFileTemp += sizeof(UInt16);

		//read the header first
		UInt16 noOfFrame = 0;
		UInt16 CurrFrame = 1;
		noOfFrame = *(UInt16 *)ResultData->pInFileTemp;
		ResultData->pInFileTemp += sizeof(UInt16);

		while (CurrFrame <= noOfFrame)
		{
			Int StateType = 0;
			StateType = *(Int *)ResultData->pInFileTemp;
			ResultData->pInFileTemp += sizeof(Int);

			UInt16 AniFrCount = 0;
			UInt16 CurrAniFrCount = 1;

			AniFrCount = *(UInt16 *)ResultData->pInFileTemp;
			ResultData->pInFileTemp += sizeof(UInt16);

			//	AniFrCount * sizeof(AnimationFrameData));
			while(CurrAniFrCount <= AniFrCount)
			{
				AnimationFrameData FData;
				FData = *(AnimationFrameData *)ResultData->pInFileTemp;
				ResultData->pInFileTemp += sizeof(AnimationFrameData);

				if (FData.ImagePicPos.OrgSize >= LoadData->EntData->MaxSize)
					LoadData->EntData->MaxSize = FData.ImagePicPos.OrgSize;
				LoadData->EntData->m_EntityHeader[StateType].push_back(FData);
				CurrAniFrCount++;
			}


			CurrFrame++;
		}

		LoadData->EntData->PicType = *(Byte *)ResultData->pInFileTemp;
		ResultData->pInFileTemp += sizeof(Byte);

		/*EnterCriticalSection(&EntityFileReadThreadLock);
		m_EntityFileReadDataLst.push_back(LoadData);
		LeaveCriticalSection(&EntityFileReadThreadLock);
		*/
		OnLoadEntityFile(LoadData);
		SafeDeleteObject(LoadData);

		AllLoadedEntityFile.insert(std::make_pair(FileNameString, ResultData));

		return ResultData;
	}
	else
		SafeDeleteObject(ResultData);
	return NULL;
}

VOID CResourcesManager::OnReleaseMapResources(MapData * pMapData)
{
	//release the data
	for (Int i = 0; i < (Int)pMapData->AlphaMaskLst.size(); i++)
		SafeDeallocate(pMapData->AlphaMaskLst[i].pData);

	for (Int i = 0; i < (Int)pMapData->ShadowMaskLst.size(); i++)
		SafeDeallocate(pMapData->ShadowMaskLst[i].pData);

	pMapData->BkAlphaIndexLstVec.clear();
	pMapData->BkShadowIndexLstVec.clear();


	if (!m_BreakThreadLoop)
	{
		for (Int i = 0; i < pMapData->totalSubFrame * pMapData->TotalPic; i ++)
		{
			//ReleaseTexture(pMapData->m_pTextureBGLst[i].BgImage);
			if (*pMapData->m_pTextureBGLst[i].pSrcData != PPic)
				SafeDeallocate(pMapData->m_pTextureBGLst[i].pDDSPicData);
		}

		for (Int i = 0; i < pMapData->totalNightFrame * pMapData->TotalPic; i ++)
		{
			//if (*pMapData->m_pTextureNightLst[i].pSrcData != PPic)
			SafeDeallocate(pMapData->m_pTextureNightLst[i].pDDSPicData);
			//ReleaseTexture(pMapData->m_pTextureNightLst[i].BgImage);
		}
	}

	if (gConfigure.uFileDataCopy)
		SafeDeallocate(pMapData->FileReadedData);
	//	if (pMapData->FileReadedData &&  g_GdfResource)
	//		SafeDeallocate(pMapData->FileReadedData);

	if (!UnmapViewOfFile(pMapData->pInFile))
	{
		Int i = 0;
		i ++;
	}
	CloseHandle(pMapData->hMapFile);
	CloseHandle(pMapData->hFile);

	SafeDeallocate(pMapData->m_pTextureBGLst);
	SafeDeallocate(pMapData->m_pTextureNightLst);
	SafeDeleteObject(pMapData);
}
VOID CResourcesManager::ReleaseMapResources(MapData * pMapData)
{
	if (pMapData == NULL || pMapData->State < DATA_FINISH_LOADING)
		return;

	StlMap<String, MapData *>::iterator MapFileItr = AllLoadedMapFile.find(pMapData->FileName);

	if (MapFileItr != AllLoadedMapFile.end())
	{
		AllLoadedMapFile.erase(MapFileItr);

		EnterCriticalSection(&MapFileReleaseThreadLock);
		m_MapFileReleaseLst.push_back(pMapData);
		LeaveCriticalSection(&MapFileReleaseThreadLock);

		//find whether there is reset map or map part reques in other thread
		EnterCriticalSection(&ResetMapRequestThreadLock);
		for (Int i = 0; i < (Int)m_ResetMapRequestLst.size(); i++)
		{
			if (m_ResetMapRequestLst[i]->pMapData == pMapData)
				m_ResetMapRequestLst[i]->pMapData = NULL;
		}
		LeaveCriticalSection(&ResetMapRequestThreadLock);

		EnterCriticalSection(&MapPartRequestThreadLock);
		for (Int i = 0; i < (Int)m_MapPartRequestLst.size(); i++)
		{
			if (m_MapPartRequestLst[i]->pMapData == pMapData)
				m_MapPartRequestLst[i]->pMapData = NULL;
		}
		LeaveCriticalSection(&MapPartRequestThreadLock);

	}
}

MapData * CResourcesManager::LoadMapResourcesFile(LPCTSTR filename, LPDIRECT3DDEVICE9	pD3DDevice, RECT EffectiveArea)
{
	//return NULL;
	//
	//check whether the data was created before
	String FileNameString;
	FileNameString = filename;

	if (FileNameString.IsEmpty())
		return NULL;

	StlMap<String, MapData *>::iterator FileItr = AllLoadedMapFile.find(FileNameString);
	if (FileItr != AllLoadedMapFile.end())
	{
		//reset the map since player might jump map
		ResetMapResourcesRequest(FileItr->second, EffectiveArea, pD3DDevice);
		return FileItr->second;
	}

	String FullFilePath;
	FullFilePath = gConfigure.strWorkingDir + FileNameString;

	MapData * ResultData = SafeCreateObject(MapData);
	if( g_GdfResource || g_GdfResourceEx )
	{
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(FullFilePath, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(FullFilePath, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			Byte * pInFile = (Byte *)fbuf;

			
			ResultData->hFile = NULL;
			ResultData->hMapFile = NULL;
			ResultData->pInFile = pInFile;
			ResultData->FileReadedData = buf;
			ResultData->FileName = FileNameString;
		}
		
	}
	else
	{
		if (!Reuben::Platform::File::IsExists(FullFilePath.c_str()))
		{
			SafeDeleteObject(ResultData);
			return NULL;
		}
		//otherwise load the data
		try
		{

			//file->Open(FullFilePath.c_str(), Reuben::Platform::File::MODE_SHARE_READ | Reuben::Platform::File::MODE_OPEN_EXISTING);
			HANDLE hFile = CreateFile(FullFilePath.c_str(), FILE_READ_DATA, FILE_SHARE_READ, 
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
			HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 
				0, 0, filename);

			Byte * pInFile = (Byte *)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);

			if (hFile && hMapFile && pInFile)
			{
				ResultData->hFile = hFile;
				ResultData->hMapFile = hMapFile;
				ResultData->pInFile = pInFile;
				ResultData->FileReadedData = pInFile;
				ResultData->FileName = FileNameString;

			}
		}
		catch(...)
		{
			SafeDeleteObject(ResultData);
			return NULL;
		}
	}

	if (ResultData->pInFile)
	{
		MapLoaderData * pMapLoaderData = SafeCreateObject(MapLoaderData);

		pMapLoaderData->pD3DDevice = pD3DDevice;
		pMapLoaderData->pMapData = ResultData;
		pMapLoaderData->EffectArea = EffectiveArea;

		//read the header first
		DWORD ReaderByte = 0;

		ResultData->Header.dwBOM = *(UInt32 *)ResultData->FileReadedData;
		ResultData->FileReadedData += sizeof(UInt32);
		ResultData->Header.wVersion = *(UInt16 *)ResultData->FileReadedData;
		ResultData->FileReadedData += sizeof(UInt16);


		if ( ResultData->Header.wVersion != QGHEADERDATA_MAPGRAPH_VER )
		{
			UnmapViewOfFile(ResultData->pInFile);
			SafeDeleteObject(ResultData);
			SafeDeleteObject(pMapLoaderData);
			return NULL;
		}

		ResultData->Header.dwMapWidth = *(UInt32 *)ResultData->FileReadedData;
		ResultData->FileReadedData += sizeof(UInt32);
		ResultData->Header.dwMapHeight = *(UInt32 *)ResultData->FileReadedData;
		ResultData->FileReadedData += sizeof(UInt32);
		ResultData->Header.wSubMapWidth = *(UInt16 *)ResultData->FileReadedData;
		ResultData->FileReadedData += sizeof(UInt16);
		ResultData->Header.wSubMapHeight = *(UInt16 *)ResultData->FileReadedData;
		ResultData->FileReadedData += sizeof(UInt16);

		ResultData->MaxBlockX = (UInt16)ResultData->Header.dwMapWidth / (UInt16)BlockSizeX;
		ResultData->MaxBlockY = (UInt16)ResultData->Header.dwMapHeight / (UInt16)(BlockSizeY / 2);

		ResultData->Header.TotalDataNum = *(Int *)ResultData->FileReadedData;
		ResultData->FileReadedData += sizeof(Int);
		for(Int nNum = 0; nNum < ResultData->Header.TotalDataNum; nNum++)
		{
			QGMapHeaderDataType Data;
			Data.Type = *(Int *)ResultData->FileReadedData;
			ResultData->FileReadedData += sizeof(Int);
			Data.FilePos = *(UInt64 *)ResultData->FileReadedData;
			ResultData->FileReadedData += sizeof(UInt64);
			Data.DataSize = *(UInt64 *)ResultData->FileReadedData;
			ResultData->FileReadedData += sizeof(UInt64);
			ResultData->Header.DataLst.push_back(Data);
		}

		//read the picture number
		ResultData->TotalPic = *(UInt16 *)ResultData->FileReadedData;
		ResultData->FileReadedData += sizeof(UInt16);

		ResultData->totalSubFrame = *(Int *)ResultData->FileReadedData;
		ResultData->FileReadedData += sizeof(Int);

		ResultData->totalNightFrame = *(Int *)ResultData->FileReadedData;
		ResultData->FileReadedData += sizeof(Int);

		ResultData->m_pTextureBGLst = SafeAllocate(BgLst , ResultData->TotalPic * ResultData->totalSubFrame);
		ZeroMemory(ResultData->m_pTextureBGLst, ResultData->TotalPic * ResultData->totalSubFrame * sizeof(BgLst));

		ResultData->m_pTextureNightLst = SafeAllocate(BgLst , ResultData->TotalPic * ResultData->totalNightFrame);
		ZeroMemory(ResultData->m_pTextureNightLst, ResultData->TotalPic * ResultData->totalNightFrame * sizeof(BgLst));

		EnterCriticalSection(&MapFileReadDataThreadLock);
		m_MapFileReadDataLst.push_back(pMapLoaderData);
		LeaveCriticalSection(&MapFileReadDataThreadLock);

		AllLoadedMapFile.insert(std::make_pair(FileNameString, ResultData));

		return ResultData;
	}
	else
		SafeDeleteObject(ResultData);
	return NULL;
}

VOID CResourcesManager::OnLoadAllColorPalette()
{
	String FullFilePath;
	FullFilePath.Format(_T("Data\\ColorPalette.pak"));

	if( g_GdfResource || g_GdfResourceEx )
	{
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(FullFilePath, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(FullFilePath, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			ULONGLONG size = *(ULONGLONG *)buf;
			buf += sizeof(ULONGLONG);
			for (Int i = 0; i < size; i ++)
			{
				BYTE len = *buf;
				buf += 1;
				char tempName[256];
				ZeroMemory(tempName, 256);
				strncpy(tempName, (char *)buf, len);
				buf += len;
				ColorPaletteStruct Data;
				Data.ColorPaletteData = SafeAllocate(Byte, ColorPaletteSize);
				memcpy(Data.ColorPaletteData, buf, ColorPaletteSize);
				buf += ColorPaletteSize;
				AllColorPalette.insert(std::make_pair(String(tempName), Data));
			}
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		if (!Reuben::Platform::File::IsExists(FullFilePath.c_str()))
			return;

		HANDLE hFile = CreateFile(FullFilePath.c_str(), FILE_READ_DATA, FILE_SHARE_READ, 
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

		String fileNameStr = FullFilePath;
		String::size_type pos = fileNameStr.find_last_of(_T("\\"));

		if (pos != String::npos)
		{
			fileNameStr = fileNameStr.substr(pos + 1);
		}

		HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 
			0, 0, fileNameStr.c_str());

		Byte * pInFile = (Byte *)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);

		if (hFile && hMapFile && pInFile)
		{
			ULONGLONG size = *(ULONGLONG *)pInFile;
			pInFile += sizeof(ULONGLONG);
			for (Int i = 0; i < size; i ++)
			{
				BYTE len = *pInFile;
				pInFile += 1;
				char tempName[256];
				ZeroMemory(tempName, 256);
				strncpy(tempName, (char *)pInFile, len);
				pInFile += len;
				ColorPaletteStruct Data;
				Data.ColorPaletteData = SafeAllocate(Byte, ColorPaletteSize);
				memcpy(Data.ColorPaletteData, pInFile, ColorPaletteSize);
				pInFile += ColorPaletteSize;
				AllColorPalette.insert(std::make_pair(String(tempName), Data));
			}

			UnmapViewOfFile(pInFile);
			CloseHandle(hMapFile);
			CloseHandle(hFile);
		}
	}
}
ColorPaletteStruct * CResourcesManager::CreateColorPalette(LPCTSTR filename)
{
	String strTempName = filename;
	if (strTempName.IsEmpty())
		return NULL;

	StlMap<String, ColorPaletteStruct>::iterator Itr =  AllColorPalette.find(strTempName);
	if (Itr != AllColorPalette.end())
		return &Itr->second;

	String FullFilePath;
	FullFilePath = gConfigure.strWorkingDir + strTempName;


	LPDIRECT3DTEXTURE9 Temp256Texture = NULL;
	if( g_GdfResource || g_GdfResourceEx )
	{
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(FullFilePath, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(FullFilePath, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			D3DXCreateTextureFromFileInMemoryEx( gGlobal.pD3DDevice, buf, fileSize, 
				MaxColorPaletteBox, MaxColorPaletteBox,1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, 
				D3DX_FILTER_NONE, D3DX_DEFAULT, 
				0, NULL, NULL, &Temp256Texture );
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		if (!Reuben::Platform::File::IsExists(FullFilePath.c_str()))
			return NULL;
		D3DXCreateTextureFromFileEx( gGlobal.pD3DDevice, FullFilePath.c_str(), 

			MaxColorPaletteBox, MaxColorPaletteBox,1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, 
			D3DX_FILTER_NONE, D3DX_DEFAULT, 
			0, NULL, NULL, &Temp256Texture );
	}

	if (Temp256Texture)
	{
		Byte * ColorPalette = SafeAllocate(Byte, ColorPaletteSize);
		D3DLOCKED_RECT d3dlt;
		Temp256Texture->LockRect(0, &d3dlt, 0, 0);
		BYTE * pTData = (BYTE *)d3dlt.pBits;

		int SubLen = (int)sqrt((double)MaxColorPaletteBox);

		pTData += (SubLen/2 * MaxColorPaletteBox + SubLen/2) * sizeof(DWORD);

		for (int i = 0 ; i < MaxColorPaletteBox; i++)
		{
			int y = (i / SubLen) * SubLen;
			int x = (i % SubLen) * SubLen;

			DWORD SrcIndex = (y * MaxColorPaletteBox  + x) * sizeof(DWORD);
			DWORD DesIndex = i * 3;
			BYTE DesR = ColorPalette[DesIndex];
			BYTE DesG = ColorPalette[DesIndex + 1];
			BYTE DesB = ColorPalette[DesIndex + 2];
			BYTE SrcR = pTData[SrcIndex];
			BYTE SrcG = pTData[SrcIndex + 1];
			BYTE SrcB = pTData[SrcIndex + 2];

			ColorPalette[DesIndex] = pTData[SrcIndex];
			ColorPalette[DesIndex + 1] = pTData[SrcIndex + 1];
			ColorPalette[DesIndex + 2] = pTData[SrcIndex + 2];
		}

		Temp256Texture->UnlockRect(0);
		Temp256Texture->Release();
		ColorPaletteStruct Data;
		Data.ColorPaletteData = ColorPalette;
		Data.PreInUseTime = ::timeGetTime();
		AllColorPalette.insert(std::make_pair(strTempName, Data));
		Itr =  AllColorPalette.find(strTempName);
		if (Itr != AllColorPalette.end())
			return &Itr->second;
	}

	return NULL;
}

LPDIRECT3DTEXTURE9 CResourcesManager::CreateTexture(Byte * pData, LPDIRECT3DDEVICE9	pD3DDevice, UInt32 SizeOfPic, UINT Width,
													UINT Height, D3DPOOL  Pool, UINT MipLevels, DWORD Usage, D3DFORMAT Format)
{
	LPDIRECT3DTEXTURE9 pTexture;
	if (pData)
	{
		if (!FAILED(D3DXCreateTextureFromFileInMemoryEx( pD3DDevice, pData, 
			SizeOfPic,
			D3DX_DEFAULT, D3DX_DEFAULT, MipLevels, Usage, Format, Pool , 
			D3DX_FILTER_NONE, D3DX_DEFAULT, 
			0, NULL, NULL, &pTexture)))
		{
			m_CreatedTexture.insert(std::make_pair((UInt)pTexture, pTexture));
			return pTexture;
		}
	}
	else
	{
		if (!FAILED(D3DXCreateTexture(pD3DDevice, Width, Height, MipLevels, Usage ,
			Format, Pool, &pTexture)))
		{
			m_CreatedTexture.insert(std::make_pair((UInt)pTexture, pTexture));
			return pTexture;
		}
	}

	return NULL;
}
VOID CResourcesManager::ReleaseTexture(LPDIRECT3DTEXTURE9 GivenTexture)
{
	StlMap<UInt, LPDIRECT3DTEXTURE9 >::iterator Itr = m_CreatedTexture.find((UInt)GivenTexture);
	if (Itr != m_CreatedTexture.end())
	{
		EnterCriticalSection(&ReleaseQueueThreadLock);
		m_ReleaseQueueLst.push_back(GivenTexture);
		LeaveCriticalSection(&ReleaseQueueThreadLock);
		m_CreatedTexture.erase(Itr);
	}
}

VOID CResourcesManager::OnReleaseTexture(LPDIRECT3DTEXTURE9 GivenTexture)
{
	if (GivenTexture)
		GivenTexture->Release();
}
VOID CResourcesManager::OnReleaseAllTexture()
{
	StlMap<UInt, LPDIRECT3DTEXTURE9 >::iterator Itr = m_CreatedTexture.begin();

	while (Itr != m_CreatedTexture.end())
	{
		Itr->second->Release();
		Itr++;
	}

	m_CreatedTexture.clear();
}

VOID CResourcesManager::StopThread()
{
	EnterCriticalSection(&ThreadLock);
	m_BreakThreadLoop = TRUE;
	LeaveCriticalSection(&ThreadLock);
}

VOID CResourcesManager::ResetMapResourcesRequest(MapData * pMapData, RECT EffectArea, LPDIRECT3DDEVICE9	pD3DDevice)
{
	if (pMapData->State < DATA_FINISH_LOADING)
		return;

	ResetMapRequestData * pData = SafeCreateObject(ResetMapRequestData);
	pData->pMapData = pMapData;
	pData->EffectArea = EffectArea;
	pData->pD3DDevice = pD3DDevice;
	EnterCriticalSection(&ResetMapRequestThreadLock);
	m_ResetMapRequestLst.push_back(pData);
	LeaveCriticalSection(&ResetMapRequestThreadLock);
	pMapData->PreInUseTime = ::timeGetTime();

}

VOID CResourcesManager::ReleaseEntityData(String FileName)
{
	if (FileName.empty())
		return;

	StlMap<String, EntityPicInfo *>::iterator EntFileItr = AllLoadedEntityFile.find(FileName);

	if (EntFileItr != AllLoadedEntityFile.end())
	{
		if (EntFileItr->second->State >= DATA_FINISH_LOADING)
		{
			EntityPicInfo * pEntPic = EntFileItr->second;
			AllLoadedEntityFile.erase(EntFileItr);
			//free the data imm
			//OnReleaseEntityResources(pEntPic);
			EnterCriticalSection(&EntityFileReleaseThreadLock);
			m_EntityFileReleaseLst.push_back(pEntPic);
			LeaveCriticalSection(&EntityFileReleaseThreadLock);
		}
	}

}

VOID CResourcesManager::ReleaseEntityData(EntityPicInfo * pEntData)
{
	if (pEntData == NULL || pEntData->State < DATA_FINISH_LOADING)
		return;

	StlMap<String, EntityPicInfo *>::iterator EntFileItr = AllLoadedEntityFile.begin();

	while (EntFileItr != AllLoadedEntityFile.end())
	{
		if (EntFileItr->second == pEntData)
		{
			EnterCriticalSection(&EntityFileReleaseThreadLock);
			m_EntityFileReleaseLst.push_back(pEntData);
			LeaveCriticalSection(&EntityFileReleaseThreadLock);

			AllLoadedEntityFile.erase(EntFileItr);
			break;
		}
		EntFileItr ++;
	}

}
VOID CResourcesManager::OnReleaseEntityResources(EntityPicInfo * pEntData)
{
	if (pEntData == NULL)
		return;

	//	for (Int i = 0; i < (Int)pEntData->m_BuffTextureLst.size(); i++)
	//	{
	//		SafeDeallocate(pEntData->m_BuffTextureLst[i].pData);
	//	}

	if (gConfigure.uFileDataCopy)
		SafeDeallocate(pEntData->m_PictureData);
//	if (pEntData->m_PictureData && g_GdfResource)
//		SafeDeallocate(pEntData->m_PictureData);
//	if (pEntData->ColorPalette && g_GdfResource)
//		SafeDeallocate(pEntData->ColorPalette);
	for (Int i = 0; i < (Int)pEntData->m_BlockingLst.size(); i++)
	{
		pEntData->m_BlockingLst[i].clear();
	}

	if (pEntData->m_UncompressedDataLst.size() > 0)
	{
		for (StlMap<UInt32, BYTE *>::iterator Itr = pEntData->m_UncompressedDataLst.begin();
			Itr != pEntData->m_UncompressedDataLst.end(); Itr ++)
		{
			SafeDeallocate(Itr->second);
		}
	}
	pEntData->m_UncompressedDataLst.clear();
	for (Int i = 0; i < MAX_ANIM_TYPE; i++)
	{
		pEntData->m_EntityHeader[i].clear();
	}

	if (pEntData->pInFile)
		UnmapViewOfFile(pEntData->pInFile);
	if (pEntData->hMapFile)
		CloseHandle(pEntData->hMapFile);
	if (pEntData->hFile)
		CloseHandle(pEntData->hFile);

	pEntData->m_BlockingLst.clear();


	SafeDeleteObject(pEntData);
}


//only thread will call this function

VOID CResourcesManager::OnMapPartResources(MapPartRequestData *pMapPartRequestData)
{
	if (pMapPartRequestData == NULL)
		return;

	MapData * ResultData = pMapPartRequestData->pMapData;

	if (ResultData->State != DATA_FINISH_LOADING)
		return;

	Int RealIndex = pMapPartRequestData->Index / ResultData->totalSubFrame;

	ULONGLONG SizeOfSubPic = 0;

	UInt32 TotalSize = ResultData->Header.wSubMapWidth * ResultData->Header.wSubMapHeight * sizeof(Color32);
	Byte * pDataPre = SafeAllocate(Byte, TotalSize);
	Byte * pExtractedData = SafeAllocate(Byte, TotalSize);
	//load normal ng first
	for (Int i = 0; i < ResultData->totalSubFrame; i++)
	{
		Int BgLstIndex = RealIndex * ResultData->totalSubFrame + i;

		if (BgLstIndex >= ResultData->totalSubFrame * ResultData->TotalPic)
			break;

		if (ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData == NULL)
		{
			Byte ImageType = 0;
			Byte * pFileData = ResultData->m_pTextureBGLst[BgLstIndex].pSrcData;
			memcpy(&ImageType, pFileData, sizeof(Byte));
			pFileData += sizeof(Byte);
			if (ImageType == IPic || ImageType == DiffPic)
			{
				ULONGLONG iFileSize = 0;

				memcpy(&iFileSize, pFileData, sizeof(ULONGLONG));
				pFileData += sizeof(ULONGLONG);

				Byte * pData = pFileData;
				pFileData += iFileSize;
				if (ImageType == DiffPic)
				{

					ZLib::uLongf TempSize = (ZLib::uLongf)SizeOfSubPic;
					ZLib::uncompress(pExtractedData, &TempSize, pData, (ZLib::uLongf)iFileSize);
					SizeOfSubPic = TempSize;
					//ZeroMemory(pExtractedData, (size_t)SizeOfSubPic);
					//ReadRLEFile(pExtractedData, (UInt32)SizeOfSubPic, pData, (UInt32)iFileSize, 1);

					for (Int i = 0; i < SizeOfSubPic; i++)
					{
						pExtractedData[i] +=  pDataPre[i];
					}

					pData = pExtractedData;
				}
				else
				{
					ZLib::uLongf TempSize = (ZLib::uLongf)TotalSize;
					ZLib::uncompress(pExtractedData, &TempSize, pData, (ZLib::uLongf)iFileSize);
					SizeOfSubPic = TempSize;
					pData = pExtractedData;
					//SizeOfSubPic = iFileSize;
				}

				memcpy(pDataPre, pData, (size_t)SizeOfSubPic);

				//ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData = SafeAllocate(BYTE, (Reuben::System::Size)SizeOfSubPic);
				//memcpy(ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData, pData, (Reuben::System::Size)SizeOfSubPic);
				Byte * pTempMapData = NULL;
				if (gConfigure.uCompressedMap)
				{
					pTempMapData = SafeAllocate(BYTE, (Reuben::System::Size)SizeOfSubPic);
					memcpy(pTempMapData, pExtractedData, (Reuben::System::Size)SizeOfSubPic);
				}
				else
				{
					pTempMapData = SafeAllocate(Byte, 
						ResultData->Header.wSubMapWidth * ResultData->Header.wSubMapHeight * sizeof(Color32));

					//divide into 4 parts
					RECT CopyRect;
					SetRect(&CopyRect, 0, 0, ResultData->Header.wSubMapWidth, ResultData->Header.wSubMapHeight);
					//SetRect(&CopyRect,33, 33, 199, 199);
					CRender::DXT1To32Bits(pExtractedData, pTempMapData, CopyRect, 
						ResultData->Header.wSubMapWidth);
				}


				ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData = pTempMapData;
				
			}
			else if (ImageType == PPic)
			{
				Int reference = 0;
				//pMapLoaderData->file->Read((Byte*)(&reference), sizeof(Int));
				memcpy(&reference, pFileData, sizeof(Int));
				pFileData += sizeof(Int);
				ResultData->m_pTextureBGLst[BgLstIndex].pDDSPicData = 
					ResultData->m_pTextureBGLst[BgLstIndex - (i - reference)].pDDSPicData;
			}

		}
	}

	for (Int k = 0; k < ResultData->totalNightFrame; k ++)
	{
		Int BgLstIndex = RealIndex * ResultData->totalNightFrame + k;

		if (BgLstIndex >= ResultData->totalNightFrame * ResultData->TotalPic)
			break;

		if (ResultData->m_pTextureNightLst[BgLstIndex].pDDSPicData == NULL)
		{
			ULONGLONG iFileSize = 0;

			Byte * pFileData = ResultData->m_pTextureNightLst[BgLstIndex].pSrcData;

			memcpy(&iFileSize, pFileData, sizeof(ULONGLONG));
			pFileData += sizeof(ULONGLONG);

			//create texture 

			ZLib::uLongf TempSize = (ZLib::uLongf)TotalSize;
			ZLib::uncompress(pExtractedData, &TempSize, pFileData, (ZLib::uLongf)iFileSize);
			SizeOfSubPic = TempSize;
			
			Byte * pTempMapData = NULL;
			if (gConfigure.uCompressedMap)
			{
				pTempMapData = SafeAllocate(BYTE, (Reuben::System::Size)SizeOfSubPic);
				memcpy(pTempMapData, pExtractedData, (Reuben::System::Size)SizeOfSubPic);
			}
			else
			{
				pTempMapData = SafeAllocate(Byte, 
					ResultData->Header.wSubMapWidth * ResultData->Header.wSubMapHeight * sizeof(Color32));

				//divide into 4 parts
				RECT CopyRect;
				SetRect(&CopyRect, 0, 0, ResultData->Header.wSubMapWidth, ResultData->Header.wSubMapHeight);
				//SetRect(&CopyRect,33, 33, 199, 199);
				CRender::DXT3To32Bits(pExtractedData, pTempMapData, CopyRect, 
					ResultData->Header.wSubMapWidth);
			}


			ResultData->m_pTextureNightLst[BgLstIndex].pDDSPicData = pTempMapData;

			pFileData += iFileSize;
			//SafeDeallocate(pData);
		}
	}

	SafeDeallocate(pDataPre);
	SafeDeallocate(pExtractedData);
}

VOID CResourcesManager::MapPartRequest(MapData * pMapData, Index Index, LPDIRECT3DDEVICE9	pD3DDevice)
{
	if (pMapData == NULL)
		return;

	if (pMapData->State < DATA_FINISH_LOADING)
		return;

	MapPartRequestData * pData = SafeCreateObject(MapPartRequestData);
	pData->pMapData = pMapData;
	pData->Index = Index;
	pData->pD3DDevice = pD3DDevice;
	EnterCriticalSection(&MapPartRequestThreadLock);
	m_MapPartRequestLst.push_back(pData);
	LeaveCriticalSection(&MapPartRequestThreadLock);
	pMapData->PreInUseTime = ::timeGetTime();
}

VOID CResourcesManager::ResProcess()
{
	//load map data quene
	StlVector<MapLoaderData *> pMapLoaderDataLst;
	EnterCriticalSection(&MapFileReadDataThreadLock);
	pMapLoaderDataLst = m_MapFileReadDataLst;
	m_MapFileReadDataLst.clear();
	LeaveCriticalSection(&MapFileReadDataThreadLock);

	for (size_t i = 0; i < pMapLoaderDataLst.size(); i++)
	{
		OnLoadMapFile(pMapLoaderDataLst[i]);
		SafeDeleteObject(pMapLoaderDataLst[i]);
		//SafeDeleteObject(pMapLoaderDataLst[i]->file);
	}
	pMapLoaderDataLst.clear();

	//load entity data quene
	EntityLoaderData * pEntLoaderData = NULL;
	//StlVector<EntityLoaderData *> pEntityLoaderDataLst;

	EnterCriticalSection(&EntityFileReadThreadLock);
	if (m_EntityFileReadDataLst.size() > 0)
	{
		pEntLoaderData = m_EntityFileReadDataLst[0];
		m_EntityFileReadDataLst.erase(m_EntityFileReadDataLst.begin());
	}
	LeaveCriticalSection(&EntityFileReadThreadLock);

	//for (Int i = 0; i < (Int)pEntityLoaderDataLst.size(); i++) 
	if (pEntLoaderData)
	{
		//load the entity resources
		OnLoadEntityFile(pEntLoaderData);
		SafeDeleteObject(pEntLoaderData);
	}

	//pEntityLoaderDataLst.clear();

	//reset map resources quene
	//StlVector<ResetMapRequestData *> pResetMapDataLst;
	ResetMapRequestData * pResetMapData = NULL;
	EnterCriticalSection(&ResetMapRequestThreadLock);
	//pResetMapDataLst = m_ResetMapRequestLst;
	//m_ResetMapRequestLst.clear();
	if (m_ResetMapRequestLst.size() > 0)
	{
		pResetMapData = m_ResetMapRequestLst[0];
		m_ResetMapRequestLst.erase(m_ResetMapRequestLst.begin());
	}
	LeaveCriticalSection(&ResetMapRequestThreadLock);

	//for (size_t i = 0; i < pResetMapDataLst.size(); i++)
	{
		if (pResetMapData)
		{
			if (pResetMapData->pMapData)
				OnResetMapResources(pResetMapData);
			SafeDeleteObject(pResetMapData);
		}
	}

	//request only one part of the map
	//StlVector<MapPartRequestData *> pMapPartRequestLst;
	MapPartRequestData * pMapPartData = NULL;
	EnterCriticalSection(&MapPartRequestThreadLock);
	if (m_MapPartRequestLst.size() > 0)
	{
		pMapPartData = m_MapPartRequestLst[0];
		m_MapPartRequestLst.erase(m_MapPartRequestLst.begin());
	}
	LeaveCriticalSection(&MapPartRequestThreadLock);

	//for (size_t i = 0; i < pMapPartRequestLst.size(); i++)
	{
		if (pMapPartData)
		{
			if (pMapPartData->pMapData)
				OnMapPartResources(pMapPartData);
			SafeDeleteObject(pMapPartData);
		}
	}

	//pMapPartRequestLst.clear();

	//release map resources
	StlVector<MapData *> pMapDataLst;
	EnterCriticalSection(&MapFileReleaseThreadLock);
	pMapDataLst = m_MapFileReleaseLst;
	m_MapFileReleaseLst.clear();
	LeaveCriticalSection(&MapFileReleaseThreadLock);

	for (size_t i = 0; i < pMapDataLst.size(); i ++)
		OnReleaseMapResources(pMapDataLst[i]);

	pMapDataLst.clear();

	//release entity resources quene
	EntityPicInfo * pReleaseEnt = NULL;
	StlVector<EntityPicInfo *> pEntDataLst;
	EnterCriticalSection(&EntityFileReleaseThreadLock);
	//pEntDataLst = m_EntityFileReleaseLst;
	//m_EntityFileReleaseLst.clear();
	if (m_EntityFileReleaseLst.size() > 0)
	{
		pReleaseEnt = m_EntityFileReleaseLst[0];
		m_EntityFileReleaseLst.erase(m_EntityFileReleaseLst.begin());
	}
	LeaveCriticalSection(&EntityFileReleaseThreadLock);

	//for (size_t i = 0; i < pEntDataLst.size(); i++)
	if (pReleaseEnt)
		OnReleaseEntityResources(pReleaseEnt);

	pEntDataLst.clear();

	//release texture quene
	StlVector<LPDIRECT3DTEXTURE9>  pTextureLst;
	EnterCriticalSection(&ReleaseQueueThreadLock);
	pTextureLst = m_ReleaseQueueLst;
	m_ReleaseQueueLst.clear();
	LeaveCriticalSection(&ReleaseQueueThreadLock);

	for (size_t i = 0; i < pTextureLst.size(); i++)
		OnReleaseTexture(pTextureLst[i]);

	pTextureLst.clear();

}
