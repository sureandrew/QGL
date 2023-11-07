//-- Common
#include "Common.h"
//-- Self
#include "Resource/MapManager.h"
//-- Library
#include <Reuben/Platform/BuffReader.h>
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/NPCManager.h"
#include "Resource/ResNPC.h"
#include "Common/ExportVersion.h"

#define MAP_DATA_VER 1

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

CMapManager::CMapManager()
{
	m_npcmgr = NULL;
	m_mapint.clear();
	m_cliMapInfo.clear();
}

CMapManager::~CMapManager()
{
	Release();
}

VOID CMapManager::Release()
{
	for( MapMapInt::iterator itr = m_mapint.begin(); itr!=m_mapint.end(); ++itr )
		SafeDeleteObject(itr->second);
	m_mapint.clear();

	for( ClientMapInfoMap::iterator itr = m_cliMapInfo.begin(); itr!=m_cliMapInfo.end(); ++itr )
		SafeDeleteObject(itr->second);
	m_cliMapInfo.clear();
}

const MapDB* CMapManager::GetMap(UInt32 mapid)
{
	MapMapInt::iterator itr = m_mapint.find(mapid);
	if( itr!=m_mapint.end() )
		return itr->second;
	return NULL;
}

BOOL CMapManager::LoadDAT(const TCHAR *filename)
{
	File file;
	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			DefaultInfoHeader  header;
			file.Read((BYTE*)&header, sizeof(DefaultInfoHeader));

			TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, header.version!=EXPORT_DATA_SERVER_MAP_INFO_FORMAT_VERSION, _T("MapManager::LoadDAT from \"%s\", file version:%i, app. version:%i"), String(filename).c_str(), header.version, EXPORT_DATA_SERVER_MAP_INFO_FORMAT_VERSION);
			if( header.version!=EXPORT_DATA_SERVER_MAP_INFO_FORMAT_VERSION )	throw;

			UInt32 nummap = 0;
			file.Read((BYTE*)&nummap, sizeof(UInt32));

      		MapDB * map = NULL;
			BinaryServerMapInfo * info = NULL;
			for( UInt i = 0; i<nummap; i++ )
			{
				map = SafeCreateObject(MapDB);
				BinaryServerMapInfo * info = SafeCreateObject(BinaryServerMapInfo);
				file.Read( (BYTE*)info, sizeof(BinaryServerMapInfo) );
				map->m_basicinfo = info;
				m_mapint.insert(std::make_pair(map->m_basicinfo->m_mapid, map));
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMapManager::LoadDAT, open error, %s"), filename);
		return FALSE;
	}
	return TRUE;
}

BOOL CMapManager::LoadClientMapInfo(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{
	if( pData && pDataLen )		// patch mode
	{
		UInt32 pos = 0;
		DefaultInfoHeader header;
		memcpy(&header, pData, sizeof(DefaultInfoHeader));
		TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, header.version!=EXPORT_DATA_CLIENT_MAP_INFO_FORMAT_VERSION, _T("MapManager::LoadDAT from \"%s\", file version:%i, app. version:%i"), String(filename).c_str(), header.version, EXPORT_DATA_CLIENT_MAP_INFO_FORMAT_VERSION);
		if( header.version!=EXPORT_DATA_CLIENT_MAP_INFO_FORMAT_VERSION )	throw;
		pos += sizeof(DefaultInfoHeader);

		UInt32 nummap;
		memcpy(&nummap, pData + pos, sizeof(UInt32));
		pos += sizeof(UInt32);

		BinaryClientMapInfo * pInfo = NULL;
		for( UInt32 i = 0; i< nummap; ++i )
		{
			pInfo = SafeCreateObject(BinaryClientMapInfo);
			if ( pInfo )
			{
				memcpy(pInfo, pData + pos, sizeof(BinaryClientMapInfo) );
				pos += sizeof(BinaryClientMapInfo);
				m_cliMapInfo.insert(std::make_pair(pInfo->m_mapid, pInfo));
			}
		}
	}else		// file mode
	{
		File file;
		try
		{
			file.Open(filename, File::MODE_READ_EXISTING);
			if (file.IsOpened())
			{
				DefaultInfoHeader  header;
				file.Read((BYTE*)&header, sizeof(DefaultInfoHeader));

				TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, header.version!=EXPORT_DATA_CLIENT_MAP_INFO_FORMAT_VERSION, _T("MapManager::LoadDAT from \"%s\", file version:%i, app. version:%i"), String(filename).c_str(), header.version, EXPORT_DATA_CLIENT_MAP_INFO_FORMAT_VERSION);
				if( header.version!=EXPORT_DATA_CLIENT_MAP_INFO_FORMAT_VERSION )	throw;

				UInt32 nummap = 0;
				file.Read((BYTE*)&nummap, sizeof(UInt32));
				
				BinaryClientMapInfo * pInfo = NULL;
				for( UInt32 i = 0; i< nummap; ++i )
				{
					pInfo = SafeCreateObject(BinaryClientMapInfo);
					if ( pInfo )
					{
						file.Read( (BYTE*)pInfo, sizeof(BinaryClientMapInfo) );

						m_cliMapInfo.insert(std::make_pair(pInfo->m_mapid, pInfo));
					}
				}
			}
		}
		catch(FileException Er)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMapManager::LoadClientMapInfo, open error, %s"), filename);
			return FALSE;
		}
	}
	return TRUE;
}

