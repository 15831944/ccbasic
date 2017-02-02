/***********************************************************************************************
// �ļ���:     filenotify.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 12:42:48
// ��������:   �����ļ�Ŀ¼��ص���
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_FILENOTIFY_H
#define BASIC_FILENOTIFY_H

//����ͷ�ļ�
#include <vector>
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma	pack(1)
__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//����
struct _FILE_NOTIFY_ITEM;					//֪ͨ�Ľṹ
struct _DIRECTORY_INFO;						//��ص�Ŀ¼

//class CBasicObject;
	class CBasicFileNotify;					//�ļ������
///////////////////////////////////////////////////////////////////////////////////////////////

//! ֪ͨ�Ľṹ
/*!
* 
*/
struct _FILE_NOTIFY_ITEM		//internal struct
{
	DWORD	m_dwAction;					/*!< ֪ͨ�����ͣ������� BASIC_FILE_NOTIFY_CHANGE_* */
	char	m_szFileName[MAX_PATH];		/*!< ֪ͨ���ļ���������·���� */
	//! ���캯��
	/*!  
	*/
	_FILE_NOTIFY_ITEM()
	{
		memset(this, 0, sizeof(_FILE_NOTIFY_ITEM));
	}
};

//! ֪ͨ�ṹ������
/*!
* 
*/
typedef  std::vector<_FILE_NOTIFY_ITEM>  _fni_array;

//! ����֪ͨ�Ļص�����
/*!
* 
*/
typedef long (*Change_Handler)(void* lpVoid, DWORD dwAction, const char* lpszFileName);
////////////////////////////////////////////////////////////////////////
#define DIW_SUBTREE		0x0001			//�����Ŀ¼

#define DIW_DEFAULT		DIW_SUBTREE

#define MAX_BUFFER			4096

//! ���Ŀ¼����Ϣ
/*!
* 
*/
struct _DIRECTORY_INFO		//internal struct
{
	HANDLE		m_hDir;							/*!< ��� */
	char		m_lpszDirName[MAX_PATH];		/*!< ��ص�Ŀ¼ */
	char		m_lpszFileName[64];				/*!< ��ص��ļ��� */
#ifdef __BASICWINDOWS
	CHAR        m_lpBuffer[MAX_BUFFER];			/*!< ��ɶ˿�ʹ�õĻ����� */
	DWORD       m_dwBufLength;					/*!< ��������С */
	OVERLAPPED  m_olOverlapped;					/*!< ��ɶ˿ڶ��� */
#endif
	time_t		m_tmLastChange;					/*!< �ļ�������޸�ʱ�� */

	DWORD		m_dwNotifyFilter;				/*!< ����ѡ�� BASIC_FILE_NOTIFY_CHANGE_* */
	DWORD		m_dwWathOptions;				/*!< ������� DIW_* */

	void*			m_lpVoid;					/*!< �û��Զ�������ݿ� */
	Change_Handler	m_pfuncHander;				/*!< �û��Զ���Ļص����� */
	
	_fni_array	m_ayFileInfo;					/*!< ֪ͨ���� */

	//! ���캯�� 
	/*!  
	*/
	_DIRECTORY_INFO()
	{
#ifdef __BASICWINDOWS
		int nSetLen = (int)(&((_DIRECTORY_INFO*)0)->m_ayFileInfo);
	#else
		int nSetLen = (char*)&m_ayFileInfo-(char*)this;
	#endif
		memset(this, 0, nSetLen);
	}
	
	//! �������� 
	/*!  
	*/
	~_DIRECTORY_INFO()
	{
#ifdef __BASICWINDOWS
		if (m_hDir != NULL && m_hDir != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hDir);		//I know this
		}
	#endif
	}
	//! �ж��Ƿ�Ҫ�����Ŀ¼
	/*!  
	*/
	BOOL IsWatchSubTree() { return (m_dwWathOptions & DIW_SUBTREE); }
};

