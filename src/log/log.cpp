#include "../inc/basic.h"

//#include <stdio.h>
//#include <stdarg.h>
//#include <iostream>
//#include <stdlib.h>
//#include <vector>

#if defined(__LINUX) || defined(__MAC) || defined(__ANDROID)
#include <syslog.h>
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////
__NS_BASIC_START

static int g_nLogLimit = (10*1024*1024);		//��־�ļ���С����
#define LOG_MESSAGE_SIZE 256

static long ReplaceFileName(char* szFileName, const char* lpszKey, const char* lpszToday)
{
	int nNameLen = __tcslen(szFileName);
	int nDayLen = __tcslen(lpszToday);
	char* pKeyPos = strstr(szFileName, lpszKey);
	char* pKeyEnd = NULL;
	int nLeft = 0;
	if (pKeyPos != NULL)
	{
		pKeyEnd = pKeyPos + __tcslen(lpszKey);
	}
	else
	{
		pKeyPos = strrchr(szFileName, '.');
		if (pKeyPos == NULL)
		{
			pKeyPos = &szFileName[nNameLen];
			*pKeyPos = '.';
			pKeyPos++;
			pKeyEnd = pKeyPos;
		}
		else
		{
			pKeyEnd = pKeyPos;
		}
	}
	nLeft = nNameLen - (pKeyEnd - szFileName);
	if (nLeft > 0)
	{
		char* pNewPos = pKeyPos + nDayLen;
		memmove(pNewPos, pKeyEnd, nLeft * sizeof(char));
	}
	__tcsncpy(pKeyPos, lpszToday, nDayLen);

	return (pKeyPos - szFileName);
}

static void GetTodayString(char* szToday)
{
	CTime cur = CTime::GetCurrentTime();
	sprintf(szToday, "%d%02d%02d_000", cur.GetYear(), cur.GetMonth(), cur.GetDay());
}

//��־�ļ����Խṹ
typedef basiclib::basic_vector<WriteLogDataBuffer> vector_logdata;
class CBasicLogChannel : public basiclib::CBasicObject
{
public:
	CBasicLogChannel(){
		m_nLogOption = 0;
		m_nSizeLimit = 0;
		m_fLog = NULL;
		m_lReplacePos = 0;

		memset(m_szLogFileName, 0, sizeof(m_szLogFileName));
		memset(m_szToday, 0, sizeof(m_szToday));

		m_pSynLogFile = NULL;
	}
	virtual ~CBasicLogChannel(){
		if (m_pSynLogFile){
			delete m_pSynLogFile;
		}
	}
public:
	long InitLogChannel(long nOption, const char* lpszLogFile);		//��ʼ��
	long ChangeLogChannel(long nOption, const char* lpszLogFile);	//��������
	void CheckChannel(const char* lpszToday);						//��ʱ��飬����������������ļ������ж��ļ���С�ȡ�

	long WriteLogData(const char* lpszText);	//д��־����
	void WriteLogDataWithBuffer(WriteLogDataBuffer& logData, bool bThreadSafe);

	void WriteLogBuffer();
	static void SetLockChannel(bool bLock){
		m_bInitLock = bLock;
	}
protected:
	long OpenLogFile()
	{
		if (m_nLogOption & LOG_BY_OPEN)
		{
			m_fLog = fopen(m_szLogFileName, "ab");
			if (m_fLog == NULL)
			{
				return LOG_ERROR_OPEN_FILE;
			}
		}
		return 0;
	}
	void CloseLogFile()
	{
		if (m_fLog != NULL)
		{
			fclose(m_fLog);
			m_fLog = NULL;
		}
	}
	FILE* GetFileHandle()
	{
		if (m_fLog == NULL)
		{
			return fopen(m_szLogFileName, "ab");
		}
		return m_fLog;
	}

