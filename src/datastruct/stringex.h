/***********************************************************************************************
// �ļ���:     stringex.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 9:17:44
// ��������:   ֧���ⲿ�����ַ�����Դ
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_STRINGEX_H
#define BASIC_STRINGEX_H

__NS_BASIC_START

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*\brief Basic_LoadStringSource �����ַ�����Դ�ļ�
*
*\param lpszFile �ַ�����Դ�ļ��� ȫ·�� ini��ʽ��ID=�ַ���
*\param lpszModuleName ģ�����ƣ������岻ͬ��Դ�ļ�ʱ���ã�ģ���ڱ��뱣��IDΨһ
*\return
*/
bool BasicLoadStringSource(const char* lpszFile, const char* lpszModuleName);

/**
*\brief BASIC_LoadString �����ַ�����Դ
*
*\param lpszFormatID
*\param lpszModuleName
*\return
*/
const char* BasicLoadString(const char* lpszFormatID, const char* lpszModuleName);
const char* BasicLoadString(unsigned long ulID, const char* lpszModuleName);
__NS_BASIC_END

#endif 


