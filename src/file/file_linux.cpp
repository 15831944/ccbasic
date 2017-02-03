#include "../inc/basic.h"

#include "file_linux.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if (defined(__LINUX) || defined(__MAC) || defined(__ANDROID))
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <utime.h>
#include <sys/stat.h>

using namespace basiclib;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//__NS_BASIC_START
BOOL CloseHandle(HANDLE hObject)
{
	return CloseFile(hObject);
}
BOOL CloseFile(HANDLE hObject)
{
	if(hObject == NULL || hObject == INVALID_HANDLE_VALUE)
	{
		SetLastError(ENOENT);
		return FALSE;
	}

	int nRet = fclose((FILE*)hObject);
	if(nRet != 0)
	{
		return FALSE;
	}
	return TRUE;
}

void SetLastError(DWORD dwError)
{
	errno = (int)dwError;
}


DWORD GetLastError()
{
	switch(errno)
	{
		case EACCES:                                //��ֻ���ļ�д����
		case EROFS:                                 //�޸�ֻ���ļ�ϵͳ
		case  EBADF:                                //ʹ�ô����ļ�����������д��ֻ����ʽ���ļ�
		{
			return BASIC_FILE_WRITE_DENIED;
			break;
		}
		case EISDIR:                                //���ݵ���Ŀ¼�����ļ�
		case ENOENT:                                //�޴��ļ�����Ŀ¼
		{
			return BASIC_FILE_NOT_FOUND;
			break;
		}
		case ELOOP:                                 //������������
		case ENOTDIR:                               //·������ΪĿ¼
		case ENAMETOOLONG:                          //�ļ�������·�� > MAX_NAME
		{
			return BASIC_FILE_BAD_PATH;
			break;
		}
		case EMFILE:                                //������ļ�������
		case ENFILE:                                //̫��ͬʱ�򿪵��ļ�
		case EMLINK:                                //�������õ����ļ�����LINK_MAX������ 
		{
			return BASIC_FILE_TOO_MANY_OPEN;
			break;
		}
		case ENOSPC:                                //�豸��ʣ��ռ䲻��
		case ENXIO:                                 //�����ڵ��豸�������ļ���ִ��I/O����
		case EIO:                                   //I/O����
		case ENOTTY:
		{
			return BASIC_FILE_HARD_IO;
			break;
		}
		case ENOMEM:                                //�ڴ治��
		{
			return BASIC_FILE_NO_MEMORY;
			break;
		}
		case EEXIST:                                //��ָ�ļ��Ѵ���
		{
			return BASIC_FILE_ALREADY_EXISTS;
			break;
		}
		case EFBIG:                                 //�ļ�����
		{
			return BASIC_FILE_TOO_LARGE;
			break;
		}
		case EXDEV:                                //�ƶ��ļ�����
		{
			return BASIC_FILE_BAD_SEEK;
			break;
		}
		case EPERM:                                 //������ִ�иò���
		case ENOTEMPTY:                             //ָ����Ŀ¼��Ϊ�գ�Ӧ��Ϊ��
		{
			return BASIC_FILE_ACCESS_DENIED;
			break;
		}
		case ETXTBSY:                               //ִ��һ������д���ļ�����дһ������ִ�е��ļ�
		case ENOLCK:                                //û�п��õ���
		{
			return BASIC_FILE_LOCK_VIOLATION;
			break;
		}
		case E2BIG:                                //�����ݵĺ��������б�̫��
		case EINVAL:                                //��������
		case EDOM:                                  //����Ĳ��������ֺ�����֮��
		case EFAULT:                                //��������֮һ������Ч��ַ
		{
			return BASIC_FILE_BAD_PARAM;
			break;
		}
		case ENOSR:                                 //��Դ��صķ���������
		case EAGAIN:                                //��Ҫ�����Դ��ʱ������
		case EBUSY:                                 //�������Դ������
		case EDEADLK:                               //����������󣬻������Դ����
		{
			return BASIC_FILE_BAD_RESOURCE;
			break;
		}
		case EINTR:                                 //������̱��ж�
		case EOVERFLOW:                             //�������
		case ECHILD:                                //�����ȴ��Ƴ��ӽ��̣��������ӽ����Ѿ��Ƴ�
		case  ESRCH:                                //ָ����Ч�Ľ���ID�ͽ�����
		case ERANGE:                                //���ú�������ֵ�����޷��÷������ͳ���
		{
			return BASIC_FILE_BAD_PROCESS;
			break;
		}
		case ENODEV:                                //û���������豸
		case ENOSYS:                                //ϵͳ��֧�ָú���
		{
			return BASIC_FILE_NOT_SUPPORT;
			break;
		}
		case ENOEXEC:                               //���������޷�ִ�е��ļ�
		{
			return BASIC_FILE_NO_EXEC;
			break;
		}
		case EPIPE:                                 //�����ڵĹܵ���������
		case ESPIPE:                                //�����ڹܵ���FIFO��ջ�ϲ���
		{
			return BASIC_FILE_BAD_PIPE;
			break;
		}
	}
	return 0;
}

