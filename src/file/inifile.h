/***********************************************************************************************
// �ļ���:     inifile.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012-2-22 23:41:54
// ��������:   
����ini�ļ�����,��д���ݴ��ģʽ��
ʹ���µĽ�������ParseIni��߽����ٶ�

����֧�֣�
1.ini�ļ����֧��4GB
2.key���֧��1024�ֽ�
3.ֵ���֧��4MB�ֽ�
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_INIFILE_H
#define BASIC_INIFILE_H 

#include <map>
#include <string>
#include <algorithm>
#include <stdio.h>

__NS_BASIC_START
//ini�ļ�������
#define INI_TYPE_SCETION		0x00000001	//section ��ʼ
#define INI_TYPE_KEY			0x00000002	//ֵ��
#define INI_TYPE_SCETION_END	0x00000004	//section ����
#define INI_COMMENT				0x00000008	//ע����

template<typename F>
long ParseIniChar(const char* pszData, long cbData, F f, long cbBeginPos = 0)
{
	long nLines = 0;
	long nType = 0;
	long nBeginPos = 0;
	long nLineBegin = cbBeginPos;
	long nNameEnd = 0;
	long nSecBegin = 0;
	long nSpaceNum = 0;
	CBasicString strLastSection;
	char d = '\0';
	int i = 0;
	for (i = cbBeginPos; i < cbData; ++i)
	{
		d = *(pszData + i);

		if (d == '[')
		{
			if (nType == 0 && (nSpaceNum >= i - nLineBegin))
			{
				nType = INI_TYPE_SCETION;
				nBeginPos = i + 1;
				if (!strLastSection.IsEmpty())
				{
					if (!f(INI_TYPE_SCETION_END, strLastSection.c_str(), strLastSection.c_str(), 0, strLastSection.GetLength(), nSecBegin, nLineBegin))
						break;
				}
				nSecBegin = i; //section��ʼλ��
			}
		}
		else if (d == ']')
		{
			if (nType == INI_TYPE_SCETION)
			{
				strLastSection = CBasicString(pszData + nBeginPos, i - nBeginPos);
				if (!f(INI_TYPE_SCETION, strLastSection.c_str(), pszData, nBeginPos, i, -1, -1))
					break;
				nType = 0;
			}
		}
		else if (d == '\r' || d == '\n')
		{
			if (nType == INI_TYPE_KEY)
			{
				if (!f(INI_TYPE_KEY, strLastSection.c_str(), pszData, nLineBegin, nNameEnd, nNameEnd + 1, i))
					break;
			}

			if (d == _T('\n'))
			{
				++nLines;
				nLineBegin = i + 1;	//���п�ʼλ��
			}

			nType = 0;
			nSpaceNum = 0;
		}
		else if (nType == 0 && d == '=')
		{
			nType = INI_TYPE_KEY;
			nNameEnd = i;
		}
		else if (nType == 0 && (d == ';' || d == '#'))	//�ж�ע����
		{
			nType = INI_COMMENT;
		}
		else if (nType == 0 && (d == ' ' || d == '\t'))
		{
			++nSpaceNum;
		}
	}

	if (!strLastSection.IsEmpty())
	{
		f(INI_TYPE_SCETION_END, strLastSection.c_str(), strLastSection.c_str(), 0, strLastSection.GetLength(), nSecBegin, i);
	}
	return nLines;
}



#ifdef _WIN32
#define		NEW_LINE			"\r\n"
#define		NEW_LINE_LEN		2 * sizeof(char)
#else	
#define		NEW_LINE			"\n"
#define		NEW_LINE_LEN		1 * sizeof(char)
#endif

#define		SEP_SIGN			"||"

//64�ֽڵ����ݴ��ƫ����
struct IndexData
{
	union
	{
		LONGLONG  m_nData64;		//64λ����
		struct  
		{
			unsigned int m_nOffset;	//����ƫ����
			union
			{				
				unsigned int m_nSectionLength;	//�γ���
				struct  
				{
					//���������Ҫ���޸�������ֵ���Ըı����֧�ֵ����ݳ��ȣ�
					//ע�⣬������ӵ���32
					unsigned int m_nKeyLen : 10;	//ֵ���Ƴ���,�1024�ֽ�
					unsigned int m_nValueLen : 22;	//ֵ���ȣ��2^22=4MB�ֽڣ�
				};
			};
		};
	};
};

namespace __private
{
	template<typename F>
	class get_section_strChar
	{
	public:
		get_section_strChar(F f) :m_f(f){};
		bool operator()(long nType, const char* lpszSection, const char* lpszData, long nV1Begin, long nV1End, long nV2Begin, long nV2End)
		{
			if (nType == INI_TYPE_KEY)
			{
				CBasicString strName(lpszData + nV1Begin, nV1End - nV1Begin);
				CBasicString strValue(lpszData + nV2Begin, nV2End - nV2Begin);
				strName.TrimLeft();
				strName.TrimRight();

				m_f(strName, strValue);
			}
			return true;
		}
	protected:
		F m_f;
	};

	//�������еĶ�
	template<typename F>
	class get_section_list_strChar
	{
	public:
		get_section_list_strChar(F f) :m_f(f){};
		bool operator()(long nType, const char* lpszSection, const char* lpszData, long nV1Begin, long nV1End, long nV2Begin, long nV2End)
		{
			if (nType == INI_TYPE_SCETION)
			{
				m_f(lpszSection);
			}
			return true;
		}
	protected:
		F m_f;
	};
}

#pragma warning (push)
#pragma warning (disable: 4251)
class _BASIC_DLL_API CBasicIniOp
{
public:
	CBasicIniOp()
	{
		m_bModified = false;
	}
	CBasicIniOp(const char* lpszFileName)
	{
		m_bModified = false;
		InitFromFile(lpszFileName);
	}
	virtual ~CBasicIniOp()
	{
		Empty();
	};
    //!get size
    int GetIniSize();
public:
	typedef		 basic_map<CBasicString, LONGLONG>::type   Dict;		//�������ԵĶ�Ӧ��ϵ	
	Dict				m_dic;					///ͨ��key�������ݵ����ݽṹ		
	CBasicString		m_file;				///Ĭ�ϵ�ini�ļ�������������ȡ���ļ���

	bool				m_bModified;			///�ж������Ƿ��޸Ĺ�
	CBasicSmartBuffer	m_buffer;			//ini�ļ��Ļ���
	CBasicSmartBuffer	m_tmp;				///��ʱ����
protected:
	char* GetDataBuffer()
	{
		return (char*)m_buffer.GetDataBuffer();
	}
	long   GetDataLength()
	{
		return m_buffer.GetDataLength() / sizeof(char);
	}
public:
	///��յ�ǰ����
	void Empty();

	///�жϵ�ǰ�Ƿ��ж�ȡ������
	///����Ϊ�շ���true�����򷵻�false
	bool IsEmpty()
	{
		return m_buffer.GetDataLength() <= 0;
	}

	///���������Ƿ��޸Ĺ�
	bool IsModified()
	{
		return m_bModified;
	}

	/// �������ļ���ʼ�������ֵ�
	/// >= 0��ȡ�ɹ�
	/// -1 ��ȡ�ļ�ʧ��
	/// -2 �ļ�������
	int InitFromFile(const char* filename);
	/// ���ڴ������г�ʼ����ANSI �汾��
	int InitFromMem(const char* lpszData, size_t cbData);

	///����һ��section��f��Ϊ�ص���������������ԭ����void f(LPCTSTR key��LPCTSTR value)
	///ÿ������һ��key���ͻ����fһ�Σ�������Ч��key=value�в��ص�����ע�͵�
	template<typename F>
	int  GetSection(const char* lpszSection, F f)
	{
		ASSERT(lpszSection);
		CBasicString strSection = lpszSection;
		strSection.MakeUpper();
		Dict::iterator iter = m_dic.find(strSection);
		if (iter == m_dic.end())
		{
			return -1;
		}

		IndexData i;
		i.m_nData64 = iter->second;
		char* pszData = GetDataBuffer();
		__private::get_section_strChar<F> _callback(f);
		return ParseIniChar(pszData + i.m_nOffset, i.m_nSectionLength, _callback);
	}
	///ɾ��һ��section
	//���sectionΪNULL����ôɾ�����������ļ�
	bool EmptySection(const char* lpszSection);

	//�������еĶ�����f��Ϊ�ص�����һ��������ԭ��Ϊ void f(LPCTSTR lpszSection);
	template<typename F>
	int GetSectionList(F f)
	{
		__private::get_section_list_strChar<F> _callback(f);
		return ParseIniChar(GetDataBuffer(), GetDataLength(), _callback);
	}

	///����ֵ��[key1]findkey2֮ǰ
	///���findkey2�����ڣ���ô��������
	///����ֵʱ��ԭ����[key1]key2ֵ����ɾ��
	///key1, key2,value, findkey2������Ϊnull
	///�ɹ�����0�� ʧ�ܷ���-1
	int InsertBefore(const char* key1, const char* key2, const char* value, const char* findkey2);

	//����ֵ�����value=NULL,��ôɾ����ֵ
	void SetData(const char* key1, const char* key2, const char* value)
	{
		_SetData(key1, key2, value);
	}

	void SetLong(const char* key1, const char* key2, long lValue)
	{
		char szBuf[128];
		sprintf(szBuf, "%d", (Net_Int)lValue);
		_SetData(key1, key2, szBuf);
	}

	void SetDouble(const char* key1, const char* key2, double fValue)
	{
		char szBuf[128];
		sprintf(szBuf, "%f", fValue);
		_SetData(key1, key2, szBuf);
	}

	CBasicString GetData(const char* key1, const char* key2, const char* defval = "")
	{
		CBasicString strRet = GetSelfData(key1, key2, defval);
		strRet.TrimLeft();
		strRet.TrimRight();
		return strRet;
	}

	///ȡ����
	double GetDouble(const char* key1, const char* key2, const char* defval = "")
	{
		return atof(GetData(key1, key2, defval).c_str());
	}

	///ȡlong������
	long  GetLong(const char* key1, const char* key2, const char* defval = "")
	{
		return atoi(GetData(key1, key2, defval).c_str());
	}

	LONG64 __atoi64Char(const char* str, int nLen/* = -1*/)
	{
		char Buff[32];
		if (nLen == -1)
		{
			nLen = strlen(str);
		}
		memset(Buff, 0, sizeof(Buff));
		strncpy_s(Buff, sizeof(Buff)-1, str, nLen);
		Basic_LTrim(Buff);
#ifdef __BASICWINDOWS
		return _atoi64(Buff);
#else
		char* pEnd;
		return strtol(Buff, &pEnd, 10);
#endif
	}
	__int64 GetInt64(const char* key1, const char* key2, __int64 nDefault = 0)
	{
		CBasicString strRet = GetData(key1, key2);
		if (strRet.GetLength() <= 0)
		{
			return nDefault;
		}
		return __atoi64Char(strRet.c_str(), strRet.GetLength());
	}
	
	///
	virtual CBasicString GetSelfData(const CBasicString& section, const CBasicString& key, const char* defval = "")
	{
		CBasicString strKey = section + SEP_SIGN + key;
		strKey.MakeUpper();
		Dict::iterator iter = m_dic.find(strKey);
		if (iter == m_dic.end())
		{
			return defval;
		}

		IndexData i;
		i.m_nData64 = iter->second;
		char* pszData = GetDataBuffer();
		return CBasicString(pszData + i.m_nOffset + i.m_nKeyLen + 1, i.m_nValueLen);
	}

	//! ������д���ļ�
	bool WriteToFile(const char* filename = NULL);



	/*!����ǰ���ݺϲ����ļ�filename�У����õ�ǰ�����ݺϲ��ļ��е�����
	*\param  ��Ҫ�ϲ����ļ�
	*\return ���ش������������
	*\remark ��ǰini�ļ��е�ע����Ϣ�ᱻ����
	*/
	long CombinToFile(const char* filename)
	{
		CBasicIniOp ini(filename);

		long lCount = ParseIniChar(GetDataBuffer(), GetDataLength(), [&](long nType, const char* lpszSection, const char* lpszData, long nV1Begin, long nV1End, long nV2Begin, long nV2End)->bool{
			return ini.ParseCombine(nType, lpszSection, lpszData, nV1Begin, nV1End, nV2Begin, nV2End);
		});

		ini.WriteToFile(filename);
		return lCount;
	}

	/*!����ǰ�����е����ݺϲ������������У����ȼ��ǵ�ǰ���� > ini
	*\param ini ��Ҫ�ϲ���CWBasicIniOp����
	*\return ���ش������������
	*\remark ��ǰ�����е�ע����Ϣ��������
	*/
	long combine_to_other(CBasicIniOp& ini)
	{
		return ParseIniChar(GetDataBuffer(), GetDataLength(), [&](long nType, const char* lpszSection, const char* lpszData, long nV1Begin, long nV1End, long nV2Begin, long nV2End)->bool{
			return ini.ParseCombine(nType, lpszSection, lpszData, nV1Begin, nV1End, nV2Begin, nV2End);
		});
	}

	/*!��������֮��Ŀ���
	*/
	CBasicIniOp& operator =(const CBasicIniOp& ini)
	{
		m_buffer = ini.m_buffer;
		m_dic = ini.m_dic;
		m_file = ini.m_file;
		return *this;
	}
