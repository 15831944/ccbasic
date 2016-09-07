/***********************************************************************************************
// �ļ���:     strutil.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 11:17:20
// ��������:   һЩ���õ��ַ�����������������
����ַ������ϲ��ַ�����string�ַ������һЩ���ò�����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_STRUTIL_H
#define BASIC_STRUTIL_H


#include <string>
#include <vector>
#include <sstream>
#include "../../inc/basic_def.h"
#include "../private.h"
#include "../../types/types.h"
#include "../../mem/mem.h"
#include "../../mem/basicallocator.h"
#include "../container.h"
#include "cppstring.h"
#include "../../debug/debug.h"

#ifdef __BASICWINDOWS
#include <shlwapi.h>
#endif

#ifdef  _MSC_VER
#pragma warning(disable: 4793)
#endif  /* _MSC_VER */

#define STRING_ALLOCATOR	DEFAULT_ALLOCATOR

using namespace std;

__NS_BASIC_START

const size_t NullLen = (size_t)-1;
const char NullAString[] = "";
const WCHAR NullWString[] = { 0 };
const LPCTSTR Null_String = _T("");
#define Null_String_S		""

//! �ַ������ַ������Ķ��塣
//! Ŀ�ģ�����ȫ��ͳһ�滻��������

template<typename CharType>
struct __BasicString
{
	typedef typename basic_basic_string<CharType>::type		StringType;
	typedef typename basic_basic_stringstream<CharType>::type	StringStreamType;
};
 
#pragma warning (push)
#pragma warning (disable: 4251)

template struct _BASIC_DLL_API __BasicString<char>;
class _BASIC_DLL_API CharBasiCWBasicString : public basic_string<char, std::char_traits<char>, DEFAULT_ALLOCATOR<char> >
{
public:
	 CharBasiCWBasicString(){}
};

template struct _BASIC_DLL_API __BasicString<wchar_t>;
class _BASIC_DLL_API WCharrBasiCWBasicString : public basic_string<wchar_t, std::char_traits<wchar_t>, DEFAULT_ALLOCATOR<wchar_t> >
{
public:
	WCharrBasiCWBasicString(){}
};

#pragma warning (pop)

typedef __BasicString<TCHAR>::StringType			tstring;
typedef __BasicString<TCHAR>::StringStreamType		tstringstream;

typedef __BasicString<char>::StringType				tstring_s;
typedef __BasicString<char>::StringStreamType		tsstringstream_s;



namespace __private
{
	MarkString(tstring);
	MarkString(tstring&);
	MarkString(const tstring);
	MarkString(const tstring&);
}

typedef	tstring_s									char_string;
typedef tstring										wchar_string;

// ���ַ�����ȡ�ö�Ӧ��usigned������
template<typename CharType>
struct __CharValue
{
	typedef CharType Result;
};

template<>
struct __CharValue<char>
{
	typedef unsigned char Result;
};

// ��unsigned������ȡ�ö�Ӧ���ַ�����
template<typename ValueType>
struct __ValueChar
{
	typedef ValueType Result;
};


template<>
struct __ValueChar<unsigned char>
{
	typedef char Result;
};
__NS_BASIC_END

#ifdef __GNUC__
#include <ext/hash_map>

namespace __gnu_cxx
{
	using namespace ::basiclib;
    template<> struct hash<long long>
    {
        size_t operator()(const long long __x) const
        {
            return __x;
        }
    };
	template<> struct hash<__BasicString<char>::StringType>
	{
		typedef __BasicString<char>::StringType StringType;
		size_t operator()(const StringType& __s) const
		{
            hash<const char*> fn;
            return fn(__s.c_str());
        }
	};

	template<> struct hash<__BasicString<WCHAR>::StringType>
	{
		typedef __BasicString<WCHAR>::StringType StringType;
		size_t operator()(const StringType& __s) const
		{
            ASSERT(0);
            return 0;
        }
	};
}
#elif defined(__SGI_SBASIC_PORT)
#include <hash_map>
namespace std
{
	using namespace ::basiclib;
	template<> struct hash<__BasicString<char>::StringType>
	{
		typedef __BasicString<char>::StringType StringType;
		size_t operator()(const StringType& __s) const
		{ return __stl_hash_string(__s.c_str()); }
	};

	template<> struct hash<__BasicString<WCHAR>::StringType>
	{
		typedef __BasicString<char>::StringType StringType;
		size_t operator()(const StringType& __s) const
		{ return __stl_hash_string(__s.c_str()); }
	};
}
#else
namespace stdext
{
	template <class _InIt>
		inline size_t VS2008_Hash_value(_InIt _Begin, _InIt _End)
	{	// hash range of elements
		size_t _Val = 2166136261U;
		while(_Begin != _End)
			_Val = 16777619U * _Val ^ (size_t)*_Begin++;
		return (_Val);
	}
	inline
		size_t hash_value(const basiclib::char_string& str)
	{	// hash _Keyval to size_t value one-to-one
		return (VS2008_Hash_value(str.begin(), str.end())); 
	}

	inline
	size_t hash_value(const basiclib::wchar_string& str)
	{	// hash _Keyval to size_t value one-to-one
		return (VS2008_Hash_value(str.begin(), str.end())); 
	}
}
#endif

__NS_BASIC_START

//! �ж��ַ����Ƿ�Ϊ��
/*!
\param p �ж��ַ����Ƿ�Ϊ��
*/
template<typename CharType>
bool IsStringEmpty(const CharType* p)
{
	return (NULL == p) || ((CharType)0 == p[0]);
}

//! ���ַ�������ָ���ķָ����ָ�����ν���func��������
/*!
\param p ������ַ������������޸���������ݡ�����Ϊ��
\param cTok �ָ��
\param func ���ڸ��ݲ���ȡ��ֵ�ķº�����
\remarks �ú������޸�p��ָ����ַ�������
*/
template<class Functor, typename CharType>
void __SpliteString(CharType* p, CharType cTok, Functor func)
{
	CharType* e = p;
	do
	{
		e = __tcschr(p, cTok);
		if (e != NULL)
			*e++ = 0;
		func(p);
		p = e;
	}while(p != NULL);
}
//! ���ַ�������ָ���ķָ��ַ����ָ�����ν���func��������
/*!
\param p Դ�ַ������������޸���������ݡ�����Ϊ�ա�
\param pszTok �ָ����ַ���
\param func ���ڸ��ݲ���ȡ��ֵ�ķº�����
\remarks �ú������޸�p��ָ����ַ�������
*/
template<class Functor, typename CharType>
void __SpliteString(CharType* p, const CharType* pszTok, Functor func)
{
	int len = __tcslen(pszTok);
	CharType* e = p;
	do
	{
		e = __tcsstr(p, pszTok);
		if (e != NULL)
		{
			*e = 0;
			e += len;
		}
		func(p);
		p = e;
	}while(p != NULL);
}

