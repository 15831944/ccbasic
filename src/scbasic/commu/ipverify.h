#ifndef INC_IPVERIFY_H_
#define INC_IPVERIFY_H_

#include "../../inc/basic.h"
#include "../scbasic_head.h"

#pragma warning (push)
#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
/////////////////////////////////////////////////////////////////////////////////////////////
// 1��֧��IP�б�������֤
// 2�����ýӿ�ʱ�����IP�ڲ��Զ���֤�Ƿ�Ϊ��ЧIP��ַ
// 3�����򴮶��壺
// �磺*;192.168.0.1:255.255.255.0@����;10.0.1.51:255.255.255.0@����
//		*��������ȫ�� �������ã�ֻΪ�����Ķ�
/////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_IP_ITEM			(int)4				// 

struct _SCBASIC_DLL_API _IPRuler
{
	BYTE		m_szIP[MAX_IP_ITEM];			// IP��ַ
	BYTE		m_szMask[MAX_IP_ITEM];			// ��������
	BYTE		m_szResult[MAX_IP_ITEM];		// IP������������(&)�������
	
	_IPRuler()
	{
		memset(this, 0, sizeof(_IPRuler));
	}
};

//֧��IP��ַ��֤
class _SCBASIC_DLL_API CIpVerify : public basiclib::CBasicObject
{
public:
	CIpVerify();
	virtual ~CIpVerify();
	
public:
	virtual int SetIPRuler(const char* lpszRuler);
	virtual BOOL IsIpTrust(const char* lpszIP);
	
protected:
	BOOL AddIPRuler(const char* lpszIP, const char* lpszMask);
	BOOL IsIPAddr(const char* lpszIP, BYTE* szIP, int cbIP);
	void EmptyRuler();

protected:
	basiclib::CPtrList	m_lsIpRuler;			// IP�����б�
	BOOL		m_bSupportAll;			// ����ȫ�� (*)
	basiclib::CMutex		m_synObj;				// ͬ������
};

/************************************************************************************************
֧�������Ͷ˿ڵ���֤ƥ��
���򣺵�ַ1:�˿�1;��ַ2:�˿�2
      172.20.0.153:8601;172.20.0.*:8601;*:8601;www.abcd.com:1234
*************************************************************************************************/
//�ַ���ƥ�����
class _SCBASIC_DLL_API CBasicStringCmpInfo : public basiclib::CBasicObject
{
public:
	CBasicStringCmpInfo();
	virtual ~CBasicStringCmpInfo();

	//���ù���
	virtual long InitRuleInfo(const char* lpszRule, const char* lpszSplit = ";");

	//�������
	BOOL AddRuleInfo(basiclib::CBasicString& strRuleInfo);

	//�ж��Ƿ��ڷ�Χ��
	long IsInRule(const char* lpszData, long lIndex = 0);

	//��ȡ״̬
	virtual void GetStatus(basiclib::CBasicString& strInfo);
protected:
	typedef basiclib::basic_vector<basiclib::CBasicStringArray> ContainRuleInfo;
	typedef ContainRuleInfo::iterator							ContainRuleInfoIterator;
	ContainRuleInfo  m_ayRuleInfo;

	basiclib::CBasicString m_strInfo;
};

//�����ַ���ƥ������,����ƥ��
class _SCBASIC_DLL_API CIpDomainVerify : public CBasicStringCmpInfo
{
public:
	CIpDomainVerify();
	virtual ~CIpDomainVerify();

	//���ù���
	virtual long InitRuleInfo(const char* lpszRule, const char* lpszSplit = ";");

	//��ȡ״̬
	virtual void GetStatus(basiclib::CBasicString& strInfo);

	//�Ƿ�������IP�Ͷ˿�
	BOOL IsTrust(const char* lpszData, WORD wPort);

	//
	basiclib::CBasicString GetPortInfo(int nIndex);
protected:
	typedef basiclib::basic_vector<unsigned short>				ContainPort;
	typedef basiclib::basic_vector<ContainPort>					ContainNumber;
	typedef ContainNumber::iterator               ContainNumberIterator;

	ContainNumber                                m_ayRuleExtre;
	BOOL                                         m_bTrustAll;  //��������
};
#pragma warning (pop)
#endif



