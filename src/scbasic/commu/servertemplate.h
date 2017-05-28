/////////////////////////////////////////////////////////////////////////////////////////////
// �ļ���:		servertemplate.h
// ������:		������
// ����ʱ��:	2010/07/07
// ��������:	��������ģ��
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef INC_SERVERTEMPLATE_H
#define INC_SERVERTEMPLATE_H

#include "ipverify.h"
#include "../scbasic/net/net.h"

#pragma warning (push)
#pragma warning (disable: 4251)
#pragma warning (disable: 4275)

class CNetServerControlClient;
class _SCBASIC_DLL_API CNetServerControl : public basiclib::CBasicSessionNetServer
{
public:
	typedef fastdelegate::FastDelegate1<basiclib::CBasicSessionNetClient*, bool> HandleVerifySuccess;

    static CNetServerControl* CreateNetServerControl(uint32_t nSessionID = basiclib::CBasicSessionNet::GetDefaultCreateSessionID()){ return new CNetServerControl(nSessionID); }
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
	//�û���¼�ɹ�
    virtual bool SuccessLogin(basiclib::CBasicSessionNetClient* pNotify);
    virtual void SuccessLoginout(basiclib::CBasicSessionNetClient* pNotify);
protected:
	virtual basiclib::CBasicSessionNetClient* ConstructSession(uint32_t nSessionID);
protected:
	HandleVerifySuccess			m_handleVerifySuccess;
	//ip���ε�ַ
	CIpVerify					m_ipTrust;
	// �������session��������	С��0 ��ʾ������
	int							m_nSessionMaxCount;

    //��֤ͨ����session
	basiclib::CMutex			m_mtxCSessionVerify;
	basiclib::MapClientSession	m_mapClientSessionVerify;

	friend class CNetServerControlClient;
};

typedef basiclib::CBasicRefPtr<CNetServerControl> CRefNetServerControl;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class _SCBASIC_DLL_API CNetServerControlClient : public basiclib::CBasicSessionNetClient
{
public:
    static CNetServerControlClient* CreateControlClient(uint32_t nSessionID = basiclib::CBasicSessionNet::GetDefaultCreateSessionID(), CRefNetServerControl pServer = nullptr){ return new CNetServerControlClient(nSessionID, pServer); }
protected:
	CNetServerControlClient(uint32_t nSessionID, CRefNetServerControl pServer);
	virtual ~CNetServerControlClient();

	virtual int32_t OnConnect(uint32_t dwNetCode);
	virtual int32_t OnDisconnect(uint32_t dwNetCode);

    virtual void SuccessLogin();
public:
	CRefNetServerControl    m_server;
    bool                    m_bVerify;

    friend class CNetServerControl;
};

#pragma warning (pop)

#endif

