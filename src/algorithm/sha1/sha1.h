/***********************************************************************************************
// �ļ���:     sha1.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2013-11-18 10:46:58
// ��������:   
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_SHA1_H
#define BASIC_SHA1_H

__NS_BASIC_START

_BASIC_DLL_API void SHA1_Perform(BYTE *indata, DWORD inlen, BYTE *outdata);
_BASIC_DLL_API void SHA1_Perform2(BYTE *indata, DWORD inlen, BYTE *indata2, DWORD inlen2, BYTE *outdata);

__NS_BASIC_END

#endif 
