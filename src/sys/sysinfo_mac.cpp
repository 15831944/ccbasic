
//
//ȡ��ϵͳ��Ϣϵ�к���
//
//
#include "../inc/basic.h"
#ifdef __MAC
#include <sys/sysctl.h>
//#include <sys/disk.h>
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/utsname.h>
#include <mach-o/dyld.h>
#include <dlfcn.h>
#include <mach/task.h>
#include <mach/mach_init.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include <mach/mach.h>
#include <mach/mach_error.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <ifaddrs.h>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define Basic_statfs        statfs
#define Basic_getmntinfo    getmntinfo


__NS_BASIC_START

//!��ȡ�Ƿ��м�����Ϣ
int BasicKBHit(){
    return 0;
}

//!ȡ�ò���ϵͳ�汾
/*!
*\param strOSVer	���ز���ϵͳ�汾��Ϣ
*\return �ɹ�����0��ʧ�ܷ��ط�0
*/
_BASIC_DLL_API int BasicGetOSystemV(CBasicString& strOSVer){
    struct utsname osbuf;
    uname(&osbuf);
    strOSVer.Format("%s %s", osbuf.sysname, osbuf.release);
    return 0;
}
//

//
//!ȡ��CPU�ں˸���
/*!
*\return ����CPU�ں˸���
*/
_BASIC_DLL_API int BasicGetCpuNumber(){
    int nName[2] = { CTL_HW,HW_NCPU };
    int nCPU = 0;
    size_t sSize = sizeof(nCPU);
    sysctl(nName, 2, &nCPU, &sSize, NULL, 0);
    return nCPU;
    //	return sysconf(_SC_NPROCESSORS_ONLN);
}
//

//
//!ȡ��CPU�ں˸���
/*!
*\return ����CPU�ں˸���
*/
_BASIC_DLL_API int BasicGetNumberOfCpu(){
    int nName[2] = { CTL_HW,HW_NCPU };
    int nCPU = 0;
    size_t sSize = sizeof(nCPU);
    sysctl(nName, 2, &nCPU, &sSize, NULL, 0);
    return nCPU;
    //	return sysconf(_SC_NPROCESSORS_ONLN);
}
//


typedef unsigned long long TIC_t;
typedef          long long SIC_t;
typedef struct CPU_t{
    unsigned long new_userTicks, new_NiceTicks, new_SysTicks, new_IdleTicks;
    unsigned long old_userTicks, old_NiceTicks, old_SysTicks, old_IdleTicks;
    unsigned id;  // the CPU ID number
} CPU_t;


static CPU_t *pCpu = NULL;
static int nCpu_tot;

static CPU_t *cpus_refresh(CPU_t *pCpu){
    mach_msg_type_number_t numCpuCount;
    processor_info_array_t ayInfoArray;
    mach_msg_type_number_t numInfoCount;
    kern_return_t kr;
    processor_cpu_load_info_data_t* ayCpuLoadInfo;
    unsigned long old_ticks, new_ticks, old_totalTicks, new_totalTicks;
    int cpu, state;

    kr = host_processor_info(mach_host_self(), PROCESSOR_CPU_LOAD_INFO, &numCpuCount, &ayInfoArray, &numInfoCount);
    if(kr)
        return NULL;

    nCpu_tot = numCpuCount;
    if(pCpu == NULL && nCpu_tot > 0){
        pCpu = (CPU_t *)malloc((nCpu_tot + 1) * sizeof(CPU_t));
        memset(pCpu, 0, (nCpu_tot + 1) * sizeof(CPU_t));
        for(int i = 0; i < nCpu_tot; i++)
            pCpu[i].id = i;

    }

    ayCpuLoadInfo = (processor_cpu_load_info_data_t*)ayInfoArray;
    if(ayCpuLoadInfo == NULL)
        return NULL;


    for(int i = 0; i < nCpu_tot; i++){
        pCpu[i].new_userTicks = ayCpuLoadInfo[i].cpu_ticks[CPU_STATE_USER];
        pCpu[i].new_NiceTicks = ayCpuLoadInfo[i].cpu_ticks[CPU_STATE_NICE];
        pCpu[i].new_SysTicks = ayCpuLoadInfo[i].cpu_ticks[CPU_STATE_SYSTEM];
        pCpu[i].new_IdleTicks = ayCpuLoadInfo[i].cpu_ticks[CPU_STATE_IDLE];
    }
    return pCpu;
}

