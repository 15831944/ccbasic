/***********************************************************************************************
// �ļ���:     net.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2016-9-12 11:50:18
// ��������:   ����TCPͨ�ŵĻ�����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_NET_H
#define BASIC_NET_H

/////////////////////////////////////////////////////////////////////////////////////////////
#include "sendbuffer.h"

__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//����
//class CBasicObject;
class CBasicSessionNet;				//����
class CBasicSessionNetNotify;		//��������

struct BasicNetStat;		//���ͽ��յ�ͳ����Ϣ
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
#define BASIC_NET_RELEASE_ERROR					(NET_ERROR | 0x0010)			//RELEASE����

//���ص�״̬����
#define BASIC_NETCODE_SUCC					0x00000001				//�ɹ�
#define BASIC_NETCODE_CLOSE_REMOTE			0x00000002				//����������ر�
#define BASIC_NETCODE_FILTER_HANDLE			0x00000004				//�����������
#define BASIC_NETCODE_FILTER_ERROR			0x00000008				//�������������

#define BASIC_NETCODE_CONNET_FAIL			0x00000040				//����ʧ��

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
_BASIC_DLL_API void SetNetInitializeGetParamFunc(pGetConfFunc func);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class _BASIC_DLL_API CBasicPreSend : public CBasicObject
{
public:
	CBasicPreSend() {}

public:
	/*\brief �����յ������� */
	// �������ɵ����ݷ���buf
	virtual int32_t OnPreReceive(const char *pszData, int32_t cbData, CBasicBitstream& buf, CBasicSessionNetNotify* pNetSession) = 0;

	/*\brief ���˷��͵����� */
	// �������ɵ����ݷ���buf
	virtual int32_t OnPreSend(const char *pszData, int32_t cbData, uint32_t dwFlag, SendBufferCacheMgr& sendBuf) = 0;

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
struct _BASIC_DLL_API  BasicNetStat{
	uint32_t	m_dwSendBytes;		/*!< ���͵��ֽ��� */
	uint32_t	m_dwSendTimes;		/*!< ���͵Ĵ��� */
	uint32_t	m_dwReceBytes;		/*!< ���յ��ֽ��� */
	uint32_t	m_dwReceTimes;		/*!< ���յĴ��� */
	time_t		m_tmLastRecTime;	//��¼����յ����ݵ�ʱ��
	double		m_fLastSendRate;
	double		m_fLastRecvRate;
	time_t		m_tLastStat;
	BasicNetStat();
	void Empty();
	void OnSendData(int nSend);
	void OnReceiveData(int nRece);
	void GetTransRate(BasicNetStat& lastData, double& dSend, double& dRecv);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TIL_RESET_MASK					0xFF000000		//����״̬

#define TIL_SS_LINK						0x0000000F		//���ӵ�״̬
#define TIL_SS_IDLE						0x00000000		//����
#define TIL_SS_CONNECTING				0x00000001		//��������
#define TIL_SS_CONNECTED				0x00000002		//������
#define TIL_SS_LISTENING				0x00000003		//���ڼ���

#define TIL_SS_CLOSE					0x000000F0		//�رյ�״̬
#define TIL_SS_NORMAL					0x00000000		//����״̬
#define TIL_SS_TOCLOSE					0x00000010		//���ڹر�

#define TIL_SS_SHAKEHANDLE_MASK			0x00000F00		//��֤
#define TIL_SS_SHAKEHANDLE_TRANSMIT		0x00000100		//

//��������close ����
#define TIL_SS_RELEASE_MASK				0xF0000000		//ɾ�����״̬��ʾ
#define TIL_SS_TOSAFEDELETE				0x10000000		//��ȫɾ����ʾ

#define ADDRESS_MAX_LENGTH		64
//////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251)
#pragma warning (disable: 4275)

class CBasicSessionNetNotify;
class CBasicNet_Socket;
class _BASIC_DLL_API CBasicSessionNet : public basiclib::EnableRefPtr<CBasicSessionNet>
{
public:
	typedef basiclib::CBasicRefPtr<CBasicSessionNet>														CRefBasicSessionNet;		//����ָ��
	typedef fastdelegate::FastDelegate4<CBasicSessionNetNotify*, uint32_t, int32_t, const char *, int32_t>	HandleReceive;
	typedef fastdelegate::FastDelegate2<CBasicSessionNetNotify*, uint32_t, int32_t>							HandleConnect;				//��������
	typedef fastdelegate::FastDelegate2<CBasicSessionNetNotify*, uint32_t, int32_t>							HandleDisConnect;			//�Ͽ�����Ϣ
	typedef fastdelegate::FastDelegate2<CBasicSessionNetNotify*, uint32_t, int32_t>							HandleIdle;					//������Ϣ
	typedef fastdelegate::FastDelegate3<CBasicSessionNetNotify*, uint32_t, int32_t, int32_t>				HandleError;				//������Ϣ

