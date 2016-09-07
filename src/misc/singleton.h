/***********************************************************************************************
// �ļ���:     singleton.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 12:06:43
// ��������:   ʵ�����ģʽ Singleton ����һ�ָĽ���ȫ�ֱ�����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_SINGLETON_H
#define BASIC_SINGLETON_H

#pragma once
__NS_BASIC_START
//////////////////////////////////////////////////////////////////////////////////////////////////
//����
#define ST_LIFETIME_LIFO		0		//last-in, first-out (LIFO) order
#define ST_LIFETIME_NODESTROY	-1		//�����ٶ���

template<class T, int nLifetime = ST_LIFETIME_LIFO>
class CBasicSingleton 
{
public:
	static T& Instance() // Unique point of access
	{ 
		if (0 == _instance) 
		{
			CSingleLock lock(&GetMutex());
			lock.Lock();
			if (0 == _instance) 
			{
				_instance = new T();
				if(nLifetime == ST_LIFETIME_LIFO)
				{
					atexit(Destroy);  //ע�����ٺ�����ʵ�� LIFO
				}
			}
		}
		return *_instance;
	}
	static T* GetInstance()
	{
		return _instance;
	}
protected:
	CBasicSingleton(){}
	~CBasicSingleton(){}
private:
	CBasicSingleton(const CBasicSingleton&);
	CBasicSingleton& operator=(const CBasicSingleton&);
private:
	static void Destroy() // Destroy the only instance
	{ 
		if ( _instance != 0 ) 
		{
			delete _instance;
			_instance = 0;
		}
	}
	static CCriticalSection& GetMutex()
	{
		static CCriticalSection _mutex;
		return _mutex;
	}
	
	static T * volatile _instance;				// The one and only instance
};

//template<class T, int nLifetime> CCriticalSection CBasicSingleton<T, nLifetime>::_mutex;
template<class T, int nLifetime> T * volatile CBasicSingleton<T, nLifetime>::_instance = 0;

__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////////
#endif 