//
//!ȡ��CPU������,��λ�ٷֱ�
/*!
*\return ����CPU�����ʰٷֱ�*100
*/
int BasicGetCPUUse(){
    pCpu = cpus_refresh(pCpu);

    if(pCpu == NULL)
        return 0;

    unsigned long nNewTotalTicks = 0;
    unsigned long nOldTotalTicks = 0;

    unsigned long nNewTicks = 0;
    unsigned long nOldTicks = 0;

    unsigned long nOldTotalSysTicks = 0;
    for(int i = 0; i< nCpu_tot; i++){
        nNewTotalTicks += pCpu[i].new_userTicks + pCpu[i].new_NiceTicks + pCpu[i].new_SysTicks + pCpu[i].new_IdleTicks;
        nOldTotalTicks += pCpu[i].old_userTicks + pCpu[i].old_NiceTicks + pCpu[i].old_SysTicks + pCpu[i].old_IdleTicks;
        nNewTicks += pCpu[i].new_userTicks + pCpu[i].new_NiceTicks + pCpu[i].new_SysTicks;
        nOldTicks += pCpu[i].old_userTicks + pCpu[i].old_NiceTicks + pCpu[i].old_SysTicks;

        pCpu[i].old_userTicks = pCpu[i].new_userTicks;
        pCpu[i].old_NiceTicks = pCpu[i].new_NiceTicks;
        pCpu[i].old_SysTicks = pCpu[i].new_SysTicks;
        pCpu[i].old_IdleTicks = pCpu[i].new_IdleTicks;
    }

    int dUsage = (nNewTotalTicks - nOldTotalTicks > 0) ? (double)(nNewTicks - nOldTicks) / (nNewTotalTicks - nOldTotalTicks) * 100 : 0;
    return dUsage;
}

//
//! ȡ���ڴ���Ϣ����λK
/*!
*\param dwPhysicalMemory	�����ڴ�
*\param dwAvailMemory		�����ڴ�
*\param dwUsedMemory		ʹ���ڴ�
*\param dwVirtualMemory	�����ڴ�
*/
_BASIC_DLL_API void BasicGetMemoryInfo(DWORD& dwPhysicalMemory,
                                       DWORD& dwAvailMemory,
                                       DWORD& dwUsedMemory,
                                       DWORD& dwVirtualMemory){
    int nName[2] = { CTL_HW,HW_PHYSMEM };
    size_t sSize = sizeof(dwPhysicalMemory);

    //User HW_MEMSIZE by 64-bit
    sysctl(nName, 2, &dwPhysicalMemory, &sSize, NULL, 0);

    nName[1] =
        sysctl(nName, 2, &dwPhysicalMemory, &sSize, NULL, 0);
}

//! ȡ�ý���ʹ���ڴ� ��λK
/*!
*\param  hProcess�����̾������hProcess==NULL,��ȡ��ǰ����
*\param  bKeepHandle�����Ӧ�ó�����Ҫ���ڶ�ʱ���ã�����Ϊtrue
*/
_BASIC_DLL_API DWORD BasicGetProcessMemory(HANDLE hProcess, bool bKeepHandle){
    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
    task_info(current_task(), TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count);
    return t_info.resident_size;
}

//! ȡ��Ӳ����Ϣ
/*!
*\param  pszDiskBuffer Ӳ����Ϣ����ռ�
*\param  nBufferLen Ӳ����Ϣ����ռ䳤��
*/
_BASIC_DLL_API DWORD BasicGetDiskInfo(TCHAR* pszDiskBuffer, int nBufferLen){

    struct  Basic_statfs *s = NULL;
    int nDiskCount = Basic_getmntinfo(&s, 0);
    TCHAR* p = pszDiskBuffer;
    int nLen = nBufferLen;
    for(int i = 0; i<nDiskCount; i++){
        int n = _stprintf_s(p, nLen, "%s %d/%d|",
                            s[i].f_mntonname,
                            (s[i].f_bavail * s[i].f_bsize) / (1024 * 1024),
                            (s[i].f_blocks * s[i].f_bsize) / (1024 * 1024)
        );
        nLen -= n;
        p += n;
    }
    return nBufferLen - nLen;
}
//

//! ȡ·�����ڴ��̵�ʣ��ռ�
/*!
*\param lpszPath ָ��·��
*\return ����ʣ����̿ռ䣬��λM
*/
_BASIC_DLL_API long BasicGetDiskFreeinfo(const char* lpszPath){
    long lRet = -1;
    struct Basic_statfs s;
    if(Basic_statfs(lpszPath, &s) < 0){
        lRet = 0;
    }
    else{
        lRet = (s.f_bavail * s.f_bsize) / (1024 * 1024);
    }
    return lRet;
}


