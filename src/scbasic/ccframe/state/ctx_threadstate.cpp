#include "ctx_threadstate.h"
#include "../log/ctx_log.h"

CreateTemplateSrc(CCtx_ThreadState)

CCtx_ThreadState::CCtx_ThreadState()
{
	m_strSignName = GlobalGetClassName(CCtx_ThreadState);
}

CCtx_ThreadState::~CCtx_ThreadState()
{

}

int CCtx_ThreadState::InitCtx(CMQMgr* pMQMgr)
{
	int nRet = CCoroutineCtx::InitCtx(pMQMgr);
	if (nRet == 0){
		//60s
		AddOnTimer(6000, OnTimerShowThreadState);
	}
	return nRet;
}
//ҵ����, ȫ��ʹ�þ�̬����
DispatchReturn CCtx_ThreadState::OnTimerShowThreadState(CCoroutineCtx* pCtx, CCorutinePlusThreadData* pData)
{
	CCFrameSCBasicLogEventV("%03d: CorutinePool(%d:%d) ",
		pData->m_dwThreadID, 
		pData->m_pool.GetCreateCorutineTimes(), pData->m_pool.GetVTCorutineSize());
	return DispatchReturn_Success;
}
