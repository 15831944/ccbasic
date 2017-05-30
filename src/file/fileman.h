/***********************************************************************************************
// �ļ���:     fileman.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 12:42:26
// ��������:   �����ļ������һЩȫ�ֺ�������
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_FILEMAN_H
#define BASIC_FILEMAN_H

#pragma once

__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//����
//class CBasicObject;
	class CBasicFileFind;				//�ļ�����

/////////////////////////////////////////////////////////////////////////////////////////////
#define WSTR_INVALID(x) (x == NULL || x[0] == _T('\0'))					//!< �ж��ַ���ָ��
#define STR_INVALID(x) (x == NULL || x[0] == '\0')					//!< �ж��ַ���ָ��


//<a class="el" href="fileman_8h-source.html">fileman.h</a>��<a class="el" href="fileman_8h-source.html#l00253">253</a>�ж��塣

//! ȡ�ļ�ȫ·��
/*! 
*\param lpszPathOut һ��ָ��������������ڴ�ָ�룬���ȫ·��
*\param lpszFileIn �������·��
*\return �ɹ�: BASIC_FILE_OK��ʧ��: ���� BASIC_FILE_* �������ļ� filedefine.h��
*\remarks 
*\warning 1�����ܼ��ȫ·���Ƿ���� 2���뵱ǰ����Ŀ¼���
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/
long _BASIC_DLL_API Basic_GetFileFullPath(char* lpszPathOut, const char* lpszFileIn);

//! ����ļ�״̬
/*! 
*\param lpszFileName �ļ�·��
*\param rStatus һ��ָ��������������ڴ�ָ�룬�ļ�״̬TLFileStatusW�ṹ
*\return �ɹ�: BASIC_FILE_OK��ʧ��:  ���� BASIC_FILE_* �������ļ� filedefine.h��
*\remarks 
*\warning 
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/
long _BASIC_DLL_API Basic_GetFileStatus(const char* lpszFileName, TLFileStatus& rStatus);
//! ����ļ���
/*! 
*\param lpszPathName �ļ�·��
*\param lpszName һ��ָ��������������ڴ�ָ�룬����ļ���
*\param nMax ָ���ڶ�������(lpszName)�ĳ��ȣ����ʵ��Ҫ�����ļ�������������ȣ���ʧ�ܲ�����ʵ��Ҫ���ص��ļ�������
*\return �ɹ�: BASIC_FILE_OK��ʧ��: ����ʵ��Ҫ���ص��ļ������ȣ�
*\remarks 
*\warning �ڶ�����������ΪNULL
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/	
long _BASIC_DLL_API Basic_GetFileName(const char* lpszPathName, char* lpszName, int nMax);

//! ��ò�����չ�����ļ���
/*! 
*\param lpszPathName �ļ�·��
*\param lpszTitle һ��ָ��������������ڴ�ָ�룬���������չ�����ļ���
*\param nMax ָ���ڶ�������(lpszTitle)�ĳ��ȣ����ʵ��Ҫ�����ļ�������������ȣ���ʧ�ܲ�����ʵ��Ҫ���ص��ļ�������
*\return �ɹ�: BASIC_FILE_OK��ʧ��: ����ʵ��Ҫ���ص��ļ������ȣ�
*\remarks 
*\warning 
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/
long _BASIC_DLL_API Basic_GetFileTitle(const char* lpszPathName, char* lpszTitle, int nMax);
//! ��ȫ·���ļ����У�ȡ��·��
/*! 
*\param lpszPathName �ļ�·��
*\param lpszDirPath һ��ָ��������������ڴ�ָ�룬����ļ����ڵ��ļ���·��
*\param nMax ָ���ڶ�������(lpszDirPath)�ĳ��ȣ����Ϊ_MAX_PATH�����ʵ��Ҫ�����ļ�������������ȣ���ʧ�ܲ�����_MAX_PATH
*\return �ɹ�: BASIC_FILE_OK��ʧ��: ����_MAX_PATH
*\remarks 
*\warning �ڶ�����������ΪNULL
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/
long _BASIC_DLL_API Basic_GetFileDirPath(const char* lpszPathName, char* lpszDirPath, int nMax);
//! ȡ�ļ�����
/*! 
*\param lpszFileName �ļ�·��
*\return �ɹ�: ����һ����ʶ�ļ������ļ������Ե�DWORD��ʧ��: ͨ��GetFileErrorID()��ô�����Ϣ��
*\remarks 
*\warning 
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/
DWORD _BASIC_DLL_API WBasic_GetFileAttributes(LPCTSTR lpszFileName);
DWORD _BASIC_DLL_API Basic_GetFileAttributes(const char* lpszFileName);

//! �����ļ�״̬
/*! 
*\param lpszFileName �ļ�·��
*\param status һ��ָ���ļ�״̬�ṹTLFileStatusW�ĳ�ָ��
*\return �ɹ�: BASIC_FILE_OK��ʧ��: ͨ��GetFileErrorID()��ô�����Ϣ��
*\remarks 
*\warning 
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>//!!!�ȴ�����
*/
long _BASIC_DLL_API WBasic_SetFileStatus(LPCTSTR lpszFileName, const TLFileStatusW& rStatus);
long _BASIC_DLL_API Basic_SetFileStatus(const char* lpszFileName, const TLFileStatus& rStatus);
//! �������ļ������ļ���
/*! 
*\param lpszOldName Ҫ�����������ļ������ļ��е�����
*\param lpszNewName ���ļ������ļ��е�����
*\return �ɹ�: BASIC_FILE_OK��ʧ��: ͨ��GetFileErrorID()��ô�����Ϣ��
*\remarks ʵ�ʵ���WinAPI��MoveFile(yhm??)
*\warning 1�����ļ����ļ��в����Ѿ����� 2�����ļ������ڲ�ͬ���ļ�ϵͳ�£��������ļ��б����������ļ�������ͬ����������
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/
long _BASIC_DLL_API WBasic_RenameFile(LPCTSTR lpszOldName, LPCTSTR lpszNewName);
long _BASIC_DLL_API Basic_RenameFile(const char* lpszOldName, const char* lpszNewName);
//! ɾ���ļ�
/*! 
*\param lpszFileName Ҫ��ɾ�����ļ���
*\return �ɹ�: BASIC_FILE_OK��ʧ��: ͨ��GetFileErrorID()��ô�����Ϣ��
*\remarks ʵ�ʵ���WinAPI��DeleteFile
*\warning 1������ɾ��ֻ���ļ�
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/
long _BASIC_DLL_API WBasic_DeleteFile(LPCTSTR lpszFileName);
long _BASIC_DLL_API Basic_DeleteFile(const char* lpszFileName);
//! �����ļ�
/*! 
*\param lpExistingFileName �Ѿ����ڵ��ļ�
*\param lpNewFileName ���ļ�
*\param bFailIfExists FALSE: ����ļ����ڣ����ǣ�TRUE: ����ļ����ڣ�������
*\return �ɹ�: BASIC_FILE_OK��ʧ��: ͨ��GetFileErrorID()��ô�����Ϣ��
*\remarks ʵ�ʵ���WinAPI��CopyFile
*\warning 
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/
long _BASIC_DLL_API WBasic_CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists);
long _BASIC_DLL_API Basic_CopyFile(const char* lpExistingFileName, const char* lpNewFileName, BOOL bFailIfExists);

//! �ж��Ƿ�ͨ���
/*! 
*\param pszFile ����������ƥ���·���ַ������MAX_PATH
*\param pszSpec ͨ���'*','?'����Ҫ��WORD�ļ���д��"*.doc"�MAX_PATH
*\return �ɹ�:TRUE��ʧ��:FALSE
*\remarks ʵ�ʵ���WinAPI��PathMatchSpec
*\warning 
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/
BOOL _BASIC_DLL_API WBasic_PathMatchSpec(LPCTSTR pszFile, LPCTSTR pszSpec);
BOOL _BASIC_DLL_API Basic_PathMatchSpec(const char* pszFile, const char* pszSpec);
//! ȡ���ļ���ָ��
/*! 
*\param lpszPathName �ļ�·��
*\return �ļ���ָ��
*\remarks ���ص��ļ���ָ��ֻ����������� lpszPathName �е�ƫ�ƣ�ͬʱ֧�� '/' '\\'
*\warning 
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/	
_BASIC_DLL_API char* Basic_FindFileName(const char* lpszPathName);

//! ����Ŀ¼
/*! 
*\param lpszPath Ҫ������Ŀ¼��
*\return �ɹ�: BASIC_FILE_OK��ʧ��: BASIC_FILE_MKDIR_ERROR��
*\remarks ������� lpszPath ͬʱ֧��'/'��'\\'���������һ��'/'��'\\'������ַ�ǰ���������Ϊ��Ҫ������Ŀ¼����������ݿ������ļ�����������Ŀ¼��
*\remarks ��Ҫ����Ŀ¼ basic��Ҫ���� d:\\basic\\ ���� d:/basic/
*\warning 
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/
long _BASIC_DLL_API Basic_mkdir(const char* lpszPath);
long _BASIC_DLL_API WBasic_mkdir(LPCTSTR lpszPath);

_BASIC_DLL_API char* Basic_TempFileName(const char* lpszDir, const char* lpszHead, const char* lpszExt, char* lpszBuffer, int nMax);

#define BASIC_FO_MOVE           0x0001			//!< �ļ��ƶ�
#define BASIC_FO_COPY           0x0002			//!< �ļ�����
#define BASIC_FO_DELETE         0x0003			//!< �ļ�ɾ��
#define BASIC_FO_RENAME         0x0004			//!< �ļ�����

//! �ļ�Ŀ¼����
/*! 
*\param wFunc �ļ�����ѡ��
*  <ul>
*  <li> �ļ�����ѡ��
*     <ol>
*     <li> BASIC_FO_MOVE		�ļ��ƶ�
*     <li> BASIC_FO_COPY		�ļ�����
*     <li> BASIC_FO_DELETE		�ļ�ɾ��
*     <li> BASIC_FO_RENAME		�ļ�����
*     </ol>
*  </ul>
*\param pFrom ԭ�ļ�����Ŀ¼��
*\param pTo Ŀ���ļ�����Ŀ¼��
*\return 
*\remarks 
*\warning  ֧��ɾ�������ļ�
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/
long _BASIC_DLL_API Basic_FileOperation(uint32_t wFunc, const char* pFrom, const char* pTo);

#define BASIC_FIND_SUBDIR			0x0001			//!< ������Ŀ¼
#define BASIC_FIND_DIR				0x0002			//!< ����Ŀ¼������

//! ����Ŀ¼����������ļ�
/*! 
*\param lpszFilePath ָ��Ҫ���ҵ��ļ�·��
*\param lpszFileName ͨ���
*\param dwFindMode ���ҵ�ѡ�� �����壺BASIC_FIND_* ��BASIC_FIND_SUBDIR:������Ŀ¼  BASIC_FIND_DIR������Ŀ¼������
*\param f ģ�庯�����Բ��ҵ����ļ����д�������ԭ�� long func(LPCTSTR lpszFileName, time_t tmCreateTime, time_t tmModifyTime, long lFileLength, BYTE cAttr)
          cAttr �ļ������ԣ������� enum TLFileAttribute 
*\return ���ش�·���µķ����������ļ���
*\remarks ��� lpszFilePath ���ļ��������� lpszFileName ��Ч�����Ҫ����ȫ�������⣬lpszFileName = NULL
*\warning
*/

