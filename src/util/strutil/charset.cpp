#include "charset/charset_def.h"
#include "charset.h"
#include "cppstring.h"

__NS_BASIC_START

const char*	CPCodeToCPStr(int nCPCode)
{
	const char* p = CPS_ACP;
	switch(nCPCode)
	{
	case BASIC_CP_ACP:
		break;
	case CP_GBK:
		p = CPS_GBK;
		break;
	case CP_GB2312:
		p = CPS_GB2312;
		break;
	case CP_GB18030:
		p = CPS_GB18030;
		break;
	case CP_BIG5:
		p = CPS_BIG5;
		break;
	case BASIC_CP_UTF8:
		p = CPS_UTF8;
		break;
	case BASIC_CP_UTF7:
		p = CPS_UTF7;
		break;
	}
	return p;
}

int CPStrToCPCode(const char* strCPStr)
{
	int nCPCode = BASIC_CP_ACP;
	if (0 == __tcscmp(strCPStr, CPS_GB2312))
		return CP_GB2312;
	else if (0 == __tcscmp(strCPStr, CPS_GBK))
		return CP_GBK;
	else if (0 == __tcscmp(strCPStr, CPS_BIG5))
		return CP_BIG5;
	else if (0 == __tcscmp(strCPStr, CPS_UTF8))
		return BASIC_CP_UTF8;
	else if (0 == __tcscmp(strCPStr, CPS_UTF7))
		return BASIC_CP_UTF7;
	else if (0 == __tcscmp(strCPStr, CPS_GB18030))
		return CP_GB18030;
	else if (0 == __tcscmp(strCPStr, CPS_UTF16))
		return CP_UTF16;
	return nCPCode;
}
//-------------------------------------------------------------------------------
wchar_string Basic_MultiStringToWideString(const char* str, int len, unsigned int nCodePage)
{
	wchar_string dest;
	return Basic_MultiStringToWideString(str, len, dest, nCodePage);
}

char_string Basic_WideStringToMultiString(const WCHAR* str, int len, unsigned int nCodePage)
{
	char_string dest;
	return Basic_WideStringToMultiString(str, len , dest, nCodePage);
}

char_string Basic_MultiStringToMultiString(const char* str, int len, unsigned int nCPFrom, unsigned int nCPTo)
{
	char_string dest;
	return Basic_MultiStringToMultiString(str, len, dest, nCPFrom, nCPTo);
}

char_string Basic_MultiStringToUTF8(const char* str, int len, unsigned int nCodePage)
{
	char_string dest;
	return Basic_MultiStringToUTF8(str, len, dest, nCodePage);
}
//---------------------------------------------------------------------------------
wchar_string Basic_MultiStringToWideString(const char* str, int len, const char* from)
{
	wchar_string dest;
	return Basic_MultiStringToWideString(str, len, dest, from);
}

char_string	Basic_WideStringToMultiString(const WCHAR* str, int len, const char* to)
{
	char_string dest;
	return Basic_WideStringToMultiString(str, len, dest, to);
}

char_string	Basic_MultiStringToMultiString(const char* str, int len, const char* from, const char* to)
{
	char_string dest;
	return Basic_MultiStringToMultiString(str, len, dest, from, to);
}

char_string	Basic_MultiStringToUTF8(const char* str, int len, const char* from)
{
	char_string dest;
	return Basic_MultiStringToUTF8(str, len, dest, from);
}
//--------------------------------------------------------------------------------
char_string	Basic_GbToBig5(const char* str, int len)
{
	char_string dest;
	return Basic_GbToBig5(str, len, dest);
}

wchar_string Basic_GbToBig5(const WCHAR* str, int len)
{
	wchar_string dest;
	return Basic_GbToBig5(str, len, dest);
}
//--------------------------------------------------------------------------------
bool Basic_IsUTF8Str(const char* str, int length)
{
	if (length < 0)
		length = __tcslen(str);
	int i = 0;
	unsigned long nBytes = 0;//UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�
	unsigned char chr = 0;
	bool bAllAscii = true; //���ȫ������ASCII, ˵������UTF-8
	for(i = 0; i < length; ++i)
	{
		chr = *(str + i);
		if( (chr & 0x80) != 0 ) // �ж��Ƿ�ASCII����,�������,˵���п�����UTF-8,ASCII��7λ����,����һ���ֽڴ�,���λ���Ϊ0,o0xxxxxxx
			bAllAscii = false;
		if(nBytes == 0) //�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���
		{
			if(chr >= 0x80)
			{
				if(chr >= 0xFC && chr <= 0xFD)
					nBytes = 6;
				else if (chr >= 0xF8)
					nBytes = 5;
				else if (chr >= 0xF0)
					nBytes = 4;
				else if (chr >= 0xE0)
					nBytes = 3;
				else if (chr >= 0xC0)
					nBytes = 2;
				else
				{
					return false;
				}
				nBytes--;
			}
		}
		else //���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx
		{
			if( (chr & 0xC0) != 0x80 )
			{
				return false;
			}
			nBytes--;
		}
	}
	if( nBytes > 0 ) //Υ������
	{
		return false;
	}
	if( bAllAscii ) //���ȫ������ASCII, ˵������UTF-8
	{
		return false;
	}
	return true;
} 

//! ��һ�� TCHAR �ַ���ת���� char �ַ�����ֻ���� UNICODE �汾�����Ҫת����
CTStringToMultiString::CTStringToMultiString(LPCTSTR lpszText)
{
	if(lpszText != NULL)
	{
	#if defined(_UNICODE)
		m_str =	Basic_WideStringToMultiString(lpszText, -1);
		m_psz = m_str.c_str();
	#else
		m_psz = lpszText;
	#endif
	}
	else
	{
		m_psz = NULL;
	}
}
const char* CTStringToMultiString::GetMultiString(long& lTextLength)
{
	if(m_psz != NULL)
	{
		lTextLength = __tcslen(m_psz);
	}
	return m_psz;
}

__NS_BASIC_END
