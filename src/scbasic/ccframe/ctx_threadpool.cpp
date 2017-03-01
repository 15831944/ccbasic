#include "ctx_threadpool.h"
#include <basic.h>
#include "dllmodule.h"
#include "log/ctx_log.h"

////////////////////////////////////////////////////////////////////////////////////////////
CCorutinePlusThreadData::CCorutinePlusThreadData(CCtx_ThreadPool* pThreadPool)
{
	m_pThreadPool = pThreadPool;
	m_globalCToken.InitQueue(pThreadPool->m_globalMQMgrModule);
}

CCorutinePlusThreadData::~CCorutinePlusThreadData()
{
	if (m_pParam){
		m_pThreadPool->m_pReleaseFunc(m_pParam);
	}
}

void CCorutinePlusThreadData::InitThreadData(basiclib::CBasicThreadTLS* pTLS)
{
	m_dwThreadID = basiclib::BasicGetCurrentThreadId();
	pTLS->SetValue(this);
	m_pParam = m_pThreadPool->m_pCreateFunc(this);
}

void CCorutinePlusThreadData::ExecThreadOnWork()
{
	InitThreadData(&m_pThreadPool->m_threadIndexTLS);

	m_pool.InitCorutine(1024);
	CMQMgr* pMgrQueue = &m_pThreadPool->m_globalMQMgrModule;
	CMQMgr* pThreadMQ = &m_threadMgrQueue;
	uint32_t& usPacketNumber = m_pThreadPool->m_nDPacketNumPerTime;
	bool &bRunning = m_pThreadPool->m_bRunning;

	CCtxMessageQueue* pQ = nullptr;
	CCtxMessageQueue* pThreadQ = nullptr;

	CCFrameSCBasicLogEventV(this, "����ExecThreadOnWork %d ThreadID(%d)", basiclib::BasicGetCurrentThreadId(), m_dwThreadID);
	while (bRunning){
		pQ = DispathCtxMsg(pMgrQueue, pQ, usPacketNumber);
		if (pQ == nullptr) {
			pThreadQ = DispathCtxMsg(pThreadMQ, pThreadQ, usPacketNumber);
			if (pThreadQ == nullptr)
				pMgrQueue->WaitForGlobalMQ(100);
		}
	}
	CCFrameSCBasicLogEventV(this, "�˳�ExecThreadOnWork %d ThreadID(%d)", basiclib::BasicGetCurrentThreadId(), m_dwThreadID);
}