	void CreateNewFileName()
	{
		if (m_lReplacePos < 0)
		{
			m_lReplacePos = ReplaceFileName(m_szLogFileName, LOG_NAME_DAY_S, m_szToday);
		}
		char* pPos = &m_szLogFileName[m_lReplacePos + 9];		//��ſ�ʼ��λ��
		char szTemp[32];
		do
		{
			sprintf(szTemp, "%04d", (uint32_t)GetNewFileNo());
			strncpy(pPos, szTemp, 4);
			if (_access(m_szLogFileName, 0) != 0)
			{
				strncpy(&m_szToday[9], szTemp, 3);
				break;
			}
		} while (1);
	}

	void _FillLogDataBuffer(WriteLogDataBuffer& logData);			//����ѡ��������
	void _AddLogDataBuffer(WriteLogDataBuffer& logData);			//���ӵ���������
	long _WriteLogDataBuffer(WriteLogDataBuffer& logData);

	long _SetLogChannel(long nOption, const char* lpszLogFile);			//����

	static long GetNewFileNo()
	{
		return BasicInterlockedIncrement(&m_lLastFileNo);
	}

private:
	long	m_nLogOption;					//��־�����ԣ������� LOG_*
	long	m_nSizeLimit;					//��־�ļ��Ĵ�С���ơ�0��ʾ������
	FILE*	m_fLog;							//��־�ļ����
	char	m_szLogFileName[MAX_PATH];
	char	m_szToday[16];					//��������ں���š���ʽ��YYMMDD_XXX  �� 20110720_000
	long	m_lReplacePos;					//�滻���ڻ�����ŵ���ʼλ��

	static long	m_lLastFileNo;				//���ڵ������ļ��Զ����ɵ��ļ���š�
	static bool m_bInitLock;				//�Ƿ�����

	CCriticalSection* m_pSynLogFile;		//������Ա�������ٽ���

	vector_logdata		m_vLogData;			//���������
    basiclib::SpinLock  m_lock;
};

long	CBasicLogChannel::m_lLastFileNo = 0;		//���ڵ������ļ��Զ����ɵ��ļ���š�
bool	CBasicLogChannel::m_bInitLock = true;		//Ĭ������
static  bool m_bInitLogFunc = false;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_LOG_CHANNEL		8			//��־�ļ�����

static long g_nCurrentChannel = 0;

static CBasicLogChannel* g_pLogChannel[MAX_LOG_CHANNEL] = { 0 };
static DebugLevel g_DebugLevel = DebugLevel_Info;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CWriteLogThread : public CBasic_Thread
{
public:
	CWriteLogThread()
	{
		m_bRuning = false;
	}
	~CWriteLogThread()
	{
		Stop();
	}
public:
	virtual void Run()
	{
		int nCount = 0;
		while (m_bRuning)
		{
			OnTimerBasicLog();
			BasicSleep(30000);		//��Ϣ 30 ����
		}
	}
	void Start()
	{
		m_bRuning = true;
		CBasic_Thread::Start();
	}
	void Stop()
	{
		m_bRuning = false;
		Join();
		OnTimerBasicLog();
	}
protected:
	volatile bool m_bRuning;
};

static CWriteLogThread* g_LogThread = NULL;

long CBasicLogChannel::InitLogChannel(long nOption, const char* lpszLogFile)
{
	_SetLogChannel(nOption, lpszLogFile);
	long lRet = OpenLogFile();
	if (lRet == 0)
	{
		if (m_bInitLock)
			m_pSynLogFile = new CCriticalSection;
	}

	return lRet;
}

long CBasicLogChannel::ChangeLogChannel(long nOption, const char* lpszLogFile)
{
	CSingleLock lock(m_pSynLogFile);
	lock.Lock();

	CloseLogFile();

	_SetLogChannel(nOption, lpszLogFile);

	return OpenLogFile();
}