//! ȡ��ϵͳ����ʱ�� 
/*!
*\return ����ʱ��ֵ����λ������
*\remark ���ڲ�����ʱ
*/
DWORD BasicGetTickTime(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

double  BasicGetTickTimeCount(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    double dRet = tp.tv_sec;
    dRet *= 1000;
    return dRet + tp.tv_usec / 1000;
}

//! ȡ��ģ����������ȫ·��
/*!
*\param hModule  ��hModule==NULL����ȡ��ǰ����������
*\return ģ����
*/
_BASIC_DLL_API CBasicString BasicGetModuleName(HANDLE hModule){
    char szPath[_MAX_PATH];
    BasicGetModuleName(hModule, szPath, _MAX_PATH);
    return CBasicString(szPath);
}

//! ȡ��ģ����������ȫ·��
/*!
*\param hModule  ��hModule==NULL����ȡ��ǰ����������
*\param pszBuffer �������ݵĿռ�
*\param nBufLen �������ݵĿռ䳤��
*\return  ����ʵ�ʳ���
*/
_BASIC_DLL_API long BasicGetModuleName(HANDLE hModule, char* pszBuffer, int nBufLen){
    //??unicode
    unsigned int nPathLen = nBufLen;
    return _NSGetExecutablePath(pszBuffer, &nPathLen);
}


//! ȡ��ģ������������ȫ·��
/*!
*\param hModule  ��hModule==NULL����ȡ��ǰ����������
*\param bExt �Ƿ������չ��
*\return ģ����
*/
_BASIC_DLL_API CBasicString BasicGetModuleTitle(HANDLE hModule, bool bExt){
    CBasicString strModule = BasicGetModuleName(hModule);
    int nPos = strModule.ReverseFind(PATHSPLIT);
    if(nPos >= 0)
        strModule = strModule.Mid(nPos + 1);
    return strModule;
}

//! ȡ��ģ��·��
/*!
*\param hModule  ��hModule==NULL����ȡ��ǰ������·��
*\return ģ��·��
*/
_BASIC_DLL_API CBasicString BasicGetModulePath(HANDLE hModule){
    char szPath[_MAX_PATH];
    CBasicString strPath = basiclib::BasicGetModuleName(hModule);
    __tcscpyn(szPath, _MAX_PATH, strPath.c_str());
    int i = 0;
    for(i = strlen(szPath) - 1; i >= 0 && szPath[i] != PATHSPLIT; i--);
    i++;
    szPath[i] = '\0';
    return szPath;
}

//! �ж�ĳ�������Ƿ����
/*!
*\param dwProcessID	����ID
*/
_BASIC_DLL_API bool BasicProcessIsTerminated(DWORD dwProcessID){
    CBasicString strFile;
    CBasicString strCmd;
    strCmd.Format("ps -p %d", dwProcessID);
    FILE* pTmp = popen(strCmd.c_str(), "r");
    if(pTmp != NULL){
        char szBuf[4096];
        int nReadLen = 0;
        while((nReadLen = fread(szBuf, 1, sizeof(szBuf), pTmp)) > 0){
            strFile += CBasicString(szBuf, nReadLen);
        }
        fclose(pTmp);
    }
    if(strFile.GetLength() <= 0){
        return true;
    }
    CBasicStringArray ayItem;
    BasicSpliteString(strFile.c_str(), '\n', IntoContainer_s<CBasicStringArray>(ayItem));
    int nLineCnt = ayItem.GetSize();
    if(nLineCnt == 2){
        return false;
    }
    return true;
}

//! �޸�ϵͳʱ��
/*!
* \param tTime:��Ҫ���õ�ʱ�䣬��ȷ����
* \return �޸ĳɹ�����ʧ��
*/
_BASIC_DLL_API bool BasicSetSysTime(time_t tTime){
    return true;
}

#define MAC_PROCESS_CMD				"ps -el"		// Linux
#define PS_CMD_COL_PID				"PID"		// Linux 
#define PS_CMD_COL_PPID				"PPID"		// Linux 
#define PS_CMD_COL_CMD				"CMD"		// Linux 


//! ȡ����IP��ַ����������
/*!
*\param pBuffer cbBuffer ����IP��ַ��Ϣ�ռ�
*\return ����IP��ַ����
*/
_BASIC_DLL_API int BasicGetLocalAddrInfo(PLOCALADDR pBuffer, int cbBuffer){
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0){
        return 0;
    }
    struct ifreq ifr;
    bool found = true;
    int i = 0;
    int nIndex = 0;
    while(found && nIndex < cbBuffer){
        sprintf(ifr.ifr_name, "en%d", i++);
        if(ioctl(sock, SIOCGIFADDR, &ifr) < 0){
            break;
        }
        struct sockaddr_in* pAddrTmp = (struct sockaddr_in*)&ifr.ifr_addr;
        strncpy(pBuffer[nIndex].m_szIP, inet_ntoa(pAddrTmp->sin_addr), MAX_IP);
        if(ioctl(sock, SIOCGIFNETMASK, &ifr) < 0){
            break;
        }
        pAddrTmp = (struct sockaddr_in*)&ifr.ifr_addr;
        strncpy(pBuffer[nIndex].m_szMask, inet_ntoa(pAddrTmp->sin_addr), MAX_IP);
        if(strlen(pBuffer[nIndex].m_szIP) <= 0 || strlen(pBuffer[nIndex].m_szMask) <= 0){
            found = false;
        }
        nIndex++;
    }
    close(sock);
    return nIndex;
}

