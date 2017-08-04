/***********************************************************************************************
// �ļ���:     net_socket.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2016-9-12 11:50:18
// ��������:   ����TCPͨ�ŵĻ�����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_NET_SOCKET_H
#define BASIC_NET_SOCKET_H

/////////////////////////////////////////////////////////////////////////////////////////////
#include "event.h"
#include "evdns.h"
#include "sendbuffer.h"

#ifdef __BASICWINDOWS
#define BASICWINDOWS_USE_IOCP
#endif

__NS_BASIC_START
#define READBUFFERSIZE_MSG			16384
/////////////////////////////////////////////////////////////////////////////////////////////
class CNetThread;
class CBasicSessionNet;
struct CEventQueueItem;
#ifdef BASICWINDOWS_USE_IOCP

/* Mingw's headers don't define LPFN_ACCEPTEX. */
typedef BOOL(WINAPI *AcceptExPtr)(SOCKET, SOCKET, PVOID, DWORD, DWORD, DWORD, LPDWORD, LPOVERLAPPED);
typedef BOOL(WINAPI *ConnectExPtr)(SOCKET, const struct sockaddr *, int, PVOID, DWORD, LPDWORD, LPOVERLAPPED);
typedef void (WINAPI *GetAcceptExSockaddrsPtr)(PVOID, DWORD, DWORD, DWORD, LPSOCKADDR *, LPINT, LPSOCKADDR *, LPINT);

/** Internal use only. Holds pointers to functions that only some versions of
Windows provide.
*/
struct IOCPExt_Func{
	AcceptExPtr AcceptEx;
	ConnectExPtr ConnectEx;
	GetAcceptExSockaddrsPtr GetAcceptExSockaddrs;
};

enum IOType{
	IORead,
	IOWrite,
	IOFunc,
	IOAccept,
	IOConnect,
};

struct OVERLAPPEDPLUS{		//internal class  no derived from ctlobject
	OVERLAPPED		m_ol;
	IOType			m_ioType;
	OVERLAPPEDPLUS(){
		ZeroMemory(this, sizeof(OVERLAPPEDPLUS));
	}
};
#endif

class CBasicNet_Socket{
public:
	typedef void(*pCallSameRefNetSessionFunc)(CBasicNet_Socket* pRefNetSession, intptr_t lRevert);
	static uint32_t GetDefaultCreateSessionID(){ return m_defaultCreateSession.fetch_add(1, memory_order_relaxed); }
public:
	CBasicNet_Socket(uint32_t nSessionID);
	virtual ~CBasicNet_Socket();
public:
	//! ��ȡsocketid
	evutil_socket_t& GetSocketID(){ return m_socketfd; }

	//! �ر�
	void Close(bool bRemote = false, bool bMustClose = false);

	//! �ص��п��԰�ȫɾ��
	virtual void SafeDelete();

	//! ע�������
	int RegistePreSend(CBasicPreSend* pFilter, uint32_t dwRegOptions = 0);

	//! ��ȡע��Ĺ�����
	CBasicPreSend* GetPreSend(){ return m_pPreSend; }

	//! ��ȡ��ʵ��session
	virtual CBasicSessionNet* GetRealSessionNet() = 0;

	//! �������
	void AddSocketCallFunc(CEventQueueItem* pItem);

	//! ִ�ж���
	void RunSocketCallFunc();

	//! ��ȡnetthread
	CNetThread* GetSelfNetThread(){ return m_pThread; }
#ifdef BASICWINDOWS_USE_IOCP
	//! server�����첽acceptʹ��
	virtual void ServerAcceptEx(OVERLAPPEDPLUS* pOverlapPlus){}

	//! client����ʹ��
	virtual void ClientConnectEx(){}
#endif
public:
	//! �Ƿ�����
	bool IsConnected();
	//! �Ƿ���֤�ɹ�
	bool IsTransmit();
	//! �ж��Ƿ�׼��close
	bool IsToClose();

	//! ����׼���ر�
	void SetToClose();
	//! ���ð�ȫɾ����־
	void SetToSafeDelete();

	//! �ж��Ƿ���Թر�
	virtual bool CanClose();

	//! ��ȡ������״̬
	uint32_t GetSessionStatus(uint32_t dwMask){ return m_unSessionStatus & dwMask; }
	void SetSessionStatus(uint32_t dwValue, uint32_t dwMask){ m_unSessionStatus &= ~dwMask; m_unSessionStatus |= (dwValue & dwMask); }
public:
	//! ontimer�߳�
	virtual bool OnTimer(unsigned int nTick);
protected:
	//! ���Ͷ�Ӧ�߳���Ϣ
	void SetLibEvent(pCallSameRefNetSessionFunc pCallback, intptr_t lRevert = 0);

	//! ���³�ʼ����Ա
	virtual void InitMember();