protected:
	//�ϲ���ʽ��ini����
	bool ParseCombine(long nType, const char* lpszSection, const char* lpszData, long nV1Begin, long nV1End, long nV2Begin, long nV2End);

	/*!����ini�ļ��Ļص�����*/
	bool ParseINI(long nType, const char* lpszSection, const char* lpszData, long nV1Begin, long nV1End, long nV2Begin, long nV2End);

	/*!�ڲ���ֵ����*/
	void _SetData(const char* key1, const char* key2, const char* value);

	/*!����Ҫ�䶯�����ݵ�ƫ�����ı�*/
	void UpdateOffset(DWORD dwBeginOffset, long nMove);

	/// ���ڴ������г�ʼ��
	/// �ڲ��������ã����ṩ����֧��
	int InitData(const char* lpszData, size_t cbData);

	/*��������
	\param dwStartPos ���λ��
	\param cbOld      ԭʼ���ݳ���
	\param lpszNewData ������
	\param cbData      �����ݳ��ȶ�
	*/
	void AddNewData(DWORD dwStartPos, long cbOldData, const char* lpszNewData, long cbData);

	/// ���ڴ������г�ʼ����ANSI �汾��
	int _InitData(const char* lpszData, size_t cbData);

	//! ���ݻص�������д������
	void _WriteTo(const std::function<long(const char*, long)>& func);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma warning (pop)


#ifndef __BASICWINDOWS
BOOL  WritePrivateProfileString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszString, LPCTSTR lpszFilename);
DWORD GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName,LPCTSTR lpDefault,LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName);
int   GetPrivateProfileInt( LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName);
#endif

__NS_BASIC_END
#endif