long _BASIC_DLL_API Basic_FindAllFileInPath(const char* lpszFilePath, const char* lpszFileName, DWORD dwFindMode, const std::function<long(const char*, time_t, time_t, long, BYTE)>& f);

//! ����Ŀ¼����������ļ�
/*! 
*\param lpszFilePath ָ��Ҫ���ҵ��ļ�·��
*\param lpszFileName ͨ���
*\param ayFile һ��CWBasicStringArrayָ��
*\param dwFindMode ���ҵ�ѡ�� �����壺BASIC_FIND_* ��BASIC_FIND_SUBDIR:������Ŀ¼
*\return ���ش�·���µķ����������ļ���
*\remarks ��� lpszFilePath ���ļ��������� lpszFileName ��Ч�����Ҫ����ȫ�������⣬lpszFileName = NULL
*\warning
*\sa <a href = "sample\file_test\fileman_TEST.cpp">fileman_TEST.cpp</a>
*/
long _BASIC_DLL_API Basic_FindAllFileInPath(const char* lpszFilePath, const char* lpszFileName, CBasicStringArray& ayFile, DWORD dwFindMode);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
class _BASIC_DLL_API CBasicFileFind : public CBasicObject
{
public:
	//! ���캯��
	/*!
	*  ��ʼ�����г�Ա����
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	CBasicFileFind();
	//! ��������
	/*!
	*  �ͷŷ�����ڴ�
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	virtual ~CBasicFileFind();

	// Attributes
public:

	//! ����ļ�����
	/*!
	*  ����һ��ULONGLONG�͵��ļ�����
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	ULONGLONG GetLength() const;

	//! ����ļ���
	/*!
	*  ����һ��CWBasicString�͵��ַ���
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	CBasicString GetFileName() const;


	//! ����ļ����ڵ��ļ���·��
	/*!
	*  ����һ��CWBasicString�͵��ַ���
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	CBasicString GetFilePath() const;

	//! ��ò����ļ���չ�����ļ���
	/*!
	*  ����һ��CWBasicString�͵��ַ���
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	CBasicString GetFileTitle() const;

	//! ��������ļ���URL
	/*!
	*  ����һ��CWBasicString�͵��ַ���
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	CBasicString GetFileURL() const;

	//! ����ļ����ڵ�Ŀ¼�����Ǹ�Ŀ¼
	/*!
	*  ����һ��CWBasicString�͵��ַ���
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	CBasicString GetRoot() const;

	//! ����ļ�����޸�ʱ��
	/*!
	*  ����һ��time_t��ʱ��
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	time_t GetLastWriteTime() const;

	//! ����ļ�������ʱ��
	/*!
	*  ����һ��time_t��ʱ��
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	time_t GetLastAccessTime() const;

	//! ����ļ�����ʱ��
	/*!
	*  ����һ��time_t��ʱ��
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	time_t GetCreationTime() const;

	///////group0812
	/** @name �ļ������ж�
	*/
	//@{
	///////group0812

