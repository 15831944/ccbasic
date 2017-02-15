/***********************************************************************************************
// �ļ���:     ctx_msgqueue.h
// ������:     ������
// Email:      zqcai@w.cn
// ��������:   ����Э�̺��첽��Э�̳�
// �汾��Ϣ:   1.0V
************************************************************************************************/

#ifndef SCBASIC_CTX_THREADPOOL_H
#define SCBASIC_CTX_THREADPOOL_H

#include "ctx_handle.h"

//call in the thread create
class basiclib::CBasicThreadTLS;
class CCtx_ThreadPool;
class _BASIC_DLL_API CCorutinePlusThreadData : public basiclib::CBasicObject
{
public:
	CCorutinePlusThreadData(CCtx_ThreadPool* pThreadPool);
	virtual ~CCorutinePlusThreadData();

	void InitThreadData(basiclib::CBasicThreadTLS* pTLS);

	//ִ��
	void ExecThreadOnWork();
protected:
	CCtxMessageQueue* DispathCtxMsg(CMQMgr* pMgrQueue, CCtxMessageQueue* pQ, uint32_t& usPacketNumber);
public:
	CCtx_ThreadPool*									m_pThreadPool;
	//ȫ����Ϣ������token
	moodycamel::ConsumerToken							m_globalCToken;

	DWORD												m_dwThreadID;
	CCorutinePlusPool									m_pool;
	CMQMgr												m_threadMgrQueue;
	void*												m_pParam;
};
CCorutinePlusThreadData* GetCorutinePlusThreadData(basiclib::CBasicThreadTLS* pTLS);

typedef fastdelegate::FastDelegate0<void*> OnCreateUDData;
class _SCBASIC_DLL_API CCtx_ThreadPool : basiclib::CBasicObject
{
public:
	CCtx_ThreadPool();
	virtual ~CCtx_ThreadPool();

	//! ��ʼ��
	bool Init(int nWorkThreadCount, 
		const std::function<void*(CCorutinePlusThreadData*)>& pCreateFunc, 
		const std::function<void*(void*)>& pReleaseParamFunc);
	//! �ȴ��˳�
	void Wait();
	//! ���������߳�

public:
	basiclib::CBasicThreadTLS& GetTLS(){ return m_threadIndexTLS; }
	CBasicOnTimer& GetOnTimerModule();
	CMQMgr& GetGlobalMQMgr(){ return m_globalMQMgrModule; }
protected:
	friend THREAD_RETURN ThreadOnWork(void*);
	
	friend THREAD_RETURN ThreadOnMonitor(void*);
	void ExecThreadOnMonitor();
protected:
	bool														m_bRunning;
	basiclib::CBasicThreadTLS									m_threadIndexTLS;
	CBasicOnTimer												m_ontimerModule;			//ִ��ontimer callback
	basiclib::basic_vector<HANDLE>								m_vtHandle;
	HANDLE														m_hMonitor;

	CMQMgr														m_globalMQMgrModule;
	uint32_t													m_nDPacketNumPerTime;

	std::function<void*(CCorutinePlusThreadData*)>				m_pCreateFunc;
	std::function<void*(void*)>									m_pReleaseFunc;
												

	friend class CCoroutineCtx;
	friend class CCorutinePlusThreadData;
};

#endif