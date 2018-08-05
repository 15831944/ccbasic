/***********************************************************************************************
// �ļ���:     extern.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 9:15:06
// ��������:   MFC like����չ�����ࡣ
��Ҫ����ʹ��MFC�Ĵ�����ֲ�����Ƽ�ʹ�á�
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_EXTERN_H
#define BASIC_EXTERN_H

#pragma once


#include "../inc/basic_def.h"
#include "array.h"
#include "basic_string.h"

__NS_BASIC_START

typedef CArray<unsigned char, unsigned char>	CByteArray;
typedef CArray<unsigned short, unsigned short>	CWordArray;
typedef CArray<unsigned long, unsigned long>	CDWordArray;
typedef CArray<unsigned int, unsigned int>		CUintArray;
typedef CArray<void*, void*>					CPtrArray;
typedef CArray<CBasicString, const char*>		CBasicStringArray;

__NS_BASIC_END

#endif 
