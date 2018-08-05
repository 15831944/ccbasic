/***********************************************************************************************
// �ļ���:     rc5.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012-2-22 23:55:10
// ��������:   ���� RC5 �ļӽ����㷨����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_RC5_H
#define BASIC_RC5_H

#pragma once
#pragma	pack(1)
__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//����

//! ʹ��RC5�㷨�ӽ�������
/*!
*\param pOutData ������ݵĻ�����xml
*\param pInData  �������ݵĻ����������Ժ� pOutData ����ͬ��
*\param lDatalen ���ݵĳ��ȡ�������8����������
*\param pKey     ���ܵ���Կ��
*\param iKeylen  ��Կ�ĳ��ȡ������� 8 16
*\param iEncrypt = 1 ����  = 0 ����
*\param nRounds  ���ܵ�����������ѡ�� 8 12 16 �� ����Խ�࣬����ǿ��Խ�󡣼����ٶȾ�Խ����
*\return �ɹ�: ���� 0  ���ɹ�����  < 0
*\remarks �������㷨ǿ�ȱ�DES�ͣ������ٶ���DES��3�����ҡ�
*\warning ������ݳ��Ȳ���8�������������ش���
*/

class IBasicSecurity{
public:
    virtual void Check() = 0;
};

_BASIC_DLL_API int Basic_RC5_ecb_encrypt(basiclib::IBasicSecurity* pSecurity, char *pOutData, char *pInData, long lDatalen, const char *pKey, int iKeylen, int iEncrypt, int nRounds = 8);

/////////////////////////////////////////////////////////////////////////////////////////////
__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif 
