
#include "../inc/basic.h"

#ifdef __BASICWINDOWS

__NS_BASIC_START

/**********************************************************************************************
* �������ܣ���lpszFormat�����debug���ںͱ�׼����ļ�
* ���������lpszFormat:��ʽ���ַ�����arglist:�����б�
* ���������
* ����ֵ  ��
**********************************************************************************************/

void WBasicTraceDebugView(LPCTSTR lpszString)
{
	OutputDebugString(lpszString);
	wprintf(lpszString);
}
void BasicTraceDebugView(const char* lpszString)
{
	OutputDebugStringA(lpszString);
	printf(lpszString);
}

__NS_BASIC_END

#endif