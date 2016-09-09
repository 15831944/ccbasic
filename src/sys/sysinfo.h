/***********************************************************************************************
// �ļ���:     sysinfo.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 8:55:21
// ��������:   ȡ��ϵͳ��Ϣϵ�к���
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_SYSINFO_H
#define BASIC_SYSINFO_H

//
//ȡ��ϵͳ��Ϣϵ�к���
//

__NS_BASIC_START


//!��ȡ�Ƿ��м�����Ϣ
_BASIC_DLL_API_C int _BASIC_DLL_API BasicKBHit();

//!ȡ�ò���ϵͳ�汾
/*! 
*\param strOSVer	���ز���ϵͳ�汾��Ϣ
*\return �ɹ�����0��ʧ�ܷ��ط�0
*/
_BASIC_DLL_API_C int _BASIC_DLL_API BasicGetOSystemV(CBasicString& strOSVer);
//

//
//!ȡ��CPU�ں˸���
/*! 
*\return ����CPU�ں˸���
*/
_BASIC_DLL_API_C  int _BASIC_DLL_API BasicGetCpuNumber();
//

//
//!ȡ��CPU������,��λ�ٷֱ�
/*!
*\return ����CPU�����ʰٷֱ�*100
 */
_BASIC_DLL_API_C  int _BASIC_DLL_API BasicGetCPUUse();
//

//
//! ȡ���ڴ���Ϣ����λK
/*!
 *\param dwPhysicalMemory	�����ڴ� 
 *\param dwAvailMemory		�����ڴ�
 *\param dwUsedMemory		ʹ���ڴ�
 *\param dwVirtualMemory	�����ڴ�
 */
_BASIC_DLL_API_C  void _BASIC_DLL_API BasicGetMemoryInfo(DWORD& dwPhysicalMemory, 
				   DWORD& dwAvailMemory, 
				   DWORD& dwUsedMemory, 
				   DWORD& dwVirtualMemory);

//! ȡ�ý���ʹ���ڴ� ��λK
/*!
 *\param  hProcess�����̾������hProcess==NULL,��ȡ��ǰ����
 *\param  bKeepHandle�����Ӧ�ó�����Ҫ���ڶ�ʱ���ã�����ΪTRUE
 */
_BASIC_DLL_API_C  DWORD _BASIC_DLL_API BasicGetProcessMem(HANDLE hProcess, BOOL bKeepHandle = FALSE);
//

//! ȡ��Ӳ����Ϣ
/*!
*\param  pszDiskBuffer Ӳ����Ϣ����ռ�
*\param  nBufferLen Ӳ����Ϣ����ռ䳤��
 */
_BASIC_DLL_API_C  DWORD _BASIC_DLL_API BasicGetDiskInfo(char* pszDiskBuffer, int nBufferLen);
//

//! ȡ·�����ڴ��̵�ʣ��ռ�
/*!
 *\param lpszPath ָ��·��
 *\return ����ʣ����̿ռ䣬��λM
 */
_BASIC_DLL_API_C  long _BASIC_DLL_API BasicGetDiskFreeinfo(const char* lpszPath);


//! ȡ��ϵͳ����ʱ�� 
/*!
 *\return ����ʱ��ֵ����λ������
 *\remark ���ڲ�����ʱ
 */
_BASIC_DLL_API_C  DWORD _BASIC_DLL_API BasicGetTickTime();
//! ȡ��ϵͳ����ʱ�䣬����DWORD
_BASIC_DLL_API_C  double _BASIC_DLL_API BasicGetTickTimeCount();

//! ȡ��ģ����������ȫ·��
/*!
 *\param hModule  ��hModule==NULL����ȡ��ǰ����������
 *\return ģ����
 */
CBasicString _BASIC_DLL_API BasicGetModuleName(HANDLE hModule = NULL);
//! ȡ��ģ����������ȫ·��
/*!
 *\param hModule  ��hModule==NULL����ȡ��ǰ����������
 *\param pszBuffer �������ݵĿռ�
 *\param nBufLen �������ݵĿռ䳤��
 *\return  ����ʵ�ʳ���
 */
long _BASIC_DLL_API BasicGetModuleName(HANDLE hModule, char* pszBuffer, int nBufLen);
//! ȡ��ģ������������ȫ·��
/*!
 *\param hModule  ��hModule==NULL����ȡ��ǰ����������
 *\param bExt �Ƿ������չ��
 *\return ģ����
 */