//! ���ַ�������ָ���ķָ����ָ�����ν���func��������
/*!
\param psz Դ�ַ���,����Ϊ��
\param cTok �ָ����
\param func ���ڸ��ݲ���ȡ��ֵ�ķº�����
\remarks ��__SpliteString�������ǣ��ú������޸�pszָ�����ݡ�
ʾ�����Բο��º���IntoContainer
*/
template<class Functor, typename CharType>
void BasicSpliteString(const CharType* psz, CharType cTok, Functor func)
{
	typedef typename __BasicString<CharType>::StringType StringType;
	StringType str(psz);
	__SpliteString((CharType*)str.c_str(), cTok, func);
}

//! ���ַ�������ָ���ķָ����ָ�����ν���func��������
/*!
\param psz Դ�ַ���������Ϊ�ա�
\param pszTok �ָ��ַ���
\param func ���ڸ��ݲ���ȡ��ֵ�ķº�����
\remarks ��__SpliteString�������ǣ��ú������޸�pszָ�����ݡ�
ʾ�����Բο��º���IntoContainer
*/
template<class Functor, typename CharType>
void BasicSpliteString(const CharType* psz, const CharType* pszTok, Functor func)
{
	typedef typename __BasicString<CharType>::StringType StringType;
	StringType str(psz);
	__SpliteString((CharType*)str.c_str(), pszTok, func);
}

//! ���ַ�������ָ���ķָ����ָ�����ν���func��������
/*!
\param psz Դ�ַ���
\
\param pszTok �ָ��ַ���,����Ϊ��
\param func ���ڸ��ݲ���ȡ��ֵ�ķº�����
\remarks ��__SpliteString�������ǣ��ú������޸�pszָ�����ݡ�
ʾ�����Բο��º���IntoContainer
*/
template<class Functor, typename CharType>
void BasicSpliteString(const CharType* psz, long length, CharType cTok, Functor func)
{
	typedef typename __BasicString<CharType>::StringType StringType;
	StringType str(psz, length);
	__SpliteString((CharType*)str.c_str(), cTok, func);
}

//! ���ַ�������ָ���ķָ����ָ�����ν���func��������
/*!
\param psz Դ�ַ���������Ϊ��
\param length Դ�ַ����ĳ���
\param pszTok �ָ��ַ���
\param func ���ڸ��ݲ���ȡ��ֵ�ķº�����
\remarks ��__SpliteString�������ǣ��ú������޸�pszָ�����ݡ�
ʾ�����Բο��º���IntoContainer
*/
template<class Functor, typename CharType>
void BasicSpliteString(const CharType* psz, long length, const CharType* pszTok, Functor func)
{
	typedef typename __BasicString<CharType>::StringType StringType;
	StringType str(psz, length);
	__SpliteString((CharType*)str.c_str(), pszTok, func);
}


//! ���ַ�������ָ���ķָ����ָ�����ν���func��������
/*!
\param psz Դ�ַ���������Ϊ��
\param pszTok ��pszTok�е��ַ����ᱻ�����ָ���ʹ��
\param func ���ڸ��ݲ���ȡ��ֵ�ķº�����
\remark �ú������޸�p��ָ����ַ�������
ʾ�����Բο��º���IntoContainer
*/
template<class Functor, typename CharType>
void __Explode(CharType* psz, const CharType* pszTok, Functor func)
{
	CharType* e = psz;
	while(0 != *e)
	{
		// �ҵ��ָ���
		if (NULL != __tcschr(pszTok, *e))
		{
			*e = (CharType)0;
			func(psz);
			psz = e + 1;
		}
		++e;
	}

	if (*psz != 0)
	{
		func(psz);
	}
}
/*! ���ַ�������ָ���ķָ�������зָ�����ν���func��������
\param psz Դ�ַ���������Ϊ��
\param pszTok ��pszTok�е��ַ����ᱻ�����ָ���ʹ��
\param func ���ڸ��ݲ���ȡ��ֵ�ķº�����
\remark ��__Explode�������Ǹú������޸�pszָ�����ݡ�
*/
template<class Functor, typename CharType>
void Basic_Explode(const CharType* psz, const CharType* pszTok, Functor func)
{
	typedef typename __BasicString<CharType>::StringType StringType;
	StringType str(psz);
	__Explode((CharType*)str.c_str(), pszTok, func);
}

/*! ���ַ�������ָ���ķָ�������зָ�����ν���func��������
\param psz Դ�ַ���������Ϊ��
\param length psz�ַ����ĳ���
\param pszTok ��pszTok�е��ַ����ᱻ�����ָ���ʹ��
\param func ���ڸ��ݲ���ȡ��ֵ�ķº�����
\remark ��__Explode�������Ǹú������޸�pszָ�����ݡ�
*/
template<class Functor, typename CharType>
void Basic_Explode(const CharType* psz, long length, const CharType* pszTok, Functor func)
{
	typedef typename __BasicString<CharType>::StringType StringType;
	StringType str(psz, length);
	__Explode((CharType*)str.c_str(), pszTok, func);
}




//! ���ַ�������ָ���ķָ����ָ�����ν���func��������ֱ���ַ���β������func����true
/*!
\param psz Դ�ַ���������Ϊ�ա�
\param cTok �ָ���
\param func ���ڸ��ݲ���ȡ��ֵ�ķº��������ú�������trueʱ�������˳���
\remarks �ú������޸�p��ָ������ݡ� 
*/
template<class Functor, typename CharType>
bool __SpliteStringBreak(CharType* p, CharType cTok, Functor func)
{
	CharType* e = p;
	do
	{
		e = __tcschr(p, cTok);
		if (e != NULL)
			*e++ = 0;
		if (func(p, e ? e - p - 1 : __tcslen(p)))
			return true;
		p = e;
	}while(p != NULL);
	return false;
}

//! ���ַ�������ָ���ķָ����ָ�����ν���func��������ֱ���ַ���β������func����true
/*!
\param psz Դ�ַ���,����Ϊ�ա�
\param cTok �ָ���
\param func ���ڸ��ݲ���ȡ��ֵ�ķº��������ú�������trueʱ�������˳���
\remarks ��__SpliteStringBreak�������ǣ��ú������޸�pszָ�����ݡ�
*/
template<class Functor, typename CharType>
bool BasicSpliteStringBreak(const CharType* psz, CharType cTok, Functor func)
{
	typedef typename __BasicString<CharType>::StringType StringType;
	StringType str(psz);
	return __SpliteStringBreak((CharType*)str.c_str(), cTok, func);
}

//! ���ַ�������ָ���ķָ����ָ�����ν���func������������'\'��ͷ���ַ�������ת�塣
/*!
\param p Դ�ַ���,����Ϊ�ա�
\param func ���ڸ��ݲ���ȡ��ֵ�ķº���
\remarks �ú������޸�p��ָ���Դ�ַ�����
*/
template<class Functor, typename CharType>
bool __SpliteStringBreakWithEscape(CharType* p, CharType cTok, Functor func)
{
	CharType* e = p;
	CharType* v = p;
	while(0 != *e)
	{
		if ((CharType)'\\' == *e &&  0 != *(e+1))	// ת�����,�Թ���һ���ַ�
		{
			*v++ = *++e;
			++ e;
		}
		else if (*e == cTok)
		{
			*e = 0;
			*v = 0;

			if (func(p))
				return true;
			p = ++ e;
			v = p;
		}
		else
		{
			*v++ = *e++;
		}
	}
	
	if (*p != 0)
	{
		*v = 0;
		return func(p);
	}
	
	return false;
}