const BinaryClientMapInfo* CMapManager::GetClientMap(UInt32 mapid)
{
	ClientMapInfoMap::iterator itr = m_cliMapInfo.find(mapid); 
	if ( itr != m_cliMapInfo.end() )
		return itr->second;
	return NULL;
}

BOOL CMapManager::SaveDAT(const TCHAR *filename)
{
	File file;
	try
	{
		file.Open(filename, File::MODE_WRITE);
		if (file.IsOpened())
		{
			BinaryBlockInfoHeader header;
			header.version = EXPORT_DATA_SERVER_MAP_INFO_FORMAT_VERSION;
			file.Write( (BYTE*)&header, sizeof(BinaryBlockInfoHeader) );
			UInt32 numMap = (UInt32)m_mapint.size();
			file.Write( (BYTE*)&numMap, sizeof(UInt32) );

			MapMapInt::iterator itr = m_mapint.begin();
			while( itr!=m_mapint.end() )
			{
				file.Write( (BYTE*)itr->second->m_basicinfo, sizeof(BinaryServerMapInfo) );
				itr++;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMapManager::SaveDAT, open error, %s"), filename);
		return FALSE;
	}
	return TRUE;
}

BOOL CMapManager::LoadZoneDAT(UInt32 mapid, const TCHAR *folder)
{
	MapMapInt::iterator itr = m_mapint.find(mapid);
	if( itr!=m_mapint.end() )
	{
		MapDB * tmpmap = itr->second;
		if (tmpmap->m_zonedata_header)		// already loaded
			return TRUE;
		String filename;
		filename.Format( _T("%sserver_%08i_zonedata.dat"), folder, tmpmap->m_basicinfo->m_mapid );

		File file;
		try
		{
			file.Open(filename.c_str(), File::MODE_READ_EXISTING);
			if (file.IsOpened())
			{
				tmpmap->m_zonedata_header = SafeCreateObject(BinaryBlockInfoHeader);
				file.Read( (BYTE*)tmpmap->m_zonedata_header, sizeof(BinaryBlockInfoHeader) );

				TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, tmpmap->m_zonedata_header->version!=EXPORT_DATA_SERVER_MAP_ZONEDATA_FORMAT_VERSION, _T("MapManager::LoadZoneDAT from \"%s\", file version:%i, app. version:%i"), filename.c_str(), tmpmap->m_zonedata_header->version, EXPORT_DATA_SERVER_MAP_ZONEDATA_FORMAT_VERSION);
				if( tmpmap->m_zonedata_header->version!=EXPORT_DATA_SERVER_MAP_ZONEDATA_FORMAT_VERSION )	throw;

				UInt32 sizeofzone;
				file.Read( (BYTE*)&sizeofzone, sizeof(UInt32) );
				BinaryServerZoneInfo* tempzone;
				for( UInt i = 0; i<sizeofzone; i++ )
				{
					tempzone = SafeCreateObject(BinaryServerZoneInfo);
					file.Read( (BYTE*)tempzone, sizeof(BinaryServerZoneInfo) );
					tmpmap->m_zoneinfo.insert(std::make_pair(tempzone->zoneid, tempzone));
				}
				tmpmap->m_blockData = SafeAllocate(WORD, 
										tmpmap->m_zonedata_header->blocksizeinbyte);
				ServerMapZoneData* tmpblockzone;
				for( UInt i = 0; i<tmpmap->m_zonedata_header->blocksizeinbyte; i++ )
				{
					tmpblockzone = SafeCreateObject(ServerMapZoneData);
					file.Read( (BYTE*)&tmpblockzone->sBlockType, sizeof(WORD));
					file.Read( (BYTE*)&tmpblockzone->MeetEnemyZoneId, sizeof(WORD) );
					file.Read( (BYTE*)&tmpblockzone->ZoneIdCount, sizeof(UInt32) );
					tmpmap->m_blockData[i] = tmpblockzone->sBlockType;
					if( tmpblockzone->ZoneIdCount > 0 )
					{
						tmpblockzone->ZoneIdLst = SafeAllocate(WORD, tmpblockzone->ZoneIdCount);
						file.Read( (BYTE*)tmpblockzone->ZoneIdLst, sizeof(WORD) * tmpblockzone->ZoneIdCount );
					}else
					{
						tmpblockzone->ZoneIdLst = NULL;
					}
					tmpmap->m_zonedata.push_back(tmpblockzone);
				}
			}
			file.Close();
		}
		catch(FileException Er)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMapManager::LoadZoneDAT, open error, %s"), filename.c_str());
			return FALSE;
		}

		LoadInstanceDAT(mapid, folder, BOOLEAN_FALSE);		// load instance data
		return TRUE;
	}
	return FALSE;
}

