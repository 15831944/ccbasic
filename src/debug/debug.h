/***********************************************************************************************
// �ļ���:     debug.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 8:33:33
// ��������:   
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_DEBUG_H
#define BASIC_DEBUG_H

/////////////////////////////////////////////////////////////////////////////////////////////
//! ���ԣ�DEBUG����Ч��RELEASE����Ч
#ifndef ASSERT

#ifdef _DEBUG

#ifdef __BASICWINDOWS

#define ASSERT(f) \
	do \
	{ \
	if (!(f) && basiclib::BasicAssertFailedLine(__FILE__, __LINE__)) \
	basiclib::BasicDebugBreak(); \
	} while (0) \

#endif	//__BASICWINDOWS

#if (defined(__LINUX) || defined(__MAC) || defined(__ANDROID))
#include <assert.h>
#define ASSERT assert
#endif //__LINUX

#else	//_DEBUG

#define ASSERT(f)          ((void)0)

#endif //_DEBUG

#endif //ASSERT

#ifdef __ANDROID
#include <android/log.h>
#endif //__ANDROID

__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//! ���ԣ�DEBUG����Ч��RELEASE����Զ���
#ifndef VERIFY
#ifdef _DEBUG
#define VERIFY(f)          ASSERT(f)
#else
#define VERIFY(f)          ((void)(f))
#endif
#endif //VERIFY

/////////////////////////////////////////////////////////////////////////////////////////////
//����ĺ궨�� ���� debug �汾��������Ҫ��������������ܵİ汾����Ҫ����� _BASIC_TRACE����
#if defined(_DEBUG) || defined(_BASIC_TRACE)
#define __basic_trace
#endif
/////////////////////////////////////////////////////////////////////////////////////////////
//! ���������Ϣ
#ifndef TRACE
#ifdef __basic_trace
#define WTRACE              basiclib::WBasicTrace
#define TRACE              basiclib::BasicTrace
#else
#define WTRACE              
#define TRACE
#endif //__basic_trace
#endif //TRACE

#ifdef __ANDROID
#define LOG_TAG "C++"
#ifdef _DEBUG
#define BASICLOG_WARN_A(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define BASICLOG_INFO_A(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define BASICLOG_ERROR_A(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define BASICLOG_FATAL_A(...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__)
#else
#define BASICLOG_WARN_A(...)
#define BASICLOG_INFO_A(...)
#define BASICLOG_ERROR_A(...)
#define BASICLOG_FATAL_A(...)
#endif	// __DEBUG __android_log_write
#endif////__ANDROID

//!��λ���Դ�����
BOOL BasicAssertFailedLine(const char* lpszFileName, int nLine);

//!�ж�
void BasicDebugBreak();

//!���������Ϣ
void BasicTrace(const char* lpszFormat, ...);

//!���������Ϣ��������
void BasicTraceDebugView(const char* lpszString);

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
__NS_BASIC_END

#endif 