	//! ƥ���ļ�����
	/*!
	*  ����TURE: ���д����ԣ�����FALSE: �����д����ԣ�
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	BOOL MatchesMask(DWORD dwMask) const;

	//! �ļ��Ƿ���Ŀ¼
	/*!
	*  ����TURE: ��Ŀ¼������FALSE: ����Ŀ¼��
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	BOOL IsDots() const;

	//! �ļ��Ƿ�ֻ��
	/*!
	*  ����TURE: ֻ��������FALSE: ��ֻ����
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	BOOL IsReadOnly() const;

	//! �ļ��Ƿ���Ŀ¼
	/*!
	*  ����TURE: ��Ŀ¼������FALSE: ����Ŀ¼��
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	BOOL IsDirectory() const;

	//! �ļ��Ƿ�ѹ��
	/*!
	*  ����TURE: �ǣ�����FALSE: ��
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	BOOL IsCompressed() const;

	//! �ļ��Ƿ���ϵͳ�ļ�
	/*!
	*  ����TURE: �ǣ�����FALSE: ��
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	BOOL IsSystem() const;

	//! �ļ��Ƿ�����
	/*!
	*  ����TURE: �ǣ�����FALSE: ��
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	BOOL IsHidden() const;

	//! �ļ��Ƿ�����ʱ�ļ�
	/*!
	*  ����TURE: �ǣ�����FALSE: ��
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	BOOL IsTemporary() const;

	//! �ļ��Ƿ���һ�����ԣ������߱������κ�����
	/*!
	*  ����TURE: �ǣ�����FALSE: ��
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	BOOL IsNormal() const;

	//! �ļ��Ƿ�鵵����
	/*!
	*  ����TURE: �ǣ�����FALSE: ��
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	BOOL IsArchived() const;

	///////group0812
	//@}
	///////group0812


	///////group0812
	/** @name �ļ�����Operations
	*/
	//@{
	///////group0812