int _basic_system(LPCTSTR lpCmd)
{
	TCHAR szCmd[1024];
	memset(szCmd, 0, sizeof(szCmd));
	LPCTSTR  pS = lpCmd;
	TCHAR*   pD = szCmd;
	BOOL bIn = FALSE;
	while(*pS)
	{
		if(*pS == '\'')
		{
			bIn = !bIn;
		}
		else if(bIn && (*pS == ' ' || *pS == '(' || *pS == ')'))		//ת��ո��
		{
			*pD++ = '\\';
			*pD++ = *pS;
		}
		else
		{
			*pD++ = *pS;
		}
		pS++;
	}
	return _tsystem(szCmd);
}

HANDLE	CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
				   DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	TCHAR szOpenFlag[8];
	memset(szOpenFlag, 0, sizeof(szOpenFlag));
	switch(dwCreationDisposition)
	{
		case CREATE_NEW:
		{
			if (_taccess(lpFileName, F_OK) == 0)	//�ļ����ڣ�ʧ��
			{
				SetLastError(EEXIST);
				return INVALID_HANDLE_VALUE;
			}
		}
		case CREATE_ALWAYS:
		{
			szOpenFlag[0] = 'w';
			break;
		}
		case OPEN_ALWAYS:
		{
			if (_taccess(lpFileName, F_OK) == 0)	//�ļ����ڣ���
			{
				szOpenFlag[0] = 'r';
			}
			else	//����
			{
				szOpenFlag[0] = 'w';
			}
			break;
		}
		case TRUNCATE_EXISTING:
		{
			if (_taccess(lpFileName, F_OK) != 0)	//�ļ������ڣ�ʧ��
			{
				SetLastError(ENOENT);
				return INVALID_HANDLE_VALUE;
			}
			szOpenFlag[0] = 'w';
			break;
		}
		default:
		case OPEN_EXISTING:
		{
			szOpenFlag[0] = 'r';
			break;
		}
	}
	szOpenFlag[1] = 'b';
	if(dwDesiredAccess & GENERIC_WRITE)
	{
		szOpenFlag[2] = '+';
	}
	FILE* fp = _tfopen(lpFileName, szOpenFlag);
	if(fp == NULL)
	{
		return INVALID_HANDLE_VALUE;
	}
	return fp;
}

BOOL WriteFile(HANDLE hFile,LPCVOID lpBuffer,DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten,LPOVERLAPPED lpOverlapped)
{
	size_t nWrite = fwrite(lpBuffer, 1, nNumberOfBytesToWrite, (FILE*)hFile);
	if(lpNumberOfBytesWritten != NULL)
	{
		*lpNumberOfBytesWritten = nWrite;
	}
	if(nWrite != nNumberOfBytesToWrite)
	{
		return ferror((FILE*)hFile) == 0;
	}
	return TRUE;

}

BOOL ReadFile(HANDLE hFile,LPVOID lpBuffer,DWORD nNumberOfBytesToRead,LPDWORD lpNumberOfBytesRead,LPOVERLAPPED lpOverlapped)
{
	size_t nRead = fread(lpBuffer, 1, nNumberOfBytesToRead, (FILE*)hFile);
	if(lpNumberOfBytesRead != NULL)
	{
		*lpNumberOfBytesRead = nRead;
	}
	if(nRead != nNumberOfBytesToRead)
	{
		return ferror((FILE*)hFile) == 0;
	}
	return TRUE;
}

BOOL FlushFileBuffers(HANDLE hFile)
{
	if(hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		SetLastError(ENOENT);
		return FALSE;
	}
	int nRet = fflush((FILE*)hFile);
	return nRet == 0;
}

DWORD SetFilePointer(HANDLE hFile,LONG lDistanceToMove,LONG* lpDistanceToMoveHigh,DWORD dwMoveMethod)
{
	if(fseek((FILE*)hFile, lDistanceToMove, dwMoveMethod) == -1)
	{	
		return (DWORD)-1;
	}	
	return ftell((FILE*)hFile);	
}