typedef  std::vector<_DIRECTORY_INFO*>  _di_array;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BASIC_FILE_NOTIFY_CHANGE_FILE_NAME    0x00000001   /*!< ����ļ����ı仯��*/
#define BASIC_FILE_NOTIFY_CHANGE_DIR_NAME     0x00000002   /*!< ����ļ��еı仯��*/
#define BASIC_FILE_NOTIFY_CHANGE_ATTRIBUTES   0x00000004   /*!< ����ļ������ļ��е����Ա仯��*/
#define BASIC_FILE_NOTIFY_CHANGE_SIZE         0x00000008   /*!< ����ļ��Ĵ�С�仯��*/
#define BASIC_FILE_NOTIFY_CHANGE_LAST_WRITE   0x00000010   /*!< ����ļ��������д�ı仯��*/
#define BASIC_FILE_NOTIFY_CHANGE_LAST_ACCESS  0x00000020   /*!< ����ļ����������ʱ仯��*/
#define BASIC_FILE_NOTIFY_CHANGE_CREATION     0x00000040   /*!< ����ļ������ļ��Ĵ�����*/
#define BASIC_FILE_NOTIFY_CHANGE_SECURITY     0x00000100	/*!< ����ļ������ļ��İ�ȫ����仯��*/

#define INVALID_WATCH_ID		0


#define TFN_OPS_MODE		0x000F			//���ģʽ
#define TFN_OPS_COMPORT		0x0000			//ʹ����ɶ˿ڣ�ֻ����Windows��ʹ��
#define TFN_OPS_SCANFILE	0x0001			//ʹ��ɨ��Ŀ¼�ķ������

////////addtogroup0812

/** @addtogroup CLASS
 *  @{
*/ 
////////addtogroup0812

//! \brief �ļ����ļ��м����
/*!
* 
*/
#pragma warning (push)
#pragma warning (disable: 4251)

class CBasicFileNotify : public CBasicObject
{

public:
	//! ���캯�� 
	/*!  
	*\sa <a href = "sample\file_test\CBasicFileNotify_TEST.cpp">CWBasicFileObj_TEST.cpp</a> 
	*/
	CBasicFileNotify(DWORD dwOptions = TFN_OPS_COMPORT);
	
	//! �������� 
	/*!  
	*\sa <a href = "sample\file_test\CBasicFileNotify_TEST.cpp">CWBasicFileObj_TEST.cpp</a> 
	*/
	virtual ~CBasicFileNotify();

public:
	//! ���Ӽ��Ŀ¼ 
	/*! 
	*\param lpszPath ��ص�·��
	*\param dwNotifyFilter  ��ص�����
	*  <ul>��ص����� 
	*     <ol>
	*     <li> BASIC_FILE_NOTIFY_CHANGE_FILE_NAME		����ļ����ı仯��
	*     <li> BASIC_FILE_NOTIFY_CHANGE_DIR_NAME		����ļ��еı仯��
	*     <li> BASIC_FILE_NOTIFY_CHANGE_ATTRIBUTES		����ļ������ļ��е����Ա仯��
	*     <li> BASIC_FILE_NOTIFY_CHANGE_SIZE			����ļ��Ĵ�С�仯��
	*     <li> BASIC_FILE_NOTIFY_CHANGE_LAST_WRITE		����ļ��������д�ı仯��
	*     <li> BASIC_FILE_NOTIFY_CHANGE_LAST_ACCESS		����ļ����������ʱ仯��
	*     <li> BASIC_FILE_NOTIFY_CHANGE_CREATION		����ļ������ļ��Ĵ�����
	*     <li> BASIC_FILE_NOTIFY_CHANGE_SECURITY		����ļ������ļ��İ�ȫ����仯��
	*     </ol>
	*  </ul>
	*\param dwWatchOps �����Ƿ�����Ŀ¼ Ĭ���� DIW_SUBTREE
	*\param lpVoid  �û��Զ�������ݿ飬������������ع�����Ϊ���ݴ�����.
	*\param pfuncHander ������еĻص�������\n
		typedef long (*Change_Handler)(void* lpVoid, DWORD dwAction, const char* lpszFileName);
	*\return ���ؼ�ص�ID����Ҫ���֣�����һ�º����Ĳ���ʹ��
	*\sa <a href = "sample\file_test\CBasicFileNotify_TEST.cpp">CWBasicFileObj_TEST.cpp</a> 
	*/
	DWORD	AddWatch(const char* lpszPath, DWORD dwNotifyFilter, DWORD dwWatchOps = DIW_DEFAULT, LPVOID lpVoid = NULL, Change_Handler pfuncHander = NULL);
	
	//! ɾ��һ�������Ŀ 
	/*! 
	*\param dwWatchID 	���ID ��AddWatch�ķ���ֵ.
	*\return ����TRUE��ʾ����ɹ���FALSEʧ�ܣ�������ID������
	*/
	BOOL	RemoveWatch(DWORD dwWatchID);
	
	//! ֹͣ����������� 
	/*!  
	*/
	void	StopWatch();
	