//! ���ַ�������ָ���ķָ����ָ�����ν���func��������֧��'\'ת��
/*!
\param psz Դ�ַ���,����Ϊ��
\param cTok �ָ���
\param func ���ڸ��ݲ���ȡ��ֵ�ķº���
\remarks ��__SpliteStringBreakWithEscape��������SpliteStringBreakWithEscape���޸�psz�����ݡ�
\code
char* buf = "a\\;;b;\\\\\\;c";	//�ַ��� a\;;b;\\\;cd
BasicSpliteStringBreakWithEscape(buf, ';', functor);

��ôfunctor�յ����ַ���Ӧ���ǣ�
a;
b
\;c
\endcode
*/
template<class Functor, typename CharType>
bool BasicSpliteStringBreakWithEscape(const CharType* psz, CharType cTok, Functor func)
{
	typedef typename __BasicString<CharType>::StringType StringType;
	StringType str(psz);
	return __SpliteStringBreakWithEscape((CharType*)str.c_str(), cTok, func);
}

#define ISSPACE(a) (((a) >=0x09 && (a) <= 0x0D) || (a)== 0x20)
#define FORWARD_SPACE(a, e, l)	while(ISSPACE(*(a)) && ((e)+l == find((e), (e)+l, (*(a))))) {++a;}
#define BACKWARD_SPACE(a)	while(ISSPACE(*(a))) {--a;}


const int PPS_OPS_LOWERKEY		= 1;	// ����һ���ַ���ת��Сд
const int PPS_OPS_LOWERVALUE	= 2;	// ���ڶ����ַ���ת��Сд
const int PPS_OPS_SKIPBLANK		= 4;	// �Թ��ַ���ǰ��Ŀո�/tab/����

namespace __private
{
// ���º�����ΪParseParamStringʹ��
template<class CharType, class T>
void SkipBlank(CharType*& p, const CharType*& pException, int len, Type2Type<T>)
{
	FORWARD_SPACE(p, pException, len);
};

template<class CharType>
void SkipBlank(CharType*& p, const CharType*& pException, int len, Type2Type<False>)
{}

template<class CharType, class T>
void Lower(CharType* p, Type2Type<T>)
{
	*p = (CharType)tolower(*p);
};


template<class CharType>
void Lower(CharType* p, Type2Type<False>)
{}

template<class CharType, class T>
void SetBlank(CharType* p, CharType*& blank, Type2Type<T>)
{
	if (ISSPACE(*p))
	{
		if (NULL == blank)
			blank = p;
	}
	else if (blank)
	{
		blank = NULL;
	}
}

template<class CharType>
void SetBlank(CharType* p, CharType*& blank, Type2Type<False>)
{
}

template<class CharType, class T>
void SetEnd(CharType* blank, Type2Type<T>)
{
	if (blank)
	{
		*blank = (CharType)0;
		blank = NULL;
	}
}

template<class CharType>
void SetEnd(CharType* blank, Type2Type<False>)
{
}

template<class LK, class LV, class SB>
struct Parse_Trait
{
	typedef Type2Type<LK>	LowerKey;
	typedef Type2Type<LV>	LowerValue;
	typedef Type2Type<SB>	SkipBlank;
};
}

//! ����������"a=b&c=d&e=f"���ַ���������(a,b),(c,d),(e,f)���δ���
/*!
\param psz Դ�ַ�
\param tok[0]�ֱ��ʶ��Էָ�����tok[1]��ʶ��Ŀ�ָ���
\param func �ַ����Խ��д���ĺ������߷º���
\param ParseTraits	�Ծ���һЩ���Ե���ȡ
\return �޷���ֵ
*/
template<class Functor, typename CharType, typename ParseTraits>
void __ParseParamString_Aux(CharType* psz, const CharType tok[], Functor func, ParseTraits)
{
	
	enum {
		PARSE_KEY,
		PARSE_VALUE
	} parseStatus;
	parseStatus = PARSE_KEY;
	__private::SkipBlank(psz, tok, 2, typename ParseTraits::SkipBlank());

	CharType* p = psz;
	CharType* pKey = psz;
	CharType* pValue = NULL;
	CharType* blank = NULL;
	while(*p != 0)
	{
		switch(parseStatus)
		{
		case PARSE_KEY:
			if (tok[1] == *p)
			{ // reach end before get value
				*p = 0;
				__private::SetEnd(blank, typename ParseTraits::SkipBlank());
				func(pKey, pValue);
				++ p;
				__private::SkipBlank(p, tok, 2, typename ParseTraits::SkipBlank());
				pKey = p;
			}
			else if (tok[0] == *p)
			{	// ��һ���ָ��
				*p = 0;
				pValue = p + 1;
				__private::SetEnd(blank, typename ParseTraits::SkipBlank());
				__private::SkipBlank(pValue, tok, 2, typename ParseTraits::SkipBlank());
				p = pValue;
				parseStatus = PARSE_VALUE;
			}
			else
			{
				__private::SetBlank(p, blank, typename ParseTraits::SkipBlank());
				__private::Lower(p++, typename ParseTraits::LowerKey());
			}
			break;
		case PARSE_VALUE:
			if (tok[1] == *p)
			{	// ����β����
				*p = 0;
				__private::SetEnd(blank, typename ParseTraits::SkipBlank());
				func(pKey, pValue);
				++ p;
				__private::SkipBlank(p, tok, 2, typename ParseTraits::SkipBlank());
				pKey = p;

				pValue = NULL;
				parseStatus = PARSE_KEY;
			}
			else
			{
				__private::SetBlank(p, blank, typename ParseTraits::SkipBlank());			
				__private::Lower(p++, typename ParseTraits::LowerValue());
			}
			break;
		}
	}
	if (*pKey)
	{
		__private::SetEnd(blank, typename ParseTraits::SkipBlank());
		func(pKey, pValue);
	}
}


class break_exception : public exception
{
public:
	virtual const char* what() const throw()
	{
		return "callback functor break circle";
	}
};

//! ����������"a=b&c=d&e=f"���ַ���������(a,b),(c,d),(e,f)���δ���
/*!
\param psz Դ�ַ�,����Ϊ��
\param psz �ַ����ĳ���
\param tok[0]�ֱ��ʶ��Էָ�����tok[1]��ʶ��Ŀ�ָ���
\param func �ַ����Խ��д���ĺ������߷º���
\param Ops ����ѡ�ֵΪPPS_OPS_*
\return �޷���ֵ
*/