BOOL SetEndOfFile(HANDLE hFile)
{
	do 
	{
		int fno = fileno((FILE*)hFile);
		if(fno == -1)
		{
			break;
		}
		long lLength = ftell((FILE*)hFile);
		if(lLength < 0)
		{
			break;
		}
		int nRet = ftruncate(fno, lLength);
		if(nRet == -1)
		{
			break;
		}
		return TRUE;
	} while (0);
	return FALSE;
}

BOOL MoveFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName)
{
	return _trename(lpExistingFileName, lpNewFileName) == 0;
}
BOOL CopyFile(LPCTSTR lpExistingFileName,LPCTSTR lpNewFileName,BOOL bFailIfExists)
{
	char szCmd[1024];
	_stprintf(szCmd, "\\cp '%s' '%s' ", lpExistingFileName, lpNewFileName);
	if(bFailIfExists)
	{
		_tcscat(szCmd, " --reply=no ");
	}
	else
	{
		_tcscat(szCmd, " -f ");
	}
	return _basic_system(szCmd) == 0;
}

BOOL DeleteFile(LPCTSTR lpFileName)
{
	return _tremove(lpFileName) == 0;
}

#define CHECK_BUFFER        if(++lAbsLen >= lBufLen) { return _MAX_PATH;}      //��鷵�صĻ������Ƿ��㹻��
#define RETURN_ERROR		SetLastError(ELOOP);return 0;					   //���ش���·�������Ϸ�
#define NORMAL_COPY		    CHECK_BUFFER;*(++pAbsTail) = *pRelHead++;		   //��������

DWORD GetFullPathName(LPCTSTR lpFileName,DWORD nBufferLength,LPTSTR lpBuffer,LPTSTR *lpFilePart)
{
	long lBufLen = (long)nBufferLength;
	if(lpBuffer == NULL || lBufLen <= 0)
	{
		return _MAX_PATH;
	}

	memset(lpBuffer, 0, lBufLen * sizeof(TCHAR));
	if(_tgetcwd(lpBuffer, lBufLen) == NULL)
	{
		if(errno == EINVAL || errno == ERANGE)		//��������buffer�ռ䲻�㣬�������µ��ú�����
		{
			return _MAX_PATH;
		}
		return 0;
	}

	long lAbsLen = _tcslen(lpBuffer);
	if(lpFileName != NULL && lpFileName[0] != '\0')
	{
		if(*lpFileName == PATHSPLIT)
		{
			long lRelLen = _tcslen(lpFileName);
			if(lBufLen <= lRelLen)
			{
				return (lRelLen + 1);
			}
			_tcscpy(lpBuffer, lpFileName);
			lAbsLen = lRelLen;
		}
		else	
		{	
			TCHAR* pAbsTail = lpBuffer + lAbsLen - 1;
			if(*pAbsTail != PATHSPLIT)
			{
				CHECK_BUFFER;
				*(++pAbsTail) = PATHSPLIT;
			}
			TCHAR* pRelHead = (TCHAR*)lpFileName;
			while(*pRelHead)
			{
				if(*pRelHead == '.')
				{
					if(pRelHead[1] == '.')
					{
						if(pRelHead[2] != PATHSPLIT)
						{
							pRelHead++;
						}
						else
						{
							if(pAbsTail != lpBuffer)
							{
								for(;;)
								{
									*pAbsTail = '\0';
									pAbsTail--;
									lAbsLen--;
									if(*pAbsTail == PATHSPLIT)
									{
										break;
									}
									if(pAbsTail == lpBuffer)
									{
										RETURN_ERROR;
									}
								}
							}
							pRelHead += 3;
						}
					}
					else if(pRelHead[1] == PATHSPLIT)
					{
						pRelHead += 2;
					}
					else
					{
						NORMAL_COPY;
					}
				}
				else
				{
					NORMAL_COPY;
				}
			}
		}
	}

	if(lpFilePart != NULL)
	{
		*lpFilePart = basiclib::Basic_FindFileName(lpBuffer);
	}
	return (DWORD)lAbsLen;
}
BOOL SetFileTime(LPCTSTR lpFileName,const FILETIME *lpCreationTime,const FILETIME *lpLastAccessTime, const FILETIME *lpLastWriteTime)
{
	struct utimbuf ub;
	ub.actime = ub.modtime = 0;
	if(lpLastWriteTime != NULL)
	{
		ub.modtime = *lpLastWriteTime;
	}
	if(lpLastAccessTime != NULL)
	{
		ub.actime = *lpLastAccessTime;
	}

	struct utimbuf* pub = &ub;
	if(ub.actime == 0 && ub.modtime == 0)
	{
		pub = NULL;
	}
	else
	{
		if(ub.actime == 0)
		{
			ub.actime = ub.modtime;
		}
		else if(ub.modtime == 0)
		{
			ub.modtime = ub.actime;
		}
	}

	return _tutime(lpFileName, pub);
}

