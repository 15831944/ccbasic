
#include "../inc/basic.h"

#ifdef __LINUX

__NS_BASIC_START

/**********************************************************************************************
* �������ܣ���lpszFormat�����debug���ںͱ�׼����ļ�
* ���������lpszFormat:��ʽ���ַ�����arglist:�����б�
* ���������
* ����ֵ  ��
**********************************************************************************************/

void WBasicTraceDebugView(LPCTSTR lpszString)
{
	printf(lpszString);
}
void BasicTraceDebugView(const char* lpszString)
{
	printf(lpszString);
}

__NS_BASIC_END

#endif