CCtxMessageQueue* CCorutinePlusThreadData::DispathCtxMsg(CMQMgr* pMgrQueue, CCtxMessageQueue* pQ, uint32_t& usPacketNumber)
{
	if (pQ == NULL) {
		pQ = pMgrQueue->GlobalMQPop(m_globalCToken);
		if (pQ == NULL)
			return NULL;
	}
	uint32_t nCtxID = pQ->GetCtxID();
	CRefCoroutineCtx pCtx = CSingletonCoroutineCtxHandle::Instance().GetContextByHandleID(nCtxID);
	if (pCtx == nullptr){
		//�����쳣
		CCFrameSCBasicLogEventError(this, "�ַ�����(DispathCtxMsg)�Ҳ���ctxid!!!");
		return pMgrQueue->GlobalMQPop(m_globalCToken);
	}
	if (pCtx->IsReleaseCtx()){
		//��ն���
		moodycamel::ConsumerToken token(*pQ);
		ctx_message msg;
		while (pQ->MQPop(token, msg)){
			msg.FormatMsgQueueToString([&](const char* pData, int nLength)->void{
				CCFrameSCBasicLogEvent(this, pData);
			});
		}
		CSingletonCoroutineCtxHandle::Instance().UnRegister(pCtx.GetResFunc());
		return pMgrQueue->GlobalMQPop(m_globalCToken);
	}

	DispatchReturn nDispathRet = DispatchReturn_Success;
	for (uint32_t i = 0; i < usPacketNumber; i++){
		ctx_message msg;
		if (!pQ->MQPop(msg)){
			return pMgrQueue->GlobalMQPop(m_globalCToken);
		}
		//��������
		nDispathRet = pCtx->DispatchMsg(msg, this);
		if (nDispathRet < DispatchReturn_Success){
			CCFrameSCBasicLogEventErrorV(this, "DispatchMsg���ִ���(%d) Ctx(%d) Name(%s) SignName(%s)", nDispathRet, nCtxID, pCtx->GetCtxName().c_str(), pCtx->GetCtxSignName().c_str());
		}
		else if (nDispathRet == DispatchReturn_LockCtx){
			continue;
		}
		msg.ReleaseData();
	}
	CCtxMessageQueue* pNextQ = pMgrQueue->GlobalMQPop(m_globalCToken);
	if (pNextQ) {
		//�����Ͳ���Ҫ�ŵ�ȫ�ֶ���
		if (nDispathRet != DispatchReturn_LockCtx)
			pMgrQueue->GlobalMQPush(pQ);
		pQ = pNextQ;
	}
	return pQ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CCorutinePlusThreadData* GetCorutinePlusThreadData(basiclib::CBasicThreadTLS* pTLS)
{
	return (CCorutinePlusThreadData*)pTLS->GetValue();
}

///////////////////////////////////////////////////////////////////////////////////////////////

CCtx_ThreadPool* m_pCtxThreadPool;
CCtx_ThreadPool* CCtx_ThreadPool::GetThreadPool(){
	return m_pCtxThreadPool;
}
void CCtx_ThreadPool::CreateThreadPool(CCtx_ThreadPool* pPool){
	m_pCtxThreadPool = pPool;
}
CCorutinePlusThreadData* CCtx_ThreadPool::GetSelfThreadData(){
	return GetCorutinePlusThreadData(&m_pCtxThreadPool->m_threadIndexTLS);
}
//////////////////////////////////////////////////////////////////////////////////////////////
CCtx_ThreadPool::CCtx_ThreadPool()
{
	m_nDPacketNumPerTime = 1;
	m_bRunning = false;
	m_pLog = nullptr;
}

CCtx_ThreadPool::~CCtx_ThreadPool()
{

}

bool CCtx_ThreadPool::Init(const std::function<void*(CCorutinePlusThreadData*)>& pCreateFunc,
	const std::function<void*(void*)>& pReleaseParamFunc)
{
	int nWorkThreadCount = atol(GetCtxInitString(InitGetParamType_Config, "workthread", "4"));
	m_nDPacketNumPerTime = atol(GetCtxInitString(InitGetParamType_Config, "threadworkpacketnumber", "1"));
	//default log
	CCoroutineCtxTemplate* pLogTemplate = m_mgtDllCtx.GetCtxTemplate(GetCtxInitString(InitGetParamType_Config, "logtemplate", ""));
	if (nullptr == pLogTemplate){
		m_pLog = new CCoroutineCtx_Log();
		m_pLog->InitCtx(&m_globalMQMgrModule);
	}
	else{
		pLogTemplate->GetCreate()()->InitCtx(&m_globalMQMgrModule);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//log��������
	m_bRunning = true;

	m_pCreateFunc = pCreateFunc;
	m_pReleaseFunc = pReleaseParamFunc;
	//����indextls
	m_threadIndexTLS.CreateTLS();

	m_ontimerModule.InitTimer();

	DWORD dwThreadID = 0;
	for (int i = 0; i < nWorkThreadCount; i++){
		m_vtHandle.push_back(basiclib::BasicCreateThread(ThreadOnWork, new CCorutinePlusThreadData(this), &dwThreadID));
	}
	m_hMonitor = basiclib::BasicCreateThread(ThreadOnMonitor, this, &dwThreadID);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	basiclib::SetNetInitializeGetParamFunc([](const char* pParam)->basiclib::CBasicString{
		return CCtx_ThreadPool::GetThreadPool()->GetCtxInitString(InitGetParamType_Config, pParam, "4");
	});

	//main module
	CCoroutineCtxTemplate* pMainTemplate = m_mgtDllCtx.GetCtxTemplate(GetCtxInitString(InitGetParamType_Config, "maintemplate", ""));
	if (pMainTemplate == nullptr){
		printf("��ȡ������MainTemplateʧ��%s", GetCtxInitString(InitGetParamType_Config, "maintemplate", ""));
		return false;
	}
	pMainTemplate->GetCreate()()->InitCtx(&m_globalMQMgrModule);

	//����״̬������
	CCoroutineCtxTemplate* pThreadStateTemplate = m_mgtDllCtx.GetCtxTemplate(GetCtxInitString(InitGetParamType_Config, "statetemplate", ""));
	if (pThreadStateTemplate == nullptr){
		CCFrameSCBasicLogEventErrorV("��ȡ������StateTemplateʧ��%s", m_config.m_strStateTemplate.c_str());
		return;
	}

	return true;
}

//! �ȴ��˳�
void CCtx_ThreadPool::Wait()
{
	basiclib::BasicWaitThread(m_hMonitor);
	for (auto& workHandle : m_vtHandle){
		basiclib::BasicWaitThread(workHandle);
	}
	m_ontimerModule.CloseTimer();
	m_ontimerModule.WaitThreadExit();
}

THREAD_RETURN ThreadOnWork(void* pArgv)
{
	//�������
	srand(time(NULL) + basiclib::BasicGetTickTime() + basiclib::BasicGetCurrentThreadId());
	CCorutinePlusThreadData* pThreadData = (CCorutinePlusThreadData*)pArgv;
	pThreadData->ExecThreadOnWork();
	return 0;
}


THREAD_RETURN ThreadOnMonitor(void* pArgv)
{
	//�������
	srand(time(NULL) + basiclib::BasicGetTickTime() + basiclib::BasicGetCurrentThreadId());
	CCtx_ThreadPool* pPool = (CCtx_ThreadPool*)pArgv;
	pPool->ExecThreadOnMonitor();
	return 0;
}
void CCtx_ThreadPool::ExecThreadOnMonitor()
{
	uint32_t& nTotalCtx = CCoroutineCtx::GetTotalCreateCtx();
	CCFrameSCBasicLogEventV("����ExecThreadOnMonitor %d", basiclib::BasicGetCurrentThreadId());
	while (nTotalCtx != 0){
		basiclib::BasicSleep(100);
	}
	m_bRunning = false;
	CCFrameSCBasicLogEventV("�˳�ExecThreadOnMonitor %d", basiclib::BasicGetCurrentThreadId());
}

