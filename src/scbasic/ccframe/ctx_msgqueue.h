/***********************************************************************************************
// �ļ���:     ctx_msgqueue.h
// ������:     ������
// Email:      zqcai@w.cn
// ��������:   �����ĵ���Ϣ����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef SCBASIC_CTX_MSGQUEUE_H
#define SCBASIC_CTX_MSGQUEUE_H

#include <basic.h>
#include "../scbaisc_head.h"

#define CTXMESSAGE_TYPE_NOTHING					0		//��ʵ�ǳ���İ�
#define CTXMESSAGE_TYPE_RUNFUNC					1		//ִ�к���		
#define CTXMESSAGE_TYPE_RUNCOROUTINE			2		//����Э��
#define CTXMESSAGE_TYPE_ONTIMER					3		//ontimer
#define CTXMESSAGE_TYPE_SOCKETMSG				4		//socketmsg

//ִ�к����Ķ���
class CCoroutineCtx;
typedef void(*pRunFuncCtxMessageCallback)(CCoroutineCtx*, intptr_t);

#pragma	pack(1)
struct _SCBASIC_DLL_API ctx_message
{
	uint32_t		m_nCtxID;
	intptr_t		m_session;
	uint32_t		m_nType;
	void*			m_data;
	size_t			sz;
	ctx_message(){
		memset(this, 0, sizeof(ctx_message));
	}
	ctx_message(uint32_t nCtxID){
		memset(this, 0, sizeof(ctx_message));
		m_nCtxID = nCtxID;
	}
	//! ִ�к���
	ctx_message(uint32_t nCtxID, pRunFuncCtxMessageCallback pFunc, intptr_t nSession = 0){
		memset(this, 0, sizeof(ctx_message));
		m_nCtxID = nCtxID;
		m_data = pFunc;
		m_session = nSession;
	}
	ctx_message(ctx_message&& msg){
		*this = msg;
		msg.sz = 0;
	}
	~ctx_message(){
		if (sz > 0){
			basiclib::BasicDeallocate(m_data);
		}
	}
	ctx_message& operator = (ctx_message&& msg){
		*this = msg;
		msg.sz = 0;
		return *this;
	}
	void CloneData(const char* pData, int nLength){
		if (nLength != 0){
			sz = nLength;
			m_data = basiclib::BasicAllocate(sz);
			memcpy(m_data, pData, sz);
		}
	}
	//! �ɶ����ַ���
	void FormatMsgQueueToString(const std::function<void(const char* pData, int nLength)>& func){
		char szBuf[128] = { 0 };
		func(szBuf, 
			sprintf(szBuf, "C(%d) S(%x%x) T(%d) SZ(%d) D(%x%x)", m_nCtxID, (uint32_t)(m_session >> 32), (uint32_t)m_session, m_nType, sz, (uint32_t)((intptr_t)m_data >> 32), (uint32_t)m_data));
	}
};
#pragma	pack()

class CMQMgr;
//�������ߵ�������
class CCtxMessageQueue : public basiclib::CLockFreeMessageQueue<ctx_message, 32>
{
public:
	CCtxMessageQueue();
	virtual ~CCtxMessageQueue();

	//! ��ʼ������
	void InitCtxMsgQueue(uint32_t nCtxID, CMQMgr* pMgr);
	
	//! ������Ϊһ���ɹ������ش����Ƿ�֪ͨ,���token�������Լ���
	bool MQPush(moodycamel::ProducerToken& token, ctx_message& message);
	bool MQPush(ctx_message& message);
	//! ���������ߣ����̣߳�,�����Ƿ��Ѿ�ȫ��ȡ��
	bool MQPop(moodycamel::ConsumerToken& token, ctx_message& message);
	bool MQPop(ctx_message& message);

	void PushToMQMgr();
	uint32_t GetCtxID(){ return m_nCtxID; }
protected:
	uint32_t									m_nCtxID;
	std::atomic<uint32_t>						m_nAddTimes;
	CMQMgr*										m_pMgr;

	friend class CMQMgr;
	friend class CCoroutineCtx;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CMQMgr : public basiclib::CLockFreeMessageQueue<CCtxMessageQueue*, 1024>
{
public:
	CMQMgr();
	virtual ~CMQMgr();

	void GlobalMQPush(CCtxMessageQueue* queue);
	void GlobalMQPush(moodycamel::ProducerToken& token, CCtxMessageQueue* queue);
	CCtxMessageQueue* GlobalMQPop(moodycamel::ConsumerToken& token);

	//wait timeout ms
	void WaitForGlobalMQ(unsigned int nTimeout);
protected:
	basiclib::CEvent*		m_pEvent;
	std::atomic<bool>		m_bWaitThread;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif