/***********************************************************************************************
// �ļ���:     crc32.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2014-1-16 16:36:23
// ��������:   ���� CRC16 ���㷨����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_CRC32_H
#define BASIC_CRC32_H


#pragma once
__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//����

//!�����ڴ��CRC32ֵ
/*! 
*\param pszData		 ��Ҫ����CRC�����ݻ�����ָ��
*\return �ɹ�: ���� �����CRC
*\remarks 
*\warning
*/

uint32_t _BASIC_DLL_API Basic_crc32(unsigned char* pszData, unsigned int nDataLength);
long _BASIC_DLL_API Basic_crc32_File(const char* lpszFileName, uint32_t &crc);

//! ʹ�ü����ļ���CRC32ֵ
/*! 
*\param lpszFileName���ļ���
*\param crc32��������ļ�CRC32ֵ
*\return �ɹ�: BASIC_FILE_OK�� ʧ�ܷ��ط�0 ֵ���ο��ļ�����
*\remarks 
*\warning
*/
//long Basic_crc32_File(LPCTSTR lpszFileName, unsigned long &crc32);

/////////////////////////////////////////////////////////////////////////////////////////////
__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////

#endif 