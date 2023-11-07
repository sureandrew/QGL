#ifndef _APOLLOSVRLIB_
#define _APOLLOSVRLIB_
namespace Apollo {
#define VERSION "20090429130"

#define MAX_OVERTIME_COUNT	3

#define DEFINE_FIRSTPACKET "FirstPacket"
#define MAX_PLUGIN_COUNT	100

#define MAX_DATA_INFO_LEN 1024
typedef struct _DATA_STREAM_HEAD{
	DWORD m_dwBlockSize;//本次所包含的数据大小,不包括HEAD
	DWORD m_dwCount;//数据编号
	DWORD m_dwNumbers;//总数据个数
	BOOL  m_bCompressed;//是否压缩了
	DWORD m_dwInitKeyValue;//密钥初始化参数
	DWORD m_dwCompressedBlockSize;//整个Block的压缩后大小
	DWORD m_dwCryptType;//加密类型
	DWORD m_dwSize;//Block的完整大小
	DWORD m_dwBlockCRC32;//Block的CRC32
	BYTE  m_abyData[];
}DATA_STREAM_HEAD,*PDATA_STREAM_HEAD;

typedef struct _DATA_STREAM_HEAD_INFO{
	DWORD m_dwBlockSize;//本次所包含的数据大小,不包括HEAD
	DWORD m_dwCount;//数据编号
	DWORD m_dwNumbers;//总数据个数
	BOOL  m_bCompressed;//是否压缩了
	DWORD m_dwInitKeyValue;//密钥初始化参数
	DWORD m_dwCompressedBlockSize;//整个Block的压缩后大小
	DWORD m_dwCryptType;//加密类型
	DWORD m_dwSize;//Block的完整大小
	DWORD m_dwBlockCRC32;//Block的CRC32
	BYTE  m_abyData[MAX_DATA_INFO_LEN];//数据,最多2048个字节
}DATA_STREAM_HEAD_INFO,*PDATA_STREAM_HEAD_INFO;

typedef struct _GAME_INFO{
	char cpIP[16];				//登陆游戏IP地址
	char cpAccounts[20];		//登陆游戏帐号
	char cpCharacterName[32];	//登陆游戏角色名
	char cpServerName[32];		//登陆游戏服务器名
	DWORD dwCharacterID;		//游戏角色ID
	DWORD dwOnlyId;				//必须 唯一ID
	PVOID pReserved;			//保留
}GAME_INFO,*PGAME_INFO;

class CGameInfo{
public:
	CGameInfo()
	{
		memset(&m_GameInfo,0,sizeof(GAME_INFO));
		memset(&m_pPluginContext,0,sizeof(PVOID)*MAX_PLUGIN_COUNT);
	}
	//功能：设置游戏信息
	void SetGameInfo(PGAME_INFO pGameInfo)
	{
		memcpy(&m_GameInfo,(char*)pGameInfo,sizeof(GAME_INFO));
	}
	//功能：获得游戏信息
	void GetGameInfo(PGAME_INFO pGameInfo)
	{
		memcpy((char*)pGameInfo,&m_GameInfo,sizeof(GAME_INFO));
	}

	//功能：设置登陆游戏唯一标志
	void SetOnlyID(DWORD dwOnlyID){ m_GameInfo.dwOnlyId=dwOnlyID; }
	//功能：设置登陆游戏IP地址
	void SetIPAddress(char *pIPAddress){ strncpy(m_GameInfo.cpIP,pIPAddress,sizeof(m_GameInfo.cpIP)-1); }
	//功能：设置登陆游戏帐号
	void SetAccounts(char *pAccounts){ strncpy(m_GameInfo.cpAccounts,pAccounts,sizeof(m_GameInfo.cpAccounts)-1); }
	//功能：设置登陆游戏角色名
	void SetCharacterName(char *pCharacterName){ strncpy(m_GameInfo.cpCharacterName,pCharacterName,sizeof(m_GameInfo.cpCharacterName)-1); }
	//功能：设置登陆游戏角色ID
	void SetCharacterID(DWORD dwCharacterID){ m_GameInfo.dwCharacterID=dwCharacterID; }
	//功能：设置登陆游戏服务器
	void SetServerName(char *pServerName){ strncpy(m_GameInfo.cpServerName,pServerName,sizeof(m_GameInfo.cpServerName)-1); }

