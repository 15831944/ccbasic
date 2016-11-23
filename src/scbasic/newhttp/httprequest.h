#pragma once
#ifndef INC_CIFOX_HTTPREQUEST_H__
#define INC_CIFOX_HTTPREQUEST_H__

#include "httpparser.h"

// �����ĵĴ����� add by mojiayong
class CContextHandle : public basiclib::CBasicObject
{
public:
	// param[in] pContent ��Ҫ��������
	CContextHandle(void* pContent) : m_pContent(pContent){}
	//! param[in] pContext ����������õ���������
	//! param[in][out] buffer �������õ����� 
	virtual long HandleContext(void* pContext, basiclib::CBasicSmartBuffer& buffer) = 0;
protected:
	void* m_pContent;
};


class HttpRequest : public IHttpParseHandler
{
public:
	typedef basiclib::basic_hash_map<basiclib::char_string, basiclib::char_string>::type	HeaderContainer;
	typedef basiclib::basic_hash_map<basiclib::char_string, CContextHandle*>::type			ContextHandleContainer;
public:
	HttpRequest(void);
	~HttpRequest(void);

public:
	// ��ȡ����ʽ.
	// GET,POST��
	virtual void	MethodHandler(const char* method);
	// ��ȡURI
	// /index.php?a=b&c=d
	virtual void	URIHandler(const char* URI);
	// ��ȡЭ��
	// HTTP/1.1
	virtual void	ProtocolHandler(const char* protocol);
	// �õ�httpͷ
	// Accept-Encoding: gzip, deflate
	virtual void	HeaderHandler(const char* key, const char* value);
	// ���post������
	virtual void	PostDataHandler(const char* data, size_t len);
	// һ���������������
	virtual void	RequestEndHandler();
	// ���ԭʼ��������[not implement]
	virtual const char* GetRequestData(const char *szkey) const;

public:
	// ȡ��headerͷ��ֵ
	bool	GetHeaderValue(const char* key, basiclib::char_string& value) const;
	const char*	GetHeaderValue(const char* key) const;
    bool	GetHeaderValue(const char* key, basiclib::CTime& value) const;
	bool	GetHeaderValue(const char* key, long& value) const;

	// ȡ��Cookie��ֵ
	const char* GetCookie(const char* key) const;

	// ���Э��ͷ����Ϣ
	const basiclib::char_string&	GetMethod() const;
	const basiclib::char_string&	GetURI() const;
	const basiclib::char_string&	GetProtocol() const;

	const basiclib::char_string&	GetFileName() const;

	// ���GET��ʽ��ͷ��Ϣ
	const char* GetParamValue(const char* key, const char* pDefault = NULL) const;
	// 
	const char* GetContentData() const;
	unsigned long GetContentLength() const;

	bool	CanCache() const;

	template<class Functor>
	void	ForEachParam(Functor func) const
	{
		for_each(m_Params.begin(), m_Params.end(), func);
	}

	template<class Functor>
	void	ForEachHeader(Functor func) const
	{
		for_each(m_Headers.begin(), m_Headers.end(), func);
	}

	// ���������ĵ�һ��������Ϣ
	void	SetContext(void* pContext);
	void*	GetContext() const;

	// KeepAlive
	BOOL	IsKeepAlive() const;
	BOOL	IsSupportGZip() const;

	// ����
	void	Reset();

	// ���öԶ˵�ַ
	void		SetPeerAddr(const char* addr);
	const char*	GetPeerAddr() const;
	basiclib::CBasicString& GetRefPeerAddr();

	void	SetFileName(const char* file);

public: // �����ĵĴ���
	//! ���������Ĵ���
	void	AddContextHandle(const char* szKey, CContextHandle* pHandle);
	//! ���ݹؼ���ɾ�������Ĵ���
	void	RemoveContextHandle(const char* szKey);
	//! ���ô�����������һ��buffer
	long	HandleContext(const char* szKey, void* pContext, basiclib::CBasicSmartBuffer& buffer) const;

//	static	void	DecodeString(const char* strIn, char_string& strOut);
protected:
	void	ParseURI(const char* URI);
	
	CContextHandle* FindHandle(const char* szKey) const;
protected:
	basiclib::char_string		m_strMethod;
	basiclib::char_string		m_strURI;
	basiclib::char_string		m_strProtocol;

	basiclib::char_string		m_strFile;
	basiclib::char_string		m_strArgs;

	basiclib::CBasicString		m_strPeerAddr;

	HeaderContainer	m_Params;
	HeaderContainer	m_Cookies;
	HeaderContainer	m_Headers;
	basiclib::CBasicStaticBuffer	m_bufBody;

	void*			m_pContext;
	ContextHandleContainer m_conHandle;
};

#endif // INC_CIFOX_HTTPREQUEST_H__