template<class Functor, typename CharType>
void Basic_ParseParamString(const CharType* psz, size_t len, const CharType tok[], Functor func, int nOps = PPS_OPS_LOWERKEY|PPS_OPS_SKIPBLANK)
{
	using namespace __private;
	typename __BasicString<CharType>::StringType s(psz, len);

	switch(nOps)
	{
	case 0:
		__ParseParamString_Aux((CharType*)s.c_str(), tok , func, Parse_Trait<False, False, False>());
		break;
	case PPS_OPS_LOWERKEY:		// 1
		__ParseParamString_Aux((CharType*)s.c_str(), tok , func, Parse_Trait<True, False, False>());
		break;
	case PPS_OPS_LOWERVALUE:	// 2
		__ParseParamString_Aux((CharType*)s.c_str(), tok , func, Parse_Trait<False, True, False>());
		break;
	case PPS_OPS_LOWERKEY|PPS_OPS_LOWERVALUE:	// 3
		__ParseParamString_Aux((CharType*)s.c_str(), tok , func, Parse_Trait<True, True, False>());
		break;
	case PPS_OPS_SKIPBLANK:		// 4
		__ParseParamString_Aux((CharType*)s.c_str(), tok , func, Parse_Trait<False, False, True>());
		break;
	case PPS_OPS_LOWERKEY|PPS_OPS_SKIPBLANK:	// 5
		__ParseParamString_Aux((CharType*)s.c_str(), tok , func, Parse_Trait<True, False, True>());
		break;
	case PPS_OPS_LOWERVALUE|PPS_OPS_SKIPBLANK:	// 6
		__ParseParamString_Aux((CharType*)s.c_str(), tok , func, Parse_Trait<False, True, True>());
		break;
	case PPS_OPS_LOWERKEY|PPS_OPS_LOWERVALUE|PPS_OPS_SKIPBLANK:	// 7
		__ParseParamString_Aux((CharType*)s.c_str(), tok , func, Parse_Trait<True, True, True>());
		break;
	}
}

//! ����������"a=b&c=d&e=f"���ַ���������(a,b),(c,d),(e,f)���δ���
/*!
\param psz Դ�ַ�,����Ϊ�ա�������'\0'��β
\param tok[0]�ֱ��ʶ��Էָ�����tok[1]��ʶ��Ŀ�ָ���
\param func �ַ����Խ��д���ĺ������߷º���
\param Ops ����ѡ�ֵΪPPS_OPS_*
\return �޷���ֵ
*/
template<class Functor, typename CharType>
void Basic_ParseParamString(const CharType* psz, const CharType tok[], Functor func, int nOps = PPS_OPS_LOWERKEY|PPS_OPS_SKIPBLANK)
{
	Basic_ParseParamString(psz, __tcslen(psz), tok, func, nOps);
}

//! ����ַ�����tokָ�����ڵĲ�����������ֵ��func��ȡ�á�
/*!
\param str Դ�ַ���������Ϊ�ա�
\param tok ����ָ����ʼ������"<>"
\param func ���ڸ��ݲ���ȡ��ֵ�ķº���
\remarks str�ڵ����ݻᱻ�޸ġ�
*/
template<class Functor, typename CharType>
typename __BasicString<CharType>::StringType __FillParamString(CharType* str, const CharType tok[], Functor func)
{
	typedef typename __BasicString<CharType>::StringType StringType;
	StringType	result;
	CharType *left = NULL;
	CharType *right = NULL;
	do 
	{
		left = __tcschr(str, tok[0]);
		if (left)
		{
			++ left;
			right = __tcschr(left, tok[1]);
			if (right)
			{
				*right++ = '\0';
				result.append(str, left - str - 1);
				result.append(func(left));

				str = right;
			}
		}
	} while(left && right);

	result += str;

	return result;
}


//! ����format�ַ�����ȡ��Դ�ַ����е�ֵ
/*!
\param pszSrc Դ�ַ�,����Ϊ��
\param tok[0]�ֱ��ʶ��Էָ�����tok[1]��ʶ��Ŀ�ָ���
\param func �ַ����Խ��д���ĺ������߷º���
\return �޷���ֵ
\remarks str�ڵ����ݻᱻ�޸ġ�
*/
template <class CharType, class Functor>
void __GetParamString(CharType* lpszSrc, CharType* lpszFormat, const CharType tok[], Functor func)
{
	ASSERT(!IsStringEmpty(lpszFormat) && !IsStringEmpty(lpszSrc));

	CharType *pEnd, *pKey, *pSep;
	pEnd = pKey = pSep = NULL;
	size_t len = 0;
	while(lpszFormat && lpszSrc && 
		*lpszFormat != _T('\0') && *lpszSrc != _T('\0'))
	{
		if (*lpszFormat == *lpszSrc)
		{
			++ lpszFormat;
			++ lpszSrc;			
		}
		else if (*lpszFormat == tok[0])
		{
			pKey = lpszFormat + 1;
			pEnd = __tcschr(pKey,  tok[1]);
			if (pEnd == NULL)
				break;
			*pEnd++ = (CharType)0;
			lpszFormat = pEnd;
			// FindNext
			pEnd = __tcschr(lpszFormat, tok[0]);
			if (pEnd)
			{
				len = pEnd - lpszFormat;
				*pEnd = (CharType)0;
			}
			else
			{
				len = __tcslen(lpszFormat);
			}

			if (len == 0)
			{
				func(pKey, lpszSrc);
				break;
			}
			else
			{
				pSep = __tcsstr(lpszSrc, lpszFormat);
				if (NULL == pSep)
					break;
				*pSep = (CharType)0;
				func(pKey, lpszSrc);
				lpszSrc = pSep + len;

				if (pEnd)
				{
					*pEnd = tok[0];
				}
				lpszFormat = pEnd;
			}
		}
		else
		{
			break;
		}
	}
}

//! ����format�ַ�����ȡ��Դ�ַ����е�ֵ
/*!
\param pszSrc Դ�ַ�,����Ϊ��
\param tok[0]�ֱ��ʶ��Էָ�����tok[1]��ʶ��Ŀ�ָ���
\param func �ַ����Խ��д���ĺ������߷º���
\return �޷���ֵ
\code
Basic_GetParamString("/shase/600000.txt", "/{market}/{code}.txt", "{}", InfoMapContainer<tstring, tstring>());
\endcode
���:
market=>shase
code=>600000
*/
template <class CharType, class Functor>
void Basic_GetParamString(const CharType* lpszSrc, const CharType* lpszFormat, const CharType tok[], Functor func)
{
	using namespace __private;
	typename __BasicString<CharType>::StringType format(lpszFormat);
	typename __BasicString<CharType>::StringType src(lpszSrc);

	__GetParamString((CharType*)src.c_str(),(CharType*)format.c_str(), tok, func);
}


//! ����ַ�����tok[0]��tok[1]�ڵĲ�����������ֵ��func��ȡ�á�
/*!
\param psz Դ�ַ���������Ϊ��
\param tok ��ʶ�ؼ���������ַ������������ַ����ֱ����ڱ�ʶ��ʼ�ͽ���
\param func ���ڸ��ݲ���ȡ��ֵ�ķº���
\remarks ��__FillParamString��������Basic_FillParamString���޸�psz�����ݡ�
*/
template<class Functor, typename CharType>
typename __BasicString<CharType>::StringType	Basic_FillParamString(const CharType* psz, const CharType tok[], Functor func)
{
	typename __BasicString<CharType>::StringType	str(psz);
	return __FillParamString((CharType*)str.c_str(), tok, func);
}