BOOL CMapManager::LoadInstanceDAT(UInt32 mapid, const TCHAR *folder, Boolean bNpcOnly)
{
	MapDB * tmpmap = const_cast<MapDB*>(GetMap(mapid));
	if( tmpmap )
	{
		String filename;
		filename.Format(_T("%sserver_%08i_instance.dat"), folder, tmpmap->m_basicinfo->m_mapid );

		File file;
		try
		{
			file.Open(filename.c_str(), File::MODE_READ_EXISTING);
			if (file.IsOpened())
			{
				DefaultInfoHeader* m_instance_header = SafeCreateObject(DefaultInfoHeader);
				file.Read( (BYTE*)m_instance_header, sizeof(DefaultInfoHeader));

				TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, m_instance_header->version!=EXPORT_DATA_SERVER_MAP_INSTANCE_FORMAT_VERSION, _T("MapManager::LoadInstanceDAT from \"%s\", file version:%i, app. version:%i"), filename.c_str(), m_instance_header->version, EXPORT_DATA_SERVER_MAP_INSTANCE_FORMAT_VERSION);
				if( m_instance_header->version!=EXPORT_DATA_SERVER_MAP_INSTANCE_FORMAT_VERSION ) throw;

				SafeDeleteObject(m_instance_header);	// currently no use

				tmpmap->m_bLoaded = BOOLEAN_TRUE;
				BinaryServerNPCInstance* tmpnpc;
				UInt16 sizeofnpc;
				file.Read( (BYTE*)&sizeofnpc, sizeof(UInt16) );
				for( UInt j = 0; j<sizeofnpc; j++ )
				{
					tmpnpc = SafeCreateObject(BinaryServerNPCInstance);
					file.Read( (BYTE*)tmpnpc, sizeof(BinaryServerNPCInstance) );
					tmpmap->m_npc.push_back(tmpnpc);
				}
				if (bNpcOnly) {
					file.Close();
					return TRUE;
				}

				BinaryServerOBJInstance* tmpobj;
				UInt16 sizeofobj;
				file.Read( (BYTE*)&sizeofobj, sizeof(UInt16) );
				for( UInt j = 0; j<sizeofobj; j++ )
				{
					tmpobj = SafeCreateObject(BinaryServerOBJInstance);
					file.Read( (BYTE*)tmpobj, sizeof(BinaryServerOBJInstance) );
					tmpmap->m_obj.push_back(tmpobj);
				}

				BinaryServerJumppointInstance* tmpjp;
				UInt16 sizeofjp;
				file.Read( (BYTE*)&sizeofjp, sizeof(UInt16) );
				for( UInt j = 0; j<sizeofjp; j++ )
				{
					tmpjp = SafeCreateObject(BinaryServerJumppointInstance);
					file.Read( (BYTE*)tmpjp, sizeof(BinaryServerJumppointInstance) );
					tmpmap->m_jp.push_back(tmpjp);
				}
// Meet Enemy Zone's (= Hidden NPC)
				BinaryServerMeetEnemy tmpMeNpc;
				UInt16 sizeOfMeNpc;
				file.Read( (BYTE*)&sizeOfMeNpc, sizeof(UInt16) );
				for( UInt j = 0; j<sizeOfMeNpc; ++j )
				{
					file.Read( (BYTE*)&tmpMeNpc, sizeof(BinaryServerMeetEnemy) );

					MeetEnemyZone* tmpMeZone = tmpmap->GetMEZone(tmpMeNpc.nZoneID);
					if( tmpMeZone )
					{
						MeetEnemyData* pMeData = SafeCreateObject(MeetEnemyData);
							pMeData->NpcId = tmpMeNpc.nRefID;
							pMeData->NpcRate = tmpMeNpc.nEncount;
						tmpMeZone->encountList.push_back(pMeData);
					}else
					{
						MeetEnemyZone* pMeZone = SafeCreateObject(MeetEnemyZone);
						MeetEnemyData* pMeData = SafeCreateObject(MeetEnemyData);
							pMeData->NpcId = tmpMeNpc.nRefID;
							pMeData->NpcRate = tmpMeNpc.nEncount;
						pMeZone->encountList.push_back(pMeData);
						tmpmap->m_mezoneinfo.insert(std::make_pair(tmpMeNpc.nZoneID, pMeZone));
					}
				}
				
				for( StlMap<UInt32, MeetEnemyZone*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, MeetEnemyZone*> > >::iterator itr_me = tmpmap->m_mezoneinfo.begin();
					itr_me!=tmpmap->m_mezoneinfo.end(); itr_me++ )
				{
					UInt32 id = itr_me->first;
					MeetEnemyZone* mez = itr_me->second;
					if( mez )
					{
						for (Int i = 0; i < 24; i++)
							mez->encountTotals[i] = 0;
						for( Index j = C_INDEX(0); j < mez->encountList.size(); ++j )
						{
							
							const NpcData * pNpc  = m_npcmgr->GetNpc(mez->encountList[j]->NpcId);
							if (pNpc)
							{
								DWORD bits = 0x00000001;
								for (Int count = 0; count < 24; count ++)
								{
									if ((pNpc->appearTime & bits) > 0)
									{
										mez->encountTotals[count] += mez->encountList[j]->NpcRate;
									}
									bits *= 2;
								}
								
							}
						}
					}
				}
			}
			file.Close();
			return TRUE;
		}
		catch(FileException Er)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMapManager::LoadZoneDAT, open error, %s"), filename.c_str());
			return FALSE;
		}
	}
	return FALSE;
}

