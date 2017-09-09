/***********************************************************************************************
// �ļ���:     base64.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 11:19:23
// ��������:   base64��׼���롢������
base64Ex ��չbase64���롢�����ࣨʵ�ּ򵥼��ܣ�
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_BASE64_H
#define BASIC_BASE64_H


__NS_BASIC_START
class CBasicBase64;		///��־Base64���봦����
     class CBasicBaseEx;	///�����Լ�������Base64���봦����

//! ��׼base64���롢���봦����
class _BASIC_DLL_API CBasicBase64 : public basiclib::CBasicObject
{
public:
	CBasicBase64();
	virtual ~CBasicBase64();

	//! Base64���뺯��
	/*! 
	 *\param szDecoding ��Ҫ���������ָ��
	 *\param nSize      ����֧�ֳ���
	 *\param buf        �������ݱ���,�ɹ���ʱ�򣬽���������׷����buf����β��
	 *\return           >0 �ɹ�����֮��ĳ��� <0 ����
	*/
	virtual long Decode(BYTE* szDecoding, int nSize, CBasicSmartBuffer& buf);
	//! Base64���뺯��
	/*! 
	 *\param szDecoding ��Ҫ���������ָ��
	 *\param nSize      ����֧�ֳ���
	 *\param pOut       �����������
	 *\param nMaxSize   pOut����󳤶ȣ�����ʱָ����������ݳ��ȣ�
	 *\return           >0 �ɹ�����֮��ĳ��� <0 ����,pOut=NULLʱ��������Ҫ�����ݳ���
	*/
	virtual long Decode( BYTE* szDecoding, int nSize, BYTE* pOut, int nMaxSize);

	//! Base64���뺯��
	/*! 
	 *\param szDecoding ��Ҫ���������ָ��
	 *\param nSize      ����֧�ֳ���
	 *\param buf        �������ݱ���,�ɹ���ʱ�򣬽�����������׷����buf����β��
	 *\return           >0 �ɹ�����֮��ĳ��� <0 ����
	*/
	virtual long Encode(BYTE* szEncoding, int nSize, CBasicSmartBuffer& buf);
	//! Base64���뺯��
	/*! 
	 *\param szDecoding ��Ҫ���������ָ��
	 *\param nSize      ����֧�ֳ���
	 *\param pOut       �����������
	 *\param nMaxSize   pOut����󳤶ȣ�����ʱָ����������ݳ��ȣ�
	 *\return           >0 �ɹ�����֮��ĳ��� <0 ����,pOut=NULLʱ�� ������Ҫ�����ݳ���
	*/
	virtual long Encode( BYTE* szEncoding, int nSize, BYTE* pOut, int nMaxSize);

protected:
	virtual BYTE GetEncodeAlphabet(int nIndex)
		{ return m_sBase64Alphabet[nIndex]; }
	virtual BYTE GetDecodeAlphabet(int nIndex)
		{ return m_sBase64Decode[nIndex]; }
	virtual char GetPadding()
		{ return base64_pad; }

protected:
	static BYTE m_sBase64Alphabet[];
	static unsigned char m_sBase64Decode[];
	static char base64_pad;	
};

//! ���������base64���롢���봦���࣬���ýӿں�CBasicBase64��ͬ
class _BASIC_DLL_API CBasicBase64Ex : public CBasicBase64
{
public:
	CBasicBase64Ex();
	virtual ~CBasicBase64Ex();

protected:
	virtual BYTE GetEncodeAlphabet(int nIndex)
		{ return m_sBase64Alphabet[nIndex]; }
	virtual BYTE GetDecodeAlphabet(int nIndex)
		{ return m_sBase64Decode[nIndex]; }
	virtual char GetPadding()
		{ return base64_pad; }

protected:
	static BYTE m_sBase64Alphabet[];
	static unsigned char m_sBase64Decode[];
	static char base64_pad;	
};
__NS_BASIC_END

#endif 
