/***********************************************************************************************
// �ļ���:     net_server.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2016-9-12 11:50:18
// ��������:   ����TCPͨ�ŵĻ�����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_NET_SERVER_H
#define BASIC_NET_SERVER_H

/////////////////////////////////////////////////////////////////////////////////////////////
#include "net.h"

__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//����
//class CBasicObject;
class CBasicSessionNetServerSession;
class CBasicSessionNetServer;		//����
                                    /////////////////////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251)
#pragma warning (disable: 4275)

                                    //! ��ʹ��CreateNetServerSession
_BASIC_DLL_API CBasicSessionNetServerSession* CreateNetWithServerSession(size_t nClassSize, uint32_t nSessionID, uint16_t usRecTimeout, const std::function<CBasicSessionNetServerSession*(void*)>& func);

#define DefineCreateNetServerSession(T) \
public:\
	static T* CreateNetServerSession(uint32_t nSessionID, uint16_t usRecTimeout){return (T*)basiclib::CreateNetWithServerSession(sizeof(T), nSessionID, usRecTimeout, [&](void* pData)->CBasicSessionNetServerSession*{\
			return new (pData)T;\
		});}
#define DefineCreateNetServerSessionDefault(T) \
DefineCreateNetServerSession(T)\
protected:\
	T(){}\
	virtual ~T(){}


class CBasicNet_SocketTransfer;
class CBasicNet_SocketListen;
class _BASIC_DLL_API CBasicSessionNetServerSession : public CBasicSessionNetNotify{
    DefineCreateNetServerSessionDefault(CBasicSessionNetServerSession)
public:
    //! ��ȡsessionid������socketid
    uint32_t GetSessionID();

    //!��ȡ�Զ˵ĵ�ַ�Ͷ˿�
    const char* GetNetAddress();
    uint32_t GetNetAddressPort();
protected:
    friend class CBasicNet_SocketListen;
};
typedef basiclib::CBasicRefPtr<CBasicSessionNetServerSession> CRefBasicSessionNetServerSession;
/////////////////////////////////////////////////////////////////////////////////////////////
//! //! ��ʹ��CreateNetServer
_BASIC_DLL_API CBasicSessionNetServer* CreateNetWithServer(size_t nClassSize, const std::function<CBasicSessionNetServer*(void*)>& func);

#define DefineCreateNetServer(T) \
public:\
	static T* CreateNetServer(){return (T*)basiclib::CreateNetWithServer(sizeof(T), [&](void* pData)->CBasicSessionNetServer*{\
			return new (pData)T;\
		});}
#define DefineCreateNetServerDefault(T) \
DefineCreateNetServer(T)\
protected:\
	T(){}\
	virtual ~T(){}


typedef basiclib::basic_map<uint32_t, CRefBasicSessionNetServerSession>				MapClientSession;
typedef basiclib::basic_vector<CRefBasicSessionNetServerSession>					VTClientSession;
class _BASIC_DLL_API CBasicSessionNetServer : public CBasicSessionNet{
    DefineCreateNetServerDefault(CBasicSessionNetServer)
public:
    //! �ж��Ƿ����
    bool IsListen();

    /*formats
    -[IPv6Address]:port
    -IPv4Address:port
    */
    virtual int32_t Listen(const char* lpszAddress, bool bWaitSuccess = false);

    //! ���ý��ճ�ʱ�Ͽ�
    void SetClientRecTimeout(uint16_t uTimesSecond){
        m_usRecTimeout = uTimesSecond;
    }

    //! ���͸����пͻ���
    void SendToAll(void * pData, int nLength, DWORD dwFlag, bool bTrasmit = true);

    //! �Ͽ���������,�Ͽ�ǰ�Ļص�
    void CloseAllSession(const std::function<void(CBasicSessionNetServerSession* pSession)>& func);
    void CloseAll();

    //! ��ȡ״̬��Ϣ
    virtual void GetNetStatus(CBasicString& strStatus);

    //��ȡ�û�������
    long GetOnlineSessionCount();

    //! ��ȡsession
    CRefBasicSessionNetServerSession GetClientBySessionID(uint32_t nSessionID);

    //! �������пͻ���
    void CopyClientSession(VTClientSession& vtClient);
public:
    //! ontimer�߳�
    virtual void OnTimer(unsigned int nTick);
protected:
    //�������������
    virtual CBasicSessionNetServerSession* CreateServerClientSession(uint32_t nSessionID);
    virtual CBasicSessionNetServerSession* ConstructSession(uint32_t nSessionID);

    //! ���ն���
    void AcceptClient(CBasicSessionNetServerSession* pClient);
protected:
    //! �Ͽ�����
    int32_t OnClientDisconnectCallback(CBasicSessionNetNotify* pClient, uint32_t p2);//clientdisconnectcallback
protected:
    CCriticalSection						m_mtxCSession;
    MapClientSession						m_mapClientSession;

    basiclib::CBasicString					m_strListenAddr;

    uint16_t								m_usRecTimeout;				//��ʱʱ�䣬0������ʱ

    friend class CBasicNet_SocketListen;
};

#pragma warning (pop)
__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////////

#endif 
