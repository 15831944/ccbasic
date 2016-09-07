/***********************************************************************************************
// �ļ���:     smartbuffer.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 11:23:31
// ��������:   �ڴ�����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_SMARTBUFFER_H
#define BASIC_SMARTBUFFER_H

#pragma	pack(1)
__NS_BASIC_START
//////////////////////////////////////////////////////////////////////////////////////////////
//class CBasicObject;
//	class CWBasicSmartBuffer;						//�ɱ䳤�ȵ��ڴ��
//////////////////////////////////////////////////////////////////////////////////////////////
//! �ɱ䳤�ȵ��ڴ��
/*!
*  
*/
class _BASIC_DLL_API CBasicSmartBuffer : public CBasicObject
{
public:
	CBasicSmartBuffer();
	CBasicSmartBuffer(const basiclib::CBasicSmartBuffer &smBuf);
	virtual ~CBasicSmartBuffer();
public:
	//binddatatosmartbuffer,�󶨵������ⲿ�Ͳ���ʹ���ˣ���������ݻᱻ�ı�
	void BindOutData(char* pData, int nLength);

	BOOL IsEmpty() const { return m_pszBuffer == NULL; }
	void Free();

	char* GetDataBuffer(long& lLength) const;
	char* GetDataBuffer() const { return m_pszBuffer; }

	long GetDataLength() const{ return m_cbBuffer; }
	void SetDataLength(long lLength);

	char* AppendData(const char* pszData, long lLength);
	char* AppendDataEx(const char* pszData, long lLength);
	char* CommitData(long lLength);		//�������ݵĳ��ȣ���Ҫʱ���·����ڴ棩������ԭ����ָ�룬�����������

	void AppendString(const char* lpszText);
	void AppendData(long lVal);
	void AppendData(double fVal);

	BOOL InitFormFile(const char* lpszFile);

	CBasicSmartBuffer& operator = (const CBasicSmartBuffer& buffer);
	friend bool operator == (CBasicSmartBuffer& b1, CBasicSmartBuffer& b2);

	CBasicSmartBuffer& operator << (const char* lpszText)
	{
		AppendString(lpszText);
		return *this;
	}
	//��ȡ����
	void ReadData(void* pData, int nLength);

	//���úͻ�ȡ����
	long GetAllocBufferLength(){ return m_cbAlloc; }
protected:
	char* AllocBuffer(long lLength, long lGrowLength = 0);
	void  EmptyBuffer();
protected:
	bool	m_bSelfBuf;
	char*	m_pszBuffer;
	long	m_cbAlloc;
	long	m_cbBuffer;
};
bool operator == (CBasicSmartBuffer& b1, CBasicSmartBuffer& b2);
////////////////////////////////////////////////////////////////////////////////////////////////////////
__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif 