BOOL GetFileTime(HANDLE hFile,FILETIME* lpCreationTime,FILETIME* lpLastAccessTime,FILETIME* lpLastWriteTime)
{
	int nFile = fileno((FILE*)hFile);
	if(nFile == -1)
	{
		return FALSE;
	}
	struct stat st;
	if(_fstat(nFile, &st) != 0)
	{
		return FALSE;
	}
	if(lpCreationTime != NULL)
	{
		*lpCreationTime = st.st_ctime;
	}
	if(lpLastAccessTime != NULL)
	{
		*lpLastAccessTime = st.st_atime;
	}
	if(lpLastWriteTime != NULL)
	{
		*lpLastWriteTime = st.st_mtime;
	}
	return TRUE;
}

DWORD GetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh)
{
	if(lpFileSizeHigh != NULL)
	{
		*lpFileSizeHigh = 0;
	}
	if(hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		SetLastError(ENOENT);
		return (DWORD)-1;
	}
	FILE* file = (FILE*)hFile;

	do 
	{
		if(fseek(file, 0L, SEEK_CUR) != 0)
		{
			break;
		}
		long lCurPos = ftell(file);
		if(lCurPos < 0)
		{
			break;
		}
		if(fseek(file, 0L, SEEK_END) != 0)
		{
			break;
		}
		long lLen = ftell(file);
		fseek(file, lCurPos, SEEK_SET);

		return (DWORD)lLen;

	} while (0);

	return (DWORD)-1;
}

BOOL SetFileAttributes(LPCTSTR lpFileName,DWORD dwFileAttributes)
{
	struct stat st;
	if(_tstat(lpFileName, &st) != 0)
	{
		return INVALID_FILE_ATTRIBUTES;
	}
	DWORD dwThisAttr = 0;
	if(_taccess(lpFileName, W_OK) != 0)
	{
		dwThisAttr |= FILE_ATTRIBUTE_READONLY;
	}

	dwFileAttributes &= FILE_ATTRIBUTE_READONLY;
	if(dwFileAttributes == dwThisAttr)
	{
		return TRUE;
	}
	if(dwFileAttributes & FILE_ATTRIBUTE_READONLY)
	{
		st.st_mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
	}
	else
	{
		st.st_mode |= (S_IWUSR | S_IWGRP | S_IWOTH);
	}
	return _tchmod(lpFileName, st.st_mtime) == 0;
}

extern DWORD ChangeFileAttributes(struct _stat& st);

