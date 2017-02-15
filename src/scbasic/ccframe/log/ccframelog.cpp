#include "ccframelog.h"
#include "../coroutine_ctx.h"

using namespace basiclib;

static int g_nLogLimit = (10 * 1024 * 1024);		//��־�ļ���С����
#define CCFRAMESCBASIC_LOG_MESSAGE_SIZE 512
//////////////////////////////////////////////////////////////////////////////////////
class CCCFrameLog : public CCoroutineCtx
{
public:
	CCCFrameLog(){

	}
	virtual ~CCCFrameLog(){

	}

	void LogEvent(uint32_t nChannel, const char* pszLog){
		//ִ���߳��л�
	}
};
CCCFrameLog m_gFrameLog;


struct WriteLogDataBuffer
{
public:
	WriteLogDataBuffer()
	{
		InitMember();
	}
public:
	void InitLogData(const char* lpszText)
	{
		m_pText = (char*)lpszText;
	}
protected:
	void InitMember()
	{
		memset(this, 0, sizeof(*this));
	}

public:
	char*		m_pText;
	time_t		m_lCurTime;
	DWORD		m_dwProcessId;
	DWORD		m_dwThreadId;
};


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
	basiclib::CTime cur = basiclib::CTime::GetCurrentTime();
	sprintf(szToday, "%d%02d%02d_000", cur.GetYear(), cur.GetMonth(), cur.GetDay());
}
//��־�ļ����Խṹ
class CBasicLogChannel
{
public:
	CBasicLogChannel()
	{
		m_nLogOption = 0;
		m_nSizeLimit = 0;
		m_fLog = NULL;
		m_lReplacePos = 0;

		memset(m_szLogFileName, 0, sizeof(m_szLogFileName));
		memset(m_szToday, 0, sizeof(m_szToday));
	}
public:
	long InitLogChannel(long nOption, const char* lpszLogFile)		//��ʼ��
	{
		_SetLogChannel(nOption, lpszLogFile);
		return OpenLogFile();
	}
	long ChangeLogChannel(long nOption, const char* lpszLogFile)	//��������
	{
		CloseLogFile();

		_SetLogChannel(nOption, lpszLogFile);

		return OpenLogFile();
	}
	void CheckChannel(const char* lpszToday)						//��ʱ��飬����������������ļ������ж��ļ���С�ȡ�
	{
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

	long WriteLogData(const char* lpszText)	//д��־����
	{
		WriteLogDataBuffer logData;
		logData.InitLogData(lpszText);

		_FillLogDataBuffer(logData);

		return _WriteLogDataBuffer(logData);
	}

	void WriteLogBuffer();

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

	void _FillLogDataBuffer(WriteLogDataBuffer& logData)			//����ѡ��������
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

	long _WriteLogDataBuffer(WriteLogDataBuffer& logData)
	{
		FILE* pFile = GetFileHandle();
		if (pFile == NULL)
		{
			return -1;		//�ļ��򿪲��ɹ�
		}
		basiclib::CBasicSmartBuffer smBuf;
		smBuf.SetDataLength(1024);
		smBuf.SetDataLength(0);

		if (logData.m_lCurTime > 0)	//д��ʱ��
		{
			CTime cur(logData.m_lCurTime);
			char szLog[128];
			sprintf(szLog, "%d%02d%02d %02d%02d%02d  ", cur.GetYear(), cur.GetMonth(), cur.GetDay(), cur.GetHour(), cur.GetMinute(), cur.GetSecond());
			smBuf.AppendString(szLog);
		}

		if (logData.m_dwThreadId != 0)	//д���߳�ID
		{
			char szLog[128];
			sprintf(szLog, "pid:%d thread:%d ", (uint32_t)logData.m_dwProcessId, (uint32_t)logData.m_dwThreadId);
			smBuf.AppendString(szLog);
		}

		if (logData.m_pData != NULL && logData.m_lDataLen > 0)
		{
			smBuf.AppendData((char*)logData.m_pData, logData.m_lDataLen);
		}
		if (logData.m_pText != NULL)
		{
			smBuf.AppendString(logData.m_pText);
		}
		smBuf.AppendString("\r\n");
		fwrite(smBuf.GetDataBuffer(), 1, smBuf.GetDataLength(), pFile);
		if (pFile != m_fLog)
		{
			fclose(pFile);
		}
		return 0;
	}

	long _SetLogChannel(long nOption, const char* lpszLogFile)			//����
	{
		m_nLogOption = nOption;
		if (nOption & LOG_BY_NOLIMIT)
		{
			m_nSizeLimit = 0;	//�����ƴ�С
		}
		else
		{
			m_nSizeLimit = (nOption & LOG_SIZE_LIMIT) * 1024 * 1024;
			if (m_nSizeLimit <= 0)
			{
				m_nSizeLimit = g_nLogLimit;	//Ĭ�ϴ�С����
			}
		}

		__tcscpyn(m_szLogFileName, MAX_PATH, lpszLogFile);

		if ((m_nLogOption & LOG_BY_DAY) && !(m_nLogOption & LOG_BY_SAMENAME))
		{
			GetTodayString(m_szToday);
			m_lReplacePos = ReplaceFileName(m_szLogFileName, LOG_NAME_DAY_S, m_szToday);
		}
		else
		{
			m_lReplacePos = -1;
		}

		basiclib::Basic_mkdir(m_szLogFileName);

		return 0;
	}

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
};

long	CBasicLogChannel::m_lLastFileNo = 0;		//���ڵ������ļ��Զ����ɵ��ļ���š�

////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_LOG_CHANNEL		8			//��־�ļ�����

static long g_nCurrentChannel = 0;

CBasicLogChannel* g_pLogChannel[MAX_LOG_CHANNEL] = { 0 };

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
	if (g_pLogChannel[nIndex] == NULL && nIndex == 0)  //Ĭ�ϵ��ڲ�����
	{
		g_pLogChannel[nIndex] = _CreateLogChannel(LOG_BY_DAY | LOG_BY_SIZE | LOG_ADD_TIME | LOG_ADD_THREAD, "basiclibs.log");
	}
	return g_pLogChannel[nIndex];
}
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCFrameSCBasicLogEventV(const char* pszLog, ...)
{
	char tmp[CCFRAMESCBASIC_LOG_MESSAGE_SIZE];
	va_list argList;
	va_start(argList, pszLog);
	int len = vsnprintf(tmp, CCFRAMESCBASIC_LOG_MESSAGE_SIZE, pszLog, argList);
	va_end(argList);
	if (len >= 0 && len < CCFRAMESCBASIC_LOG_MESSAGE_SIZE)
	{
		m_gFrameLog.LogEvent(0, tmp);
	}
	else
	{
		basiclib::CBasicString strLog;
		va_start(argList, pszLog);
		strLog.FormatV(pszLog, argList);
		va_end(argList);
		m_gFrameLog.LogEvent(0, strLog.c_str());
	}
}

