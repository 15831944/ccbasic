/***************************************************************************************************
* Copyright (c) 2014, ҵ����·���޹�˾.All rights reserved
* �ļ�����: debug_android.cpp
* ժ    Ҫ: debug  android�溯��
* ��ǰ�汾: 1.0
* ��    ��: �δ���
* �������: 2014-6-3
************************************************************************************************/
#include "../inc/basic.h"

#ifdef __ANDROID

__NS_BASIC_START

/**********************************************************************************************
* �������ܣ���lpszFormat�����debug���ںͱ�׼����ļ�
* ���������lpszFormat:��ʽ���ַ�����arglist:�����б�
* ���������
* ����ֵ  ��
**********************************************************************************************/
void WBasicTraceDebugView(LPCTSTR lpszString)
{
	BASICLOG_INFO_A("%s", lpszString);
}
void BasicTraceDebugView(const char* lpszString)
{
	BASICLOG_INFO_A("%s", lpszString);
}

__NS_BASIC_END

#endif