DWORD GetFileAttributes(LPCTSTR lpFileName)
{
	struct stat st;
	if(_tstat(lpFileName, &st) != 0)
	{
		return INVALID_FILE_ATTRIBUTES;
	}
	return ChangeFileAttributes(st);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����
const TCHAR CURR_PATH[] = "./";
class CFileFindHandle
{
public:
	CFileFindHandle();
	~CFileFindHandle();
public:
	void SetSearchFileSpec(LPCTSTR lpFileName);
	BOOL IsOpen() { return m_dirp != NULL; }
public:
	LPCTSTR	m_pszPath;
	LPCTSTR	m_pszSpec;
	struct dirent* dp;
	DIR*	m_dirp;
protected:
	TCHAR	m_szFullSearch[MAX_PATH];
};

CFileFindHandle::CFileFindHandle()
        : m_dirp(NULL), m_pszSpec(NULL), m_pszPath(NULL), dp(NULL)
{

}

CFileFindHandle::~CFileFindHandle()
{
	if (m_dirp)
	{
		closedir(m_dirp);
	}
}

void CFileFindHandle::SetSearchFileSpec(LPCTSTR lpFileName)
{
	_tcscpy(m_szFullSearch, lpFileName);

	TCHAR* pSplit = _tcsrchr(m_szFullSearch, PATHSPLIT);
	if (pSplit)
	{
		*pSplit++ = '\0';
		m_pszSpec = pSplit;
		m_pszPath = m_szFullSearch;
	}
	else
	{
		m_pszPath = CURR_PATH;
		m_pszSpec = m_szFullSearch;
	}

	m_dirp = opendir(m_pszPath);
}


HANDLE FindFirstFile(LPCTSTR lpFileName,LPWIN32_FIND_DATA lpFindFileData)
{
	CFileFindHandle* pHandle = new CFileFindHandle();
	pHandle->SetSearchFileSpec(lpFileName);

	if (pHandle->IsOpen() && FindNextFile((HANDLE)pHandle, lpFindFileData))
	{
		return (HANDLE)pHandle;
	}
	delete pHandle;
	return INVALID_HANDLE_VALUE;
}

BOOL FindNextFile(HANDLE hFindFile,LPWIN32_FIND_DATA lpFindFileData)
{
	CFileFindHandle* pHandle = (CFileFindHandle*)hFindFile;

	struct dirent de;

	do
	{
		int nError = readdir_r(pHandle->m_dirp, &de, &pHandle->dp);
		if(nError != 0)
		{
			SetLastError((DWORD)nError);
			pHandle->dp = NULL;
			break;
		}
	}while(pHandle->dp && !PathMatchSpec(pHandle->dp->d_name, pHandle->m_pszSpec));

	if (pHandle->dp != NULL)
	{
		_tcsncpy(lpFindFileData->cFileName, pHandle->dp->d_name, MAX_PATH);
		lpFindFileData->dwFileAttributes = 0;

		TCHAR szFilePath[MAX_PATH];
		_tcscpy(szFilePath, pHandle->m_pszPath);
		_tcscat(szFilePath, PATHSPLITSTRING_S);
		_tcscat(szFilePath, pHandle->dp->d_name);

		lpFindFileData->dwFileAttributes = INVALID_FILE_ATTRIBUTES;

		struct _stat buf;
		int result = _stat(szFilePath, &buf);
		if (result == 0)
		{
			lpFindFileData->dwFileAttributes = ChangeFileAttributes(buf);
			
			lpFindFileData->ftCreationTime   = buf.st_ctime;
			lpFindFileData->ftLastAccessTime = buf.st_atime;
			lpFindFileData->ftLastWriteTime  = buf.st_mtime;
			lpFindFileData->nFileSizeLow     = (DWORD)buf.st_size;
			lpFindFileData->nFileSizeHigh    = 0;
		}
	}
	return pHandle->dp != NULL;
}

BOOL FindClose(HANDLE hFindFile)
{
	CFileFindHandle* pHandle = (CFileFindHandle*)hFindFile;
	if (pHandle != INVALID_HANDLE_VALUE)
	{
		delete pHandle;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static LPCTSTR StepSpec(LPCTSTR pszFileParam, LPCTSTR pszSpec)
{
	int nParamLen = _tcslen(pszFileParam);
	int nSpecLen = _tcslen(pszSpec);
	int nParamIndex = 0;
	int nSpecIndex = 0;

	while(nParamIndex < nParamLen && nSpecIndex < nSpecLen)
	{
		if (pszFileParam[nParamIndex] == pszSpec[nSpecIndex]
		|| pszSpec[nSpecIndex] == '?')
		{
			++ nParamIndex; ++ nSpecIndex;
		}
		else
		{
			nParamIndex -= (nSpecIndex - 1);
			nSpecIndex = 0;
		}
	}
	if (nSpecIndex == nSpecLen)
	{
		return pszFileParam + nParamIndex;
	}
	return NULL;
}

static BOOL MatchLast(LPCTSTR pszFileParam, LPCTSTR pszSpec)
{
	if ( '\0' == *pszSpec)
		return TRUE;

	int nSpecLen = _tcslen(pszSpec); 
	int nFileParamLen = _tcslen(pszFileParam);

	if (nSpecLen > nFileParamLen)
		return FALSE;

	return 	_tcsicmp(pszFileParam + nFileParamLen - nSpecLen, pszSpec) == 0;
}

BOOL PathMatchSpec(LPCTSTR pszFileParam, LPCTSTR pszSpec)
{
	assert(NULL != pszFileParam);
	assert(NULL != pszSpec);

	BOOL bMatch = TRUE;
	TCHAR szTemp[_MAX_PATH];
	_tcscpy(szTemp, pszSpec);

	TCHAR* p = szTemp;
	TCHAR* e = p;
	do
	{
		e = _tcschr(p, '*');
		if (e != NULL)
		{
			*e++ ='\0';
		}
		else
		{
			bMatch = MatchLast(pszFileParam, p);
			break;
		}
		pszFileParam = StepSpec(pszFileParam, p);
		if (NULL == pszFileParam)
		{
			bMatch = FALSE;
			break;
		}
		p = e;
	}while(p != NULL);

	return bMatch;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//__NS_BASIC_END
#endif //#ifdef __LINUX __MAC