	void bind_rece(const HandleReceive& func) { m_funcReceive = func; }
	void bind_connect(const HandleConnect& func) { m_funcConnect = func; }
	void bind_disconnect(const HandleDisConnect& func) { m_funcDisconnect = func; }
	void bind_idle(const HandleIdle& func) { m_funcIdle = func; }
	void bind_error(const HandleError& func) { m_funcError = func; }
	const HandleReceive GetBindRece(){ return m_funcReceive; }
	const HandleConnect GetBindConnect(){ return m_funcConnect; }
	const HandleDisConnect GetBindDisconnect(){ return m_funcDisconnect; }
	const HandleIdle GetBindIdle(){ return m_funcIdle; }
	const HandleError GetBindError(){ return m_funcError; }

	//ע�������
	int RegistePreSend(CBasicPreSend* pFilter, uint32_t dwRegOptions = 0);

	//�����ر�
	void Close(bool bNoWaitMustClose = false);

	//�ṩ��ȫɾ���Ļص��ӿ�
	virtual void SafeDelete();

	//! ��ȡע��Ĺ�����
	CBasicPreSend* GetPreSend();

	//! ��socket
	void InitSocket(CBasicNet_Socket* pSocket);
protected:
	CBasicSessionNet();
	virtual ~CBasicSessionNet();

	//! ����ָ��ɾ���ķ�ʽ
	virtual void DeleteRetPtrObject();

	//! ��ȡsocket
	CBasicNet_Socket* GetBasicNet_Socket(){ return m_pSocket; }
protected:
	CRefBasicSessionNet				m_self;
	CBasicNet_Socket*				m_pSocket;
	HandleReceive					m_funcReceive;
	HandleConnect					m_funcConnect;
	HandleDisConnect				m_funcDisconnect;
	HandleIdle						m_funcIdle;
	HandleError						m_funcError;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class _BASIC_DLL_API CBasicSessionNetNotify : public basiclib::CBasicSessionNet
{
public:
	//! �Ƿ�����
	bool IsConnected();

	//! �Ƿ���֤�ɹ�
	bool IsTransmit();

	//! ����
	virtual int32_t Send(void *pData, int32_t cbData, uint32_t dwFlag = 0);
	virtual int32_t Send(basiclib::CBasicSmartBuffer& smBuf, uint32_t dwFlag = 0);

	//! ��ȡ����״̬
	void GetNetStatInfo(BasicNetStat& netState);

	//! ��ȡ����״̬
	virtual void GetNetStatus(CBasicString& strStatus);
protected:
	CBasicSessionNetNotify();
	virtual ~CBasicSessionNetNotify();
protected:
	//! �ص�connect
	virtual uint32_t OnConnect(uint32_t dwNetCode) {
		if (m_funcConnect)
			return m_funcConnect(this, dwNetCode);
		return BASIC_NET_OK;
	}
	//! �ص�receive
	virtual int32_t OnReceive(uint32_t dwNetCode, const char *pszData, int32_t cbData) {
		if (m_funcReceive)
			return m_funcReceive(this, dwNetCode, cbData, pszData);
		return BASIC_NET_OK;
	}
	//! �ص�disconnect
	virtual uint32_t OnDisconnect(uint32_t dwNetCode) {
		if (m_funcDisconnect)
			return m_funcDisconnect(this, dwNetCode);
		return BASIC_NET_OK;
	}
	//! �ص�����
	virtual uint32_t OnError(uint32_t dwNetCode, int32_t lRetCode) {
		if (m_funcError)
			return m_funcError(this, dwNetCode, lRetCode);
		return BASIC_NET_OK;
	}
	virtual int32_t OnIdle(uint32_t dwIdleCount) {
		if (m_funcIdle)
			return m_funcIdle(this, dwIdleCount);
		return BASIC_NET_OK;
	}
protected:
	//��ʱ��,ontimer�߳�
	virtual void OnTimer(unsigned int nTick){}
protected:

	friend class CBasicNet_SocketTransfer;
};

#pragma warning (pop)
__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////////

#endif 
