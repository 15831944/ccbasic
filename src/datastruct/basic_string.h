/***********************************************************************************************
// �ļ���:     string.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 9:17:12
// ��������:   ʵ��stringt��MFC like�ӿڡ�
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_STRING_H
#define BASIC_STRING_H

//#pragma once

#include <stdio.h>
#include <string>
#include "../util/strutil/strutil.h"

using namespace std;

__NS_BASIC_START
class _BASIC_DLL_API CBasicString : public tstring_s
{
public:
	BASIC_CREATEFUNC(CBasicString)

	CBasicString();
	CBasicString(const char* lpsz);
	CBasicString(const char* lpch, int nLength);
	//! from a single character
	CBasicString(char ch, int nRepeat = 1);

	//! get data length
	int GetLength() const;
	//! TRUE if zero length
	BOOL IsEmpty() const;
	//! clear contents to empty
	void Empty();

	//! return single character at zero-based index
	char GetAt(int nIndex) const;
	//! set a single character at zero-based index
	void SetAt(int nIndex, char ch);

	//! straight character comparison
	int Compare(const char* lpsz) const;
	//! compare ignoring case
	int CompareNoCase(const char* lpsz) const;

	//! return nCount characters starting at zero-based nFirst
	CBasicString Mid(int nFirst, int nCount) const;
	//! return all characters starting at zero-based nFirst
	CBasicString Mid(int nFirst) const;
	//! return first nCount characters in string
	CBasicString Left(int nCount) const;
	//! return nCount characters from end of string
	CBasicString Right(int nCount) const;

	//! NLS aware conversion to uppercase
	void MakeUpper();
	//! NLS aware conversion to lowercase
	void MakeLower();
	//! reverse string right-to-left
	void MakeReverse();

	//! remove whitespace starting from right edge
	void TrimRight();
	//! remove whitespace starting from left side
	void TrimLeft();
	//! remove continuous occurrences of chTarget starting from right
	void TrimRight(char chTarget);
	//! remove continuous occcurrences of characters in passed string,
	//! starting from right
	void TrimRight(const char* lpszTargets);
	//! remove continuous occurrences of chTarget starting from left
	void TrimLeft(char chTarget);
	//! remove continuous occcurrences of characters in
	//! passed string, starting from left
	void TrimLeft(const char* lpszTargets);

	//! replace occurrences of chOld with chNew
	int Replace(char chOld, char chNew);
	//! replace occurrences of substring lpszOld with lpszNew;
	//! empty lpszNew removes instances of lpszOld
	int Replace(const char* lpszOld, const char* lpszNew);

	//! find character starting at left, -1 if not found
	int Find(char ch) const;
	//! find character starting at right
	int ReverseFind(char ch) const;
	//! find character starting at zero-based index and going right
	int Find(char ch, int nStart) const;
	//! find first instance of any character in passed string
	int FindOneOf(const char* lpszCharSet) const;
	//! find first instance of substring
	int Find(const char* lpszSub) const;
	//! find first instance of substring starting at zero-based index
	int Find(const char* lpszSub, int nStart) const;
	//! get pointer to modifiable buffer at least as long as nMinBufLength
	char* GetBuffer(int nMinBufLength);
	//! release buffer, setting length to nNewLength (or to first nul if -1)
	void ReleaseBuffer(int nNewLength = -1);
	//! get pointer to modifiable buffer exaCBasicy as long as nNewLength
	char* GetBufferSetLength(int nNewLength);

	//! format string content
	void Format(const char* lpszFormat, ...);
	//! format string content by arglist
	void FormatV(const char* lpszFormat, va_list argList);

	/*
	ȫ��ʹ��ʱ�������Դ�ļ���ʹ��lpszModuleName��������, ģ������Basic_LoadStringSourceʱ��ͬ
	*/
	void FormatS(const char* lpszModuleName, const char* lpszFormatID, ...);
	void FormatS(const char* lpszModuleName, DWORD dwFormatID, ...);

	/*
	ȫ��ʹ��ʱ�������Դ�ļ���ʹ��lpszModuleName��������, ģ������Basic_LoadStringSourceʱ��ͬ
	*/
	BOOL LoadString(const char* lpszFormatID, const char* lpszModuleName = "");
	BOOL LoadString(DWORD dwFormatID, const char* lpszModuleName = "");
};


inline CBasicString	operator+(const CBasicString& lhs, const CBasicString& rhs)
{
	CBasicString str = lhs;
	str += rhs;
	return str;
}

inline CBasicString	operator+(const CBasicString& lhs, const char* rhs)
{
	CBasicString str = lhs;
	str += rhs;
	return str;
}

inline CBasicString	operator+(const char* lhs, const CBasicString& rhs)
{
	CBasicString str = lhs;
	str += rhs;
	return str;
}

inline bool	operator == (const CBasicString& lhs, const CBasicString& rhs)
{
	return lhs.Compare(rhs.c_str()) == 0;
}

inline bool	operator == (const CBasicString& lhs, const char* rhs)
{
	return lhs.Compare(rhs) == 0;
}


inline bool	operator == (const char* lhs, const CBasicString& rhs)
{
	return rhs.Compare(lhs) == 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

__NS_BASIC_END

#if defined(__MSVC)
namespace stdext
{
	inline
		size_t hash_value(const basiclib::CBasicString& str)
	{	// hash _Keyval to size_t value one-to-one
			return (hash_value(str.c_str()));
	}
}
#else
#include <ext/hash_map>
namespace __gnu_cxx
{
	template<> struct hash<basiclib::CBasicString>
	{
		size_t operator()(const basiclib::CBasicString& __s) const
		{
            hash<const char*> hash_fn;
            return hash_fn(__s.c_str());
        }
	};
}
#endif

#endif 