namespace __private
{
	//! �ϲ����ݶ����ַ����Ĵ���º���
	/*!
	\struct __combine_string_helper
	*/
	template<typename CharType>
	struct __combine_string_helper
	{
		typedef typename __BasicString<CharType>::StringType	StringType;
		typedef typename __BasicString<CharType>::StringStreamType StringStreamType;

		__combine_string_helper(CharType tok, StringType& str)
			: __tok(tok), __str(str){}

		template<class T>
		__combine_string_helper& operator()(const T& s)
		{
			if (__tok != 0 && !__str.empty())
				__str += __tok;

			typedef typename IsString<T>::Result Result;
			AddString(s, Result());
			return *this;
		}

		template<class T, class U>
		void AddString(const T& s, U)
		{
			__str += s;
		}

		template<class T>
		void AddString(const T& s, struct False)
		{
			StringStreamType stream;
			stream << s;
			__str += stream.str();
		}

		CharType	__tok;
		StringType&	__str;
	};

	//! �ϲ����ݶ����ַ������Ĵ���º���
	/*!
	\struct __combine_stream_helper
	*/
	template<typename CharType>
	struct __combine_stream_helper
	{
		typedef typename __BasicString<CharType>::StringStreamType stream_type;
		__combine_stream_helper(CharType tok, stream_type& stream)
			: __tok(tok), __stream(stream), __first(true){}

		template<class T>
		__combine_stream_helper& operator()(const T& s)
		{
			if (__tok != 0 && !__first)
				__stream << __tok;
			else if (__first)
				__first = false;
			
			__stream << s;			
			return *this;
		}

		__combine_stream_helper& operator()(const unsigned char& s)
		{
			return operator()((const unsigned int)s);
		}

		__combine_stream_helper& operator()(unsigned char& s)
		{
			return operator()((unsigned int)s);
		}

		CharType		__tok;
		stream_type&	__stream;
		bool			__first;
	};
}


namespace __private
{
	const char*		__get_key_string(__private::Type2Type<char>);
	const char*		__get_esc_string(__private::Type2Type<char>);

	const WCHAR*	__get_key_string(__private::Type2Type<WCHAR>);
	const WCHAR*	__get_esc_string(__private::Type2Type<WCHAR>);

	const char*		__get_crlf_string(__private::Type2Type<char>);
	const WCHAR*	__get_crlf_string(__private::Type2Type<WCHAR>);
}


//! �ϲ������ڵ��ַ�����֮���÷ָ����ָ�
/*!
\param tok �ָ���
\param first input iterator�����ڱ�ʶ��ʼԪ��
\param last input iterator�����ڱ�ʶ����Ԫ��
����ָ��ֵΪ'\0',���ַ���ֱ�Ӻϲ���
\code
char* buf[] = {"aa", "bb", "cc"};
string str = ComineString(',', buf, sizeof(buf)/sizeof(char*));
\endcode
��ʱstr��ֵ��"aa,bb,cc"
����Ҳ���ԶԷ��ַ������������������磺
\code
int buf[] = {11, 22, 33};
string str = CombineString(',', buf, size(buf)/sizeof(int));
\endcode
��ʱstr��ֵ��"11,22,33"
*/
template<class InputIterator, typename CharType>
typename __BasicString<CharType>::StringType  Basic_CombineString(CharType cTok, InputIterator first, InputIterator last)
{
	typename __BasicString<CharType>::StringStreamType stream;
	for_each(first, last, __private::__combine_stream_helper<CharType>(cTok, stream));
	return stream.str();
}

//! �ϲ������ڵ��ַ�����֮���÷ָ����ָ�
/*!
\param tok �ָ���
\param lhs ��һ���ֶ�
\param rhs �ڶ����ֶ�
\code
string str = ComineString(',', 'abc', 456);
\endcode
��ʱstr��ֵ��"abc,456"
*/
template<typename CharType, class T1, class T2>
typename __BasicString<CharType>::StringType Basic_CombineString(CharType cTok, const T1& lhs, const T2& rhs)
{
	typename __BasicString<CharType>::StringStreamType stream;
	__private::__combine_stream_helper<CharType>(cTok, stream)(lhs)(rhs);
	return stream.str();
}

// �ϲ�ͷ
template<typename CharType, class T>
int Basic_StuffHeader(typename __BasicString<CharType>::StringType& str, const CharType* pszName, const T& value)
{
	str += pszName;
	__private::__combine_string_helper<CharType>((CharType)'=', str)(value);
	str += __private::__get_crlf_string(__private::Type2Type<CharType>());
	return str.length();
}

//! �ϲ��ַ���
template<typename CharType, class T>
int Basic_StuffString(typename __BasicString<CharType>::StringType& str, const T& psz)
{
	__private::__combine_string_helper<CharType>('\0', str)(psz);
	return str.length();
}

//! ���ַ�������
template<typename CharType>
typename __BasicString<CharType>::StringType Basic_EnesCWBasicString(const CharType* psz)
{
	typedef typename __BasicString<CharType>::StringType	StringType;
	StringType str;
	const CharType* key = __private::__get_key_string(__private::Type2Type<CharType>());
	const CharType* esc = __private::__get_esc_string(__private::Type2Type<CharType>());
	if(psz != NULL)
	{
		size_t nLength = __tcslen(psz);
		str.resize(nLength*2+1);
		size_t nOffset = 0;
		for(size_t i = 0; i < nLength; i++)
		{
			const CharType* p = __tcschr(key, psz[i]);
			if(p)
			{
				str[nOffset++] = esc[0];
				str[nOffset++] = esc[p - key];
			}
			else
			{
				str[nOffset++] = psz[i];
			}
		}
		str.resize(nOffset);
	}
	return str;
}

//! ���ַ�������
template<typename CharType>
typename __BasicString<CharType>::StringType Basic_DeesCWBasicString(const CharType* psz)
{
	typedef typename __BasicString<CharType>::StringType	StringType;
	StringType str;
	const CharType* key = __private::__get_key_string(__private::Type2Type<CharType>());
	const CharType* esc = __private::__get_esc_string(__private::Type2Type<CharType>());

	if(psz != NULL)
	{
		size_t nLength = __tcslen(psz);
		str.resize(nLength);
		size_t nOffset = 0;
		for(size_t i = 0; i < nLength; i++)
		{
			if(psz[i] == esc[0])
			{
				const CharType* p = __tcschr(esc, psz[i + 1]);
				if(p)
				{
					str[nOffset++] = key[p - esc];
					i++;
				}
			}
			else
			{
				str[nOffset++] = psz[i];
			}
		}
		str.resize(nOffset);
	}
	return str;
}

//! ���ַ�������
template<typename CharType>
int Basic_DeesCWBasicString(const CharType* psz, CharType* pszDest)
{
	int n = 0;
	const CharType* key = __private::__get_key_string(__private::Type2Type<CharType>());
	const CharType* esc = __private::__get_esc_string(__private::Type2Type<CharType>());
	if(psz != NULL)
	{
		size_t nLength = __tcslen(psz);
		for(size_t i = 0; i < nLength; i++)
		{
			if(psz[i] == esc[0])
			{
				size_t nPos = __tcschr(esc, psz[i + 1]) - esc;
				if(nPos >= 0)
				{
					pszDest[n++] = key[nPos];
					i++;
				}
			}
			else
			{
				pszDest[n++] = psz[i];
			}
		}
		pszDest[n] = '\0';
	}
	return n;
}

