
#include "../inc/basic.h"

#ifdef __MAC

__NS_BASIC_START

/**********************************************************************************************
* �������ܣ���lpszFormat�����debug����
* ���������lpszFormat:��ʽ���ַ�����arglist:�����б�
* ���������
* ����ֵ  ��
**********************************************************************************************/

void OutputDebugString(const char* lpszString)
{
    fprintf(stdout, lpszString);
    fflush(stdout);
}
#define OutputDebugStringA OutputDebugString;
void WBasicTraceDebugView(LPCTSTR lpszString)
{
	OutputDebugString(lpszString);
}
void BasicTraceDebugView(const char* lpszString)
{
	OutputDebugStringA(lpszString);
}

__NS_BASIC_END

#endif