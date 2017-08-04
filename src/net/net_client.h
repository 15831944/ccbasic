/***********************************************************************************************
// �ļ���:     net_client.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2016-9-12 11:50:18
// ��������:   ����TCPͨ�ŵĻ�����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_NET_CLIENT_H
#define BASIC_NET_CLIENT_H

/////////////////////////////////////////////////////////////////////////////////////////////
#include "net.h"

__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//����
class CBasicSessionNetClient;		//��������
//////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251)
#pragma warning (disable: 4275)

//! �����CreateNetClient
_BASIC_DLL_API CBasicSessionNetClient* CreateNetWithClient(size_t nClassSize, uint16_t usRecTimeout, const std::function<CBasicSessionNetClient*(void*)>& func);

#define DefineCreateNetClient(T) \
public:\
	static T* CreateNetClient(uint16_t usRecTimeout = 0){return (T*)basiclib::CreateNetWithClient(sizeof(T), usRecTimeout, [&](void* pData)->CBasicSessionNetClient*{\
			return new (pData)T;\
		});}
#define DefineCreateNetClientDefault(T) \
DefineCreateNetClient(T)\
protected:\
	T(){}\
	virtual ~T(){}

class _BASIC_DLL_API CBasicSessionNetClient : public CBasicSessionNetNotify
{
	DefineCreateNetClientDefault(CBasicSessionNetClient);
public:
	//! ���� formats [IPv6Address]:port || IPv4Address:port
	virtual int32_t Connect(const char* lpszAddress);

	//! ����
	int32_t DoConnect();

	//! ��ȡ���ӵ�ַ
	basiclib::CBasicString& GetConnectAddr();
};
#pragma warning (pop)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////////

#endif 