//! 	���ַ���lpszBufferԲ��������lRoundLength,ĩβ���"..."
/*!
\param lpszbuffer �����ַ���
\param lRoundLength Ŀ�곤��
\return Բ����ĳ���
*/
long Basic_RoundString(char* lpszBuffer, long lRoundLength);

#ifdef __BASICWINDOWS
BOOL Basic_StringMatchSpec(const WCHAR* pszFile, const WCHAR* pszSpec);
BOOL Basic_StringMatchSpec(const char* pszFile, const char* pszSpec);

#else
namespace __private
{
template<typename CharType>
const CharType* StepSpec(const CharType* pszFileParam, const CharType* pszSpec, bool bWildMatch)
{
	int nParamLen = __tcslen(pszFileParam), nSpecLen = __tcslen(pszSpec);
	if(bWildMatch == false && nParamLen != nSpecLen)
		return NULL;
	int nParamIndex = 0, nSpecIndex = 0;

	while(nParamIndex < nParamLen && nSpecIndex < nSpecLen)
	{
		if (pszFileParam[nParamIndex] == pszSpec[nSpecIndex]
		|| pszSpec[nSpecIndex] == (CharType)'?')
		{
			++ nParamIndex; ++ nSpecIndex;
		}
		else if (bWildMatch)
		{
			nParamIndex -= (nSpecIndex - 1);
			nSpecIndex = 0;
		}
		else
		{
			return NULL;
		}
	}
	if (nSpecIndex == nSpecLen)
	{
		return pszFileParam + nParamIndex;
	}
	return NULL;
}


template<typename CharType>
BOOL MatchLast(const CharType* pszFileParam, const CharType* pszSpec)
{
	if ( (CharType)'\0' == *pszSpec)
		return TRUE;

	int nSpecLen = __tcslen(pszSpec);
	int nFileParamLen = __tcslen(pszFileParam);

	if (nSpecLen > nFileParamLen)
		return FALSE;

	return NULL != StepSpec(pszFileParam + nFileParamLen - nSpecLen, pszSpec, false);
//	return  memcmp(pszFileParam + nFileParamLen - nSpecLen, pszSpec, nSpecLen * sizeof(CharType)) == 0;
}

}
/*!	\brief ʵ����windows��PathMatchSpec���ƵĹ��ܡ�֧��?��*ͨ���
*	\param pszFile[in] ָ����\0��β���ַ���
*	\param pszSpec[in] ָ����\0��β���ַ�������������ƥ�����
*	\return ƥ�䷵��TRUE,����FALSE
*/
template<typename CharType>
BOOL Basic_StringMatchSpec(const CharType* pszFile, const CharType* pszSpec)
{
	ASSERT(NULL != pszFile);
	ASSERT(NULL != pszSpec);

	BOOL bMatch = TRUE;
	int nLenSpec = __tcslen(pszSpec);
	size_t nAlloc = (nLenSpec + 1) * sizeof(CharType);
	CharType* p = (CharType*)BasicAllocate(nAlloc);
	memcpy(p, pszSpec, nAlloc - sizeof(CharType));
	p[nLenSpec] = (CharType)'\0';

	CharType* sp = p;
	CharType* e = p;
	bool bWildMatch = false;
	do
	{
		e = __tcschr(p, (CharType)'*');
		if (e != NULL)
		{
			*e++ = (CharType)'\0';
			bWildMatch = true;
		}
		else if (bWildMatch)
		{
			bMatch = __private::MatchLast(pszFile, p);
			break;
		}
		pszFile = __private::StepSpec(pszFile, p, bWildMatch);
		if (NULL == pszFile)
		{
			bMatch = FALSE;
			break;
		}
		p = e;
	}while(p != NULL);
	BasicDeallocate(sp, nAlloc);
	return bMatch;
}
#endif

const int	ENT_NOQUOTES	= 0;	//! ��ת�������ź�˫����
const int	ENT_COMPAT	= 1;		//! ֻת��˫����
const int	ENT_SQUOTE	= 2;		//! ֻת��������
const int	ENT_QUOTES	= ENT_COMPAT|ENT_SQUOTE;	//! ͬʱת�������ź�˫����
namespace __private

{
	const char*		__get_html_spec_string(char c, int quotestyle);
	const WCHAR*	__get_html_spec_string(WCHAR c, int quotestyle);
}


//! \brief: ��html��������ַ����б��롣
/*!
	\param	psz[in]			ָ����\0��β���ַ���
	\param	quotestyle[in]	�����ŵ�ת������
	\return	ת������ַ���
	\remark ת�����ַ���:
			'&' (ampersand)		=> "&amp;"
			'"'	(double quote)	=> "&quot;"	(û������ENT_NOQUOTES�������)
			''' (single quote)	=> "&#039;'	(����������ENT_QUOTES�������)
			'<' (less than)		=> "&lt;"
			'>'	(greater than)	=> "&gt;"
*/
template<typename CharType>
typename __BasicString<CharType>::StringType	Basic_HtmlEncode(const CharType* psz, int quotestyle = ENT_COMPAT)
{
	typedef typename __BasicString<CharType>::StringType	StringType;
	StringType ret;
	const CharType* spec = NULL;
	CharType buf[7];
	while(*psz)
	{
		spec = __private::__get_html_spec_string(*psz, quotestyle);
		if (spec)
		{
			ret += spec;
		}
		else if ((*psz) >= 0 && ((*psz) < 0x20) || *psz == 127)	// ���ɼ�
		{
			buf[0] = (CharType)'&';
			buf[1] = (CharType)'#';
			buf[2] = (CharType)((*psz)/100 + '0');
			buf[3] = (CharType)((*psz)%100/10 + '0');
			buf[4] = (CharType)((*psz)%10 + '0');
			buf[5] = (CharType)';';
			buf[6] = 0;
			ret += buf;
		}
		else
		{
			ret += *psz;
		}
		++ psz;
	}
	return ret;
}

namespace __private
{
	const char* __get_html_spec_char(const char* psz);
	const WCHAR* __get_html_spec_char(const WCHAR* psz);
	const char* __get_html_sep_tok(Type2Type<char>);
	const WCHAR* __get_html_sep_tok(Type2Type<WCHAR>);

	struct ReplaceHtmlSpec
	{
		template <typename CharType>
		typename __BasicString<CharType>::StringType	 operator()(const CharType* psz)
		{
			typedef typename __BasicString<CharType>::StringType	StringType;
			StringType ret;
			if (*psz == (CharType)'#')
			{
				ret = (CharType)__tcstol(psz + 1, NULL, 10);
			}
			else
			{
				const CharType* s = __get_html_spec_char(psz);
				if (s)
				{
					ret += (s);
				}
				else
				{
					ret += (CharType)'&';
					ret += psz;
					ret += (CharType)';';
				}
			}
			return ret;
		}
	};
}