long CBasicLogChannel::_SetLogChannel(long nOption, const char* lpszLogFile)
{
	m_nLogOption = nOption;
	if (nOption & LOG_BY_NOLIMIT){
		m_nSizeLimit = 0;	//�����ƴ�С
	}
	else{
		m_nSizeLimit = (nOption & LOG_SIZE_LIMIT) * 1024 * 1024;
		if (m_nSizeLimit <= 0){
			m_nSizeLimit = g_nLogLimit;	//Ĭ�ϴ�С����
		}
	}

	__tcscpyn(m_szLogFileName, MAX_PATH, lpszLogFile);

	GetTodayString(m_szToday);
	if ((m_nLogOption & LOG_BY_DAY) && !(m_nLogOption & LOG_BY_SAMENAME))
	{
		m_lReplacePos = ReplaceFileName(m_szLogFileName, LOG_NAME_DAY_S, m_szToday);
	}
	else
	{
		m_lReplacePos = -1;
	}

	Basic_mkdir(m_szLogFileName);

	return 0;
}

void CBasicLogChannel::CheckChannel(const char* lpszToday)
{
	CSingleLock lock(m_pSynLogFile);
	lock.Lock();
	if (m_nLogOption & LOG_BY_DAY)
	{
		if (strncmp(m_szToday, lpszToday, 8) != 0)		//���ڲ�һ�£���Ҫ�����ļ���
		{
			CloseLogFile();
			if (m_nLogOption & LOG_BY_ONEFILE)
			{
				Basic_DeleteFile(m_szLogFileName);
			}
			if (!(m_nLogOption & LOG_BY_SAMENAME))
			{
				m_lReplacePos = ReplaceFileName(m_szLogFileName, m_szToday, lpszToday);
			}
			strcpy(m_szToday, lpszToday);
			OpenLogFile();
		}
	}

	TLFileStatus fStatus;
	Basic_GetFileStatus(m_szLogFileName, fStatus);
	if (m_nSizeLimit > 0 && fStatus.m_size > m_nSizeLimit)		//���ȳ�������
	{
		CloseLogFile();
		if (m_nLogOption & LOG_BY_ONEFILE)
		{
			Basic_DeleteFile(m_szLogFileName);
		}
		CreateNewFileName();
		OpenLogFile();
	}
	WriteLogBuffer();
}

void CBasicLogChannel::WriteLogBuffer()
{
    vector_logdata tmpVt;

    tmpVt.resize(2);
    tmpVt.clear();
    {
        CSingleLock lock(m_pSynLogFile);
        lock.Lock();
        basiclib::CSpinLockFuncNoSameThreadSafe spinLock(&m_lock, TRUE);
        swap(tmpVt, m_vLogData);
    }
    int nCount = tmpVt.size();
    for (int i = 0; i < nCount; i++)
    {
        WriteLogDataBuffer& log = tmpVt[i];
        _WriteLogDataBuffer(log);
        log.ClearLogData();
    }
}

long CBasicLogChannel::WriteLogData(const char* lpszText)
{
	WriteLogDataBuffer logData;
	logData.InitLogData(lpszText);

	_FillLogDataBuffer(logData);

	CSingleLock lock(m_pSynLogFile);
	lock.Lock();

    if (g_LogThread){
        if (m_nLogOption & LOG_BY_BUFFER){
            _AddLogDataBuffer(logData);
        }
        else{
            return _WriteLogDataBuffer(logData);
        }
    }
    else{
        //���Բ�ͬ�̲߳Ż��������
        basiclib::CSpinLockFuncNoSameThreadSafe spinLock(&m_lock, TRUE);
        _AddLogDataBuffer(logData);
    }
	return 0;
}
void CBasicLogChannel::WriteLogDataWithBuffer(WriteLogDataBuffer& logData, bool bThreadSafe)
{
    if (g_LogThread){
        CSingleLock lock(m_pSynLogFile);
        lock.Lock();
        if (m_nLogOption & LOG_BY_BUFFER){
            _AddLogDataBuffer(logData);
        }
        else{
            _WriteLogDataBuffer(logData);
        }
    }
    else{
        if (bThreadSafe){
            _WriteLogDataBuffer(logData);
        }
        else{
            //���Բ�ͬ�̲߳Ż��������
            basiclib::CSpinLockFuncNoSameThreadSafe spinLock(&m_lock, TRUE);
            _AddLogDataBuffer(logData);
        }
    }
}

