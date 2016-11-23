/////////////////////////////////////////////////////////////////////////////////////
// �ļ���:    cachemgr.h
// ������:    �¸�
// ����ʱ��:  2009-03-06
// ��������:  ������CCacheMgr�� ʵ��һ��Cacheģ��
//		2009-03-22 ���⣺ ����һЩ�޸ġ�
/////////////////////////////////////////////////////////////////////////////////////

#ifndef INC_CACHEMGR_H
#define INC_CACHEMGR_H

#include "../../inc/basic.h"

//����ʹ�������cachemgr�Ļ�����
class CBaseCacheDataDefine : public basiclib::EnableRefPtr<CBaseCacheDataDefine>
{
public:
    CBaseCacheDataDefine(DWORD dwCacheDataType){m_dwCacheDataType = dwCacheDataType;}
    virtual ~CBaseCacheDataDefine(){}

    //��ȡ���͵�cache������
    DWORD GetCacheDataType(){return m_dwCacheDataType;}
protected:
    DWORD   m_dwCacheDataType;
};

typedef basiclib::CBasicRefPtr<CBaseCacheDataDefine>      CBaseCacheDataDefineRef;


template <class T>
class CacheDataInfo
{
public:
	CacheDataInfo()	{}
	CacheDataInfo(T* pData, UINT nLiveTime) 
	{
		Initialize(pData, nLiveTime);
	}
	virtual ~CacheDataInfo()
	{	
		ClearData();
	}
	void Initialize(T* pData, UINT nLiveTime)
	{
        m_pCacheData = basiclib::CBasicRefPtr<T>(pData);
		m_nInvalidTime = time(0) + nLiveTime;
	}
	// �ж��Ƿ�ʧЧ
	BOOL IsValidable(time_t now = 0)
	{
		if (0 == now)
			now = time(NULL);
		return m_nInvalidTime >= now;
	}
	basiclib::CBasicRefPtr<T> GetData()
	{
		if (IsValidable())
		{
			return m_pCacheData;
		}
		else
		{
			return NULL;
		}
	}

	// do not call this function
	basiclib::CBasicRefPtr<T> __GetDataDirect()
	{
		return m_pCacheData;
	}

	void ClearData()
	{
		m_pCacheData = NULL;
	}
private:
	// ʧЧʱ��
	time_t	m_nInvalidTime;
	// CTLRef����
	basiclib::CBasicRefPtr<T> m_pCacheData;
};

template<class T, class KeyT, class KeyT2>
class CCacheMgr
{
public:
	typedef CacheDataInfo<T>	CacheType;
	typedef typename basiclib::basic_map<KeyT2, CacheType*>::type DataContainer;
	typedef typename DataContainer::iterator DataContainerIterator;

	CCacheMgr(UINT nWatchTime = 60, const std::function<void(basiclib::CBasicRefPtr<T>)>* pCacheMgrDeleteCallback = nullptr);
	virtual ~CCacheMgr();

	// �����ַ���Key ȡCacheData
	basiclib::CBasicRefPtr<T> GetData(KeyT lpszKey);
    // ����key������T,pop���
    basiclib::CBasicRefPtr<T> PopData(KeyT lpszKey);

	// ��CacheData����Map��
	BOOL	SetData(KeyT lpszKey, T* pData, UINT nLiveTime, BOOL bReplate = TRUE);

	// ��ȡ�ܹ�����Ĵ���
	UINT	GetTotalReq() const;

	// ������еĻ���
	void	RemoveAll();

	// ����CacheDataMap ɾ��ʧЧ��Cache
	void	RemoveInvalid();
protected:
	// ���ַ���ΪKey��CacheMap
	DataContainer	m_conDataContainer;
	
	// �����������
	UINT	m_nTotalReq;

	class CWatchThread;
	// ��ѯ�߳�
	CWatchThread*               m_pWatchThread;
    basiclib::CCriticalSection	m_csLock;
	std::function<void(basiclib::CBasicRefPtr<T>)>    m_pDeleteCallback;
};


template<class T, class KeyT, class KeyT2>
CCacheMgr<T, KeyT, KeyT2>::CCacheMgr(UINT nWatchTime, const std::function<void(basiclib::CBasicRefPtr<T>)>* pCacheMgrDeleteCallback = nullptr)
{
    m_nTotalReq = 0;
    m_pWatchThread = new CWatchThread;
    m_pWatchThread->Initialize(this, nWatchTime);
    m_pWatchThread->Start();
	if (pCacheMgrDeleteCallback)
		m_pDeleteCallback = *pCacheMgrDeleteCallback;
}

template<class T, class KeyT, class KeyT2>
CCacheMgr<T, KeyT, KeyT2>::~CCacheMgr()
{
    m_pWatchThread->Stop();
    m_pWatchThread->Join();
    delete m_pWatchThread;
    RemoveAll();
}

// �����ַ���Key ȡCacheData
template<class T, class KeyT, class KeyT2>
basiclib::CBasicRefPtr<T> CCacheMgr<T, KeyT, KeyT2>::GetData(KeyT lpszKey)
{
    basiclib::CSingleLock lock(&m_csLock);
    lock.Lock();

    ++ m_nTotalReq;
    DataContainerIterator it = m_conDataContainer.find(lpszKey);
    if (it != m_conDataContainer.end())
    {
        CacheType *pCacheDataInfo = it->second;
        if (pCacheDataInfo != NULL)
        {
            return pCacheDataInfo->GetData();
        }
    }
    return NULL;
}

