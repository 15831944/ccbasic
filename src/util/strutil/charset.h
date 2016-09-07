/***********************************************************************************************
// �ļ���:     charset.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 11:18:55
// ��������:   ���ڱ���ת����ͷ�ļ���
�ھ���ʵ�ֺͲ���ϵͳ�йء�
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_CHARSET_H
#define BASIC_CHARSET_H

#pragma once

#include "charset/charset_def.h"
#include "strutil.h"

__NS_BASIC_START

const char*	CPCodeToCPStr(int nCPCode);
int CPStrToCPCode(const char* strCPStr);

/*!�ַ���תΪUNICODE(UTF-16LE)����
* \param str [in]Դ�ַ���
* \param len [in]Դ�ַ�������
* \param nCodePage [in]Դ�ַ�����code page��Ĭ��0
* \return ת����Ŀ��ַ���
*/
wchar_string Basic_MultiStringToWideString(const char* str, int len, unsigned int nCodePage = 0);

/*!�ַ���תΪUNICODE(UTF-16LE)����
* \param src [in]Դ�ַ���
* \param len [in]Դ�ַ�������
* \param dest [out]ת����������ַ�����
* \param nCodePage [in]Դ�ַ�����code page
* \return ת����Ŀ��ַ���,ͬdest
*/
wchar_string& Basic_MultiStringToWideString(const char* src, int len, wchar_string& dest, unsigned int nCodePage = 0);

/*!UNICODE(UTF-16)������ַ���תΪĿ���ַ�������
* \param str [in]Դ�ַ���
* \param len [in]Դ�ַ�������
* \param nCodePage [in]Ŀ���ַ�����code page
* \return ת������ַ���
*/
char_string Basic_WideStringToMultiString(const WCHAR* str, int len, unsigned int nCodePage = 0);

/*!UNICODE(UTF-16)������ַ���תΪĿ���ַ�������
* \param src [in]Դ�ַ���
* \param len [in]Դ�ַ�������
* \param dest [out]Ŀ���ַ���
* \param nCodePage [in]Ŀ���ַ�����code page
* \return ת�����ַ���,ͬdest
*/
char_string& Basic_WideStringToMultiString(const WCHAR* src, int len, char_string& dest, unsigned int nCodePage = 0);


/*!���ַ�UTF-16������ת��
* \param str [in]Դ�ַ���
* \param len [in]Դ�ַ�������
* \param nCPFrom [in]Դ�ַ�����code page
* \param nCPTo [in]Ŀ���ַ�����code page
* \return ת������ַ���
*/
char_string Basic_MultiStringToMultiString(const char* str, int len, unsigned int nCPFrom, unsigned int nCPTo);

/*!���ַ�UTF-16������ת��
* \param src [in]Դ�ַ���
* \param len [in]Դ�ַ�������
* \param dest [out]Ŀ���ַ���
* \param nCPFrom [in]Դ�ַ�����code page
* \param nCPTo [in]Ŀ���ַ�����code page
* \return  ת�����ַ����ĳ���
*/
char_string& Basic_MultiStringToMultiString(const char* src, int len, char_string& dest, unsigned int nCPFrom, unsigned int nCPTo);


/*!�ַ���ת��ΪUTF8
* \param str [in]Դ�ַ�������UTF-16����
* \param len [in]Դ�ַ�������
* \param nCodePage [in]ԭ�ַ���code page
* \return ת������ַ�����
*/
char_string Basic_MultiStringToUTF8(const char* str, int len, unsigned int nCodePage = 0);

/*!�ַ���ת��ΪUTF8,strΪ��UNICODE6����
* \param src [in]Դ�ַ�������UTF-16����
* \param len [in]Դ�ַ�������
* \param dest [out]Ŀ���ַ���
* \param nCodePage [in]Դ�ַ���code page
* \return ת������ַ�����ͬdest
*/
char_string& Basic_MultiStringToUTF8(const char* src, int len, char_string& dest, unsigned int nCodePage = 0);

//---------------------8<------------------code page ��ͬ�ķָ���--------------------

/*!�ַ���תΪUNICODE(UTF-16)����
* \param str [in]Դ�ַ�������UTF-16����
* \param len [in]Դ�ַ�������
* \param from [in]ԭ�ַ�������
* \return ת������ַ���
*/
wchar_string Basic_MultiStringToWideString(const char* str, int len, const char* from);

