/***********************************************************************************************
// �ļ���:     thread.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 12:01:19
// ��������:   �̲߳���ģ��
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_THREAD_H
#define BASIC_THREAD_H

__NS_BASIC_START
//////////////////////////////////////////////////////////////////////////////////////////
//!�̵߳ĺ�������ģ��
//!
//�̹߳�����������ֵ����
#ifdef __BASICWINDOWS
#define THREAD_RETURN DWORD		//!< Windows�̺߳�������ֵ
#else
#define THREAD_RETURN void*		//!< Linux�̺߳�������ֵ
#endif

//!
//�̹߳�����������
//�磺THREAD_RETURN DemoProc(void* pParam);
typedef THREAD_RETURN ( *PBASIC_THREAD_START_ROUTINE)( void* lpThreadParameter );
typedef PBASIC_THREAD_START_ROUTINE LPBASIC_THREAD_START_ROUTINE;

/*!
* �����߳�
* \param ��������ָ�롢������������
* \return ���� �߳�ID
*/
HANDLE BasicCreateThread(LPBASIC_THREAD_START_ROUTINE lpStartAddress,  void* lpParameter, LPDWORD lpThreadId);

/*!
* �ȴ��߳��˳�
* \param hThread �߳̾��
* \param dwWaitTime �ȴ��˳���ʱ��(����)
* \return �����Ч���߳ɹ��˳�����true����ʱ����false
*/
BOOL BasicWaitThread(HANDLE hThread,	DWORD  dwWaitTime = INFINITE);

/*!
* ǿ�н����߳�
* \param hThread �߳̾��
* \return ��
*/
void BasicTerminateThread(HANDLE hThread);

/*!
* ȡ���߳�ID
* \return �߳�ID
*/
DWORD BasicGetCurrentThreadId();

/*!
* ȡ���߳�
* \return �߳�
*/
HANDLE Basic_GetCurrentThread();

/*!
* ȡ�ý���ID
* \return ����ID
*/
DWORD Basic_GetCurrentProcessId();

//! ȡ������ID ����0ʱȡ��ǰ���̸�����
/*!
* 
*/
DWORD Basic_GetParentProcessID(DWORD dwProcessID = 0);


/*!
* ����
* \param dwMilliseconds ����ʱ�䣬��λ����
* \return ��
*/
_BASIC_DLL_API_C  _BASIC_DLL_API void BasicSleep( DWORD dwMilliseconds );

//////////////////////////////////////////////////////////////////////////////////////////
//!�̵߳������ģ��

#if defined(__LINUX) || defined(__MAC) || defined(__ANDROID)
#define HAVE_PTHREAD_H
#endif

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#else
#ifdef __BASICWINDOWS
#include <windows.h>
#include <process.h>
#endif
#endif

#if defined HAVE_PTHREAD_H
#define CRFPP_USE_THREAD 1
#endif

#if(defined(_WIN32) && ! defined (__CYGWIN__))
#define CRFPP_USE_THREAD 1
#define BEGINTHREAD(src, stack, func, arg, flag, id) \
     (HANDLE)_beginthreadex((void *)(src),(unsigned)(stack), \
                       (unsigned(_stdcall *)(void *))(func),(void *)(arg), \
                       (unsigned)(flag),(unsigned *)(id))
#endif

//!
//�߳���
class CBasic_Thread : public CBasicObject
{
public:

	//! �̺߳���
	/*! 
	*/
	static THREAD_RETURN Wrapper(void *ptr);

	//! �̹߳��̣�����ʵ��
	/*! 
	*/
    virtual void Run() {}

	//! �����߳�
	/*! 
	*/
    void Start();

	//! �ȴ��߳̽���
	/*! 
	*/
	void Join();

    virtual ~CBasic_Thread() {};

private:
#ifdef HAVE_PTHREAD_H
	pthread_t hnd_;		//!< Linux�߳�����
#else
#ifdef _WIN32
	HANDLE  hnd_;		//!< Windows�߳̾��
#endif
#endif

};

__NS_BASIC_END

#endif 
