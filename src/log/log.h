/***********************************************************************************************
// �ļ���:     log.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 13:11:33
// ��������:   ��־��¼������
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_LOG_H
#define BASIC_LOG_H

__NS_BASIC_START

//����debuglevel
enum DebugLevel
{
    DebugLevel_None = 0,
    DebugLevel_Error = 1,
    DebugLevel_Info = 2,
};
//!�¼���¼
/*!
 * д��Ĭ����־�ļ�
 *\remarks ��־�ļ�Ĭ��·����ͨ��BasicSetLogEventMode����
 *\remarks �ɸ�ʽ������
 */
_BASIC_DLL_API_C  _BASIC_DLL_API void BasicLogEventV(DebugLevel level, const char* pszLog, ...);
_BASIC_DLL_API_C  _BASIC_DLL_API void BasicLogEventErrorV(const char* pszLog, ...);

//!�¼���¼
/*!
* д��Ĭ����־�ļ�
*\remarks ��־�ļ�Ĭ��·����ͨ��BasicSetLogEventMode����
*/
_BASIC_DLL_API_C  _BASIC_DLL_API void BasicLogEvent(DebugLevel level, const char* pszLog);
_BASIC_DLL_API_C  _BASIC_DLL_API void BasicLogEventError(const char* pszLog);
//////////////////////////////////////////////////////////////////////////////////////////////
//
//BasicSetLogEventModeѡ��
#define	LOG_BY_DAY		0x00010000		//!< �����¼������������ƵĴ�С��һ����ܻ��ж���ļ���
#define	LOG_BY_SIZE		0x00020000		//!< ����С��¼���������ƵĴ�С���½�һ���ļ���
#define	LOG_BY_ONEFILE	0x00040000		//!< ʼ��ֻ��һ���ļ���������죬��ɾ��ǰһ������ݣ�������С���ƣ�ɾ��ԭ�����ļ���
#define	LOG_BY_BUFFER	0x00080000		//!< ��־������д�̣��ȱ����ڻ��������棬��ʱд�̡����Ч�ʡ�
#define	LOG_BY_OPEN		0x01000000		//!< ��־�ļ����ִ�״̬�����رա�
#define	LOG_BY_NOLIMIT	0x02000000		//!< �������ļ���С
#define LOG_BY_SAMENAME	0x04000000		//!< �����������


//
#define LOG_ADD_TIME	0x00100000		//!< ��¼ʱ��
#define LOG_ADD_THREAD	0x00200000		//!< ��¼�߳�ID
//
#define LOG_SIZE_LIMIT	0x0000ffff		//!< ��־�ļ���С����	��λ��MB
#define LOG_NAME_DAY_S	"S%DAY%"


#define LOG_ERROR_NAME_EMPTY		-1	//!< �ļ���Ϊ��
#define LOG_ERROR_OPEN_FILE			-2	//!< ���ļ�ʧ��
#define LOG_ERROR_FULL				-3	//!< ��־��¼ͨ���Ѿ����ˡ�

//! ������־��¼�ļ���
_BASIC_DLL_API_C  _BASIC_DLL_API void InitBasicLogLevel(DebugLevel level);
_BASIC_DLL_API_C  _BASIC_DLL_API DebugLevel GetBasicLogLevel();
//! bThreadCheckSelf true ��������(�޷�ʵʱ��¼) false������ �̰߳�ȫ(������bufferģʽʵʱ)
_BASIC_DLL_API_C  _BASIC_DLL_API bool InitBasicLog(bool bThreadCheckSelf);
//! ��������Լ��߳���Ҫ30s����һ��
_BASIC_DLL_API_C  _BASIC_DLL_API void OnTimerBasicLog();
//
//! ����Ĭ�ϵ��¼���¼ģʽ
/*
 * ���ú�ȫ����Ч����Ӧ�ó�������ʱ����һ�μ���
 *
 *\param		nOption:	��¼ģʽLOG_*
 *\param		pszLogFile:	��־�ļ���,�����·�����ᶯ̬����
 *					
 *\return		= 0 Ĭ�ϵ���־���
 *				<0 ����
 *\remarks  ֻ����������Ĭ�ϵ�ͨ������ͨ��ID = 0 ����������ã�ʹ���ļ��� basiclib.log
 */
_BASIC_DLL_API_C  _BASIC_DLL_API long BasicSetDefaultLogEventMode(long nOption, const char* pszLogFile);
_BASIC_DLL_API_C  _BASIC_DLL_API long BasicSetDefaultLogEventErrorMode(long nOption, const char* pszLogFile);
//

//
//! �����¼���¼ģʽ
/*
 * ���ú�ȫ����Ч����Ӧ�ó�������ʱ����һ�μ���
 *
 *\param		nOption:	��¼ģʽLOG_*
 *\param		pszLogFile:	��־�ļ���,�����·�����ᶯ̬����
 *					
 *\return		>0 ��־��������ڴ��ݸ�BasicLogEvent
 *				<0 ����
 */
_BASIC_DLL_API_C  _BASIC_DLL_API long BasicSetLogEventMode(long nOption, const char* pszLogFile);
//

//! �ر���־��¼
/*
 * �ڳ����˳�ǰ���߹ر�ĳ����־��¼��ʱ�����
 *
 *\param		lLogChannel:	��־��������� BasicSetLogEventMode �ķ���ֵ ���С�� 0 ���ر����е�
 *					
 *\return		>0 ��־��������ڴ��ݸ�BasicLogEvent
 *				<0 ����
 */
_BASIC_DLL_API_C  _BASIC_DLL_API long BasicCloseLogEvent(long lLogChannel);
//

//
//!�¼���¼��д����־�ļ���
/*
 *\remarks ��־�ļ�·����ͨ��BasicSetLogEventMode����
 *\remarks �ɸ�ʽ������
 */
_BASIC_DLL_API void BasicLogEventV(DebugLevel level, long lLogChannel, const char* pszLog, ...);
//
//
//
//


//
//!�¼���¼��д����־�ļ���
/*
 *\remarks ��־�ļ�·����ͨ��BasicSetLogEventMode����
 */
_BASIC_DLL_API void BasicLogEvent(DebugLevel level, long lLogChannel, const char* pszLog);
//
//
//

class _BASIC_DLL_API WriteLogDataBuffer : basiclib::CBasicObject
{
public:
    WriteLogDataBuffer();
    virtual ~WriteLogDataBuffer();

    void InitLogData(const char* lpszText);
    void CopyLogData(WriteLogDataBuffer& logData);

    void ClearLogData();
protected:
    void InitMember();
public:
	char*		m_pText;
	time_t		m_lCurTime;
	DWORD		m_dwProcessId;
	DWORD		m_dwThreadId;
};
_BASIC_DLL_API_C  _BASIC_DLL_API void BasicWriteByLogDataBuffer(long lLogChannel, WriteLogDataBuffer& data, bool bThreadSafe);

__NS_BASIC_END

#endif 
