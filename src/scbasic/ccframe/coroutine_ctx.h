/***********************************************************************************************
// �ļ���:     coroutine_ctx.h
// ������:     ������
// Email:      zqcai@w.cn
// ��������:   Э����Դ����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef SCBASIC_COROUTINE_CTX_H
#define SCBASIC_COROUTINE_CTX_H

#include "ctx_msgqueue.h"

enum DispatchReturn
{
	DispatchReturn_Error = -1,
	DispatchReturn_Success = 0, //�����������ɲ���Ҫ����
	DispatchReturn_Release = 1,	//����release
	DispatchReturn_LockCtx = 1,	//��������ctx�����ڲ��ͷ�	
};

#pragma warning (push)
#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
typedef const char*(*pInitGetParamFunc)(int, const char*, const char*);
typedef fastdelegate::FastDelegate0<void> HandleOnTimer;			//������Ϣ
typedef void(*pCallOnTimerFunc)();
class CCoroutineCtx;
typedef DispatchReturn(*pCallbackOnTimerFunc)(CCoroutineCtx* pCtx);
class _SCBASIC_DLL_API CCoroutineCtx : public basiclib::CBasicObject, public basiclib::EnableRefPtr<CCoroutineCtx>
{
public:
	CCoroutineCtx(const char* pName = nullptr);
	virtual ~CCoroutineCtx();

	/*  ��ʼ��0����ɹ�
		�ص�����
		0 ���� ��lua�ļ�ȡ
	*/
	virtual int InitCtx(pInitGetParamFunc pFunc, CMQMgr* pMQMgr);
	//! ����Ҫ�Լ�delete��ֻҪ����release
	virtual void ReleaseCtx();
	/*	�������� <0�������*/
	virtual DispatchReturn DispatchMsg(ctx_message& msg, CCorutinePlusThreadData& data);
	//! ����timer
	bool AddOnTimer(int nTimes, pCallbackOnTimerFunc pCallback);
	bool AddOnTimeOut(int nTimes, pCallbackOnTimerFunc pCallback);
	void DelTimer(pCallbackOnTimerFunc pCallback);
public:
	uint32_t GetCtxID(){ return m_ctxID; }
	basiclib::CBasicString& GetCtxName(){ return m_strCtxName; }
	basiclib::CBasicString& GetCtxSignName(){ return m_strSignName; }
	//����һ���µ�sessionid
	int32_t GetNewSessionID();
public:
	static uint32_t& GetTotalCreateCtx();

	static bool PushMessageByID(ctx_message& msg);
	bool PushMessage(ctx_message& msg);
	///////////////////////////////////////////////////////////////////////////////////////////
	//�����̰߳�ȫ�ĺ���
public:
	bool IsReleaseCtx(){ return m_bRelease; }
protected:
	bool										m_bRelease;
	uint32_t									m_ctxID;
	int32_t										m_sessionID_Index;
	basiclib::CBasicString						m_strCtxName;
	basiclib::CBasicString						m_strSignName;
	CCtxMessageQueue							m_ctxMsgQueue;
	moodycamel::ConsumerToken					m_Ctoken;		//�Ż���ȡ

	//ͳ���ܹ�������������
	static uint32_t				m_nTotalCtx;
	
	friend class CCoroutineCtxHandle;
};
typedef basiclib::CBasicRefPtr<CCoroutineCtx> CRefCoroutineCtx;
#pragma warning (pop)

#endif