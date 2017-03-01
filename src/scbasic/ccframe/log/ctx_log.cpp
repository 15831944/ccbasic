#include "ctx_log.h"
#include "../ctx_threadpool.h"

using namespace basiclib;

CreateTemplateSrc(CCoroutineCtx_Log)

CCoroutineCtx_Log* m_pLog = nullptr;
CCoroutineCtx_Log::CCoroutineCtx_Log(){

}

CCoroutineCtx_Log::~CCoroutineCtx_Log(){

}

int CCoroutineCtx_Log::InitCtx(CMQMgr* pMQMgr){
	m_pLog = this;
	basiclib::InitBasicLog(false, false);
	int nRet = CCoroutineCtx::InitCtx(pMQMgr);
	if (nRet == 0){
		//30s
		AddOnTimer(3000, OnTimerBasicLog);
		basiclib::CBasicString strDefaultLogPath = basiclib::BasicGetModulePath() + "/log/";
		basiclib::CBasicString strLogPath = CCtx_ThreadPool::GetThreadPool()->GetCtxInitString(InitGetParamType_Config, "logpath", strDefaultLogPath.c_str());
		basiclib::CBasicString strDefaultKey = "ccframelog";
		basiclib::CBasicString strKey = CCtx_ThreadPool::GetThreadPool()->GetCtxInitString(InitGetParamType_Config, "ccframekey", strDefaultKey.c_str());
		basiclib::CBasicString strDefaultLogFileName = strLogPath + strKey + ".log";
		basiclib::CBasicString strDefaultErrorFileName = strLogPath + strKey + ".error";

		basiclib::BasicSetDefaultLogEventMode(0, strDefaultLogFileName.c_str());
		basiclib::BasicSetDefaultLogEventErrorMode(0, strDefaultErrorFileName.c_str());

	}
	return nRet;
}

void CCoroutineCtx_Log::LogEvent(CCorutinePlusThreadData* pThreadData, int nChannel, const char* pszLog){
	basiclib::WriteLogDataBuffer logData;
	logData.InitLogData(pszLog);;
	logData.m_lCurTime = time(NULL);
	logData.m_dwProcessId = basiclib::Basic_GetCurrentProcessId();
	logData.m_dwThreadId = basiclib::BasicGetCurrentThreadId();
	if (pThreadData){
		LogEventCorutine(pThreadData, nChannel, logData);
	}
	else{
		CCorutinePlusThreadData* pThreadData = CCtx_ThreadPool::GetSelfThreadData();
		if (pThreadData){
			LogEventCorutine(pThreadData, nChannel, logData);
		}
		else{
			int nLength = sizeof(basiclib::WriteLogDataBuffer) + strlen(pszLog);
			basiclib::CBasicSmartBuffer smBuf;
			smBuf.SetDataLength(nLength);
			smBuf.SetDataLength(0);
			smBuf.AppendData((char*)&logData, sizeof(basiclib::WriteLogDataBuffer));
			smBuf.AppendData(pszLog, nLength - sizeof(basiclib::WriteLogDataBuffer));
			logData.InitLogData(smBuf.GetDataBuffer() + sizeof(basiclib::WriteLogDataBuffer));

			ctx_message ctxMsg(0, [](CCoroutineCtx* pCtx, ctx_message* pMsg, CCorutinePlusThreadData* pData)->void{
				basiclib::WriteLogDataBuffer* pWriteBuf = (basiclib::WriteLogDataBuffer*)pMsg->m_data;
				//д��־
				basiclib::BasicWriteByLogDataBuffer(pMsg->m_session, *pWriteBuf);
			}, nChannel);
			ctxMsg.ExportFromSmartBuffer(smBuf);
			m_ctxMsgQueue.MQPush(ctxMsg);
		}
	}
}