	//功能：获得登陆游戏唯一标志
	DWORD GetOnlyID(){ return m_GameInfo.dwOnlyId; }
	//功能：获得登陆游戏IP地址
	char *GetIPAddress(){ return m_GameInfo.cpIP; }
	//功能：获得登陆游戏帐号
	char *GetAccounts(){ return m_GameInfo.cpAccounts; }
	//功能：获得登陆游戏角色名
	char *GetCharacterName(){ return m_GameInfo.cpCharacterName; }
	//功能：获得登陆游戏服务器
	char *GetServerName(){ return m_GameInfo.cpServerName; }
	//功能：获得登陆游戏角色ID
	DWORD GetCharacterID(){ return m_GameInfo.dwCharacterID; }
public:
	GAME_INFO m_GameInfo;
	PVOID m_pPluginContext[MAX_PLUGIN_COUNT];
};

#define GenRand(MIN,MAX) ( MIN + (DWORD)(GetTickCount()*0x87654321)%(MAX-MIN))

class CApolloSrvState{
public:
	CApolloSrvState(){
		ResetOvertimeCount();
		ResetLastSendTime();
		m_dwCheckMaxRetTime=m_iCurFunctionIndex=m_iNextFunctionIndex=-1;
		m_dwKey=GenRand(100,1000);
	}

	void ResetLastSendTime(){ m_dwLastSendTime=0; }
	void ResetOvertimeCount(){ m_iOvertimeCount=0; }

	//功能：增加超时次数
	int AddOvertimeCount(){ m_iOvertimeCount+=1; return m_iOvertimeCount; }
	//功能：设置最后发送 >0 最后发送时间
	void SetLastSendTime(DWORD dwTime){ m_dwLastSendTime=dwTime; }

	//功能：获得超时次数
	int GetOvertimeCount(){ return m_iOvertimeCount; }
	//功能：获得最后发送时间 >0 最后发送时间
	DWORD GetLastSendTime(){ return m_dwLastSendTime; }
	//功能：获得检查发送数据后最大返回时间(秒) -1不检测返回时间，>0检测最大返回时间(秒)
	DWORD GetCheckMaxRetTime(){ return m_dwCheckMaxRetTime; }

	int GetCurPluginFuncIndex(){return m_iCurFunctionIndex; }
	void SetNextPluginFuncIndex(int iIndex){ m_iNextFunctionIndex=iIndex; }

public:
	DWORD m_dwLastSendTime;
	DWORD m_dwCheckMaxRetTime;
	int m_iOvertimeCount;
protected:
	int m_iNextFunctionIndex;
	int m_iCurFunctionIndex;
	DWORD m_dwKey;
};

#define MAX_BUFFER_LEN 0x10000
#define MAX_SEND_SINGLE_PACKET_NUM 32
class CApolloSrvLib : public CApolloSrvState
{
public:
	CApolloSrvLib();
	virtual ~CApolloSrvLib();

	//功能：获得AUTH发送封包； 
	//参数：cpAuthPluginName AUTH插件功能,NULL 脚本自动控制；pOutPacket 返回AUTH封包；pOutLen 封包长度
	//返回：成功：GET_AUTH_SUCCESS；还有封包 GET_AUTH_CONTINUE，失败：GET_AUTH_FAIL，暂时关闭 AUTH_DISABLE_STATE，系统关闭：CLOSE_AUTH
	int GetDataFromApolloSRV(char *cpAuthPluginName,char *pOutPacket,int *pOutLen);
	//功能：随机获得AUTH发送封包； 
	//参数：pOutPacket 返回AUTH封包；pOutLen 封包长度
	//返回：成功：GET_AUTH_SUCCESS；还有封包 GET_AUTH_CONTINUE，失败：GET_AUTH_FAIL，暂时关闭 AUTH_DISABLE_STATE，系统关闭：CLOSE_AUTH
	int GetDataFromApolloRandomSRV(char *pOutPacket,int *pOutLen);
#define GET_AUTH_CONTINUE	1			//还有剩余AUTH封包
#define GET_AUTH_SUCCESS	0			//最后一个AUTH封包
#define GET_AUTH_FAIL		-1			//获得AUTH封包错误
#define AUTH_DISABLE_STATE	-2			//当前获取AUTH插件被动态卸载
#define AUTH_CLOSE_STATE	-3			//Apollo系统关闭状态

