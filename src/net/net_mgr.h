/***********************************************************************************************
// �ļ���:     net_mgr.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2016-9-12 11:50:18
// ��������:   ����TCPͨ�ŵĻ�����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_NET_MGR_H
#define BASIC_NET_MGR_H

#include "net_socket.h"
#include "net.h"

__NS_BASIC_START
///////////////////////////////////////////////////////////////////////////////
struct CEventQueueItem{
    CBasicNet_Socket*								m_pRefNetSession;
    CBasicNet_Socket::pCallSameRefNetSessionFunc	m_pCallFunc;
    intptr_t										m_lRevert;
};

class CNetThread : public basiclib::CBasicObject{
public:
    CNetThread();
    virtual ~CNetThread();

    //! �����¼�
    void SetEvent(CBasicNet_Socket* pSession, CBasicNet_Socket::pCallSameRefNetSessionFunc pCallFunc, intptr_t lRevert);

    //! ׼���ر�
    void ReadyToClose();

    //! ���뵽ȫ����Ϣ����
    void AddMessageQueue(CBasicNet_Socket* pSocket);

    //! ������Ϣ����
    void RunMessageQueue();
#ifdef BASICWINDOWS_USE_IOCP
    //! IOCP�߳�
    static unsigned __stdcall ThreadIOCPFunc(void* lpWorkContext);

    //! ��ʼ��������
    bool StartRecvData(CBasicNet_SocketTransfer* pSocket);

    //! ��ȡ��չfunc
    static IOCPExt_Func& GetExtFunc(){ return m_funcExt; }
#else
    //! �첽dns����
    bool DNSParse(const char* pName, evdns_getaddrinfo_cb pCallback, CBasicNet_Socket* pSession);
#endif

#ifdef BASICWINDOWS_USE_IOCP
protected:
    static IOCPExt_Func			m_funcExt;
    static bool					m_bExtInit;
public:
    DWORD						m_dwThreadID;
    HANDLE						m_hCompletionPort;
    OVERLAPPEDPLUS				m_olFunc;
    DWORD						m_dwIoSize;
    ULONG						m_ulFlags;
#else
public:
    DWORD						m_dwThreadID;
    evutil_socket_t				m_pair[2];
    struct event_base*			m_base;
    struct evdns_base*			m_dnsbase;
    struct event				notify_event;
#endif

    //ȫ�ֵ���Ϣ֪ͨ
    basiclib::SpinLock				m_lockMsg;
    basiclib::CBasicSmartBuffer		m_smBuf;
    basiclib::CBasicSmartBuffer		m_smRunBuf;
};
/////////////////////////////////////////////////////////////////////////////
//net mgr
class CBasicNetMgv : public CBasicObject{
public:
    typedef basic_vector<CBasicNet_Socket*>						VTOnTimerSessionList;
    typedef VTOnTimerSessionList::iterator						VTOnTimerSessionListIterator;
public:
    CBasicNetMgv();
    virtual ~CBasicNetMgv();

    //! ontimer�߳�
    static THREAD_RETURN ThreadCheckFunc(void* lpWorkContext);

    //! ��ʼ���߳�
    void Initialize(pGetConfFunc func);

    //! �˳�
    void CloseNetSocket();

    //! ����timer
    void AddToTimer(CBasicNet_Socket* pSocket);

    //! ɾ��timer
    void DelToTimer(CBasicNet_Socket* pSocket);

    //! ontimer�߳�
    void OnTimer();
public:
    bool					m_bTimeToKill;
    bool					m_bTimerStop;

    VTOnTimerSessionList	m_vtOnTimerList;

    SpinLock				m_spinLockAdd;
    VTOnTimerSessionList	m_vtAddList;
    VTOnTimerSessionList	m_vtAddListDeal;

    VTOnTimerSessionList	m_vtDelList;
    VTOnTimerSessionList	m_vtDelListDeal;

    typedef basic_vector<CBasicSessionNet::CRefBasicSessionNet>	VTDeathSessionList;
    VTDeathSessionList		m_vtDeathSession;
    VTDeathSessionList		m_vtDeathSessionDeal;
};

//���嵥̬
typedef CBasicSingleton<CBasicNetMgv>	CBasicSingletonNetMgv;

__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////////

#endif