	// Operations

	//! �ر��ļ����Ҿ��
	/*!
	*  �ر��ļ����Ҿ��
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	void Close();

	//! �����ļ�
	/*!
	*\param lpszFilePath �ļ�·��
	*\param lpszFileName �ļ��������=NULL����ʾ�������е��ļ������ lpszFilePath ���ļ������ò�����Ч��
	*\return �ɹ�: BASIC_FILE_OK��ʧ��: ͨ��GetFileErrorID()��ô�����Ϣ��
	*\remarks
	*\warning
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	long FindFile(const char* lpszFilePath, const char* lpszFileName = NULL);

	//! ������һ���ļ�
	/*!
	*  �ڵ���FindFile����Ҫ�������������֮����ܵ�����������
	*\sa <a href = "sample\file_test\CBasicFileFindW_TEST.cpp">CBasicFileFindW_TEST.cpp</a>
	*/
	long FindNextFile();

	///////group0812
	//@}
	///////group0812

protected:
	void CloseContext();		//!< �رղ���������

protected:
	void* m_pFoundInfo;			//!< �Ѿ����ҵ����ļ���Ϣ
	void* m_pNextInfo;			//!< ��һ���ļ���Ϣ
	HANDLE m_hContext;			//!< �ļ��������
	CBasicString m_strRoot;		//!< �ļ�Ŀ¼·��
};
//////////////////addtogroup0812
/** @} */
//////////////////addtogroup0812

//·���ָ�������
#ifdef __BASICWINDOWS
#define WIDEPATHSPLIT 				_T('\\')		//�����ַ�
#define PATHSPLIT_S					'\\'
#define PATHSPLITSTRING_S			"\\"			//�ַ���
#else
#define WIDEPATHSPLIT 				_T('/')			//�����ַ�
#define PATHSPLIT_S 				'/'			//�����ַ�
#define PATHSPLIT					PATHSPLIT_S
#define PATHSPLITSTRING_S			"/"				//�ַ���
#define PATHSPLIT_OTHER				'\\'		//�����ַ�
#define PATHSPLITSTRING_OTHER		"\\"		//�ַ���
#endif

//! ��ʽ��·��
/*! 
*\param strPath ·��
*/
void _BASIC_DLL_API Basic_RegulatePathString(CBasicString& strPath);

//! ��ʽ���ļ���
/*! 
*\param strFileName �ļ���
*/
void _BASIC_DLL_API Basic_RegulateFileNameString(CBasicString& strFileName);
__NS_BASIC_END
/////////////////////////////////////////////////////////////////////////////////////////////

#endif 
