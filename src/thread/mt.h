/***********************************************************************************************
// �ļ���:     mt.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 12:00:18
// ��������:   ͬ���������ļ�
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_MT_H
#define BASIC_MT_H

/////////////////////////////////////////////////////////////////////////////
__NS_BASIC_START

//class CBasicObject;
	class CBasicSyncObject;
		class CSemaphore;
		class CMutex;
		class CEvent;
		class CCriticalSection;

class CSingleLock;

/////////////////////////////////////////////////////////////////////////////
//! ͬ���������
/*! 
*  
*/

class _BASIC_DLL_API CBasicSyncObject : public CBasicObject
{

// Constructor
public:
	CBasicSyncObject(LPCTSTR pstrName);

// Attributes
public:
	operator HANDLE() const;
	HANDLE  m_hObject;

	//! ����
	/*! 
	*\param dwTimeOut ��ʱʱ��,��λ���롣INFINITE��ʾ���賬ʱʱ��
	*\return TRUE�����ɹ� FALSE����ʧ��
	*/
	virtual BOOL Lock(DWORD dwTimeout = INFINITE);

	//! ����
	/*! 
	*\return TRUE�����ɹ� FALSE����ʧ��
	*/
	virtual BOOL Unlock() = 0;

// Implementation
public:
	virtual ~CBasicSyncObject();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif
	friend class CSingleLock;
};

/////////////////////////////////////////////////////////////////////////////
//! �ź�����
/*! 
*  
*/

class CSemaphore : public CBasicSyncObject
{

// Constructor
public:
	CSemaphore(LONG lInitialCount = 1, LONG lMaxCount = 1,
		LPCTSTR pstrName=NULL);

// Implementation
public:
	virtual ~CSemaphore();
	virtual BOOL Unlock();
	virtual BOOL Unlock(LONG lCount, LPLONG lprevCount = NULL);
};

/////////////////////////////////////////////////////////////////////////////
//! ��������
/*! 
*  
*/

class _BASIC_DLL_API CMutex : public CBasicSyncObject
{

// Constructor
public:
	CMutex(BOOL bInitiallyOwn = FALSE, LPCTSTR lpszName = NULL);

#if defined(__LINUX) || defined(__MAC) || defined(__ANDROID)
	virtual BOOL Lock(DWORD dwTimeout = INFINITE);
#endif
	
// Implementation
public:
	virtual ~CMutex();
	BOOL Unlock();
};

/////////////////////////////////////////////////////////////////////////////
//! �¼���
/*! 
*  
*/

class _BASIC_DLL_API CEvent : public CBasicSyncObject
{

// Constructor
public:
	CEvent(BOOL bInitiallyOwn = FALSE, BOOL bManualReset = FALSE,
		LPCTSTR lpszNAme = NULL);

// Operations
public:
	BOOL SetEvent();
	BOOL PulseEvent();
	BOOL ResetEvent();
	BOOL Unlock();

// Implementation
public:
	virtual ~CEvent();
};

/////////////////////////////////////////////////////////////////////////////
// CCriticalSection
#if defined(__LINUX) || defined(__MAC) || defined(__ANDROID)
typedef struct _RBASIC_CRITICAL_SECTION {

	DWORD OwningThread;			//!< ��¼�߳�ID
	HANDLE LockSemaphore;		//!< �ź�������

} RBASIC_CRITICAL_SECTION, *PRBASIC_CRITICAL_SECTION;

typedef RBASIC_CRITICAL_SECTION CRITICAL_SECTION;
typedef PRBASIC_CRITICAL_SECTION PCRITICAL_SECTION;
typedef PRBASIC_CRITICAL_SECTION LPCRITICAL_SECTION;

#endif

//! �ٽ�����
/*! 
*  
*/
class _BASIC_DLL_API CCriticalSection : public CBasicSyncObject
{

// Constructor
public:
	CCriticalSection();

// Attributes
public:
	operator CRITICAL_SECTION*();
	CRITICAL_SECTION m_sect;

// Operations
public:
	BOOL Unlock();

	//! ����
	/*! 
	*\return TRUE�����ɹ� FALSE����ʧ��
	*/
	BOOL Lock();

	//! ����
	/*! 
	*\param dwTimeOut ��ʱʱ��
	*\return TRUE�����ɹ� FALSE����ʧ��
	*/
	BOOL Lock(DWORD dwTimeout);

// Implementation
public:
	virtual ~CCriticalSection();
};

/////////////////////////////////////////////////////////////////////////////
//! ��������
/*! 
*  
*/

class _BASIC_DLL_API CSingleLock
{
// Constructors
public:
	CSingleLock(CBasicSyncObject* pObject, BOOL bInitialLock = FALSE);

// Operations
public:
	//! ����
	/*! 
	*\param dwTimeOut ��ʱʱ��,��λ���롣INFINITE��ʾ���賬ʱʱ��
	*\return TRUE�����ɹ� FALSE����ʧ��
	*/
	BOOL Lock(DWORD dwTimeOut = INFINITE);

