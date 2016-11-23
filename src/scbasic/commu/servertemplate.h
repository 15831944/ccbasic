/////////////////////////////////////////////////////////////////////////////////////////////
// �ļ���:		servertemplate.h
// ������:		������
// ����ʱ��:	2010/07/07
// ��������:	��������ģ��
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef INC_SERVERTEMPLATE_H
#define INC_SERVERTEMPLATE_H

#include "ipverify.h"

class CNetServerControl : public basiclib::CBasicSessionNetServer
{
public:
	static CNetServerControl* CreateNetServerControl(Net_UInt nSessionID){ return new CNetServerControl(nSessionID); }
protected:
	CNetServerControl(Net_UInt nSessionID);
	virtual ~CNetServerControl();
public:
	Net_Int StartServer(const char* lpszAddress, basiclib::CBasicPreSend* pPreSend = nullptr);
	BOOL IsListen();
	void SetIpTrust(const char* lpszIpTrust);
	void SetSessionMaxCount(int nCount);

	//����Ҫ�ⲿ����
	virtual void OnTimer(Net_UInt nTick);
protected:
	virtual basiclib::CBasicSessionNetClient* ConstructSession(Net_UInt nSessionID);
protected:
	//ip���ε�ַ
	CIpVerify			m_ipTrust;
	// �������session��������	С��0 ��ʾ������
	int	m_nSessionMaxCount;

	friend class CNetServerControlClient;
};

typedef basiclib::CBasicRefPtr<CNetServerControl> CRefNetServerControl;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CNetServerControlClient : public basiclib::CBasicSessionNetClient
{
public:
	static CNetServerControlClient* CreateControlClient(Net_UInt nSessionID, CRefNetServerControl pServer){ return new CNetServerControlClient(nSessionID, pServer); }
protected:
	CNetServerControlClient(Net_UInt nSessionID, CRefNetServerControl pServer);
	virtual ~CNetServerControlClient();

	virtual Net_Int OnConnect(Net_UInt dwNetCode);
	virtual Net_Int OnDisconnect(Net_UInt dwNetCode);
	virtual Net_Int OnReceive(Net_UInt dwNetCode, const char *pszData, Net_Int cbData);
public:
	CRefNetServerControl m_server;
};

#endif