CBasicString _BASIC_DLL_API BasicGetModuleTitle(HANDLE hModule = NULL, BOOL bExt = FALSE);

//! ȡ��ģ��·��
/*!
 *\param hModule  ��hModule==NULL����ȡ��ǰ������·��
 *\return ģ��·��
 */
 CBasicString _BASIC_DLL_API BasicGetModulePath(HANDLE hModule = NULL);

#define	BASIC_PSL_RET_ERROR				-1			// ������ȷ��״̬
#define	BASIC_PSL_RET_NOT_EXIST			0			// ���̲�����
#define	BASIC_PSL_RET_STILL_LIVE		1			// ���̴���
#define	BASIC_PSL_RET_NO_RIGHT			2			// �����ڵ���û��Ȩ��

//! �ж�ĳ�������Ƿ����
/*!
*\param dwProcessID	����ID
 */
_BASIC_DLL_API_C  BOOL _BASIC_DLL_API BasicProcessIsTerminated(DWORD dwProcessID);

//! �޸�ϵͳʱ��
/*!
* \param tTime:��Ҫ���õ�ʱ�䣬��ȷ����
* \return �޸ĳɹ�����ʧ��
 */
_BASIC_DLL_API_C  BOOL _BASIC_DLL_API BasicSetSysTime(time_t tTime);

//////////////////////////////////////////////////////////////////////////
//!���������Ϣ

#define MAX_IP						(int)16			//!< ֧�ֵ�IP��ַ��
// !ȡIP��ַ����������ṹ
typedef struct   tagLocalAddr
{
	char		m_szIP[MAX_IP];		//!< IP��ַ
	char		m_szMask[MAX_IP];	//!< ����
	tagLocalAddr()
	{
		memset(this, 0, sizeof(*this));
	}
} LOCALADDR, *PLOCALADDR;

//! ȡ����IP��ַ����������
/*!
 *\param pBuffer cbBuffer ����IP��ַ��Ϣ�ռ�
 *\return ����IP��ַ����
 */
int BasicGetLocalAddrInfo(PLOCALADDR pBuffer, int cbBuffer);

///////////////////////////////////////////////////////////////////////////////
//!��̬����õĺ���

//! ���ض�̬��
/*!
 *\param lpszLibFileName	��̬���ļ���
 *\return �ɹ����ط��㶯̬������ʧ�ܷ���NULL
 */
void* BasicLoadLibrary(const char* lpszLibFileName);

//! �ͷŶ�̬��
/*!
 *\param hModule	��̬����
 *\return �ɹ�����0������ʧ��
 */
long	BasicFreeLibrary(void* hModule);

//!ȡ��̬�⺯�����
/*!
 *\param hModule	��̬����
 *\param lpszProcName ������
 *\return �ɹ����ط��㺯����ַ��ʧ�ܷ���NULL
 */
void*	BasicGetProcAddress(void* hModule, const char* lpszProcName);
/*
 * \brief �û�������̵�CPUʹ����
 */
class   CProcessInfo
{
public:
	CProcessInfo(DWORD nProcessId);
	virtual ~CProcessInfo();
	
	/*
	 * \brief ��ȡ���̵�CPUռ���ʣ�����ֵΪ�ٷֱ�
	 */
	int GetProcessCpu();
protected:
	DWORD m_nProcessId;
#ifdef __BASICWINDOWS
	LONGLONG m_nLastSystemTime;  
	LONGLONG m_nLastTime;
	DWORD m_nCpuCount;
#else
	LONGLONG m_nLastUtime;
	LONGLONG m_nLastStime;
	LONGLONG m_nCutime;
	LONGLONG m_nCstime;
	FILE *fpidstat;
#endif
};

//! ȡ�߾��ȵ�ϵͳ��������ϵͳ������ʼ����λ��΢�루10-6 �룩��
/*! 
*\return  ����ɹ����� ��ϵͳ���������ڵ�΢������
*\remarks ���������ҪӲ��֧�֣�HRT������Ҫ�������ܼ�¼�е�ʱ���¼
*/
double BasicGetHighPerformanceCounter();

__NS_BASIC_END

#endif 