	//! ����
	/*! 
	*\return TRUE�����ɹ� FALSE����ʧ��
	*/
	BOOL Unlock();

// Implementation
public:
	~CSingleLock();

protected:
	CBasicSyncObject*	m_pObject;		//!< ͬ������
	BOOL			m_bAcquired;	//!< �Ƿ�ռ�ñ��
};

//////////////////////////////////////////////////////////////////////////
//������
struct SpinLock
{
	int m_nLock;
	SpinLock()
	{
		m_nLock = 0;
	}
};

class CSpinLockFunc
{
public:
	CSpinLockFunc(SpinLock& lock);
	virtual ~CSpinLockFunc();

	void Lock();
	void LockAndSleep(unsigned short usSleep = 100);
	void UnLock();
	bool IsLock();
protected:
	SpinLock* 		m_pLock;
	bool			m_bAcquired;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////

//! ԭ�Ӳ�������������
/*! 
*\param lpAddend ����ָ��
*\return *lpAddend + 1
*\remarks *lpAddend = *lpAddend + 1
*/
LONG BasicInterlockedIncrement (LONG volatile *lpAddend);

//! ԭ�Ӳ����������ݼ�
/*! 
*\param lpAddend ����ָ��
*\return *lpAddend - 1
*\remarks *lpAddend = *lpAddend - 1
*/
LONG BasicInterlockedDecrement (LONG volatile *lpAddend);

//! ԭ�Ӳ�����������ֵ
/*! 
*\param Target	Ŀ�����
*\param Value	��ֵ����
*\return Value
*\remarks *Target = Value
*/
LONG BasicInterlockedExchange (LONG volatile *Target, LONG Value);

//! ԭ�Ӳ������������
/*! 
*\param Addend	Ŀ�����
*\param Value	�Ӳ�������
*\return *Addend��ʼֵ
*\remarks *Addend = *Addend + Value
*/
LONG BasicInterlockedExchangeAdd (LONG volatile *Addend, LONG Value);
LONG BasicInterlockedExchangeSub (LONG volatile *Addend, LONG Value);
//! ԭ�Ӳ����������Ƚ�
/*! 
*\param Destination	Ŀ�����
*\param Exchange	��ֵ����
*\param Comperand	�Ƚϱ���
*\return *Destination��ʼֵ
*\remarks ��Comperand==*Destination,��ִ��*Destination=Exchange
*/
LONG BasicInterlockedCompareExchange (LONG volatile *Destination, LONG Exchange, LONG Comperand);

//! �����¼�����
/*! 
*\param bManualReset	��λ��ʽ
*  <ul>
*  <li> ��λ��ʽ
*     <ol>
*     <li> TRUE		������BasicResetEvent�ֹ��ָ������ź�״̬
*     <li> FALSE	�¼����ȴ��߳��ͷź��Զ��ָ������ź�״̬
*     </ol>
*  </ul>
*\param bInitialState	��ʼ״̬����TRUE��ʼΪ���ź�״̬������Ϊ���ź�״̬
*\param lpName			�¼���������������������
*\return �¼�����������ʧ�ܷ���NULL
*/
HANDLE BasicCreateEvent(BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName);

//! �����¼��ź�
/*! 
*\param hEvent	�¼�������
*\return ������ɹ����ط���ֵ�����򷵻�0
*/
BOOL BasicSetEvent(HANDLE hEvent);

//! �����¼������ź�״̬
/*! 
*\param hEvent	�¼�������
*\return ������ɹ����ط���ֵ�����򷵻�0
*/
BOOL BasicResetEvent(HANDLE hEvent);

//! �����¼��ź�
/*! 
*\param hEvent	�¼�������
*\return ������ɹ����ط���ֵ�����򷵻�0
*/
BOOL BasicDestoryEvent(HANDLE hEvent);

//! ���ٶ�����
/*! 
*\param hObject	������
*\return ������ɹ����ط���ֵ�����򷵻�0
*/
BOOL BasicCloseHandle(HANDLE hObject);


//! �ȴ��¼��ź�
/*! 
*\param hHandle	������
*\param dwMilliseconds ��ʱʱ�䣬��λ���롣-1Ϊ����ʱ��
*\return ���ź�״̬����WAIT_OBJECT_0����ʱ����WAIT_TIMEOUT
*/
DWORD BasicWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);

//�ź�״̬����ֵ����
#ifndef WAIT_OBJECT_0
#define STATUS_WAIT_0                    ((DWORD   )0x00000000L)
#define WAIT_OBJECT_0                    ((STATUS_WAIT_0 ) + 0 )	//!< ���ź�״̬����
#define WAIT_FAILED                     (DWORD)0xFFFFFFFF			//!< �ź�״̬ʧ��
#define STATUS_TIMEOUT                   ((DWORD   )0x00000102L)
#define WAIT_TIMEOUT                        STATUS_TIMEOUT			//!< ��ʱ����
#endif

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE (HANDLE)-1		//!< ��Ч������
#endif

__NS_BASIC_END

#endif 

/////////////////////////////////////////////////////////////////////////////
