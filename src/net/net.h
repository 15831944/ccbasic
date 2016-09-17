/***********************************************************************************************
// �ļ���:     net.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2016-9-12 11:50:18
// ��������:   ����TCP(UDP)ͨ�ŵĻ�����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_NET_H
#define BASIC_NET_H

typedef basiclib::CBasicString		Net_CBasicString;                                   //define the cstring
template <typename T>
struct Net_Vector
{
	typedef typename basiclib::basic_vector<T>::type ContainForNet;
};

template <typename KEY, typename VAL>
struct Net_Map
{
	typedef typename basiclib::basic_hash_map<KEY, VAL>::type ContainForNet;
};
template <typename T>
struct Net_Set
{
	typedef typename basiclib::basic_set<T>::type ContainForNet;
};
//֧�����л���map��vector����
typedef Net_Vector<Net_Int>::ContainForNet									VTNetInt;
typedef VTNetInt::iterator													VTNetIntIterator;
typedef VTNetInt::const_iterator											VTNetIntIteratorConst;
typedef Net_Vector<Net_UInt>::ContainForNet                                 VTNetUInt;
typedef VTNetUInt::iterator													VTNetUIntIterator;
typedef VTNetUInt::const_iterator											VTNetUIntIteratorConst;
typedef Net_Map<Net_Int, Net_Int>::ContainForNet							MapNetIntToInt;
typedef MapNetIntToInt::iterator											MapNetIntToIntIterator;
typedef MapNetIntToInt::const_iterator										MapNetIntToIntIteratorConst;
typedef Net_Map<Net_UInt, Net_Int>::ContainForNet							MapNetUIntToInt;
typedef MapNetUIntToInt::iterator											MapNetUIntToIntIterator;
typedef MapNetUIntToInt::const_iterator										MapNetUIntToIntIteratorConst;
typedef Net_Map<Net_UInt, Net_UInt>::ContainForNet                          MapNetUIntToUInt;
typedef MapNetUIntToUInt::iterator											MapNetUIntToUIntIterator;
typedef MapNetUIntToUInt::const_iterator									MapNetUIntToUIntIteratorConst;

__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//����
//class CBasicObject;
class CBasicSessionNet;				//����
class CBasicSessionNetServer;		//����
class CBasicSessionNetClient;		//��������

struct BasicNetStat;		//���ͽ��յ�ͳ����Ϣ
/////////////////////////////////////////////////////////////////////////////////////////////
#include "event.h"
#include "evdns.h"

#pragma	pack(1)

/////////////////////////////////////////////////////////////////////////////////////////////
//���ش�����
#define NET_ERROR								(0xE0000000 | _ERROR_NET)	    //�������

#define BASIC_NET_OK							0								//�ɹ���û�д���

#define BASIC_NET_GENERIC_ERROR					(NET_ERROR | 0x0001)			//һ���Դ��󣬼�δ֪�Ĵ���
#define BASIC_NET_OUTOF_MAXPACKET				(NET_ERROR | 0x0002)			//����������ݰ�������
#define BASIC_NET_NO_MEMORY						(NET_ERROR | 0x0003)			//�ڴ治��
#define BASIC_NET_ADDRESS_ERROR					(NET_ERROR | 0x0004)			//��ַ����
#define BASIC_NET_BIND_ERROR					(NET_ERROR | 0x0005)			//bind �������ô���
#define BASIC_NET_LISTEN_ERROR					(NET_ERROR | 0x0006)			//listen �������ô���
#define BASIC_NET_TOCLOSE_ERROR					(NET_ERROR | 0x0007)			//���ڹر�
#define BASIC_NET_CONNECTING_ERROR				(NET_ERROR | 0x0008)			//��������
#define BASIC_NET_ALREADY_LISTEN				(NET_ERROR | 0x0009)			//�Ѿ�����
#define BASIC_NET_NO_CONNECT					(NET_ERROR | 0x000a)			//û������
#define BASIC_NET_ALREADY_CONNECT				(NET_ERROR | 0x000b)			//�Ѿ�����
#define BASIC_NET_INVALID_ADDRESS				(NET_ERROR | 0x000c)			//��ַ�Ƿ�
#define BASIC_NET_FILTER_WRITE_FAIL				(NET_ERROR | 0x000d)			//���������� OnWriteData ʧ��
#define BASIC_NET_FILTER_READ_FAIL				(NET_ERROR | 0x000e)			//���������� OnReadData  ʧ��
#define BASIC_NET_SOCKET_ERROR					(NET_ERROR | 0x000f)			//socket �������ô���

//���ص�״̬����
#define BASIC_NETCODE_SUCC					0x00000001				//�ɹ�
#define BASIC_NETCODE_CLOSE_REMOTE			0x00000002				//����������ر�
#define BASIC_NETCODE_FILTER_HANDLE			0x00000004				//�����������
#define BASIC_NETCODE_FILTER_ERROR			0x00000008				//�������������

//�󶨺��� HandleConnect ����ֵ
#define BASIC_NET_HC_RET_HANDSHAKE			0x00000001		//������Ҫ���ֹ���

//�󶨺��� HandleReceive ����ֵ
#define BASIC_NET_HR_RET_HANDSHAKE			0x00000001		//���ֹ��̳ɹ�


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! ����������ֵ
#define PACK_FILTER_FAIL		-1		/*!<ʧ��*/
#define PACK_FILTER_SKIP		0		/*!<�ɹ�������δ�Ķ�*/
#define PACK_FILTER_NEXT		1		/*!<�ɹ�������������Ҫ��������*/
#define PACK_FILTER_HANDLED		2		/*!<�ɹ�����������*/
#define PACK_FILTER_SEARCH		3		/*!<�ɹ������ݸ���һ��*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
* ��ʼ��ͨ������Ĳ�����
* \return ���� BASIC_NET_*
* \remarks ��ʹ��ͨ������ǰ������øú���
*/
typedef basiclib::CBasicString(*pGetConfFunc)(const char* pParam);
void SetNetInitializeGetParamFunc(pGetConfFunc func);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class _BASIC_DLL_API CBasicPreSend : public CBasicObject
{
public:
	CBasicPreSend() {}

public:
	/*\brief �����յ������� */
	// �������ɵ����ݷ���buf
	virtual Net_Int OnPreReceive(const char *pszData, Net_Int cbData, CBasicSmartBuffer& buf, CBasicSessionNetClient* pNetSession) = 0;

	/*\brief ���˷��͵����� */
	// �������ɵ����ݷ���buf
	virtual Net_Int OnPreSend(const char *pszData, Net_Int cbData, Net_UInt dwFlag, CBasicSmartBuffer& buf) = 0;

	/*\brief �����µ�ʵ�� */
	// ����Accept
	virtual CBasicPreSend* Construct() = 0;

	/*\brief ���ù�����״̬ */
	virtual void ResetPreSend() = 0;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! ���ͽ��յ�ͳ����Ϣ
/*!
*  ����ͳ�Ʒ��ͽ��յ��ֽ����ʹ������������㿪ʼ��
*/
struct _BASIC_DLL_API  BasicNetStat
{
	Net_UInt	m_dwSendBytes;		/*!< ���͵��ֽ��� */
	Net_UInt	m_dwSendTimes;		/*!< ���͵Ĵ��� */
	Net_UInt	m_dwReceBytes;		/*!< ���յ��ֽ��� */
	Net_UInt	m_dwReceTimes;		/*!< ���յĴ��� */

	BasicNetStat()
	{
		Empty();
	}

	void Empty()
	{
		memset(this, 0, sizeof(BasicNetStat));
	}
	void OnSendData(int nSend)
	{
		if (nSend > 0)
		{
			m_dwSendBytes += nSend;
			m_dwSendTimes++;
		}
	}
	void OnReceiveData(int nRece)
	{
		if (nRece > 0)
		{
			m_dwReceBytes += nRece;
			m_dwReceTimes++;
		}
	}
	void Clone(BasicNetStat* pNetStat)
	{
		if (pNetStat != NULL && pNetStat != this)
		{
			memcpy(this, pNetStat, sizeof(BasicNetStat));
		}
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TIL_SS_LINK						0x0000000F		//���ӵ�״̬
#define TIL_SS_IDLE						0x00000000		//����
#define TIL_SS_CONNECTING				0x00000001		//��������
#define TIL_SS_CONNECTED				0x00000002		//������

#define TIL_SS_CLOSE					0x000000F0		//�رյ�״̬
#define TIL_SS_NORMAL					0x00000000		//����״̬
#define TIL_SS_TOCLOSE					0x00000010		//���ڹر�

#define TIL_SS_SHAKEHANDLE_MASK			0x00000F00		//��֤
#define TIL_SS_SHAKEHANDLE_TRANSMIT		0x00000100		//

#define ADDRESS_MAX_LENGTH		64
//////////////////////////////////////////////////////////////////////////////


class CNetThread;
class CBasicSessionNet : public basiclib::CBasicObject, public basiclib::EnableRefPtr<CBasicSessionNet>
{
public:
	//global
	static void Initialize(pGetConfFunc func);
	static void CloseSocket();
public:
	typedef basiclib::CBasicRefPtr<CBasicSessionNet> CRefBasicSessionNet;
	typedef void(*pCallSameRefNetSessionFunc)(CBasicSessionNet* pRefNetSession, Net_PtrInt lRevert);
	typedef std::function<Net_Int(CBasicSessionNet*, Net_UInt, Net_Int, const char *)> HandleReceive;			//֪ͨ���յ�������
	typedef std::function<Net_Int(CBasicSessionNet*, Net_UInt, Net_Int)> HandleSend;			//�������ݵ�֪ͨ
	typedef std::function<Net_Int(CBasicSessionNet*, Net_UInt)> HandleConnect;				//��������
	typedef std::function<Net_Int(CBasicSessionNet*, Net_UInt)> HandleDisConnect;			//�Ͽ�����Ϣ
	typedef std::function<Net_Int(CBasicSessionNet*, Net_UInt)> HandleIdle;				//������Ϣ
	typedef std::function<Net_Int(CBasicSessionNet*, Net_UInt, Net_Int)> HandleError;			//������Ϣ

	void bind_rece(const HandleReceive& func){ m_funcReceive = func; }
	void bind_send(const HandleSend& func){ m_funcSend = func; }
	void bind_connect(const HandleConnect& func){ m_funcConnect = func; }
	void bind_disconnect(const HandleDisConnect& func){ m_funcDisconnect = func; }
	void bind_idle(const HandleIdle& func){ m_funcIdle = func; }
	void bind_error(const HandleError& func){ m_funcError = func; }
	Net_Int _handle_rece(Net_UInt dwNetCode, const char *pszData, Net_Int cbData)
	{
		Net_Int lRet = BASIC_NET_OK;
		if (m_funcReceive)
		{
			m_refSelf->AddRef();
			lRet = m_funcReceive(this, dwNetCode, cbData, pszData);
			m_refSelf->DelRef();
		}
		return lRet;
	}
	Net_Int _handle_send(Net_UInt dwNetCode, Net_Int cbSend)
	{
		Net_Int lRet = BASIC_NET_OK;
		if (m_funcSend)
		{
			m_refSelf->AddRef();
			lRet = m_funcSend(this, dwNetCode, cbSend);
			m_refSelf->DelRef();
		}
		return lRet;
	}
	Net_Int _handle_connect(Net_UInt dwNetCode)
	{
		Net_Int lRet = BASIC_NET_OK;
		if (m_funcConnect)
		{
			m_refSelf->AddRef();
			lRet = m_funcConnect(this, dwNetCode);
			m_refSelf->DelRef();
		}
		return lRet;
	}
	Net_Int _handle_disconnect(Net_UInt dwNetCode)
	{
		Net_Int lRet = BASIC_NET_OK;
		if (m_funcDisconnect)
		{
			m_refSelf->AddRef();
			lRet = m_funcDisconnect(this, dwNetCode);
			m_refSelf->DelRef();
		}
		return lRet;
	}
	Net_Int _handle_idle(Net_UInt dwNetCode)
	{
		Net_Int lRet = BASIC_NET_OK;
		if (m_funcIdle)
		{
			m_refSelf->AddRef();
			lRet = m_funcIdle(this, dwNetCode);
			m_refSelf->DelRef();
		}
		return lRet;
	}
	Net_Int _handle_error(Net_UInt dwNetCode, Net_Int lRetCode)
	{
		Net_Int lRet = BASIC_NET_OK;
		if (m_funcError)
		{
			m_refSelf->AddRef();
			lRet = m_funcError(this, dwNetCode, lRetCode);
			m_refSelf->DelRef();
		}
		return lRet;
	}
public:
	//release the object
	void Release();
	//close net
	void Close(BOOL bRemote = FALSE);
	//register presend filter
	int RegistePreSend(CBasicPreSend* pFilter, Net_UInt dwRegOptions = 0);
	
	const char* GetLibeventMethod();
protected:
	CBasicSessionNet(Net_UInt nSessionID);
	virtual ~CBasicSessionNet();

	virtual void InitMember();

	BOOL IsToClose(){ return GetSessionStatus(TIL_SS_CLOSE) >= TIL_SS_TOCLOSE; }
	void SetToClose() { SetSessionStatus(TIL_SS_TOCLOSE, TIL_SS_CLOSE); }
	

	Net_UInt GetSessionStatus(Net_UInt dwMask){ return m_unSessionStatus & dwMask; }
	void SetSessionStatus(Net_UInt dwValue, Net_UInt dwMask){ m_unSessionStatus &= ~dwMask; m_unSessionStatus |= (dwValue & dwMask); }
protected:
	void SetLibEvent(pCallSameRefNetSessionFunc pCallback, Net_PtrInt lRevert = 0, bool bWait = false);
	void ReleaseCallback();
	virtual void CloseCallback(BOOL bRemote);
protected:
	CRefBasicSessionNet		m_refSelf;
	HandleReceive			m_funcReceive;
	HandleSend				m_funcSend;
	HandleConnect			m_funcConnect;
	HandleDisConnect		m_funcDisconnect;
	HandleIdle				m_funcIdle;
	HandleError				m_funcError;

	CNetThread*				m_pThread;
	CBasicPreSend*			m_pPreSend;

	Net_UInt				m_unSessionStatus;				//״̬  TIL_SS_*

	evutil_socket_t			m_socketfd;
	event					m_revent;

	friend class CNetThread;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_BUFFER				4096
struct SendBuffer
{
	char	buf[MAX_BUFFER];
	int		len;

	SendBuffer()
	{
		memset(this, 0, sizeof(SendBuffer));
	}
};

class CBasicSessionNetClient : public CBasicSessionNet
{
public:
	static CBasicSessionNetClient* CreateClient(Net_UInt nSessionID){ return new CBasicSessionNetClient(nSessionID); }
protected:
	//must be new object
	CBasicSessionNetClient(Net_UInt nSessionID);
	virtual ~CBasicSessionNetClient();

public:
	//uni id
	Net_UInt GetSessionID(){ return m_nSessionID; }

	/*formats
	-[IPv6Address]:port
	-IPv4Address:port
	*/
	Net_Int Connect(const char* lpszAddress);
	Net_Int DoConnect();
	BOOL IsConnected() { return GetSessionStatus(TIL_SS_LINK) == TIL_SS_CONNECTED; }

	virtual Net_Int Send(void *pData, Net_Int cbData, Net_UInt dwFlag = 0);
	
	void GetNetAddress(basiclib::CBasicString& strAddr){ strAddr = m_szPeerAddr; }
	UINT GetNetAddressPort(){ return m_nPeerPort; }
	//out timer 1s
	virtual void OnTimer();
protected:
	virtual Net_Int OnConnect(Net_UInt dwNetCode);
	virtual Net_Int OnDisconnect(Net_UInt dwNetCode);
	virtual void OnSendData(Net_UInt dwIoSize);
	virtual Net_Int OnReceive(Net_UInt dwNetCode, const char *pszData, Net_Int cbData);
	virtual Net_Int OnIdle(Net_UInt dwIdleCount);		//���д���
protected:
	void InitClientEvent(evutil_socket_t socketfd, bool bAddWrite = true);
	void AddWriteEvent();
	virtual void InitMember();
	virtual void CloseCallback(BOOL bRemote);

	void Accept(evutil_socket_t s, sockaddr_storage& addr);
	friend void AcceptToSelf(CBasicSessionNetClient* p, evutil_socket_t s, sockaddr_storage& addr);

	void SetIdleCallback();
	void _OnIdle();

	Net_Int SendData(void *pData, Net_Int cbData, Net_UInt dwFlag);
	BOOL ReadBuffer(Net_Int lSend);
	BOOL SendDataFromQueue();

	void OnReadEvent();
	void OnWriteEvent();
	friend void OnLinkRead(int fd, short event, void *arg);
	friend void OnLinkWrite(int fd, short event, void *arg);
	void OnReceiveData(const char* pszData, Net_UInt dwIoSize);
protected:
	//��������������
	Net_Int PreReceiveData(Net_UInt dwNetCode, const char *pszData, Net_Int cbData);
	/*\brief ���ù�����״̬ */
	void ResetPreSend();
protected:
	Net_UInt				m_nSessionID;
	//default shakehandle timeout time
	unsigned short			m_usTimeoutShakeHandle;
	Net_UInt				m_unIdleCount;				//������еĴ���

	event					m_wevent;
	CBasicSendBufferQueue	m_quSend;
	basiclib::SpinLock		m_lockSend;
	SendBuffer				m_outBuffer;		//���͵Ļ���

	char					m_szPeerAddr[ADDRESS_MAX_LENGTH];
	UINT					m_nPeerPort;
	basiclib::CBasicString	m_strConnectAddr;

	BasicNetStat			m_stNet;
};
typedef basiclib::CBasicRefPtr<CBasicSessionNetClient> CRefBasicSessionNetClient;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef basiclib::basic_map<Net_UInt, CRefBasicSessionNetClient>::type		MapClientSession;
typedef basiclib::basic_vector<CRefBasicSessionNetClient>::type				VTClientSession;
class CBasicSessionNetServer : public CBasicSessionNet
{
public:
	static CBasicSessionNetServer* CreateServer(Net_UInt nSessionID = 0){ return new CBasicSessionNetServer(nSessionID); }
protected:
	//must be new object
	CBasicSessionNetServer(Net_UInt nSessionID = 0);
	virtual ~CBasicSessionNetServer();

public:
	BOOL IsListen();
	/*formats
	-[IPv6Address]:port
	-IPv4Address:port
	*/
	Net_Int Listen(const char* lpszAddress = NULL, bool bWaitSuccess = false);

	//�ⲿontimer����1s
	virtual void OnTimer(int nTick);
protected:
	void InitListenEvent(evutil_socket_t socketfd);
	void OnListenReadEvent();
	friend void OnLinkListenRead(int fd, short event, void *arg);
	void AcceptClient();
	void CopyClientSession(VTClientSession& vtClient);
protected:
	CBasicSessionNetClient* CreateServerClientSession(Net_UInt nSessionID);
	virtual CBasicSessionNetClient* ConstructSession(Net_UInt nSessionID);
protected:
	Net_UInt							m_sessionIDMgr;
	
	CMutex								m_mtxCSession;
	MapClientSession					m_mapClientSession;
};

__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif 