void CBasicLogChannel::_FillLogDataBuffer(WriteLogDataBuffer& logData)
{
	if (m_nLogOption & LOG_ADD_TIME)
	{
		logData.m_lCurTime = time(NULL);
	}
	if (m_nLogOption & LOG_ADD_THREAD)
	{
		logData.m_dwProcessId = Basic_GetCurrentProcessId();
		logData.m_dwThreadId = BasicGetCurrentThreadId();
	}
}

void CBasicLogChannel::_AddLogDataBuffer(WriteLogDataBuffer& logData)
{
	WriteLogDataBuffer thisLogData;
	thisLogData.CopyLogData(logData);

    m_vLogData.push_back(thisLogData);
}

long CBasicLogChannel::_WriteLogDataBuffer(WriteLogDataBuffer& logData)
{
	FILE* pFile = GetFileHandle();
	if (pFile == NULL)
	{
		return -1;		//�ļ��򿪲��ɹ�
	}
	static basiclib::CBasicSmartBuffer smBufCache;
	basiclib::CBasicSmartBuffer smBuf;
	basiclib::CBasicSmartBuffer* pWriteBuf = nullptr;
	if (m_bInitLock){
		pWriteBuf = &smBuf;
		pWriteBuf->SetDataLength(1024);
		pWriteBuf->SetDataLength(0);
	}
	else{
		pWriteBuf = &smBufCache;
		pWriteBuf->SetDataLength(0);
	}

	if (logData.m_lCurTime > 0)	//д��ʱ��
	{
		CTime cur(logData.m_lCurTime);
		char szLog[64];
		sprintf(szLog, "%d%02d%02d %02d%02d%02d  ", cur.GetYear(), cur.GetMonth(), cur.GetDay(), cur.GetHour(), cur.GetMinute(), cur.GetSecond());
		pWriteBuf->AppendString(szLog);
	}

	if (logData.m_dwThreadId != 0)	//д���߳�ID
	{
		char szLog[64];
		sprintf(szLog, "pid:%d thread:%d ", (uint32_t)logData.m_dwProcessId, (uint32_t)logData.m_dwThreadId);
		pWriteBuf->AppendString(szLog);
	}

	if (logData.m_pText != NULL)
	{
		pWriteBuf->AppendString(logData.m_pText);
	}
	pWriteBuf->AppendString("\r\n");
	fwrite(pWriteBuf->GetDataBuffer(), 1, pWriteBuf->GetDataLength(), pFile);
	if (pFile != m_fLog)
	{
		fclose(pFile);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static CBasicLogChannel* _CreateLogChannel(long nOption, const char* pszLogFile)
{
	CBasicLogChannel* pChannel = new CBasicLogChannel;
	long lRet = pChannel->InitLogChannel(nOption, pszLogFile);
	if (lRet < 0)
	{
		delete pChannel;
		pChannel = NULL;
	}
	return pChannel;
}

CBasicLogChannel* _GetChannel(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_LOG_CHANNEL)
	{
		return NULL;
	}
	if (g_pLogChannel[nIndex] == NULL)  //Ĭ�ϵ��ڲ�����
	{
		if(nIndex == 0)
			g_pLogChannel[nIndex] = _CreateLogChannel(LOG_BY_DAY | LOG_BY_SIZE | LOG_ADD_TIME | LOG_ADD_THREAD, "basiclibs.log");
		else if(nIndex == 1)
			g_pLogChannel[nIndex] = _CreateLogChannel(LOG_BY_DAY | LOG_BY_SIZE | LOG_ADD_TIME | LOG_ADD_THREAD, "basiclibserror.log");
	}
	return g_pLogChannel[nIndex];
}


long BasicSetLogEventMode(long nOption, const char* pszLogFile)
{
    //Ĭ���Լ������̣߳��̰߳�ȫ
    InitBasicLog(true);
	if (g_nCurrentChannel < 0 || g_nCurrentChannel >= MAX_LOG_CHANNEL)
	{
		return LOG_ERROR_FULL;
	}
	if (pszLogFile == NULL || pszLogFile[0] == '\0')
	{
		return LOG_ERROR_NAME_EMPTY;
	}

	int nIndex = BasicInterlockedIncrement(&g_nCurrentChannel);
	if (nIndex >= MAX_LOG_CHANNEL)
	{
		return LOG_ERROR_FULL;
	}

	g_pLogChannel[nIndex] = _CreateLogChannel(nOption, pszLogFile);

	return nIndex;
}

long BasicSetDefaultLogEventErrorMode(long nOption, const char* pszLogFile)
{
    //Ĭ���Լ������̣߳��̰߳�ȫ
    InitBasicLog(true);
	if (pszLogFile == NULL || pszLogFile[0] == '\0')
	{
		return LOG_ERROR_NAME_EMPTY;
	}  
	if (g_pLogChannel[1] == NULL)
	{
        BasicInterlockedIncrement(&g_nCurrentChannel);
		g_pLogChannel[1] = _CreateLogChannel(nOption, pszLogFile);
	}
	else
	{
		g_pLogChannel[1]->ChangeLogChannel(nOption, pszLogFile);
	}
	return 0;
}

long BasicSetDefaultLogEventMode(long nOption, const char* pszLogFile)
{
    //Ĭ���Լ������̣߳��̰߳�ȫ
    InitBasicLog(true);
	if (pszLogFile == NULL || pszLogFile[0] == '\0')
	{
		return LOG_ERROR_NAME_EMPTY;
	}
	if (g_pLogChannel[0] == NULL)
	{
        BasicInterlockedIncrement(&g_nCurrentChannel);
		g_pLogChannel[0] = _CreateLogChannel(nOption, pszLogFile);
	}
	else
	{
		g_pLogChannel[0]->ChangeLogChannel(nOption, pszLogFile);
	}


	return 0;
}

long BasicCloseLogEvent(long lLogChannel)
{
	if(lLogChannel < 0)		//�ر����е�
	{
		if(g_LogThread != NULL)
		{
			g_LogThread->Stop();
		}
		g_nCurrentChannel = 0;			//���������ڴ�й¶
	}
	else if(lLogChannel >= 0 || lLogChannel < MAX_LOG_CHANNEL)
	{
		if(g_pLogChannel[lLogChannel] != NULL)
		{
			g_pLogChannel[lLogChannel]->WriteLogBuffer();
			g_pLogChannel[lLogChannel] = NULL;		//���������ڴ�й¶
		}
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//�¼���¼��д��Ĭ����־�ļ���
//
void BasicLogEventErrorV(const char* pszLog, ...)
{
	char tmp[LOG_MESSAGE_SIZE];
	va_list argList;
	va_start(argList, pszLog);
	int len = vsnprintf(tmp, LOG_MESSAGE_SIZE, pszLog, argList);
	va_end(argList);
	if (len >= 0 && len < LOG_MESSAGE_SIZE)
	{
        BasicLogEvent(DebugLevel_Error, 1, tmp);
	}
	else
	{
		CBasicString strLog;
		va_start(argList, pszLog);
		strLog.FormatV(pszLog, argList);
		va_end(argList);
        BasicLogEvent(DebugLevel_Error, 1, strLog.c_str());
	}
}
void BasicLogEventV(DebugLevel level, const char* pszLog, ...)
{
	char tmp[LOG_MESSAGE_SIZE];
	va_list argList;
	va_start(argList, pszLog);
	int len = vsnprintf(tmp, LOG_MESSAGE_SIZE, pszLog, argList);
	va_end(argList);
	if (len >= 0 && len < LOG_MESSAGE_SIZE)
	{
        BasicLogEvent(level, 0, tmp);
	}
	else
	{
		CBasicString strLog;
		va_start(argList, pszLog);
		strLog.FormatV(pszLog, argList);
		va_end(argList);
        BasicLogEvent(level, 0, strLog.c_str());
	}
}

void BasicLogEventError(const char* pszLog)
{
    BasicLogEvent(DebugLevel_Error, 1, pszLog);
}
void BasicLogEvent(DebugLevel level, const char* pszLog)
{
    BasicLogEvent(level, 0, pszLog);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

void BasicLogEventV(DebugLevel level, long lLogChannel, const char* pszLog, ...)
{
	char tmp[LOG_MESSAGE_SIZE];
	va_list argList;
	va_start(argList, pszLog);
	int len = vsnprintf(tmp, LOG_MESSAGE_SIZE, pszLog, argList);
	va_end(argList);
	if (len >= 0 && len < LOG_MESSAGE_SIZE)
	{
        BasicLogEvent(level, lLogChannel, tmp);
	}
	else
	{
		CBasicString strLog;
		va_start(argList, pszLog);
		strLog.FormatV(pszLog, argList);
		va_end(argList);
        BasicLogEvent(level, lLogChannel, strLog.c_str());
	}
}


//
//�¼���¼��д����־�ļ���
//
void BasicLogEvent(DebugLevel level, long lLogChannel, const char* pszLog)
{
	if (pszLog == NULL || pszLog[0] == '\0')
	{
		return;
	}

	TRACE("LOG%d:%s\r\n", lLogChannel, pszLog);

    if (level > g_DebugLevel)
        return;
	//����Ҳ���ֱ������������
	_GetChannel(lLogChannel)->WriteLogData(pszLog);
}

void BasicWriteByLogDataBuffer(long lLogChannel, WriteLogDataBuffer& data, bool bThreadSafe)
{
    _GetChannel(lLogChannel)->WriteLogDataWithBuffer(data, bThreadSafe);
}

void InitBasicLogLevel(DebugLevel level)
{
    g_DebugLevel = level;
}
DebugLevel GetBasicLogLevel()
{
    return g_DebugLevel;
}

//! bThreadCheckSelf true ��������(�޷�ʵʱ��¼) false������ �̰߳�ȫ(������bufferģʽʵʱ)
bool InitBasicLog(bool bThreadCheckSelf)
{

    if (m_bInitLogFunc){
        return false;
    }

    m_bInitLogFunc = true;
    CBasicLogChannel::SetLockChannel(bThreadCheckSelf);
    if (bThreadCheckSelf){
        if (g_LogThread == NULL)
        {
            g_LogThread = new CWriteLogThread;
            g_LogThread->Start();
        }
	}
	else{
		if (g_LogThread){
			g_LogThread->Stop();
			delete g_LogThread;
		}
	}
    return true;
}

//! ��������Լ��߳���Ҫ30s����һ��
void OnTimerBasicLog()
{
	int nSize = g_nCurrentChannel + 1;
	if (nSize > MAX_LOG_CHANNEL)
	{
		nSize = MAX_LOG_CHANNEL;
	}
	if (nSize > 1)
	{
		char szToday_s[32];
		GetTodayString(szToday_s);
		for (int i = 0; i < nSize; i++)
		{
			if (g_pLogChannel[i] != NULL)
			{
				g_pLogChannel[i]->CheckChannel(szToday_s);
			}
		}
	}
}

WriteLogDataBuffer::WriteLogDataBuffer()
{
    InitMember();
}
WriteLogDataBuffer::~WriteLogDataBuffer()
{
}
void WriteLogDataBuffer::InitLogData(const char* lpszText)
{
    m_pText = (char*)lpszText;
}
void WriteLogDataBuffer::CopyLogData(WriteLogDataBuffer& logData)
{
    if (logData.m_pText != NULL)
    {
        int nLen = __tcslen(logData.m_pText);
        m_pText = (char*)BasicAllocate((nLen + 1) * sizeof(char));
        __tcscpy(m_pText, logData.m_pText);
    }
    m_lCurTime = logData.m_lCurTime;
    m_dwProcessId = logData.m_dwProcessId;
    m_dwThreadId = logData.m_dwThreadId;
}
void WriteLogDataBuffer::ClearLogData()
{
    if (m_pText != NULL)
    {
        BasicDeallocate(m_pText);
    }
    InitMember();
}
void WriteLogDataBuffer::InitMember()
{
    m_pText = nullptr;
    m_lCurTime = 0;
    m_dwProcessId = 0;
    m_dwThreadId = 0;
}

__NS_BASIC_END