//! \brief: ��html��������ַ����н���
/*!
\param	psz[in]			ָ����\0��β���ַ���
\return	ת������ַ���
\remark ת�����ַ���:
	'&' (ampersand)		=> "&amp;"
	'"'	(double quote)	=> "&quot;"
	''' (single quote)	=> "&#039;"
	'<' (less than)		=> "&lt;"
	'>'	(greater than)	=> "&gt;"
	������"&#xxx;"��ʽ���ַ�����
*/
template<typename CharType>
typename __BasicString<CharType>::StringType	Basic_HtmlDecode(const CharType* psz)
{
	typedef typename __BasicString<CharType>::StringType	StringType;
	StringType ret = Basic_FillParamString(psz, __private::__get_html_sep_tok(__private::Type2Type<CharType>()), __private::ReplaceHtmlSpec());
	return ret;
}



namespace __private
{
	template<typename CharType>
	typename __CharValue<CharType>::Result get_v(CharType c)
	{
		if (c >= (CharType)'0' && c <= (CharType)'9')
		{
			return c - (CharType)'0';
		}
		else if (c >= (CharType)'a' && c <= (CharType)'f')
		{
			return c - (CharType)'a' + 10;
		}
		else if (c >= (CharType)'A' && c <= (CharType)'F')
		{
			return c - (CharType)'A' + 10;
		}
		else
		{
			return 0;
		}
	}
}
//! \brirf: ���ַ�����url����
/*!
\param	psz[in]		ָ����\0��β���ַ���
\return ת������ַ���
\remark	
ת�����򣺽��ַ����г���'-'��'_'������з���ĸ�ͷ������ַ����滻��%�����λʮ�����������ո������Ϊ�Ӻ�(+)��
�ú�������Multi�汾��û��UNICODE�汾��
*/
char_string	Basic_URLEncode(const char* psz);

//! ��url�������ַ���������
/*!
\param	psz[in]		ָ����\0��β���ַ���
\return ת������ַ���
*/
template<typename CharType>
typename __BasicString<CharType>::StringType Basic_URLDecode(const CharType* psz)
{
	typedef typename __BasicString<CharType>::StringType StringType;

	enum {
		CharNone,
		CharFirst,
		CharSecond
	} status = CharNone;
	typename __CharValue<CharType>::Result c = 0;
	StringType ret;
	while(psz && *psz)
	{
		switch(*psz)
		{
		case (CharType)'+':
			ret += ' ';
			break;
		case (CharType)'%':
			switch(status)
			{
			case CharNone:
				status = CharFirst;
				break;
			case CharFirst:
				status = CharNone;
				ret += (CharType)'%';
				break;
			case CharSecond:	// �϶����Ǳ�׼��,��ͨ����
				ret += (CharType)'%';
				ret += (CharType)c;
				status = CharFirst;
				break;
			}
			c = 0;
			break;
		default:
			switch(status)
			{
			case CharNone:
				ret += *psz;
				break;
			case CharFirst:
				c = *psz;
				status = CharSecond;
				break;
			case CharSecond:
				c = __private::get_v(c) << 4 | __private::get_v(*psz);
				ret += (CharType)c;
				status = CharNone;
				break;
			}
			break;
		}
		++ psz;
	}
	return ret;
}

//! ת��16���Ƶ��ַ���
int Basic_ConvertStringToHexString(const char *pszSrc, int nCount, char* pszDest, int nDest);

//! ��HEX�������ַ���ת��ΪhexԴ��
/*!
\param	pszSrc[in]		Դ�ַ���
\param	nCount[in]		pszSrc�ĳ���
\param	pszDest[out]	ת�������
\param	nDest[out]		�����pszDest���ַ�������(�����ڴ泤��)
\return ת������ַ����ĳ���
*/
template<class CharType>
int Basic_ConvertStringToHex(const char *pszSrc, int nCount, CharType* pszDest, int nDest)
{
	if(pszSrc == NULL || nCount <= 0 || pszDest == NULL || nDest <= 0 || nCount*2 > nDest)
	{
		return nCount << 1; // *2
	}
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		int n1 = (*(unsigned char *)(pszSrc+i)) % 16;
		int n2 = (*(unsigned char *)(pszSrc+i)) / 16;
		pszDest[2 * i] = (CharType)'A' + n1;
		pszDest[2 * i + 1] = (CharType)'A' + n2;
	}
	return nCount * 2;
}


//! ��HEX�������ַ���ת��ΪhexԴ��
/*!
\param	pszSrc[in]		Դ����
\param	nCount[in]		pszSrc�ĳ���
\param	pszDest[out]	ת�������
\param	nDest[out]		������ַ������ȴ�С.(CharType*nDest�Ĵ�С)
\return ת����Buffer�ĳ���
*/
template<class CharType>
int Basic_ConvertHexToString(const CharType *pszSrc, int nCount, char* pszDest, int nDest)
{
	if(pszSrc == NULL || pszDest == NULL || nCount > nDest * 2)
	{
		return nCount >> 1;	// /2
	}

	nDest = nCount / 2;
	int i = 0, j = 0;
	for (i = 0; i < nDest; i++)
	{
		j = i << 1;	// * 2
		//ASSERT(j < nCount - 1);
		int n1 = pszSrc[j] - (CharType)('A');
		int n2 = pszSrc[j + 1]  - (CharType)('A');
		pszDest[i] = (char)(n2 << 4) | n1;
	}
	return nDest;
}


//! 

//! ���ַ�������
template<typename CharType>
typename __BasicString<CharType>::StringType Basic_EncodeString(const CharType* psz)
{
	typedef typename __BasicString<CharType>::StringType	StringType;
	StringType str;

	if(psz != NULL)
	{
		size_t nLength = __tcslen(psz);
		StringType encode;
		encode.resize(nLength + 2);

		CharType *p = (CharType*)encode.c_str();
		srand((unsigned)time(NULL)); 
		p[0] = (CharType)'\0';
		while(p[0] == (CharType)'\0')
		{
			p[0] = rand() % 256;
		}

		__tcscpy(&p[1], psz);
		int nCount = __tcslen(p);
		int i = 0;
		for (i = 1; i < nCount; i++)
		{
			p[i] = p[i - 1] ^ p[i];
		}

		CharType EncodeBufffer[256];
		EncodeBufffer[0] = (CharType)'1';
		int nRet = Basic_ConvertStringToHex((const char*)p, sizeof(CharType) * nCount, &EncodeBufffer[1], sizeof(EncodeBufffer)-1);
		if (nRet < 255)
		{
			EncodeBufffer[nRet + 1] = (CharType)'\0';
		}
		str = EncodeBufffer;
	}
	return str;
}