// ����key������T,pop���
template<class T, class KeyT, class KeyT2>
basiclib::CBasicRefPtr<T> CCacheMgr<T, KeyT, KeyT2>::PopData(KeyT lpszKey)
{
    basiclib::CSingleLock lock(&m_csLock);
    lock.Lock();

	basiclib::CBasicRefPtr<T> pRet = NULL;
    ++ m_nTotalReq;
    DataContainerIterator it = m_conDataContainer.find(lpszKey);
    if (it != m_conDataContainer.end())
    {
        CacheType *pCacheDataInfo = it->second;
        m_conDataContainer.erase(it);
        if (pCacheDataInfo != NULL)
        {
			pRet = pCacheDataInfo->GetData();
			BASIC_DeleteObject(pCacheDataInfo);
			return pRet;
        }
    }
	return pRet;
}

// ��CacheData����Map��
template<class T, class KeyT, class KeyT2>
BOOL CCacheMgr<T, KeyT, KeyT2>::SetData(KeyT lpszKey, T* pData, UINT nLiveTime, BOOL bReplate)
{	
    basiclib::CSingleLock lock(&m_csLock);
    lock.Lock();

    CacheType *pCacheDataInfo = NULL;
    DataContainerIterator it = m_conDataContainer.find(lpszKey);
    if (it != m_conDataContainer.end())
    {
        if (bReplate)
        {
            pCacheDataInfo = it->second;
        }
        else
        {
            //�����滻��ֱ�ӷ��ش���
            return FALSE;
        }
    }
    else
    {
        pCacheDataInfo = basiclib::Basic_NewObject<CacheType>();
        m_conDataContainer[lpszKey] = pCacheDataInfo;
    }

    if (pCacheDataInfo)
    {
        pCacheDataInfo->Initialize(pData, nLiveTime);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// ��ȡ�ܹ�����Ĵ���
template<class T, class KeyT, class KeyT2>
UINT CCacheMgr<T, KeyT, KeyT2>::GetTotalReq() const
{
    return m_nTotalReq;
}

template<class T, class KeyT, class KeyT2>
void CCacheMgr<T, KeyT, KeyT2>::RemoveAll()
{
    basiclib::CSingleLock lock(&m_csLock);
    lock.Lock();
    for_each(m_conDataContainer.begin(), m_conDataContainer.end(), basiclib::DeleteObject());

    m_conDataContainer.clear();
}

template<class T, class KeyT, class KeyT2>
void CCacheMgr<T, KeyT, KeyT2>::RemoveInvalid()
{
    basiclib::CSingleLock lock(&m_csLock);
    lock.Lock();

    time_t tNowTime = time(NULL);
    CacheType *pCacheDataInfo = NULL;
    for (DataContainerIterator it = m_conDataContainer.begin(); it != m_conDataContainer.end();)
    {
        pCacheDataInfo = it->second;
        if (pCacheDataInfo != NULL && !pCacheDataInfo->IsValidable(tNowTime))
        {
            //�ص�
            if (m_pDeleteCallback)
            {
                m_pDeleteCallback(pCacheDataInfo->__GetDataDirect());
            }
            // ����Ч
            m_conDataContainer.erase(it++);
            BASIC_DeleteObject(pCacheDataInfo);
        }
        else
        {
            ++it;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class T, class KeyT, class KeyT2>
class CCacheMgr<T, KeyT, KeyT2>::CWatchThread :public basiclib::CBasic_Thread
{
public:
	CWatchThread();
	CWatchThread(CCacheMgr<T, KeyT, KeyT2>* pMgr, UINT nTime = 60);
	virtual ~CWatchThread();

	void Initialize(CCacheMgr<T, KeyT, KeyT2>* pMgr, UINT nTime = 60);
	virtual void Stop();
	virtual void Run();
protected:
	CCacheMgr<T, KeyT, KeyT2>	*m_pMgr;
	BOOL			            m_bRunning;
	UINT			            m_nTime;			// ÿ��m_nTime�� ��ѯһ��
};

template <class T, class KeyT, class KeyT2>
CCacheMgr<T, KeyT, KeyT2>::CWatchThread::CWatchThread()
{

}

template <class T, class KeyT, class KeyT2>
CCacheMgr<T, KeyT, KeyT2>::CWatchThread::CWatchThread(CCacheMgr<T, KeyT, KeyT2>* pMgr, UINT nTime)
{
	Initialize(pMgr, nTime);
}

template <class T, class KeyT, class KeyT2>
CCacheMgr<T, KeyT, KeyT2>::CWatchThread::~CWatchThread()
{

}

template <class T, class KeyT, class KeyT2>
void CCacheMgr<T, KeyT, KeyT2>::CWatchThread::Initialize(CCacheMgr<T, KeyT, KeyT2>* pMgr, UINT nTime)
{
	m_pMgr	= pMgr;
	m_nTime = nTime;
	m_bRunning = TRUE;
}

template <class T, class KeyT, class KeyT2>
void CCacheMgr<T, KeyT, KeyT2>::CWatchThread::Run()
{
	while (m_bRunning)
	{
		for (UINT i = 0; i < m_nTime && m_bRunning; i++)
		{
            basiclib::BasicSleep(1000);
		}
		if (m_pMgr != NULL && m_bRunning)
		{
			m_pMgr->RemoveInvalid();
		}
	}
}

template <class T, class KeyT, class KeyT2>
void CCacheMgr<T, KeyT, KeyT2>::CWatchThread::Stop()
{
	m_bRunning = FALSE;
}
#endif
