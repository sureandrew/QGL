#ifndef APOLLO_CLIENT_SDK
#define APOLLO_CLIENT_SDK

//以下为CallBack消息

//有数据等待发送
#define CALLBACK_MSG_WAIT_SEND_DATA   0x00000001
//系统即时消息
#define CALLBACK_MSG_INSTANT_MESSAGE  0x00000002

//CallBack消息定义结束

//以下为CallBack函数返回值

//CallBack函数执行正常
#define CALLBACK_OK                 0x00000001
//未定义的Message ID
#define CALLBACK_ERROR_UNKNOWN_MSG          -1

//以下为CallBack函数返回值定义结束

//系统初始化函数返回值定义
#define APOLLO_INIT_OK                         1
#define APOLLO_INIT_FAIL_UNKNOWN               0
#define APOLLO_INIT_FAIL_UPDATE_FAIL          -1
#define APOLLO_INIT_FAIL_LOAD_SYSTEM_FAIL     -2
#define APOLLO_INIT_FAIL_FUNC_INIT_FAIL       -3

//初始化函数返回值定义结束

//其它函数返回值定义
#define APOLLO_RESULT_OK                       1
#define APOLLO_RESULT_FAIL                     0
//其它函数返回值定义结束

//Apollo系统使用的回调函数声明
//iApolloMsgId为回调类型 可能的值为 CALLBACK_MSG_WAIT_SEND_DATA, CALLBACK_MSG_INSTANT_MESSAGE
//其他参数根据iApolloMsgId不同包含数据不同， 具体参见开发文档和例子程序
typedef int (__stdcall *pApolloCallBack)(int iApolloMsgId,unsigned long ulArg0, unsigned long ulArg1);

//初始化反外挂系统
//成功返回1, 失败返回0
long __stdcall InitApolloCT(char *szLicenseData);

//卸载Apollo系统
//成功返回APOLLO_INIT_OK, 失败返回APOLLO_INIT_FAIL_???????
long __stdcall UninitApolloCT();

//发送数据到Apollo系统
//成功返回APOLLO_RESULT_OK, 失败返回APOLLO_RESULT_FAIL
//lpIn为输入数据指针
//ulSize为数据大小
long __stdcall SendDataToApolloCT(unsigned char * lpIn,unsigned long ulSize);

//设置Apollo系统回调
//lpCallBack为回调函数指针
//void __stdcall SetApolloCTCallBack(pApolloCallBack lpCallBack);

//从Apollo系统获得返回数据
//成功返回APOLLO_RESULT_OK, 失败返回APOLLO_RESULT_FAIL
//lpOut 为输出数据指针
//pulSize 指向保存输出数据大小的unsigned long变量的指针
long __stdcall GetDataFromApolloCT(unsigned char *lpOut,unsigned long * pulSize);


//检测Apollo系统是否工作正常
//成功返回APOLLO_RESULT_OK, 失败返回APOLLO_RESULT_FAIL
long __stdcall ApolloCTCheck();



#endif