	//功能：设置游戏接收AUTH封包
	void SendDataToApolloSRV(char *pInPacket,int iInLen);
public:
	CGameInfo m_GameInfo;
protected:
	DATA_STREAM_HEAD_INFO m_SendBuffer[MAX_SEND_SINGLE_PACKET_NUM];
	char *m_pRecvBuffer;
	int m_iRecvBufferLen;
};


//////////////////////////////////////////////////////////////////
//线程安全
bool WINAPI AddApolloAutoRequestUserSRV(CGameInfo *pGameInfo);
bool WINAPI DelApolloAutoRequestUserSRV(DWORD dwOnlyId);
void WINAPI SendDataToApolloSRV(DWORD dwOnlyId,char *pInPacket,int iInLen);
void WINAPI SetApolloSeedSRV(DWORD dwTimeSec);
//////////////////////////////////////////////////////////////////

//服务器端内部错误
#define APOLLO_SERVER_ERROR		1

typedef struct _SERVER_ERROR_INFO{
	//客户端返回的包没有找到相应的插件处理
#define SERVER_ERROR_CODE_PACKET_NO_PLUGIN		2
	//在插件处理客户端返回的包时异常
#define SERVER_ERROR_CODE_PLUGIN_EXCEPTION		3
	//客户端返回的包效验错误
#define SERVER_ERROR_CODE_PACKET_CRC_ERROR		4
	//服务端LOG信息
#define SERVER_ERROR_CODE_LOG_INFO				5
	CGameInfo *pGameInfo;
	DWORD dwErrorCode;
	char cpInfo[MAX_PATH];
}SERVER_ERROR_INFO,*PSERVER_ERROR_INFO;

#define APOLLO_CLIENT_ERROR		5

//功能：踢游戏人物
//命令
#define APOLLO_SERVER_KICK_GAME_CHARACTER		10
//信息结构
typedef struct _KICK_GAME_INFO{
	CGameInfo *pGameInfo;
	char cpReasonInfo[MAX_PATH];
}KICK_GAME_INFO,*PKICK_GAME_INFO;

//功能：发送AUTH包
//命令
#define APOLLO_SERVER_SEND_AUTH_PACKET			11
//信息结构
typedef struct _SEND_AUTH_PACKET_INFO{
	CGameInfo *pGameInfo;
	BYTE pPacket[sizeof(DATA_STREAM_HEAD_INFO)];
	int iPacketLen;
}SEND_AUTH_PACKET_INFO,*PSEND_AUTH_PACKET_INFO;

typedef long atime_t;

//功能：记录数据库
//命令
#define APOLLO_SERVER_LOG_DATABASE_INFO			12
//信息结构
#define LOG_TYPE_CRC		4
//帐号	时间	插件类型	数据
typedef struct _LOG_DATABASE_INFO{
	CGameInfo *pGameInfo;
	atime_t tTime;							//时间
	DWORD dwType;							//类型
	char cpTypeName[20];					//类型名字
	char cpLogInfo[MAX_PATH];				//信息
}LOG_DATABASE_INFO,*PLOG_DATABASE_INFO;

//////////////////////////////////////////////////////////////////
typedef BOOL(WINAPI *FAPOLLOCALLBACKINFO)(DWORD dwIndex,PVOID pInfo,int iLen);
void SetApolloCallBackSRV(FAPOLLOCALLBACKINFO fApolloCallbackInfo);
};
#endif //_APOLLOSVRLIB_