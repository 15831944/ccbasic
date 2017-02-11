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

	static CNetServerControl* CreateNetServerControl(uint32_t nSessionID = 0){ return new CNetServerControl(nSessionID); }
protected:
	CNetServerControl(uint32_t nSessionID);
	virtual ~CNetServerControl();
public:
	void bind_verifysuccess(const HandleVerifySuccess& func){ m_handleVerifySuccess = func; }

	virtual int32_t StartServer(const char* lpszAddress, basiclib::CBasicPreSend* pPreSend = nullptr);
	BOOL IsListen();
	void SetIpTrust(const char* lpszIpTrust);
	void SetSessionMaxCount(int nCount);

	//����Ҫ�ⲿ����
	virtual void OnTimer(uint32_t nTick);
protected:
	virtual int32_t OnUserVerify(basiclib::CBasicSessionNetClient* pNotify, uint32_t dwNetCode, int32_t cbData, const char *pszData);
	virtual int32_t OnVerifyDisconnectCallback(basiclib::CBasicSessionNetClient* pClient, uint32_t p2);
	//�û���¼�ɹ�
	virtual bool SuccessLogin(basiclib::CBasicSessionNetClient* pNotify);
protected:
	virtual basiclib::CBasicSessionNetClient* CreateServerClientSession(uint32_t nSessionID);
	virtual basiclib::CBasicSessionNetClient* ConstructSession(uint32_t nSessionID);
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
	static CNetServerControlClient* CreateControlClient(uint32_t nSessionID, CRefNetServerControl pServer){ return new CNetServerControlClient(nSessionID, pServer); }
protected:
	CNetServerControlClient(uint32_t nSessionID, CRefNetServerControl pServer);
	virtual ~CNetServerControlClient();

	virtual int32_t OnConnect(uint32_t dwNetCode);
	virtual int32_t OnDisconnect(uint32_t dwNetCode);
	virtual int32_t OnReceive(uint32_t dwNetCode, const char *pszData, int32_t cbData);
public:
	CRefNetServerControl m_server;
};

#endif

