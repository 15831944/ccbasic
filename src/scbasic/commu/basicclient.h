/***********************************************************************************************
// �ļ���:     basicclient.h
// ������:     owen
// Email:      zqcai@w.cn
// ����ʱ��:   2012-3-10 18:35:56
// ��������:   ����ͻ���ͨѶ��
// �汾��Ϣ:   1.0V
// ʹ��˵��:   ֱ��ʹ��CBasicClientSession��ֻҪ��������������:
				OnConnect��Ӧ
				OnReceive��Ӧ
				OnIdle��Ӧ,����ʱ��Ĵ���
************************************************************************************************/
#ifndef BASIC_BASICCLIENT_H
#define BASIC_BASICCLIENT_H

#include "../../inc/basic.h"
#include "../scbasic_head.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//���ӹ���
class _SCBASIC_DLL_API CCommonClientSession : public basiclib::CBasicSessionNetClient
{
public:
    static CCommonClientSession* CreateCCommonClientSession(uint32_t nSessionID = basiclib::CBasicSessionNet::GetDefaultCreateSessionID()){ return new CCommonClientSession(nSessionID); }

protected:
	CCommonClientSession(uint32_t nSessionID);
	virtual ~CCommonClientSession();
public:
	virtual int32_t Connect(const char* lpszAddress);
	virtual int32_t OnDisconnect(uint32_t dwNetCode);
};

#endif 
