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

class _BASIC_DLL_API CSemaphore : public CBasicSyncObject
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
struct _BASIC_DLL_API SpinLock
{
	int		m_nLock;
	DWORD	m_lockThreadID;
	SpinLock()
	{
		m_nLock = 0;
		m_lockThreadID = 0;
	}
};
class _BASIC_DLL_API CSpinLockFuncNoSameThreadSafe
{
public:
	CSpinLockFuncNoSameThreadSafe(SpinLock* pLock, BOOL bInitialLock = FALSE);
	virtual ~CSpinLockFuncNoSameThreadSafe();

	virtual void Lock();
	virtual void LockAndSleep(unsigned short usSleep = 100);
	virtual bool LockNoWait();
	virtual void UnLock();
	bool IsLock();
protected:
	SpinLock* 		m_pLock;
	bool			m_bAcquired;
};

class _BASIC_DLL_API CSpinLockFunc : public CSpinLockFuncNoSameThreadSafe
{
public:
	CSpinLockFunc(SpinLock* pLock, BOOL bInitialLock = FALSE) : CSpinLockFuncNoSameThreadSafe(pLock, bInitialLock){
	}
	virtual ~CSpinLockFunc(){
	}

	virtual void Lock();
	virtual void LockAndSleep(unsigned short usSleep = 100);
	virtual bool LockNoWait();
	virtual void UnLock();
};

//spinlock��mutex��ϵ�lock
class CSpinLockAndMutex : public basiclib::CBasicObject
{
public:
	CSpinLockAndMutex();
	virtual ~CSpinLockAndMutex();

protected:
	SpinLock	m_spinLock;
	CMutex		m_synLock;
};

#ifdef __BASICWINDOWS
//������vista�Լ�server 2008������ϵͳ
#define RWLOCK_VAR					SRWLOCK
#define INIT_RWLOCK_VAR(v)			InitializeSRWLock(&(v))
#define ENTER_READ_LOCK_VAR(v)		AcquireSRWLockShared(&(v))
#define LEAVE_READ_LOCK_VAR(v)		ReleaseSRWLockShared(&(v))
#define ENTER_WRITE_LOCK_VAR(v)		AcquireSRWLockExclusive(&(v))
#define LEAVE_WRITE_LOCK_VAR(v)		ReleaseSRWLockExclusive(&(v))
#define DELETE_RWLOCK_VAR(v)  
#else
#define RWLOCK_VAR					pthread_rwlock_t
#define INIT_RWLOCK_VAR(v)			pthread_rwlock_init(&(v), NULL)
#define ENTER_READ_LOCK_VAR(v)		pthread_rwlock_rdlock(&(v))
#define LEAVE_READ_LOCK_VAR(v)		pthread_rwlock_unlock(&(v))
#define ENTER_WRITE_LOCK_VAR(v)		pthread_rwlock_wrlock(&(v))
#define LEAVE_WRITE_LOCK_VAR(v)		pthread_rwlock_unlock(&(v))
#define DELETE_RWLOCK_VAR(v)		pthread_rwlock_destroy(&(v))
#endif 

struct _BASIC_DLL_API RWLock
{
	RWLOCK_VAR	lock;
	RWLock()
	{
		INIT_RWLOCK_VAR(lock);
	}
	~RWLock()
	{
		DELETE_RWLOCK_VAR(lock);
	}
};
//RWLock
class _BASIC_DLL_API CRWLockFunc
{
public:
	CRWLockFunc(RWLock* lock, bool bInitialWLock = false, bool bInitialRLock = false)
	{
		m_pLock = lock;
		m_bAcquiredRead = false;
		m_bAcquiredWrite = false;
		if (bInitialWLock)
			LockWrite();
		if (bInitialRLock)
			LockRead();
	}
	virtual ~CRWLockFunc()
	{
		UnLockRead();
		UnLockWrite();
	}

