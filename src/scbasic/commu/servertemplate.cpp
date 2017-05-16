#include "servertemplate.h"

CNetServerControlClient::CNetServerControlClient(uint32_t nSessionID, CRefNetServerControl pServer) : basiclib::CBasicSessionNetClient(nSessionID, false)
{
	m_server = pServer;
    m_bVerify = false;
}
CNetServerControlClient::~CNetServerControlClient()
{

}
void CNetServerControlClient::SuccessLogin(){
    m_server->SuccessLogin(this);
}
int32_t CNetServerControlClient::OnConnect(uint32_t dwNetCode)
{
	if (!m_server->m_ipTrust.IsIpTrust(m_szPeerAddr))
	{
		basiclib::BasicLogEventErrorV("��ַ��%s ���ڰ�������!", m_szPeerAddr);
		Close();
		return BASIC_NET_OK;
	}

	if (m_server->m_nSessionMaxCount >= 0)
	{
		if (m_server->GetOnlineSessionCount() > m_server->m_nSessionMaxCount)
		{
            basiclib::BasicLogEventErrorV("��ַ��%s ������������� %d!", m_szPeerAddr, m_server->m_nSessionMaxCount);
			Close();
			return BASIC_NET_OK;
		}
	}
	//��Ҫ��֤
    int32_t nRet = basiclib::CBasicSessionNetClient::OnConnect(dwNetCode);
    if (nRet == BASIC_NET_OK){
        SuccessLogin();
    }
    return nRet;
}
int32_t CNetServerControlClient::OnDisconnect(uint32_t dwNetCode)
{
    if (m_bVerify){
        m_server->SuccessLoginout(this);
    }
	return basiclib::CBasicSessionNetClient::OnDisconnect(dwNetCode);
}
//////////////////////////////////////////////////////////////////////////////////////
CNetServerControl::CNetServerControl(uint32_t nSessionID) : basiclib::CBasicSessionNetServer(nSessionID){
	m_nSessionMaxCount = -1;
	m_handleVerifySuccess = nullptr;
}

CNetServerControl::~CNetServerControl(){
}

void CNetServerControl::SetIpTrust(const char* lpszIpTrust){
	m_ipTrust.SetIPRuler(lpszIpTrust);
}

void CNetServerControl::SetSessionMaxCount(int nCount){
	m_nSessionMaxCount = nCount;
}

void CNetServerControl::OnTimer(uint32_t nTick){
	if (nTick % 10 == 1)
	{
		IsListen();
	}
	basiclib::CBasicSessionNetServer::OnTimer(nTick);
}

BOOL CNetServerControl::IsListen(){
	BOOL bRet = FALSE;
	if (m_strListenAddr.IsEmpty())
	{
		long lRet = Listen(m_strListenAddr.c_str());

		if (BASIC_NET_OK == lRet)
		{
            basiclib::BasicLogEventV(basiclib::DebugLevel_Info, "ListenPort: [%s] Success!", m_strListenAddr.c_str());
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

int32_t CNetServerControl::StartServer(const char* lpszAddress, basiclib::CBasicPreSend* pPreSend){
	if (pPreSend)
	{
		RegistePreSend(pPreSend);
	}
	int32_t nRet = Listen(lpszAddress, true);
	if (nRet == BASIC_NET_OK)
	{
        basiclib::BasicLogEventV(basiclib::DebugLevel_Info, "ListenPort: %s Success!", lpszAddress);
	}
	else
	{
		Close();
        basiclib::BasicLogEventV(basiclib::DebugLevel_Info, "ListenPort: %s Error!", lpszAddress);
	}
	return nRet;
}

void CNetServerControl::SuccessLoginout(basiclib::CBasicSessionNetClient* pNotify){
    ((CNetServerControlClient*)pNotify)->m_bVerify = false;
    basiclib::CSingleLock lockUser(&m_mtxCSessionVerify, TRUE);
    m_mapClientSessionVerify.erase(pNotify->GetSessionID());
    lockUser.Unlock();
}
//�û���¼�ɹ�
bool CNetServerControl::SuccessLogin(basiclib::CBasicSessionNetClient* pNotify){
    //�󶨵�������������
    pNotify->bind_rece(m_funcReceive);
    ((CNetServerControlClient*)pNotify)->m_bVerify = true;
	//�����û�����
	uint32_t nSessionID = pNotify->GetSessionID();
	basiclib::CSingleLock lockUser(&m_mtxCSessionVerify, TRUE);
	basiclib::MapClientSession::iterator iter = m_mapClientSessionVerify.find(nSessionID);
	if (iter != m_mapClientSessionVerify.end()){
		basiclib::BasicLogEventErrorV("SuccessLogin ���sessionʧ�� SessionID(%d)", pNotify->GetSessionID());
		pNotify->Close(0);
		return false;
	}
	m_mapClientSessionVerify[nSessionID] = pNotify;

	if (m_handleVerifySuccess)
		if (!m_handleVerifySuccess(pNotify)){
			pNotify->Close(0);
			return false;
		}
	return true;
}

basiclib::CBasicSessionNetClient* CNetServerControl::ConstructSession(uint32_t nSessionID){
	return CNetServerControlClient::CreateControlClient(nSessionID, this);
}

