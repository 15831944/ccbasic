/////////////////////////////////////////////////////////////////////////////////////////////
// �ļ���:		servertemplate.h
// ������:		������
// ����ʱ��:	2010/07/07
// ��������:	��������ģ��
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef INC_SERVERTEMPLATE_H
#define INC_SERVERTEMPLATE_H

#include "../../inc/basic.h"
#include "ipverify.h"

#pragma warning (push)
#pragma warning (disable: 4251)
#pragma warning (disable: 4275)

class CNetServerControlSession;
class _SCBASIC_DLL_API CNetServerControl : public basiclib::CBasicSessionNetServer
{
	DefineCreateNetServer(CNetServerControl)
public:
	typedef fastdelegate::FastDelegate1<CNetServerControlSession*, bool> HandleVerifySuccess;
public:
	//! ���ò���
	void SetIpTrust(const char* lpszIpTrust);
	void SetSessionMaxCount(int nCount);

	//! ����֤�ɹ��ص�
	void bind_verifysuccess(const HandleVerifySuccess& func){ m_handleVerifySuccess = func; }

	//! ��������
	virtual int32_t StartServer(const char* lpszAddress, basiclib::CBasicPreSend* pPreSend = nullptr);
protected:
	//�û���¼�ɹ�
    virtual bool SuccessLogin(CNetServerControlSession* pNotify);
protected:
	//! ����֤�ɹ����Ͳ���Ҫconnect
	virtual int32_t OnUserConnect(basiclib::CBasicSessionNetNotify* pNotify, Net_UInt dwNetCode);

	virtual basiclib::CBasicSessionNetServerSession* ConstructSession(uint32_t nSessionID);
protected:
	CNetServerControl();
	virtual ~CNetServerControl();
protected:
	HandleVerifySuccess			m_handleVerifySuccess;
	//ip���ε�ַ
	CIpVerify					m_ipTrust;
	// �������session��������	С��0 ��ʾ������
	int							m_nSessionMaxCount;

	friend class CNetServerControlSession;
};
typedef basiclib::CBasicRefPtr<CNetServerControl>														CRefNetServerControl;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DefineCreateNetServerSessionWithServer(T) \
public:\
	static T* CreateNetServerSessionWithServer(uint32_t nSessionID, uint16_t usRecTimeout, CNetServerControl* pServer){\
		T* pRet = (T*)basiclib::CreateNetWithServerSession(sizeof(T), nSessionID, usRecTimeout, [&](void* pData)->CBasicSessionNetServerSession*{\
			return new (pData)T;\
		});\
		pRet->BindServer(pServer);\
		return pRet;\
	}
#define DefineCreateNetServerSessionWithServerDefault(T) \
DefineCreateNetServerSessionWithServer(T)\
protected:\
	T(){}\
	virtual ~T(){}

class _SCBASIC_DLL_API CNetServerControlSession : public basiclib::CBasicSessionNetServerSession
{
	DefineCreateNetServerSessionWithServer(CNetServerControlSession)
protected:
	virtual uint32_t OnConnect(uint32_t dwNetCode);

    virtual void SuccessLogin();
protected:
	CNetServerControlSession();
	virtual ~CNetServerControlSession();

	void BindServer(CNetServerControl* pServer){
		m_server = pServer;
	}
public:
	CRefNetServerControl	m_server;
    bool                    m_bVerify;

    friend class CNetServerControl;
};

#pragma warning (pop)

#endif

