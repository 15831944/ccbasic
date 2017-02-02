/***********************************************************************************************
// �ļ���:     key_valye.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 8:18:40
// ��������:   ���������ַ���key=value�Ľ��������ɡ�����url�������ַ���
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_KEY_VALUE_H
#define BASIC_KEY_VALUE_H
#pragma once
#pragma	pack(1)
/////////////////////////////////////////////////////////////////////////////////////////////
//����
/////////////////////////////////////////////////////////////////////////////////////////////
__NS_BASIC_START
#pragma warning (push)
#pragma warning (disable: 4251)

class _BASIC_DLL_API CBasicKey2Value : public basiclib::CBasicObject
{
public:
	CBasicKey2Value(int nHashSize = 7);

	BOOL HasValue(const char* lpszKey, CBasicString& strValue);
	CBasicString GetValue(const char* lpszKey) const;
	const char* GetValueRef(const char* lpszKey) const;

	void SetValue(const char* lpszKey, const char* lpszValue);
	void SetValue(const char* lpszKey, long lValue);

	BOOL RemoveKey(const char* lpszKey);

	void RemoveAll()
	{
		m_map.RemoveAll();
	}

	CBasicString& operator [](CBasicString& strKey)
	{
		strKey.MakeLower();
		return m_map[strKey];
	}

	BOOL IsEmpty()
	{
		return m_map.IsEmpty();
	}
public:
	//Ĭ�Ϸָ��� =&
	void ParseTextURL(const char* pszBuffer, int nLen = -1);
	//Ĭ�Ϸָ��� =\n
	void ParseTextLine(const char* pszBuffer, int nLen = -1);
	//�ⲿָ���ָ���
	void ParseText(const char* pszBuffer, const char* lpszTok, int nLen = -1);

	//�ϲ����ַ���
	//���ص�ָ��Ϊ sbBuf ���������
	const char* CombineToString(CBasicSmartBuffer& sbBuf, const char* lpszTok = NULL);
public:
	template<class Function>
	void ForEach(Function  func)
	{
		CMapStringToString_S::iterator i = m_map.begin();
		for (; i != m_map.end(); i++)
		{
			func(i->first.c_str(), i->second.c_str());
		}
	}
protected:
	void _ParseText(const char* pszBuffer, const char* lpszTok, int nLen = -1);
protected:
	void SetRawValue(const char* lpszKey, const char* lpszValue);
	virtual void SetValueAt(CBasicString& strKey, const char* lpszValue);
protected:
	CMapStringToString_S	m_map;
};

#pragma warning (pop)

__NS_BASIC_END

#pragma	pack()

/////////////////////////////////////////////////////////////////////////////////////////////
#endif //#define BASIC_KEY_VALUE_H
