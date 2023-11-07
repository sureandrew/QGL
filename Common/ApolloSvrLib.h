#ifndef _APOLLOSVRLIB_
#define _APOLLOSVRLIB_
namespace Apollo {
#define VERSION "20090429130"

#define MAX_OVERTIME_COUNT	3

#define DEFINE_FIRSTPACKET "FirstPacket"
#define MAX_PLUGIN_COUNT	100

#define MAX_DATA_INFO_LEN 1024
typedef struct _DATA_STREAM_HEAD{
	DWORD m_dwBlockSize;//���������������ݴ�С,������HEAD
	DWORD m_dwCount;//���ݱ��
	DWORD m_dwNumbers;//�����ݸ���
	BOOL  m_bCompressed;//�Ƿ�ѹ����
	DWORD m_dwInitKeyValue;//��Կ��ʼ������
	DWORD m_dwCompressedBlockSize;//����Block��ѹ�����С
	DWORD m_dwCryptType;//��������
	DWORD m_dwSize;//Block��������С
	DWORD m_dwBlockCRC32;//Block��CRC32
	BYTE  m_abyData[];
}DATA_STREAM_HEAD,*PDATA_STREAM_HEAD;

typedef struct _DATA_STREAM_HEAD_INFO{
	DWORD m_dwBlockSize;//���������������ݴ�С,������HEAD
	DWORD m_dwCount;//���ݱ��
	DWORD m_dwNumbers;//�����ݸ���
	BOOL  m_bCompressed;//�Ƿ�ѹ����
	DWORD m_dwInitKeyValue;//��Կ��ʼ������
	DWORD m_dwCompressedBlockSize;//����Block��ѹ�����С
	DWORD m_dwCryptType;//��������
	DWORD m_dwSize;//Block��������С
	DWORD m_dwBlockCRC32;//Block��CRC32
	BYTE  m_abyData[MAX_DATA_INFO_LEN];//����,���2048���ֽ�
}DATA_STREAM_HEAD_INFO,*PDATA_STREAM_HEAD_INFO;

typedef struct _GAME_INFO{
	char cpIP[16];				//��½��ϷIP��ַ
	char cpAccounts[20];		//��½��Ϸ�ʺ�
	char cpCharacterName[32];	//��½��Ϸ��ɫ��
	char cpServerName[32];		//��½��Ϸ��������
	DWORD dwCharacterID;		//��Ϸ��ɫID
	DWORD dwOnlyId;				//���� ΨһID
	PVOID pReserved;			//����
}GAME_INFO,*PGAME_INFO;

class CGameInfo{
public:
	CGameInfo()
	{
		memset(&m_GameInfo,0,sizeof(GAME_INFO));
		memset(&m_pPluginContext,0,sizeof(PVOID)*MAX_PLUGIN_COUNT);
	}
	//���ܣ�������Ϸ��Ϣ
	void SetGameInfo(PGAME_INFO pGameInfo)
	{
		memcpy(&m_GameInfo,(char*)pGameInfo,sizeof(GAME_INFO));
	}
	//���ܣ������Ϸ��Ϣ
	void GetGameInfo(PGAME_INFO pGameInfo)
	{
		memcpy((char*)pGameInfo,&m_GameInfo,sizeof(GAME_INFO));
	}

	//���ܣ����õ�½��ϷΨһ��־
	void SetOnlyID(DWORD dwOnlyID){ m_GameInfo.dwOnlyId=dwOnlyID; }
	//���ܣ����õ�½��ϷIP��ַ
	void SetIPAddress(char *pIPAddress){ strncpy(m_GameInfo.cpIP,pIPAddress,sizeof(m_GameInfo.cpIP)-1); }
	//���ܣ����õ�½��Ϸ�ʺ�
	void SetAccounts(char *pAccounts){ strncpy(m_GameInfo.cpAccounts,pAccounts,sizeof(m_GameInfo.cpAccounts)-1); }
	//���ܣ����õ�½��Ϸ��ɫ��
	void SetCharacterName(char *pCharacterName){ strncpy(m_GameInfo.cpCharacterName,pCharacterName,sizeof(m_GameInfo.cpCharacterName)-1); }
	//���ܣ����õ�½��Ϸ��ɫID
	void SetCharacterID(DWORD dwCharacterID){ m_GameInfo.dwCharacterID=dwCharacterID; }
	//���ܣ����õ�½��Ϸ������
	void SetServerName(char *pServerName){ strncpy(m_GameInfo.cpServerName,pServerName,sizeof(m_GameInfo.cpServerName)-1); }

	//���ܣ���õ�½��ϷΨһ��־
	DWORD GetOnlyID(){ return m_GameInfo.dwOnlyId; }
	//���ܣ���õ�½��ϷIP��ַ
	char *GetIPAddress(){ return m_GameInfo.cpIP; }
	//���ܣ���õ�½��Ϸ�ʺ�
	char *GetAccounts(){ return m_GameInfo.cpAccounts; }
	//���ܣ���õ�½��Ϸ��ɫ��
	char *GetCharacterName(){ return m_GameInfo.cpCharacterName; }
	//���ܣ���õ�½��Ϸ������
	char *GetServerName(){ return m_GameInfo.cpServerName; }
	//���ܣ���õ�½��Ϸ��ɫID
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

	//���ܣ����ӳ�ʱ����
	int AddOvertimeCount(){ m_iOvertimeCount+=1; return m_iOvertimeCount; }
	//���ܣ���������� >0 �����ʱ��
	void SetLastSendTime(DWORD dwTime){ m_dwLastSendTime=dwTime; }

