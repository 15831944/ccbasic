#pragma once
#ifndef INC_CIFOX_HTTPPARSER_H__
#define INC_CIFOX_HTTPPARSER_H__

#include "../../inc/basic.h"

class IHttpParseHandler : public basiclib::CBasicObject
{
public:
	// ��ȡ����ʽ.
	// GET,POST��
	virtual void	MethodHandler(const char* method) = 0;
	// ��ȡURI
	// /index.php?a=b&c=d
	virtual void	URIHandler(const char* URI) = 0;
	// ��ȡЭ��
	// HTTP/1.1
	virtual void	ProtocolHandler(const char* protocol) = 0;
	// �õ�httpͷ
	// Accept-Encoding: gzip, deflate
	virtual void	HeaderHandler(const char* key, const char* value) = 0;
	// ���post������
	virtual void	PostDataHandler(const char* data, size_t len) = 0;
	// һ���������������
	virtual void	RequestEndHandler() = 0;
};


const int	HTTP_ERROR_WANTMORE			= 1;	// ����δ��������Ҫ����
const int	HTTP_ERROR_NEWREQUEST		= 2;	// �µ�������
const int	HTTP_ERROR_FINISH			= 0;	// ����
const int	HTTP_ERROR_NODATA			= -1;	// ������
const int	HTTP_ERROR_PROTOCOL			= -2;	// Э�鲻֧��
const int	HTTP_ERROR_URI_TOOLONG		= -3;	// URI�����������
const int	HTTP_ERROR_HEADER_TOOLONG	= -4;	// httpͷ���ȳ����������
const int	HTTP_ERROR_SYNTAX			= -5;	// ��ʽ����
const int	HTTP_ERROR_METHOD			= -6;	// method��֧��
const int	HTTP_ERROR_GENERIC			= -99;	// ����

enum HttpParseStatus
{
	PS_Method,
	PS_URI,
	PS_Protocol,
	PS_Header_Key,
	PS_Header_Value,
	PS_Body,
	PS_End
};

struct HttpParseContext
{
	HttpParseStatus	m_eStatus;
	basiclib::CBasicSmartBuffer	m_bufTemp;	// ��ʱbuffer�����ڴ洢�ϸ������δ�����겿��
	basiclib::char_string		m_strKey;	// ����headerʱ��keyֵ��
	size_t			m_nBodyLen;
	
	HttpParseContext()
	{
		m_eStatus = PS_Method;
		m_nBodyLen = 0;
	}
	void Reset()
	{
		m_eStatus = PS_Method;
		m_nBodyLen = 0;
//		m_bufTemp.SetDataLength(0);
		m_strKey.empty();
	}
};

// http�������Ľ�����
class CHttpParser : public basiclib::CBasicObject
{
public:
	CHttpParser();
	~CHttpParser();

	int		Parse(const char* buffer, size_t len, size_t& remain, IHttpParseHandler* pHandler);
	static void DecodeString(const char* strIn, basiclib::char_string& strOut);

protected:
	HttpParseContext	__context;
};



/*!
����URI��Ϣ����key��value����uri�����utf8���룬�ŵ�map�����С�
*/
template<class MapContainer>
class FillURIParam
{
public:
	FillURIParam(MapContainer& container) : __container(container) {}

	void operator()(const char* key, const char* value)
	{
		tstring_s k, v;
		CHttpParser::DecodeString(key, k);
		CHttpParser::DecodeString(value, v);
		__container[k] = v;
	}
protected:
	MapContainer&	__container;
};



#endif //INC_CIFOX_HTTPPARSER_H__