//ȡ����״̬��Ϣ
int GetMacInfo(int& nAdapterIndex, char* pMac, int nLength){
    char szNet[8];
    memset(szNet, 0, 8);
    if(nAdapterIndex < 0){
        nAdapterIndex = 0;
    }
    sprintf(szNet, "en%d", nAdapterIndex);
    struct ifaddrs *ifas = NULL;
    struct ifaddrs *ifasTemp = NULL;
    if(getifaddrs(&ifas) != 0){
        return -1;
    }

    ifasTemp = ifas;

    for(; ifasTemp != NULL; ifasTemp = (*ifasTemp).ifa_next){
        if((ifasTemp->ifa_addr)->sa_family == AF_LINK){
            struct sockaddr_dl *pAddr = (struct sockaddr_dl*)ifasTemp->ifa_addr;
            if(strcmp(szNet, ifasTemp->ifa_name) == 0){
                char* basemac = &(pAddr->sdl_data[pAddr->sdl_nlen]);
                memcpy(pMac, basemac, pAddr->sdl_alen > nLength ? nLength : pAddr->sdl_alen);
                break;
            }
        }
    }

    freeifaddrs(ifas);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//!��̬����õĺ���

//! ���ض�̬��
/*!
*\param lpszLibFileName	��̬���ļ���
*\return �ɹ����ط��㶯̬������ʧ�ܷ���NULL
*/
_BASIC_DLL_API void*	BasicLoadLibrary(const char* lpszLibFileName){
    void* hDll = dlopen(lpszLibFileName, RTLD_NOW | RTLD_GLOBAL);
    if(hDll == NULL){
        char* pError = dlerror();
        if(pError != NULL)
            basiclib::BasicLogEventErrorV("linking (%s) error occurred: (%s) \n", lpszLibFileName, pError);
    }
    return hDll;
}


//! �ͷŶ�̬��
/*!
*\param hModule	��̬����
*\return �ɹ�����0������ʧ��
*/
_BASIC_DLL_API long	BasicFreeLibrary(void* hModule){
    return dlclose(hModule);
}

//!ȡ��̬�⺯�����
/*!
*\param hModule	��̬����
*\param lpszProcName ������
*\return �ɹ����ط��㺯����ַ��ʧ�ܷ���NULL
*/
_BASIC_DLL_API void*	BasicGetProcAddress(void* hModule, LPCTSTR lpszProcName){
    return dlsym(hModule, lpszProcName);
}

/*
* \brief �û�������̵�CPUʹ����
*/
CProcessInfo::CProcessInfo(DWORD nProcessId){
    m_nProcessId = nProcessId;

    m_nLastUtime = 0;

    m_nLastStime = 0;

    m_nCutime = 0;

    m_nCstime = 0;

    fpidstat = NULL;


}

CProcessInfo::~CProcessInfo(){

}

int CProcessInfo::GetProcessCpu(){
    return BasicGetCPUUse();
}


//! ȡ�߾��ȵ�ϵͳ��������ϵͳ������ʼ����λ��΢�루10-6 �룩��
/*!
*\return  ����ɹ����� ��ϵͳ���������ڵ�΢������
*\remarks ���������ҪӲ��֧�֣�HRT������Ҫ�������ܼ�¼�е�ʱ���¼
*/

_BASIC_DLL_API  double BasicGetHighPerformanceCounter(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)tp.tv_sec * 1000000 + tp.tv_usec);
}


#define cpuid(in,a,b,c,d)  asm("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));
bool getcpuid(char *id, size_t max){
    int i;
    unsigned long li, maxi, maxei, eax, ebx, ecx, edx, unused;

    cpuid(0, maxi, unused, unused, unused);
    maxi &= 0xffff;

    if(maxi < 3){
        return false;
    }

    cpuid(3, eax, ebx, ecx, edx);

    snprintf(id, max, "%08lx %08lx %08lx %08lx", eax, ebx, ecx, edx);
    return true;
}

//! ��ȡȡ������������
/*!
*/
bool BasicGetMachineSerial(CBasicString& str){
    char szBuf[256] = { 0 };
    if(!getcpuid(szBuf, 128)){
        return false;
    }
    str = basiclib::Basic_MD5(szBuf, 256);
    return true;
}

__NS_BASIC_END


#endif //__MAC
