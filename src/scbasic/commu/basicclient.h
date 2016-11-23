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

////////////////////////////////////////////////////////////////////////////////////////////////
//���ӹ���
class CCommonClientSession : public basiclib::CBasicSessionNetClient
{
public:
	static CCommonClientSession* CreateCCommonClientSession(Net_UInt nSessionID){ return new CCommonClientSession(nSessionID); }

protected:
	CCommonClientSession(Net_UInt nSessionID);
	virtual ~CCommonClientSession();
public:
	virtual Net_Int Connect(const char* lpszAddress);
	virtual Net_Int OnDisconnect(Net_UInt dwNetCode);

	//�� �������ڶ�ʱ��,���ⲿ����
	virtual void OnTimer(Net_UInt nTickTime);
};

#endif 
