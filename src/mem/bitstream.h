/***********************************************************************************************
// �ļ���:     smartbuffer.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 11:23:31
// ��������:   �ڴ�����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_BITSTREAMBUFFER_H
#define BASIC_BITSTREAMBUFFER_H

__NS_BASIC_START

#pragma pack(1)

class CNetBasicValue;
class CBasicBitstream : public basiclib::CBasicSmartBuffer
{
public:
	CBasicBitstream();
	CBasicBitstream(const string& s);
	CBasicBitstream(void* buf, int size);
	CBasicBitstream(const char* buf, int size);
	virtual ~CBasicBitstream();

	/////////////////////////////////////////////////////////////////////////////
	//���л�
	//�ַ�������
	CBasicBitstream& operator << (const Net_UChar v);
	CBasicBitstream& operator << (const Net_Char v);
	CBasicBitstream& operator << (const Net_UShort v);
	CBasicBitstream& operator << (const Net_Short v);
	CBasicBitstream& operator << (const Net_UInt v);
	CBasicBitstream& operator << (const Net_Int v);
	CBasicBitstream& operator << (const Net_LONGLONG v);
	CBasicBitstream& operator << (const Net_Double v);
	CBasicBitstream& operator << (const CNetBasicValue* pV);
	CBasicBitstream& operator << (const Net_CBasicString* pV);
	CBasicBitstream& operator << (const basiclib::CBasicSmartBuffer* pV);
	CBasicBitstream& operator << (const Net_Char* v);
	CBasicBitstream& operator << (const Net_UChar* v);
	CBasicBitstream& operator << (const Net_CBasicString& data);
	CBasicBitstream& operator << (const basiclib::CBasicSmartBuffer& pV);
	template<typename A>
	CBasicBitstream& operator << (const typename Net_Set<A>::ContainForNet& data)
	{
		Net_UShort uSize = data.size();
		*this << uSize;
		for (Net_Set<A>::ContainForNet::const_iterator iter = data.begin(); iter != data.end(); iter++)
		{
			*this << *iter;
		}
		return *this;
	}
	template<class A>
	CBasicBitstream& operator << (const typename Net_Vector<A>::ContainForNet& data)
	{
		Net_UShort uSize = data.size();
		*this << uSize;
		for (Net_Vector<A>::ContainForNet::const_iterator iter = data.begin(); iter != data.end(); iter++)
		{
			*this << *iter;
		}
		return *this;
	}
	template<class A, class B>
	CBasicBitstream& operator << (const typename Net_Map<A, B>::ContainForNet& data){
		Net_UShort uSize = data.size();
		*this << uSize;
		for (Net_Map<A, B>::ContainForNet::const_iterator iter = data.begin(); iter != data.end(); iter++)
		{
			*this << iter->first;
			*this << iter->second;
		}
		return *this;
	}
	/////////////////////////////////////////////////////////////////////////////
	//�����л�
	//�ַ���
	CBasicBitstream& operator >> (Net_UChar& v);
	CBasicBitstream& operator >> (Net_Char& v);
	CBasicBitstream& operator >> (Net_UShort& v);
	CBasicBitstream& operator >> (Net_Short& v);
	CBasicBitstream& operator >> (Net_UInt& v);
	CBasicBitstream& operator >> (Net_Int& v);
	CBasicBitstream& operator >> (Net_LONGLONG& v);
	CBasicBitstream& operator >> (Net_Double& v);
	CBasicBitstream& operator >> (Net_CBasicString* pV);
	CBasicBitstream& operator >> (basiclib::CBasicSmartBuffer* pV);
	CBasicBitstream& operator >> (CNetBasicValue* pV);
	CBasicBitstream& operator >> (Net_Char* v);
	CBasicBitstream& operator >> (Net_UChar* v);
	CBasicBitstream& operator >> (Net_CBasicString& data);
	CBasicBitstream& operator >> (basiclib::CBasicSmartBuffer& pV);
	template<class A>
	CBasicBitstream& operator >> (typename Net_Set<A>::ContainForNet& data)
	{
		Net_UShort uSize = 0;
		*this >> uSize;
		if (uSize > 0)
		{
			for (Net_UShort i = 0; i < uSize; i++)
			{
				A intKey;
				*this >> intKey;
				data.insert(intKey);
			}
		}
		return *this;
	}
	template<class A>
	CBasicBitstream& operator >> (typename Net_Vector<A>::ContainForNet& data)
	{
		Net_UShort uSize = 0;
		*this >> uSize;
		if (uSize > 0)
		{
			data.resize(uSize);
			for (Net_UShort i = 0; i < uSize; i++)
			{
				*this >> data[i];
			}
		}
		return *this;
	}
	template<class A, class B>
	CBasicBitstream& operator >> (typename Net_Map<A, B>::ContainForNet& data){
		Net_UShort uSize = 0;
		*this >> uSize;
		for (Net_UShort i = 0; i < uSize; i++)
		{
			A intKey = 0;
			*this >> intKey;
			*this >> data[intKey];
		}
		return *this;
	}
	/////////////////////////////////////////////////////////////////////////////
	void SerializeOneByte(const Net_UChar v);
	void SerializeTwoByte(const Net_UShort v);
	void SerializeFourByte(const Net_UInt v);
	void SerializeEightLongLongByte(const Net_LONGLONG v);
	void SerializeEightDoubleByte(const Net_Double v);
	void SerializeCStringByte(const Net_CBasicString* pV);
	void SerializeSmbufByte(const basiclib::CBasicSmartBuffer* pV);
	void SerializeLikeCString(const char* pData, Net_UShort nLength);
	/////////////////////////////////////////////////////////////////////////////
	Net_UChar UnSerializeUChar();
	Net_Char UnSerializeChar();
	Net_UShort UnSerializeUShort();
	Net_Short UnSerializeShort();
	Net_UInt UnSerializeUInt();
	Net_Int UnSerializeInt();
	Net_LONGLONG UnSerializeLongLong();
	Net_Double UnSerializeDouble();
	void UnSerializeCString(Net_CBasicString* pV);
	void UnSerializeSmbuf(basiclib::CBasicSmartBuffer* pV);
	void UnSerializeLikeCString(const std::function<void(const char* pData, Net_UShort nLength)>& func);
protected:
		//�������ݵĲ���
		void Write(const char* pszData, long lLength);
		void Write(void* pszData, long lLength);
		//��ȡ����
		void Read(char* pBuffer, int nLength);
protected:
	char m_szBuf[8];			//������8�ֽ�����
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const double TINY_VALUE = (1E-10);

const char		g_strDoubleNull[8] = { (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0, (char)0x80 };
const double	DTE_DOUBLE_NULL = *(double*)g_strDoubleNull;

#define DTE_CHAR_NULL		_T("NUL")		//�ַ�����ֵ
#define DTE_LONG_NULL		LONG_MIN		//������ֵ
#define DTE_DATETIME_NULL	SS_MINTIME		//����ʱ���ֵ
#define DTE_COLOR_NULL		0x7fffffff		//��ɫCOLORREF

#define IsColorNull(x)		((x) == DTE_COLOR_NULL)
#define SetColorNull(x)		(x) = DTE_COLOR_NULL

#define IsDoubleNullExt(x)		((((x)==basiclib::DTE_DOUBLE_NULL)) && ((char*)(&x))[7] == (char)0x80)
#define IsDoubleNull(x)			(((x)==NULL) || (((*(x)==basiclib::DTE_DOUBLE_NULL)) && ((char*)(x))[7] == (char)0x80))
#define SetDoubleNull(x)		x=basiclib::DTE_DOUBLE_NULL

#define	IsLongNull(x)			(x==DTE_LONG_NULL)
#define SetLongNull(x)			x=DTE_LONG_NULL

#define	IsTimeNull(x)			(x == DTE_DATETIME_NULL)
#define SetTimeNull(x)			x = DTE_DATETIME_NULL

const unsigned char NETVALUE_NULL = 0;
const unsigned char NETVALUE_CHAR = 1;
const unsigned char NETVALUE_STRING = 2;
const unsigned char NETVALUE_LONG = 3;
const unsigned char NETVALUE_DOUBLE = 4;
const unsigned char NETVALUE_LONGLONG = 5;

class CNetBasicValue
{
public:
	CNetBasicValue();
	~CNetBasicValue();
	CNetBasicValue(const Net_Double& value);
	CNetBasicValue(const Net_LONGLONG& value);
	CNetBasicValue(const Net_Int value);
	CNetBasicValue(const char* value, size_t len = NullLen);
	CNetBasicValue(const CNetBasicValue& value);

	void			SetLong(const Net_Int value);
	void			SetDouble(const Net_Double& value);
	void			SetLongLong(const Net_LONGLONG& value);
	void			SetString(const char* value, size_t len = NullLen);

	Net_Int			GetLong() const;
	Net_Double		GetDouble() const;
	Net_LONGLONG	GetLongLong() const;
	CBasicString	GetString() const;
	const char*		GetStringRef() const;

	Net_UChar		GetDataType() const{ return m_type; }
	long			GetDataLength() const;

	bool			IsNull() const;
	void			SetNull();
	bool			IsString() const;

	// data comparison
	int CompareBasicValue(const CNetBasicValue* pRhs) const;
	int CompareInt(Net_Int rhs) const;
	int CompareDouble(const Net_Double& rhs) const;
	int CompareLongLong(Net_LONGLONG rhs) const;
	int ComparePointString(const char* rhs, size_t len) const;

	// assignment
	const CNetBasicValue& operator = (const CNetBasicValue& rhs);
	const CNetBasicValue& operator = (const Net_Int rhs);
	const CNetBasicValue& operator = (const Net_Double& rhs);
	const CNetBasicValue& operator = (const Net_LONGLONG& rhs);
	const CNetBasicValue& operator = (const char* rhs);
	// add
	const CNetBasicValue& operator += (const CNetBasicValue& rhs);
	const CNetBasicValue& operator += (const Net_Int rhs);
	const CNetBasicValue& operator += (const Net_Double& rhs);
	const CNetBasicValue& operator += (const Net_LONGLONG& rhs);
	const CNetBasicValue& operator += (const char* rhs);
	// sub
	const CNetBasicValue& operator -= (const CNetBasicValue& rhs);
	const CNetBasicValue& operator -= (const Net_Int rhs);
	const CNetBasicValue& operator -= (const Net_Double& rhs);
	const CNetBasicValue& operator -= (const Net_LONGLONG& rhs);
	// multi
	const CNetBasicValue& operator *= (const CNetBasicValue& rhs);
	const CNetBasicValue& operator *= (const Net_Int rhs);
	const CNetBasicValue& operator *= (const Net_Double& rhs);
	const CNetBasicValue& operator *= (const Net_LONGLONG& rhs);
	// div
	const CNetBasicValue& operator /= (const CNetBasicValue& rhs);
	const CNetBasicValue& operator /= (const Net_Int rhs);
	const CNetBasicValue& operator /= (const Net_Double& rhs);
	const CNetBasicValue& operator /= (const Net_LONGLONG& rhs);

	//���л�
	void SeriazeSMBuf(basiclib::CBasicSmartBuffer& smBuf) const;
	void UnSeriazeSMBuf(basiclib::CBasicSmartBuffer& smBuf);

	int Seriaze(char* pData, int nLength);
	int UnSeriaze(const char* pData, int nLength);
	//��ȡ���л�����
	int GetSeriazeLength() const;
protected:
	void			toData(Net_Int& lValue) const;
	void			toData(Net_Double& fValue) const;
	void			toData(Net_LONGLONG& llValue) const;

	void	Release();
	void	EmptyValue();
	void	AddString(const char* lpStr);
	static	CBasicString DataToString(Net_UShort nDataType, const void* value);
protected:
	Net_UChar				m_type;
	union{
		Net_Int				m_lValue[2];
		Net_LONGLONG		m_llValue;
		Net_Double			m_dValue;
		Net_Char			m_cValue[8];
		Net_CBasicString*	m_pBufValue;
	};
};

inline CNetBasicValue operator+(const CNetBasicValue& lhs, const CNetBasicValue& rhs)
{
	CNetBasicValue res(lhs);
	res += rhs;
	return res;
}

inline CNetBasicValue operator-(const CNetBasicValue& lhs, const CNetBasicValue& rhs)
{
	CNetBasicValue res(lhs);
	res -= rhs;
	return res;
}

inline CNetBasicValue operator*(const CNetBasicValue& lhs, const CNetBasicValue& rhs)
{
	CNetBasicValue res(lhs);
	res *= rhs;
	return res;
}

inline CNetBasicValue operator/(const CNetBasicValue& lhs, const CNetBasicValue& rhs)
{
	CNetBasicValue res(lhs);
	res /= rhs;
	return res;
}

inline bool operator>(const CNetBasicValue& lhs, const CNetBasicValue& rhs)
{
	return lhs.CompareBasicValue(&rhs) > 0;
}
inline bool operator>(const CNetBasicValue& lhs, const Net_Int rhs)
{
	return lhs.CompareInt(rhs) > 0;
}

inline bool operator>=(const CNetBasicValue& lhs, const CNetBasicValue& rhs)
{
	return lhs.CompareBasicValue(&rhs) >= 0;
}
inline bool operator>=(const CNetBasicValue& lhs, const Net_Int rhs)
{
	return lhs.CompareInt(rhs) >= 0;
}

inline bool operator<(const CNetBasicValue& lhs, const CNetBasicValue& rhs)
{
	return lhs.CompareBasicValue(&rhs) < 0;
}
inline bool operator<(const CNetBasicValue& lhs, const Net_Int rhs)
{
	return lhs.CompareInt(rhs) < 0;
}
inline bool operator<=(const CNetBasicValue& lhs, const CNetBasicValue& rhs)
{
	return lhs.CompareBasicValue(&rhs) <= 0;
}
inline bool operator<=(const CNetBasicValue& lhs, const Net_Int rhs)
{
	return lhs.CompareInt(rhs) <= 0;
}

inline bool operator==(const CNetBasicValue& lhs, const CNetBasicValue& rhs)
{
	return lhs.CompareBasicValue(&rhs) == 0;
}
inline bool operator==(const CNetBasicValue& lhs, const Net_Int rhs)
{
	return lhs.CompareInt(rhs) == 0;
}
inline bool operator!=(const CNetBasicValue& lhs, const CNetBasicValue& rhs)
{
	return lhs.CompareBasicValue(&rhs) != 0;
}
inline bool operator!=(const CNetBasicValue& lhs, const Net_Int rhs)
{
	return lhs.CompareInt(rhs) != 0;
}


#pragma pack()
__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////////


#endif 
