/***********************************************************************************************
// �ļ���:     basic_def.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 8:20:48
// ��������:   ���뻷��������
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_BASIC_DEF_H
#define BASIC_BASIC_DEF_H

#ifdef __BASICWINDOWS
#define __WINDOWS		// windowsƽ̨
#ifndef _WIN64
#ifndef _WIN32
#define _WIN32
#endif
#ifndef WIN32
#define WIN32
#endif
#endif
#elif defined (__MAC)
//#define _DARWIN_C_SOURCE			// macosƽ̨
#elif defined(__ANDROID)
#else
#ifndef __LINUX
#define __LINUX			// linuxƽ̨
#endif
#endif

#ifdef __GNUC__
#define __GCC		// gcc������
#elif defined(_MSC_VER)
#define __MSVC		// MSVC������
#if _MSC_VER >= 1600
#define USING_MORE_10
#endif
#endif

#ifdef BASIC_DLL_EXPORTS	//��̬��汾
#ifdef __BASICWINDOWS
#define _BASIC_DLL_API 	__declspec(dllexport)
#endif
#endif	//BASIC_DLL_EXPORTS

#ifdef BASIC_DLL_IMPORTS	//��̬��汾��Ӧ�ó�����Ҫ����ĺ꣩
#ifdef __BASICWINDOWS
#define _BASIC_DLL_API __declspec(dllimport)
#else
#ifdef BASIC_DLL_IMPORTS_C
#define _BASIC_DLL_API	__declspec(dllimport)
#endif
#endif
#endif	//BASIC_DLL_IMPORTS

#ifndef _BASIC_DLL_API
#ifdef basicdll_EXPORTS
#ifdef __BASICWINDOWS
#define _BASIC_DLL_API 	__declspec(dllexport)
#else
#define _BASIC_DLL_API
#endif
#else
#define _BASIC_DLL_API
#endif
#endif

#define _BASIC_DLL_API_C extern "C"

//���������ռ�
#define __NS_BASIC_START	namespace basiclib{
#define __NS_BASIC_END	}

#define DEFAULT_ALLOCATOR	basicallocator

#define _NO_TRY			//��ʹ�� try catch

#ifdef _NO_TRY
#define _basic_try
#else
#define _basic_try  try
#endif

#endif 
