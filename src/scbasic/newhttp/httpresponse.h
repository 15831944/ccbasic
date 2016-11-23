#pragma once
#ifndef INC_CIFOX_HTTPRESPONSE_H__
#define INC_CIFOX_HTTPRESPONSE_H__

#include "../../inc/basic.h"

struct HttpStatus;
class HttpResponse : public basiclib::CBasicObject
{
public:
	typedef basiclib::basic_hash_map<basiclib::char_string, basiclib::char_string>::type	HeaderContainer;
public:
	HttpResponse(void);
	~HttpResponse(void);

	/*!
	* ���÷��ص�HttpStatus״̬
	* \param uStatus ���õ�״ֵ̬��Ĭ��200
	*/
	void	SetStatus(unsigned short uStatus);
	
	/*!
	* ��ȡ���ص�HttpStatus״̬
	* \return ��ǰ��״̬
	*/
	unsigned short GetStatus() const;

	/*!
	* ����Http�汾�������version��ʽ����У�飬�����߱������б�֤��ʽ��ȷ��
	* \param pszVersion HTTP�汾��Ϣ��eg: 1.0��1.1
	*/
	void	SetVersion(const char* pszVersion);

	/*!
	* ��ȡ�汾��Ϣ
	* \return �汾�ַ���
	*/
	const char*	GetVersion() const;

	/*!
	* ����Httpͷ��ֵ
	* \param key ����
	* \param value ��ֵ
	* \param replace ���ڵ�ǰ����ü�ֵ�Ѿ����ڵĴ���true:�滻; false:����ʧ��
	*/
	void	SetHeader(const char* key, const char* value, bool replace = true);

	/*!
	* ����Cookie��ֵ
	* \param key ����
	* \param value ��ֵ
	* \param expires ʧЧʱ��
	* \param path cookie����Ч·��
	* \param domain cookie����Ч��
	*/
	void	SetCookie(const char* key, const char* value, time_t expires = 0, const char* path = NULL, const char* domain = NULL);

	/*!
	* ��ȡ�Ѿ����õ�Httpͷ��ֵ
	* \param key ����
	* \param def ����Ҳ���Ĭ�Ϸ��ص�ֵ
	* \return ��ȡ�ļ�ֵ
	*/
	const char* GetHeader(const char* key, const char* def = NULL) const;

	/*!
	* ׷�ӷ��ذ�������
	* \param buf ָ��׷�ӵ��ڴ�ָ��
	* \param len buf�ĳ���
	*/
	void	AppendContent(const char* buf, size_t len);

	/*!
	* �������Httpͷ�������ɵ�ͷ���ݿ�
	* \param buffer[out]���洢���ص�����
	*/
	void	GetHeaderData(basiclib::CBasicSmartBuffer* buffer, long lContentLen = -1) const;

	/*!
	* �������Http���ݿ������
	* \param buffer[out]���洢���ص�����
	*/
	void	GetContent(basiclib::CBasicSmartBuffer* buffer) const;

	/*!
	* �����ص�Http���ݿ�ѹ����ͬʱ������HttpͷContent-Encoding������
	*/
	bool	ZipOutput();
	
	/*!
	* ����status���ɷ��ص������ı�����
	* \param status Http Status��ֵ
	* \return �����ַ���
	*/
	static	const char*	GetStatusString(unsigned short status);

	/*!
	* ��ʽ��ʱ��
	* \param t ʱ��
	* \param gmtime �Ƿ��ʽ��ΪGMTʱ��
	* \return ��ʽ�����ʱ���ַ���
	*/
	static	basiclib::char_string	GetTimeString(time_t t, bool gmtime);

	/*!
	* ��������http���������������(��HttpЭ���һ������)
	* \param pBuffer ָ�����ָ��
	* \param len ���峤��
	* \return HTTP_SUCC�ɹ���
	*/
	int		FromCacheBuffer(const char* pBuffer, size_t len);

	/*!
	* ����Http����(��HttpЭ���һ������)
	* \param pBuffer[out]���ص����ݰ�
	* \return HTTP_SUCC
	*/
	int		MakeCacheBuffer(basiclib::CBasicSmartBuffer* pBuffer) const;

	/*!
	* �ж��Ƿ�KeepAlive
	* \return true: KeepAlive; false: Close
	*/
	bool	IsKeepAlive() const;
protected:
	void	__CombineHeader(basiclib::CBasicSmartBuffer* buffer, const char* key, const char* value) const;
	static	void AppendHeader(basiclib::CBasicSmartBuffer* buffer, const char* key, const char* value);

	void	FillHeader(basiclib::CBasicSmartBuffer* pBuffer, const char* key) const;

	class WrapFillHeader;
	class WrapFillResponse;
protected:
	unsigned short		m_nStatus;
	basiclib::char_string			m_strVersion;
	HeaderContainer		m_Headers;
	basiclib::CBasicSmartBuffer		m_bufContent;
	bool				m_bZipped;
};


#endif // INC_CIFOX_HTTPRESPONSE_H__