BOOL CMapManager::LoadPFDAT(UInt32 mapid, const TCHAR *folder, PathFindLoadData& pPathFindData, UInt32& pNumGuide)
{
	MapMapInt::iterator itr = m_mapint.find(mapid);
	if( itr!=m_mapint.end() )
	{
		MapDB * tmpmap = itr->second;
		if (!tmpmap->m_zonedata_header) {		// already loaded 
			//return FALSE;
			return TRUE;
		}

		String filename;
		filename.Format( _T("%sserver_%08i_pathfind.dat"), folder, tmpmap->m_basicinfo->m_mapid );

		File file;
		try
		{
			UInt x = 0;
			UInt y = 0;
			file.Open(String(filename).c_str(), File::MODE_READ_EXISTING);
			if (file.IsOpened())
			{
				Real32* fbufw	= NULL;
				Real32* fbufd	= NULL;
				UInt16* ubuf	= NULL;
				DefaultPFHeader header;
				file.Read( (BYTE*)&header, sizeof(DefaultPFHeader) );
				TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, header.version!=EXPORT_DATA_MAP_PATHFIND_FORMAT_VERSION, _T("MapManager::LoadPFDAT from \"%s\", file version:%i, app. version:%i"), String(filename).c_str(), header.version, EXPORT_DATA_MAP_PATHFIND_FORMAT_VERSION);
				if( header.version!=EXPORT_DATA_MAP_PATHFIND_FORMAT_VERSION ) throw;

				if( header.sizeWeightGraph>0 )
				{
					fbufw = SafeAllocate(Real32, header.sizeWeightGraph);
					file.Read( (BYTE*)fbufw, header.sizeWeightGraph * sizeof(Real32) );
				}
				if( header.sizeDistGraph>0 )
				{
					fbufd = SafeAllocate(Real32, header.sizeDistGraph);
					file.Read( (BYTE*)fbufd, header.sizeDistGraph * sizeof(Real32) );
				}
				if( header.sizeLinkGraph>0 )
				{
					ubuf = SafeAllocate(UInt16, header.sizeLinkGraph);
					file.Read( (BYTE*)ubuf, header.sizeLinkGraph * sizeof(UInt16) );
				}
				file.Close();

				pPathFindData.weightData	= fbufw;
				pPathFindData.distData		= fbufd;
				pPathFindData.linkData		= ubuf;
				pNumGuide = header.numGuildPoint;
			}
		}
		catch(FileException Er)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMapManager::LoadZoneDAT, open error, %s"), filename.c_str());
			return TRUE;	// if cannot load path finder data, do dynamic pre-calculation
		}
		return TRUE;
	}
	
	//return FALSE;
	return TRUE;		// if cannot load path finder data, do dynamic pre-calculation
}