/*!�ַ���תΪUNICODE(UTF-16)����
* \param src [in]Դ�ַ�������UTF-16����
* \param len [in]Դ�ַ�������
* \param dest [out]Ŀ���ַ���
* \param from [in]ԭ�ַ�������
* \return ת������ַ�����ͬdest
*/
wchar_string& Basic_MultiStringToWideString(const char* src, int len, wchar_string& dest, const char* from);

/*!UNICODE(UTF-16)������ַ���תΪĿ���ַ�������
* \param str [in]Դ�ַ���
* \param len [in]Դ�ַ�������
* \param to [in]Ŀ���ַ�����code page
* \return char_string ת������ַ���
*/
char_string	Basic_WideStringToMultiString(const WCHAR* str, int len, const char* to);


/*!UNICODE(UTF-16)������ַ���תΪĿ���ַ�������
* \param src [in]Դ�ַ���
* \param len [in]Դ�ַ�������
* \param dest [out]Ŀ���ַ���
* \param to [in]Ŀ���ַ�����code page
* \return ת�����ַ���,ͬdest
*/
char_string& Basic_WideStringToMultiString(const WCHAR* src, int len, char_string& dest, const char* to);


/*!���ַ�UTF-16������ת��
* \param str [in]Դ�ַ���
* \param len [in]Դ�ַ�������
* \param from [in]Դ�ַ�����code page
* \param to [in]Ŀ���ַ�����code page
* \return ת������ַ���
*/
char_string	Basic_MultiStringToMultiString(const char* str, int len, const char* from, const char* to);

/*!���ַ�UTF-16������ת��
* \param src [in]Դ�ַ���
* \param len [in]Դ�ַ�������
* \param dest [out]Ŀ���ַ���
* \param from [in]Դ�ַ�����code page
* \param to [in]Ŀ���ַ�����code page
* \return  ת�����ַ����ĳ���
*/
char_string& Basic_MultiStringToMultiString(const char* src, int len, char_string& dest, const char* from, const char* to);


/*!�ַ���ת��ΪUTF8
* \param str [in]Դ�ַ�������UTF-16����
* \param len [in]Դ�ַ�������
* \param from [in]ԭ�ַ���code page
* \return ת������ַ�����
*/
char_string	Basic_MultiStringToUTF8(const char* str, int len, const char* from);

/*!�ַ���ת��ΪUTF8,strΪ��UNICODE6����
* \param src [in]Դ�ַ�������UTF-16����
* \param len [in]Դ�ַ�������
* \param dest [out]Ŀ���ַ���
* \param from [in]Դ�ַ���code page
* \return ת������ַ�����ͬdest
*/
char_string& Basic_MultiStringToUTF8(const char* src, int len, char_string& dest, const char* from);

// --------------------8<-----------------���崦��ķָ���------------------

/*!����ת����*/
template<typename CharType>
typename __BasicString<CharType>::StringType Basic_GbToBig5(const CharType* str, int len)
{
	typedef typename __BasicString<CharType>::StringType StringType;
	StringType dest;
	return Basic_GbToBig5(str, len, dest);

}
/*!����ת����*/
char_string&	Basic_GbToBig5(const char* src, int len, char_string& dest);
/*!����ת����*/
wchar_string&	Basic_GbToBig5(const WCHAR* str, int len, wchar_string& dest);

/*!����ת����*/
template<typename CharType>
typename __BasicString<CharType>::StringType Basic_Big5ToGb(const CharType* str, int len)
{
	typedef typename __BasicString<CharType>::StringType StringType;
	StringType dest;
	return Basic_Big5ToGb(str, len, dest);
}
/*!����ת����*/
char_string&	Basic_Big5ToGb(const char* str, int len, char_string& dest);
/*!����ת����*/
wchar_string&	Basic_Big5ToGb(const WCHAR* str, int len, wchar_string& dest);


/*!�ж�һ���ַ����Ƿ���UTF8����*/
bool			Basic_IsUTF8Str(const char* str, int len);

//! ��һ�� TCHAR �ַ���ת���� char �ַ�����ֻ���� UNICODE �汾�����Ҫת����
class CTStringToMultiString
{
public:
	CTStringToMultiString(LPCTSTR lpszText);
public:
	const char* GetMultiString(long& lTextLength);
protected:
	const char* m_psz;
#if defined(_UNICODE)
	char_string	m_str;
#endif
};

__NS_BASIC_END
#endif 