	//! ����ض����е���Ŀ
	/*! 
	*\param dwWatchID 	���ID ��AddWatch�ķ���ֵ.
	*\return ���ض����м����Ŀ������ 
	*/
	long	CheckNotifyItem(DWORD dwWatchID);
	
	//! ��ռ�ض����е���Ŀ
	/*! 
	*\param dwWatchID 	���ID ��AddWatch�ķ���ֵ.
	*\return ���ض����м����Ŀ������ 
	*/
	void	ClearNotifyItem(DWORD dwWatchID);
	
	
	//! ��ò������һ�������Ŀ
	/*! 
	*\param dwWatchID 	���ID ��AddWatch�ķ���ֵ.
	*\return ��������ֵ
	*  <ul>���� windowsϵͳ
	*     <ol>
	*     <li> FILE_ACTION_ADDED		
	*     <li> FILE_ACTION_REMOVED		
	*     <li> FILE_ACTION_MODIFIED		
	*     <li> FILE_ACTION_RENAMED_OLD_NAME			
	*     <li> FILE_ACTION_RENAMED_NEW_NAME		 
	*     </ol>
	*  </ul> 
	*/
	DWORD	PopFirstNotifyItem(DWORD dwWatchID, char* lpszFileName, int nMaxLength);
	
	//! ��ò�������һ�������Ŀ
	/*! 
	*\param dwWatchID 	���ID ��AddWatch�ķ���ֵ.
	*\return ��������ֵ 
	*  <ul>���� windowsϵͳ
	*     <ol>
	*     <li> FILE_ACTION_ADDED		����ļ����ı仯��
	*     <li> FILE_ACTION_REMOVED		����ļ��еı仯��
	*     <li> FILE_ACTION_MODIFIED		����ļ������ļ��е����Ա仯��
	*     <li> FILE_ACTION_RENAMED_OLD_NAME			����ļ��Ĵ�С�仯��
	*     <li> FILE_ACTION_RENAMED_NEW_NAME		����ļ��������д�ı仯�� 
	*     </ol>
	*  </ul> 
	*/
	DWORD	PopLastNotifyItem(DWORD dwWatchID, char* lpszFileName, int nMaxLength);
protected:
	
	static THREAD_RETURN HandleDirectoryChange(LPVOID lpVoid);			/*!< ��ش����߳� */

	void	HandleChangeNotify(_DIRECTORY_INFO* lpdi, const CBasicString& strFileName, DWORD dwAction);  /*!< ���֪ͨ���� */
	
	void	StartWatch();		/*!< ��ʼ��� */
	void	EndWatch();			/*!< ֹͣ��� */
	BOOL	IsThreadRunning() const {return m_eRunFlag == ThreadRun && m_hThread != NULL;}  /*!< �߳��Ƿ����� */
	BOOL	IsValidDirectoryInfo(_DIRECTORY_INFO* lpdi);  /*!< �����Ϣ�Ƿ�Ϸ� */

	DWORD	PopNotifyItem(DWORD dwWatchID, char* lpszFileName, int nMaxLength, int nPos);	/*!< ����һ��֪ͨ */
	
	BOOL	IsCompPort() { return (m_dwNotifyOps & TFN_OPS_MODE) == TFN_OPS_COMPORT; }		/*!< �Ƿ�����ɶ˿ڵ� */
	BOOL	IsScanFile() { return (m_dwNotifyOps & TFN_OPS_MODE) == TFN_OPS_SCANFILE; }		/*!< �Ƿ���ɨ��Ŀ¼�� */
protected:
#ifdef __BASICWINDOWS
	void	HandleDirectoryChange_CompPort();		/*!< ��ɶ˿ڵļ�غ��� */
#endif
	void	HandleDirectoryChange_ScanDir();			/*!< ɨ��Ŀ¼�Ĵ����� */
	int		ScanDir(_DIRECTORY_INFO* lpdi, const char* lpszDir, const time_t tmLastChange, time_t& tmCurChange, time_t tmBefore);	/*!< ɨ��Ŀ¼ */
private:
	_di_array	m_ayDirInfo;	/*!<���Ŀ¼���� */
	HANDLE		m_hCompPort;	/*!<��� */
	HANDLE		m_hThread;		/*!<�߳̾�� */
	DWORD		m_dwNotifyOps;	/*!<��ص�����  */

	enum {ThreadRun, ThreadStop} m_eRunFlag;	/*!<�߳�״̬ */

	CCriticalSection		m_mtxAddRemove;		/*!<�������е��� */
};

#pragma warning (pop)

//////////////////addtogroup0812
/** @} */
//////////////////addtogroup0812


__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma	pack()
#endif 