BOOL CMapManager::LoadAllNpc(const TCHAR *folder)
{
	MapDB* mapdb;
	for (MapMapInt::iterator it = m_mapint.begin(); it != m_mapint.end(); it++)
	{
		mapdb = it->second;
		if (mapdb && !mapdb->m_bLoaded)		// load only NPC instance
			LoadInstanceDAT(it->first, folder, BOOLEAN_TRUE);
	}
	return TRUE;
}

void CMapManager::GetMapList(MapMapInt& mapList)
{
	for (MapMapInt::iterator it = m_mapint.begin(); it != m_mapint.end(); it++)
		mapList.insert(std::make_pair(it->first, it->second));
}

BOOL CMapManager::GetNpcPosition(UInt32 map_id, UInt32 npc_id, UInt16 &x, UInt16 &y)
{
	const MapDB* mapdb = GetMap(map_id);
	if (mapdb) {
		for (NpcInstList::const_iterator it = mapdb->m_npc.begin();
			it != mapdb->m_npc.end(); ++it)
		{
			BinaryServerNPCInstance *pnpc = *it;
			if (pnpc->nNPCUserID == npc_id) {
				x = pnpc->npc_position_x;
				y = pnpc->npc_position_y;
			}
		}
	}
	return FALSE;
}

BinaryServerNPCInstance* CMapManager::GetNpcInstance(UInt32 map_id, UInt32 npc_id)
{
	const MapDB* mapdb = GetMap(map_id);
	if (mapdb) {
		for (NpcInstList::const_iterator it = mapdb->m_npc.begin();
			it != mapdb->m_npc.end(); ++it)
		{
			BinaryServerNPCInstance *pnpc = *it;
			if (pnpc->nNPCUserID == npc_id)
				return pnpc;
		}
	}
	return NULL;
}

BOOL CMapManager::TestMapData(UInt32 map_id)
{
	const MapDB* pMapDB = GetMap(map_id);
	TRACE_ENSURE(pMapDB);
	TRACE_ENSURE(pMapDB->m_basicinfo);
	TRACE_ENSURE(!pMapDB->m_header);
	TRACE_ENSURE(!pMapDB->m_zoneinfo.empty());
	TRACE_ENSURE(!pMapDB->m_zonedata.empty());

	UInt32 w = pMapDB->m_zonedata_header->width;
	UInt32 h = pMapDB->m_zonedata_header->height;
	TRACE_ENSURE(w);
	TRACE_ENSURE(h);
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("testmap mapid=%d w=%d h=%d"), map_id, w, h);

	for( UInt32 i = 0; i < h; ++i )
	{
		for( UInt32 j = 0; j < w; ++j )
		{
			UInt32 pos = i * w + j;
			ServerMapZoneData& zoneData = *pMapDB->m_zonedata[pos];

			WORD*& idList = zoneData.ZoneIdLst;
			UInt32& idCount = zoneData.ZoneIdCount;

			TRACE_ENSURE(idCount);
			for( Index k = C_INDEX(0); k < idCount; ++k )
			{
				WORD& id = idList[k];
//				TRACE_INFODTL_3(GLOBAL_LOGGER, _F("testmap mapid=%d w=%d h=%d"), map_id, w, h);
				TRACE_ENSURE(id < pMapDB->m_zoneinfo.size());
			}
		}
	}
	return TRUE;
}