	void LockRead()
	{
		if (!m_bAcquiredRead)
		{
			ENTER_READ_LOCK_VAR(m_pLock->lock);
			m_bAcquiredRead = true;
		}
	}
	void UnLockRead()
	{
		if (m_bAcquiredRead)
		{
			LEAVE_READ_LOCK_VAR(m_pLock->lock);
			m_bAcquiredRead = false;
		}
	}
	void LockWrite()
	{
		if (!m_bAcquiredWrite)
		{
			ENTER_WRITE_LOCK_VAR(m_pLock->lock);
			m_bAcquiredWrite = true;
		}
	}
	void UnLockWrite()
	{
		if (m_bAcquiredWrite)
		{
			LEAVE_WRITE_LOCK_VAR(m_pLock->lock);
			m_bAcquiredWrite = false;
		}
	}

protected:
	RWLock* 		m_pLock;
	bool			m_bAcquiredRead;
	bool			m_bAcquiredWrite;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////

//! ԭ�Ӳ�������������
/*! 
*\param lpAddend ����ָ��
*\return *lpAddend + 1
*\remarks *lpAddend = *lpAddend + 1
*/
_BASIC_DLL_API LONG BasicInterlockedIncrement(LONG volatile *lpAddend);

//! ԭ�Ӳ����������ݼ�
/*! 
*\param lpAddend ����ָ��
*\return *lpAddend - 1
*\remarks *lpAddend = *lpAddend - 1
*/
_BASIC_DLL_API LONG BasicInterlockedDecrement(LONG volatile *lpAddend);

//! ԭ�Ӳ�����������ֵ
/*! 
*\param Target	Ŀ�����
*\param Value	��ֵ����
*\return resValue
*\remarks *Target = Value
*/
_BASIC_DLL_API LONG BasicInterlockedExchange(LONG volatile *Target, LONG Value);

//! ԭ�Ӳ������������
/*! 
*\param Addend	Ŀ�����
*\param Value	�Ӳ�������
*\return *Addend��ʼֵ
*\remarks *Addend = *Addend + Value
*/
_BASIC_DLL_API LONG BasicInterlockedExchangeAdd(LONG volatile *Addend, LONG Value);
_BASIC_DLL_API LONG BasicInterlockedExchangeSub(LONG volatile *Addend, LONG Value);
//! ԭ�Ӳ����������Ƚ�
/*! 
*\param Destination	Ŀ�����
*\param Exchange	��ֵ����
*\param Comperand	�Ƚϱ���
*\return *Destination��ʼֵ
*\remarks ��Comperand==*Destination,��ִ��*Destination=Exchange
*/
_BASIC_DLL_API LONG BasicInterlockedCompareExchange(LONG volatile *Destination, LONG Exchange, LONG Comperand);

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
_BASIC_DLL_API HANDLE BasicCreateEvent(BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName);

//! �����¼��ź�
/*! 
*\param hEvent	�¼�������
*\return ������ɹ����ط���ֵ�����򷵻�0
*/
_BASIC_DLL_API BOOL BasicSetEvent(HANDLE hEvent);

//! �����¼������ź�״̬
/*! 
*\param hEvent	�¼�������
*\return ������ɹ����ط���ֵ�����򷵻�0
*/
_BASIC_DLL_API BOOL BasicResetEvent(HANDLE hEvent);

//! �����¼��ź�
/*! 
*\param hEvent	�¼�������
*\return ������ɹ����ط���ֵ�����򷵻�0
*/
_BASIC_DLL_API BOOL BasicDestoryEvent(HANDLE hEvent);

//! ���ٶ�����
/*! 
*\param hObject	������
*\return ������ɹ����ط���ֵ�����򷵻�0
*/
_BASIC_DLL_API BOOL BasicCloseHandle(HANDLE hObject);


//! �ȴ��¼��ź�
/*! 
*\param hHandle	������
*\param dwMilliseconds ��ʱʱ�䣬��λ���롣-1Ϊ����ʱ��
*\return ���ź�״̬����WAIT_OBJECT_0����ʱ����WAIT_TIMEOUT
*/
_BASIC_DLL_API DWORD BasicWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);

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