//! ���ַ�������
template<typename CharType>
typename __BasicString<CharType>::StringType Basic_DecodeString(const CharType* psz)
{
	typedef typename __BasicString<CharType>::StringType	StringType;
	StringType str;

	CharType DecodeBufffer[256];
	int nCount = __tcslen(psz);

	memset(DecodeBufffer, 0, sizeof(DecodeBufffer));
	int nOffset = 0;
	CharType cVer = 0;
	if(nCount % 2)
	{
		cVer = psz[0];
		nOffset = 1;
		if(cVer != (CharType)'1')
		{
			return str;
		}
	}
	int nDest = nCount / 2 + 2;
	CharType *p = new CharType[nDest];

	Basic_ConvertHexToString(&psz[nOffset], nCount, (char*)p, nDest);
	nDest -= 2;
	nDest /= sizeof(CharType);
	if(cVer == 0)
	{
		int i = 1;
		for (i = 1; i < nDest; i++)
		{
			p[i] = p[i - 1] ^ p[i];
		}
		p[i] = (CharType)'\0';
	}
	else
	{
		CharType cMask = p[0];
		CharType cOld  = cMask;
		int i = 1;
		for (i = 1; i < nDest; i++)
		{
			cOld  = p[i];
			p[i]  = cMask ^ p[i];
			cMask = cOld;
		}
		p[i] = (CharType)'\0';
	}

	str = &p[1];
	delete [] p;

	return str;
}

template<class CharType>
const CharType*	Basic_LTrim(const CharType* str)
{
	FORWARD_SPACE(str, Null_String, 0);
	return str;
}
const char* Basic_LTrim(const char* str);

template<class CharType>
CharType*	Basic_LTrim(CharType* str)
{
	FORWARD_SPACE(str, Null_String, 0);
	return str;
}
char* Basic_LTrim(char* str);

template<class CharType>
CharType* Basic_RTrim(CharType* str, size_t len = NullLen)
{
	if (NullLen == len)
		len = __tcslen(str);
	if (len > 0)
	{
		CharType* p = str + len - 1;
		while(ISSPACE(*p) && (len--) > 0)
			-- p;
		*(p + 1) = (CharType)0;
	}

	return str;   
}

template<class CharType>
CharType* Basic_Trim(CharType* str)
{
	return Basic_RTrim(Basic_LTrim((CharType*)str));
}

template<class CharType>
void Basic_InsertNumberSpace(typename __BasicString<CharType>::StringType& str, CharType tok)
{
	const CharType* ps = str.c_str();
	size_t len = str.size();
	size_t pos = len;
	const CharType* p = __tcschr(ps, (CharType)'.');
	if (p)
		pos = p - ps;

	do 
	{
		int count = pos / 3;
		if (count == 0)
			break;

		pos = pos % 3;
		CBasicStaticBuffer buf;
		buf.Assign(2*(len + 1) * sizeof(CharType), 0);
		CharType* p = (CharType*)buf.GetBuffer();

		if (pos > 0)
		{
			p = __tcsncpy(p, ps, pos);
			p += pos;
			*p++ = _T(',');
			ps += pos;

		}
		for (int i = 0; i < count - 1; ++ i)
		{
			__tcsncpy(p, ps, 3);
			p += 3;
			*p ++ = _T(',');
			ps += 3;
		}
		__tcscpy(p, ps);
		str = buf.GetString();
	} while (false);
}


template<typename CharType>
CharType* __tcscpyn(CharType* strDest, size_t nDest, const CharType* strSource, size_t nSource = NullLen, bool bTrim = false)
{
	if(nSource == NullLen)
	{
		nSource = __tcslen(strSource);
	}
	if(bTrim)
	{
		size_t i = 0;
		for(i = nSource - 1; i != NullLen; -- i)
		{
			if(!ISSPACE(strSource[i]))
			{
				break;
			}
		}
		nSource = i + 1;
	}
	size_t nLen = MIN(nSource, nDest);
	if(nLen > 0)
	{
		memcpy(strDest, strSource, nLen * sizeof(CharType));
	}
	if(nLen < nDest)
	{
		strDest[nLen] = (CharType)0;
	}
	return strDest;
}

/** 
*\brief __atoi64 �ַ���ת64λ����
* 
*\param str 
*\param nLen Ĭ��ֵ -1
*\return  
*/
LONG64 __atoi64W(const TCHAR* str, int nLen/* = -1*/);
LONG64 __atoi64_s(const char* str, int nLen/* = -1*/);


// һЩ����basic_string�Ĳ���
namespace strutil
{
	const char*		__get_blank_string(__private::Type2Type<char>);
	const WCHAR*	__get_blank_string(__private::Type2Type<WCHAR>);

	//! �ַ������д
	template<class StringType>
	void makeupper(StringType& s)
	{
		transform(s.begin(), s.end(), s.begin(), (int(*)(int))toupper);
	}

	//! �ַ�����Сд
	template<class StringType>
	void makelower(StringType& s)
	{
		transform(s.begin(), s.end(), s.begin(), (int(*)(int))tolower);
	}

	//! ȥ����������lpszTarget�е��ַ�
	template<class StringType>
	void ltrim(StringType& s, typename StringType::const_pointer lpszTarget)
	{
		s.erase(0, s.find_first_not_of(lpszTarget));
	}


	//! ȥ�������ַ�
	template<class StringType>
	void ltrim(StringType& s)
	{
		typedef typename StringType::value_type value_type;
		ltrim(s, __get_blank_string(__private::Type2Type<value_type>()));
	}

	//! ȥ�����ָ���ַ�
	template<class StringType>
	void ltrim(StringType& s, typename StringType::value_type cTarget)
	{
		s.erase(0, s.find_first_not_of(cTarget));
	}

	//! ȥ���Ҳ������lpszTarget�е��ַ�
	template<class StringType>
	void rtrim(StringType& s, typename StringType::const_pointer lpszTarget)
	{
		s.erase(s.find_last_not_of(lpszTarget) + 1);
	}

	//! ȥ���Ҳ���ַ�
	template<class StringType>
	void rtrim(StringType& s)
	{
		typedef typename StringType::value_type value_type;
		rtrim(s, __get_blank_string(__private::Type2Type<value_type>()));
	}

	//! ȥ���Ҳ�ָ���ַ�
	template<class StringType>
	void rtrim(StringType& s, typename StringType::value_type cTarget)
	{
		s.erase(s.find_last_not_of(cTarget) + 1);
	}

	//! �滻�ַ���
	template<class StringType>
	int replace(StringType& s, typename StringType::const_pointer string_to_replace, typename StringType::const_pointer new_string)
	{
		typedef typename StringType::size_type size_type;
		int ret = 0;
		size_t oldlen = __tcslen(string_to_replace);
		size_t newlen = __tcslen(new_string);
		size_type pos = s.find(string_to_replace);
		while(StringType::npos != pos)
		{
			++ ret;
			s.replace(pos, oldlen, new_string);
			pos = s.find(string_to_replace, pos + newlen);
		}
		return ret;
	}


	template<typename CharType>
	struct __replace_char
	{
		__replace_char(CharType cOld, CharType cNew, int& ret) : __old(cOld), __new(cNew), __ret(ret){}

		CharType operator()(CharType c)
		{
			if (__old == c)
			{
				++ __ret;
				return __new;
			}
			else
				return c;
		}

		int&		__ret;
		CharType	__old;
		CharType	__new;
	};

	//! �滻�ַ�
	template<class StringType>
	int replace(StringType& s, typename StringType::value_type cOld, typename StringType::value_type cNew)
	{
		typedef typename StringType::value_type value_type;
		int ret = 0;
		transform(s.begin(), s.end(), s.begin(), __replace_char<value_type>(cOld, cNew, ret));
		return ret;
	}
}

__NS_BASIC_END

#endif 
