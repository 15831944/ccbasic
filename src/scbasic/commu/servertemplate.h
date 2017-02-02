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
	typedef fastdelegate::FastDelegate1<basiclib::CBasicSessionNetClient*, bool> HandleVerifySuccess;

	static CNetServerControl* CreateNetServerControl(Net_UInt nSessionID = 0){ return new CNetServerControl(nSessionID); }
protected:
	CNetServerControl(Net_UInt nSessionID);
	virtual ~CNetServerControl();
public:
	void bind_verifysuccess(const HandleVerifySuccess& func){ m_handleVerifySuccess = func; }

	virtual Net_Int StartServer(const char* lpszAddress, basiclib::CBasicPreSend* pPreSend = nullptr);
	BOOL IsListen();
	void SetIpTrust(const char* lpszIpTrust);
	void SetSessionMaxCount(int nCount);

	//����Ҫ�ⲿ����
	virtual void OnTimer(Net_UInt nTick);
protected:
	virtual Net_Int OnUserVerify(basiclib::CBasicSessionNetClient* pNotify, Net_UInt dwNetCode, Net_Int cbData, const char *pszData);
	virtual Net_Int OnVerifyDisconnectCallback(basiclib::CBasicSessionNetClient* pClient, Net_UInt p2);
	//�û���¼�ɹ�
	virtual bool SuccessLogin(basiclib::CBasicSessionNetClient* pNotify);
protected:
	virtual basiclib::CBasicSessionNetClient* CreateServerClientSession(Net_UInt nSessionID);
	virtual basiclib::CBasicSessionNetClient* ConstructSession(Net_UInt nSessionID);
protected:
	HandleVerifySuccess			m_handleVerifySuccess;
	//ip���ε�ַ
	CIpVerify					m_ipTrust;
	// �������session��������	С��0 ��ʾ������
	int							m_nSessionMaxCount;

	basiclib::CMutex			m_mtxCSessionVerify;
	basiclib::MapClientSession	m_mapClientSessionVerify;

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

