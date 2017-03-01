/***********************************************************************************************
// �ļ���:     basicpoll.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2017/2/9 12:00:18
// ��������:   �¼�����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_BASICPOLL_H
#define BASIC_BASICPOLL_H

#include "event.h"

typedef void(*pCallBasicPollNotify)(void* m_ud);
struct CBasicPollNotify
{
	pCallBasicPollNotify							m_func;
	void*											m_ud;
};

//����libeventʵ��,�ײ��������fd�����ĻῨ��
class CBasicPoll : public basiclib::CBasicObject
{
public:
	CBasicPoll();
	virtual ~CBasicPoll();

	/* ��ʼ���߳�basicpool
	*/
	bool Init(event_config *cfg);
protected:
	friend void BasicPollReadSelfOrder(int fd, short event, void *arg);
protected:
	evutil_socket_t		m_pair[2];
	struct event_base*	m_base;
	struct event		notify_event;
};

#endif