	//���ܣ���ó�ʱ����
	int GetOvertimeCount(){ return m_iOvertimeCount; }
	//���ܣ���������ʱ�� >0 �����ʱ��
	DWORD GetLastSendTime(){ return m_dwLastSendTime; }
	//���ܣ���ü�鷢�����ݺ���󷵻�ʱ��(��) -1����ⷵ��ʱ�䣬>0�����󷵻�ʱ��(��)
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

	//���ܣ����AUTH���ͷ���� 
	//������cpAuthPluginName AUTH�������,NULL �ű��Զ����ƣ�pOutPacket ����AUTH�����pOutLen �������
	//���أ��ɹ���GET_AUTH_SUCCESS�����з�� GET_AUTH_CONTINUE��ʧ�ܣ�GET_AUTH_FAIL����ʱ�ر� AUTH_DISABLE_STATE��ϵͳ�رգ�CLOSE_AUTH
	int GetDataFromApolloSRV(char *cpAuthPluginName,char *pOutPacket,int *pOutLen);
	//���ܣ�������AUTH���ͷ���� 
	//������pOutPacket ����AUTH�����pOutLen �������
	//���أ��ɹ���GET_AUTH_SUCCESS�����з�� GET_AUTH_CONTINUE��ʧ�ܣ�GET_AUTH_FAIL����ʱ�ر� AUTH_DISABLE_STATE��ϵͳ�رգ�CLOSE_AUTH
	int GetDataFromApolloRandomSRV(char *pOutPacket,int *pOutLen);
#define GET_AUTH_CONTINUE	1			//����ʣ��AUTH���
#define GET_AUTH_SUCCESS	0			//���һ��AUTH���
#define GET_AUTH_FAIL		-1			//���AUTH�������
#define AUTH_DISABLE_STATE	-2			//��ǰ��ȡAUTH�������̬ж��
#define AUTH_CLOSE_STATE	-3			//Apolloϵͳ�ر�״̬

	//���ܣ�������Ϸ����AUTH���
	void SendDataToApolloSRV(char *pInPacket,int iInLen);
public:
	CGameInfo m_GameInfo;
protected:
	DATA_STREAM_HEAD_INFO m_SendBuffer[MAX_SEND_SINGLE_PACKET_NUM];
	char *m_pRecvBuffer;
	int m_iRecvBufferLen;
};


//////////////////////////////////////////////////////////////////
//�̰߳�ȫ
bool WINAPI AddApolloAutoRequestUserSRV(CGameInfo *pGameInfo);
bool WINAPI DelApolloAutoRequestUserSRV(DWORD dwOnlyId);
void WINAPI SendDataToApolloSRV(DWORD dwOnlyId,char *pInPacket,int iInLen);
void WINAPI SetApolloSeedSRV(DWORD dwTimeSec);
//////////////////////////////////////////////////////////////////

//���������ڲ�����
#define APOLLO_SERVER_ERROR		1

typedef struct _SERVER_ERROR_INFO{
	//�ͻ��˷��صİ�û���ҵ���Ӧ�Ĳ������
#define SERVER_ERROR_CODE_PACKET_NO_PLUGIN		2
	//�ڲ������ͻ��˷��صİ�ʱ�쳣
#define SERVER_ERROR_CODE_PLUGIN_EXCEPTION		3
	//�ͻ��˷��صİ�Ч�����
#define SERVER_ERROR_CODE_PACKET_CRC_ERROR		4
	//�����LOG��Ϣ
#define SERVER_ERROR_CODE_LOG_INFO				5
	CGameInfo *pGameInfo;
	DWORD dwErrorCode;
	char cpInfo[MAX_PATH];
}SERVER_ERROR_INFO,*PSERVER_ERROR_INFO;

#define APOLLO_CLIENT_ERROR		5

//���ܣ�����Ϸ����
//����
#define APOLLO_SERVER_KICK_GAME_CHARACTER		10
//��Ϣ�ṹ
typedef struct _KICK_GAME_INFO{
	CGameInfo *pGameInfo;
	char cpReasonInfo[MAX_PATH];
}KICK_GAME_INFO,*PKICK_GAME_INFO;

//���ܣ�����AUTH��
//����
#define APOLLO_SERVER_SEND_AUTH_PACKET			11
//��Ϣ�ṹ
typedef struct _SEND_AUTH_PACKET_INFO{
	CGameInfo *pGameInfo;
	BYTE pPacket[sizeof(DATA_STREAM_HEAD_INFO)];
	int iPacketLen;
}SEND_AUTH_PACKET_INFO,*PSEND_AUTH_PACKET_INFO;

typedef long atime_t;

//���ܣ���¼���ݿ�
//����
#define APOLLO_SERVER_LOG_DATABASE_INFO			12
//��Ϣ�ṹ
#define LOG_TYPE_CRC		4
//�ʺ�	ʱ��	�������	����
typedef struct _LOG_DATABASE_INFO{
	CGameInfo *pGameInfo;
	atime_t tTime;							//ʱ��
	DWORD dwType;							//����
	char cpTypeName[20];					//��������
	char cpLogInfo[MAX_PATH];				//��Ϣ
}LOG_DATABASE_INFO,*PLOG_DATABASE_INFO;

//////////////////////////////////////////////////////////////////
typedef BOOL(WINAPI *FAPOLLOCALLBACKINFO)(DWORD dwIndex,PVOID pInfo,int iLen);
void SetApolloCallBackSRV(FAPOLLOCALLBACKINFO fApolloCallbackInfo);
};
#endif //_APOLLOSVRLIB_