void BasicLogEventErrorV(const char* pszLog, ...)
{
	char tmp[CCFRAMESCBASIC_LOG_MESSAGE_SIZE];
	va_list argList;
	va_start(argList, pszLog);
	int len = vsnprintf(tmp, CCFRAMESCBASIC_LOG_MESSAGE_SIZE, pszLog, argList);
	va_end(argList);
	if (len >= 0 && len < CCFRAMESCBASIC_LOG_MESSAGE_SIZE)
	{
		m_gFrameLog.LogEvent(1, tmp);
	}
	else
	{
		basiclib::CBasicString strLog;
		va_start(argList, pszLog);
		strLog.FormatV(pszLog, argList);
		va_end(argList);
		m_gFrameLog.LogEvent(1, strLog.c_str());
	}
}

void CCFrameSCBasicLogEventError(const char* pszLog)
{
	m_gFrameLog.LogEvent(1, pszLog);
}
void CCFrameSCBasicBasicLogEvent(const char* pszLog)
{
	m_gFrameLog.LogEvent(0, pszLog);
}


long CCFrameSCBasicSetDefaultLogEventMode(long nOption, const char* pszLogFile)
{
	if (pszLogFile == NULL || pszLogFile[0] == '\0')
	{
		return LOG_ERROR_NAME_EMPTY;
	}
	if (g_pLogChannel[0] == NULL)
	{
		g_pLogChannel[0] = _CreateLogChannel(nOption, pszLogFile);
	}
	else
	{
		g_pLogChannel[0]->ChangeLogChannel(nOption, pszLogFile);
	}

	if (g_LogThread == NULL)
	{
		g_LogThread = new CWriteLogThread;
		g_LogThread->Start();
	}
	return 0;
}


long CCFrameSCBasicSetDefaultLogEventErrorMode(long nOption, const char* pszLogFile)
{
	if (pszLogFile == NULL || pszLogFile[0] == '\0')
	{
		return LOG_ERROR_NAME_EMPTY;
	}
	if (g_pLogChannel[1] == NULL)
	{
		g_pLogChannel[1] = _CreateLogChannel(nOption, pszLogFile);
	}
	else
	{
		g_pLogChannel[1]->ChangeLogChannel(nOption, pszLogFile);
	}

	if (g_LogThread == NULL)
	{
		g_LogThread = new CWriteLogThread;
		g_LogThread->Start();
	}
	return 0;
}


long BasicSetLogEventMode(long nOption, const char* pszLogFile)
{
	if (g_nCurrentChannel < 0 || g_nCurrentChannel >= MAX_LOG_CHANNEL)
	{
		return LOG_ERROR_FULL;
	}
	if (pszLogFile == NULL || pszLogFile[0] == '\0')
	{
		return LOG_ERROR_NAME_EMPTY;
	}

	int nIndex = basiclib::BasicInterlockedIncrement(&g_nCurrentChannel);
	if (nIndex >= MAX_LOG_CHANNEL)
	{
		return LOG_ERROR_FULL;
	}

	g_pLogChannel[nIndex] = _CreateLogChannel(nOption, pszLogFile);

	if (g_LogThread == NULL)
	{
		g_LogThread = new CWriteLogThread;
		g_LogThread->Start();
	}

	return nIndex;
}
