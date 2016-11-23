#include "servertemplate.h"

CNetServerControlClient::CNetServerControlClient(Net_UInt nSessionID, CRefNetServerControl pServer) : basiclib::CBasicSessionNetClient(nSessionID, false)
{
	m_server = pServer;
}
CNetServerControlClient::~CNetServerControlClient()
{

}

Net_Int CNetServerControlClient::OnConnect(Net_UInt dwNetCode)
{
	if (!m_server->m_ipTrust.IsIpTrust(m_szPeerAddr))
	{
		basiclib::BasicLogEventV("��ַ��%s ���ڰ�������!", m_szPeerAddr);
		Close();
		return BASIC_NET_OK;
	}

	if (m_server->m_nSessionMaxCount >= 0)
	{
		if (m_server->GetOnlineSessionCount() > m_server->m_nSessionMaxCount)
		{
			basiclib::BasicLogEventV("��ַ��%s ������������� %d!", m_szPeerAddr, m_server->m_nSessionMaxCount);
			Close();
			return BASIC_NET_OK;
		}
	}
	//��Ҫ��֤
	return basiclib::CBasicSessionNetClient::OnConnect(dwNetCode);
}
Net_Int CNetServerControlClient::OnDisconnect(Net_UInt dwNetCode)
{
	return basiclib::CBasicSessionNetClient::OnDisconnect(dwNetCode);
}
Net_Int CNetServerControlClient::OnReceive(Net_UInt dwNetCode, const char *pszData, Net_Int cbData)
{
	return basiclib::CBasicSessionNetClient::OnReceive(dwNetCode, pszData, cbData);
}
//////////////////////////////////////////////////////////////////////////////////////
CNetServerControl::CNetServerControl(Net_UInt nSessionID) : basiclib::CBasicSessionNetServer(nSessionID)
{
	m_nSessionMaxCount = -1;
}

CNetServerControl::~CNetServerControl()
{

}

void CNetServerControl::SetIpTrust(const char* lpszIpTrust)
{
	m_ipTrust.SetIPRuler(lpszIpTrust);
}
void CNetServerControl::SetSessionMaxCount(int nCount)
{
	m_nSessionMaxCount = nCount;
}

void CNetServerControl::OnTimer(Net_UInt nTick)
{
	if (nTick % 10 == 1)
	{
		IsListen();
	}
	basiclib::CBasicSessionNetServer::OnTimer(nTick);
}

BOOL CNetServerControl::IsListen()
{
	BOOL bRet = FALSE;
	if (m_strListenAddr.IsEmpty())
	{
		long lRet = Listen(m_strListenAddr.c_str());

		if (BASIC_NET_OK == lRet)
		{
			basiclib::BasicLogEventV("ListenPort: [%s] Success!", m_strListenAddr.c_str());
			bRet = TRUE;
		}
		else if (BASIC_NET_ALREADY_LISTEN != lRet)
		{
			Close();
		}
		else if (BASIC_NET_ALREADY_LISTEN == lRet)
		{
			bRet = TRUE;
		}
	}
	return bRet;
}

Net_Int CNetServerControl::StartServer(const char* lpszAddress, basiclib::CBasicPreSend* pPreSend)
{
	if (pPreSend)
	{
		RegistePreSend(pPreSend);
	}
	Net_Int nRet = Listen(lpszAddress, true);
	if (nRet == BASIC_NET_OK)
	{
		basiclib::BasicLogEventV("ListenPort: %s Success!", lpszAddress);
	}
	else
	{
		Close();
		basiclib::BasicLogEventV("ListenPort: %s Error!", lpszAddress);
	}
	return nRet;
}

basiclib::CBasicSessionNetClient* CNetServerControl::ConstructSession(Net_UInt nSessionID)
{ 
	return CNetServerControlClient::CreateControlClient(nSessionID, this);
}