void CCoroutineCtx_Log::LogEventCorutine(CCorutinePlusThreadData* pThreadData, int nChannel, basiclib::WriteLogDataBuffer& logData){
	CCorutinePlus* pCorutine = pThreadData->m_pool.GetCorutine();
	pCorutine->ReInit([](CCorutinePlus* pCorutine)->void{
		bool bFree = false;
		const int nDefaultSize = 256;
		char szBuf[nDefaultSize] = { 0 };
		basiclib::WriteLogDataBuffer bufSelf;
		char* pBuffer = szBuf;
		{
			bufSelf = *pCorutine->GetResumeParamPoint<basiclib::WriteLogDataBuffer>(0);
			int nLength = strlen(bufSelf.m_pText);
			if (nLength > nDefaultSize){
				bFree = true;
				pBuffer = (char*)basiclib::BasicAllocate(nLength);
				memcpy(pBuffer, bufSelf.m_pText, nLength);
			}
			else{
				memcpy(szBuf, bufSelf.m_pText, nLength);
			}
			bufSelf.InitLogData(pBuffer);
		}
		int nChannel = pCorutine->GetResumeParam<int>(1);
		pCorutine->YieldCorutine();
		//д��־
		basiclib::BasicWriteByLogDataBuffer(nChannel, bufSelf);
		if (bFree){
			basiclib::BasicDeallocate(pBuffer);
		}
	});
	pCorutine->Resume(&pThreadData->m_pool, logData, nChannel);
	//ִ��
	ctx_message ctxMsg(0, pCorutine);
	m_ctxMsgQueue.MQPush(ctxMsg);
}

DispatchReturn CCoroutineCtx_Log::OnTimerBasicLog(CCoroutineCtx* pCtx, CCorutinePlusThreadData* pData){
	basiclib::OnTimerBasicLog();
	return DispatchReturn_Success;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CCFRAME_LOG_MESSAGE_SIZE 256
void CCFrameSCBasicLogEvent(const char* pszLog){
	m_pLog->LogEvent(nullptr, 0, pszLog);
}
void CCFrameSCBasicLogEventError(const char* pszLog){
	m_pLog->LogEvent(nullptr, 1, pszLog);
}

void CCFrameSCBasicLogEventV(CCorutinePlusThreadData* pThreadData, const char* pszLog, ...){
	char tmp[CCFRAME_LOG_MESSAGE_SIZE];
	va_list argList;
	va_start(argList, pszLog);
	int len = vsnprintf(tmp, CCFRAME_LOG_MESSAGE_SIZE, pszLog, argList);
	va_end(argList);
	if (len >= 0 && len < CCFRAME_LOG_MESSAGE_SIZE){
		m_pLog->LogEvent(pThreadData, 0, tmp);
	}
	else{
		CBasicString strLog;
		va_start(argList, pszLog);
		strLog.FormatV(pszLog, argList);
		va_end(argList);
		m_pLog->LogEvent(pThreadData, 0, strLog.c_str());
	}
}
void CCFrameSCBasicLogEventErrorV(CCorutinePlusThreadData* pThreadData, const char* pszLog, ...){
	char tmp[CCFRAME_LOG_MESSAGE_SIZE];
	va_list argList;
	va_start(argList, pszLog);
	int len = vsnprintf(tmp, CCFRAME_LOG_MESSAGE_SIZE, pszLog, argList);
	va_end(argList);
	if (len >= 0 && len < CCFRAME_LOG_MESSAGE_SIZE){
		m_pLog->LogEvent(pThreadData, 1, tmp);
	}
	else{
		CBasicString strLog;
		va_start(argList, pszLog);
		strLog.FormatV(pszLog, argList);
		va_end(argList);
		m_pLog->LogEvent(pThreadData, 1, strLog.c_str());
	}
}
void CCFrameSCBasicLogEvent(CCorutinePlusThreadData* pThreadData, const char* pszLog){
	m_pLog->LogEvent(pThreadData, 0, pszLog);
}
void CCFrameSCBasicLogEventError(CCorutinePlusThreadData* pThreadData, const char* pszLog){
	m_pLog->LogEvent(pThreadData, 1, pszLog);
}