	//! �߳���ִ�к���
	virtual void CloseCallback(BOOL bRemote, DWORD dwNetCode = 0);
protected:
	static std::atomic<uint32_t>	m_defaultCreateSession;
	CNetThread*						m_pThread;
#ifndef BASICWINDOWS_USE_IOCP
	event							m_revent;
#endif
	evutil_socket_t					m_socketfd;

	CBasicPreSend*					m_pPreSend;

	uint32_t						m_unSessionStatus;				//״̬  TIL_SS_*

	//��Ϣ����
	basiclib::SpinLock				m_lockMsg;
	basiclib::CBasicSmartBuffer		m_smBuf;
	basiclib::CBasicSmartBuffer		m_smIOCPBuf;

	friend class CNetThread;
};

#define MAX_BUFFER_SEND_BUF				4096
class CBasicNet_SocketTransfer : public CBasicNet_Socket{
public:
	CBasicNet_SocketTransfer(CBasicSessionNetNotify* pFather, uint32_t nSessionID, uint16_t usRecTimeout = 0);
	virtual ~CBasicNet_SocketTransfer();

	//! ��������
	virtual int32_t Send(void *pData, int32_t cbData, uint32_t dwFlag = 0);

	//�ڲ��ͷ�
	int32_t SendData(SendBufferCacheMgr& sendData);

	//! �ж��Ƿ���Թر�
	virtual bool CanClose();

	//! �ж��Ƿ�ʱû�յ�����
	BOOL IsRecTimeout(time_t tmNow, uint16_t nTimeoutSecond);

	//! ��ȡnetstate
	void GetNetStatInfo(BasicNetStat& netState){ netState = m_stNet; }

	//! ��ȡ״̬
	virtual void GetNetStatus(CBasicString& strStatus) = 0;
public:
	//! ontimer�߳�
	virtual bool OnTimer(unsigned int nTick);

	//! onidle
	void OnIdle();
protected:
	friend void OnLinkRead(evutil_socket_t fd, short event, void *arg);
	friend void OnLinkWrite(evutil_socket_t fd, short event, void *arg);

	//! ���³�ʼ����Ա
	virtual void InitMember();

	//! �߳���ִ�к���
	virtual void CloseCallback(BOOL bRemote, DWORD dwNetCode = 0);

	//! ���¼�
	void OnReadEvent();

	//! д�¼�
	void OnWriteEvent();

	//! ֻ��libevent�߳�ʹ��
	void SendDataFromQueue();
#ifdef BASICWINDOWS_USE_IOCP
	//! IOCP�ص����ͳɹ��ֽ�
	void SendDataSuccessAndCheckSend(DWORD dwIoSize);
#endif

	//! ���뷢�Ͷ���
	void AddSendQueue(SendBufferCache* pSendCache);

	//! �յ�����
	void OnReceiveData(const char* pszData, uint32_t dwIoSize);

	//! ���ù�����״̬
	void ResetPreSend();

	//! �Ͽ���Ϣ
	void OnDisconnect(uint32_t dwNetCode);
	void OnSendData(uint32_t dwIoSize);
	void OnConnect(uint32_t dwNetCode);
	uint32_t OnReceive(uint32_t dwNetCode, const char *pszData, int32_t cbData);
	void OnError(uint32_t dwNetCode, int32_t lRetCode);
	//! ��������������
	void PreReceiveData(uint32_t dwNetCode, const char *pszData, int32_t cbData);
protected:
	virtual CBasicSessionNet* GetRealSessionNet(){ return m_pFather; }
protected:
	static unsigned short	m_usTimeoutShakeHandle;		//default shakehandle timeout time
	CBasicSessionNetNotify*	m_pFather;
	uint16_t				m_usRecTimeout;				//��ʱʱ�䣬0������ʱ
	uint32_t				m_unIdleCount;				//������еĴ���

	BasicNetStat			m_stNet;
	BasicNetStat			m_lastNet;

	//�߳���ʹ��
	CBasicBitstream			m_bufCacheTmp;
#ifdef BASICWINDOWS_USE_IOCP
	bool					m_bSend;
	OVERLAPPEDPLUS			m_olRead;
	OVERLAPPEDPLUS			m_olWrite;
	WSABUF					m_wsaInBuffer;
	char					m_byInBuffer[MAX_BUFFER_SEND_BUF];		//Ԥ����Ľ������ݻ�����
	WSABUF					m_wsaOutBuffer;
	char					m_byOutBuffer[MAX_BUFFER_SEND_BUF];		//Ԥ����ķ������ݻ�����
#else
	event					m_wevent;
#endif

private:
	//���͵Ļ�����
	CMsgSendBuffer			m_msgQueue;

	friend class CNetThread;
};

__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////////

#endif 
