#ifndef APOLLO_CLIENT_SDK
#define APOLLO_CLIENT_SDK

//����ΪCallBack��Ϣ

//�����ݵȴ�����
#define CALLBACK_MSG_WAIT_SEND_DATA   0x00000001
//ϵͳ��ʱ��Ϣ
#define CALLBACK_MSG_INSTANT_MESSAGE  0x00000002

//CallBack��Ϣ�������

//����ΪCallBack��������ֵ

//CallBack����ִ������
#define CALLBACK_OK                 0x00000001
//δ�����Message ID
#define CALLBACK_ERROR_UNKNOWN_MSG          -1

//����ΪCallBack��������ֵ�������

//ϵͳ��ʼ����������ֵ����
#define APOLLO_INIT_OK                         1
#define APOLLO_INIT_FAIL_UNKNOWN               0
#define APOLLO_INIT_FAIL_UPDATE_FAIL          -1
#define APOLLO_INIT_FAIL_LOAD_SYSTEM_FAIL     -2
#define APOLLO_INIT_FAIL_FUNC_INIT_FAIL       -3

//��ʼ����������ֵ�������

//������������ֵ����
#define APOLLO_RESULT_OK                       1
#define APOLLO_RESULT_FAIL                     0
//������������ֵ�������

//Apolloϵͳʹ�õĻص���������
//iApolloMsgIdΪ�ص����� ���ܵ�ֵΪ CALLBACK_MSG_WAIT_SEND_DATA, CALLBACK_MSG_INSTANT_MESSAGE
//������������iApolloMsgId��ͬ�������ݲ�ͬ�� ����μ������ĵ������ӳ���
typedef int (__stdcall *pApolloCallBack)(int iApolloMsgId,unsigned long ulArg0, unsigned long ulArg1);

//��ʼ�������ϵͳ
//�ɹ�����1, ʧ�ܷ���0
long __stdcall InitApolloCT(char *szLicenseData);

//ж��Apolloϵͳ
//�ɹ�����APOLLO_INIT_OK, ʧ�ܷ���APOLLO_INIT_FAIL_???????
long __stdcall UninitApolloCT();

//�������ݵ�Apolloϵͳ
//�ɹ�����APOLLO_RESULT_OK, ʧ�ܷ���APOLLO_RESULT_FAIL
//lpInΪ��������ָ��
//ulSizeΪ���ݴ�С
long __stdcall SendDataToApolloCT(unsigned char * lpIn,unsigned long ulSize);

//����Apolloϵͳ�ص�
//lpCallBackΪ�ص�����ָ��
//void __stdcall SetApolloCTCallBack(pApolloCallBack lpCallBack);

//��Apolloϵͳ��÷�������
//�ɹ�����APOLLO_RESULT_OK, ʧ�ܷ���APOLLO_RESULT_FAIL
//lpOut Ϊ�������ָ��
//pulSize ָ�򱣴�������ݴ�С��unsigned long������ָ��
long __stdcall GetDataFromApolloCT(unsigned char *lpOut,unsigned long * pulSize);


//���Apolloϵͳ�Ƿ�������
//�ɹ�����APOLLO_RESULT_OK, ʧ�ܷ���APOLLO_RESULT_FAIL
long __stdcall ApolloCTCheck();



#endif