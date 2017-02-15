/***********************************************************************************************
// �ļ���:     fileobj.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 12:43:13
// ��������:   �����ļ��������
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_FILEOBJ_H
#define BASIC_FILEOBJ_H

#pragma once
#pragma	pack(1)
__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//����
//�ļ��򿪷�ʽ
//�����ļ�����ֻ�����д�����������Ʒ�ʽ��
//ȱʡ��ʽ������һ�����ļ�������ļ����ڣ����ݲ��䡣
#define PF_OPEN_ONLY	0x0001		/*!< ����ļ������ڣ���ʧ�ܡ�*/
#define PF_OPEN_TRUN	0x0002		/*!< ����ļ����ڣ�������Ϊ�㡣*/
#define PF_READ_ONLY	0x0004		/*!< ���ļ���ֻ����*/
#define PF_CREATE_ONLY	0x0008		/*!< ����ļ����ڣ���ʧ�ܣ����򴴽�һ�����ļ���*/
#define PF_MEM_ONLY		0x0010		/*!< �ڴ��ļ������ܸı��С*/

//�ļ�����
#define PF_FILE_MASK	0xF000		/*!< �ļ�����*/

#define PF_DISK_FILE	0x2000		/*!< �����ļ�*/
#define PF_MEM_FILE		0x3000		/*!< �ڴ��ļ�*/

//class CBasicObject;
	class CBasicFileObj;				//�ļ��������
/////////////////////////////////////////////////////////////////////////////
class CFileBase;
//! ��װ�˶Դ����ļ����ڴ��ļ���һϵ�в���
/*!
* ��װ�˶��ļ���һϵ�в������������ļ����ݵĶ�д�����ļ��Ŀ�����ɾ�����ļ�״̬�Ĳ���
*/
class _BASIC_DLL_API CBasicFileObj : public basiclib::CBasicObject
{
	//BASIC_DECLARE_DYNCREATE(CWBasicFileObj)
public:
	// Constructors  
	//! ���캯��
	/*!
	*  ��ʼ�����еĳ�Ա������
	*/
	CBasicFileObj();

	//! ���캯�� 
	/*!
	* ���������ڴ��ļ�����Ĺ��캯��
	*\param pBuffer ��������ʼ���ڴ��ļ���������ݿ飬���������꣬�ö�����Ա�����
	*\param lCount  ָ������1�������ݿ�ĳ���
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	CBasicFileObj(void* pBuffer, long lCount);

	//! ���캯��
	/*!
	* �������������ļ�����Ĺ��캯���������ο�Open����˵��
	*\param lpszFileName
	*\param dwOpenFlags
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	CBasicFileObj(const char* lpszFileName, DWORD dwOpenFlags);

	//! ��������
	/*!
	*/
	virtual ~CBasicFileObj();

	// Attributes
	//! \brief �ж�ָ�����ļ��Ƿ��ڴ�״̬ 
	/*!
	*\return TRUE:��ʾ�ļ��Ǵ򿪵�;FALSE:�ļ�û�д�
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	BOOL IsOpen() const;

	//! \brief �ж�ָ�����ļ��Ƿ��ڽ�ֹд״̬
	/*!
	*\return TRUE:��ʾ�ļ���ֻ��;FALSE:�ļ���ֻ��,Ҳ�����ļ�û�д�
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	BOOL IsReadOnly() const { return  (m_dwOpenFlags & PF_READ_ONLY); }

	//! \brief �ж�ָ�����ļ��Ƿ�ֻ��״̬
	/*!
	*\return TRUE:��ʾ�ļ���OpenOnly;FALSE:�ļ���OpenOnly,Ҳ�����ļ�û�д�
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	BOOL IsOpenOnly() const { return  (m_dwOpenFlags & PF_OPEN_ONLY); }

	//! \brief �ж�ָ�����ļ��Ƿ����ڴ��ļ�
	/*!
	*\return TRUE:��ʾ��ǰ���������ڴ��ļ�;FALSE:���ڴ��ļ�,Ҳ�����ļ�û�д�
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	BOOL IsMemoryFile() const;

	//! \brief �ж�ָ�����ļ��Ƿ���ֻ�����ڴ��ļ�
	/*!
	*\return TRUE:��ʾ��ǰ��������ֻ�����ڴ��ļ�;FALSE:��ֻ�������߷��ڴ��ļ�������û�д�
	*/
	BOOL IsMemoryOnly() const { return  (m_dwOpenFlags & PF_MEM_ONLY); }

	//! \brief ����ļ���������ǰ��λ��
	/*!
	*\return ����>=0�ĵ�ǰλ��
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	long GetPosition() const;

	//! \brief ����ļ��Ѿ����ļ���״̬
	/*!
	*����ļ��Ѿ����ļ���״̬
	*\param rStatus �ļ�״̬�Ľṹ
	*\return ����������óɹ����� BASIC_FILE_OK ���򷵻�һ����0��ֵ(������ filedefine.h)
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	long GetStatus(TLFileStatus& rStatus) const;


	//! \brief ����ļ������ڵ�Ŀ¼
	/*!
	*\return ����������óɹ����ش��ļ�����Ŀ¼(����·��)
	*\remarks �������OpenMemFile�򿪺���CWBasicFileObj(void* pBuffer, long lCount)����Ķ��󽫷��ؿ�
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	CBasicString GetFileDirPath() const;

	//! \brief ����ļ���
	/*!
	*\return ����������óɹ����ش��ļ����ļ���
	*\remarks �������OpenMemFile�򿪺���CWBasicFileObj(void* pBuffer, long lCount)����Ķ��󽫷��ؿ�
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	CBasicString GetFileName() const;

	//! \brief ���û�к�׺���ļ���
	/*!
	*\return ����������óɹ����ش��ļ����ļ���
	*\remarks �������OpenMemFile�򿪺���CWBasicFileObj(void* pBuffer, long lCount)����Ķ��󽫷��ؿ�
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	CBasicString GetFileTitle() const;

	//! \brief ����ļ���ȫ·��
	/*!
	*\return ����������óɹ����ش��ļ���ȫ·��
	*\remarks �������OpenMemFile�򿪺���CWBasicFileObj(void* pBuffer, long lCount)����Ķ��󽫷��ؿ�
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	CBasicString GetFilePath() const;

	//! \brief �ж��ļ��Ƿ�Ϸ�
	/*!
	*\return ����������óɹ�����TRUE�����򷵻�FALSE��
	*\remarks �����������Ҫ������������������ж��нṹ���ļ��Ƿ�Ϸ���
	*/
	virtual BOOL IsValidFile() { return TRUE; }
	// Operations
	//! \brief ���ļ�
	/*!
	*\param lpszFileName �ļ���·��(����·�������·��)
	*\param dwOpenFlags  �ļ�����ѡ��
	*  <ul>�ļ�����ѡ��
	*     <ol>
	*     <li> PF_OPEN_ONLY		����ļ������ڣ���ʧ�� </li>
	*     <li> PF_OPEN_TRUN		����ļ����ڣ�������Ϊ��</li>
	*     <li> PF_READ_ONLY		���ļ���ֻ��</li>
	*     <li> PF_CREATE_ONLY	����ļ����ڣ���ʧ�ܣ����򴴽�һ�����ļ�</li>
	*     <li> PF_MEM_ONLY		�ڴ��ļ������ܸı��С</li>
	*     <li> PF_DISK_FILE		�����ļ�</li>
	*     <li> PF_MEM_FILE		����ļ����ڣ���ʧ�ܣ����򴴽�һ�����ļ�</li>
	*     </ol>
	*  </ul>
	*\return ����������óɹ����� BASIC_FILE_OK ���򷵻�һ����0��ֵ (������ filedefine.h)
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	virtual long Open(const char* lpszFileName, DWORD dwOpenFlags);

	//! \brief ���ڴ��ļ�
	/*!
	*\param pBuffer ��������ʼ���ڴ��ļ���������ݿ飬���������꣬�ö�����Ա�����
	*\param lCount  ָ������1�������ݿ�ĳ���
	*\param lLength ָ�������ڴ��ļ��Ĵ�С�����lLength < lCount �� lLength = lCount
	*\return ����������óɹ����� BASIC_FILE_OK ���򷵻�һ����0��ֵ(������ filedefine.h)
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	virtual long OpenMemFile(void* pBuffer, long lCount, long lLength = 0);

	//! \brief ���´��ļ�
	/*!
	*\return ����������óɹ����� BASIC_FILE_OK ���򷵻�һ����0��ֵ
	*\remarks �������OpenMemFile�򿪺���CWBasicFileObj(void* pBuffer, long lCount)����Ķ���õ��ý�ʧ��
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	virtual long ReOpen();

	//! \brief ���ļ��Ĳ���λ�����õ����
	/*!
	*\return ���ص�ǰ��λ��
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	long SeekToEnd();

	//! \brief ���ļ��Ĳ���λ�����õ���ʼ
	/*!
	*\return ���ص�ǰ��λ��
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	long SeekToBegin();

	// Overridables
	//! \brief �����ļ��ĵ�ǰλ��
	/*!
	*\param lOff  �������Ӧ����2��ƫ��
	*\param nFrom
	*  <ul>
	*     <ol>����ƫ�Ƶ����ֵ
	*     <li> BASIC_FILE_BEGIN		�ļ�ͷ
	*     <li> BASIC_FILE_CURRENT		��ǰλ��
	*     <li> BASIC_FILE_END			�ļ�β
	*     </ol>
	*  </ul>
	*\return ���ص�ǰ��λ��
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	virtual long Seek(long lOff, uint32_t nFrom);

	//! \brief �����ļ��ĳ���
	/*!
	*\param lNewLen  �����ļ����³���
	*\param cFill    �����ݵ����ֵ
	*\return ����������óɹ����� BASIC_FILE_OK ���򷵻�һ����0��ֵ (�������� filedefine.h)
	*\remarks ����������� PF_MEM_ONLY �򿪵��ļ�����֧�ָù��ܣ�����BASIC_FILE_SETLEN_DENIED
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	virtual long SetLength(long lNewLen, char cFill = 0);

	//! \brief ����ļ��ĳ���
	/*!
	*\return ���صĳ���
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	virtual long GetLength();

	//! \brief ����ļ��ĳ���
	/*!
	*\param lpBuf  ��ǰ׼����������Ŷ������ݵĻ���
	*\param lCount ָ������Ĵ�С
	*\return ����0���ض������ݵĳ��ȣ�С��0�Ǵ�����루�� filedefine.h��
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	virtual long Read(void* lpBuf, long lCount);

	//! \brief ��ָ������д�뵽�ļ���
	/*!
	*\param lpBuf  Ҫд���ļ������ݵĵ�ַ
	*\param lCount Ҫд���ļ������ݵĴ�С
	*\param lRepeat Ҫд���ļ���������Ҫ�ظ�д��Ĵ�����Ĭ����1
	*\param cFill   �����ݵ����ֵ,Ĭ����0
	*\return ����0����д�����ݵĳ��ȣ�С��0�Ǵ�����루�� filedefine.h��
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	virtual long Write(const void* lpBuf, long lCount, long lRepeat = 1, char cFill = 0);

	//! \brief ��ָ�����ݲ����뵽�ļ���
	/*!
	*\param lpBuf  Ҫд���ļ������ݵĵ�ַ
	*\param lCount Ҫд���ļ������ݵĴ�С
	*\param lRepeat Ҫд���ļ���������Ҫ�ظ�д��Ĵ�����Ĭ����1
	*\param cFill   �����ݵ����ֵ,Ĭ����0
	*\return ����0���ز������ݵĳ��ȣ�С��0�Ǵ�����루�� filedefine.h��
	*\remarks �ú����Ĳ�����Write�����ƣ��������Seek����һ��ʹ�ã�ָ������ĵ�ǰλ��
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	virtual long Insert(const void* lpBuf, long lCount, long lRepeat = 1, char cFill = 0);

	//! \brief ˢ���ļ��Ļ���
	/*!
	*/
	virtual long Flush();

	//! \brief ��¡��һ���µ��ļ���������
	/*!
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	//virtual CWBasicFileObj* Duplicate() const;

	//! \brief �ر��ļ�
	/*!
	*/
	virtual void Close();

	//! \brief ����ǰ�ļ������ݿ�����ָ�����ļ��� 
	/*!
	*\remarks �����ڴ��ļ������lpszFileName=NULL���൱�ڵ�ǰ�ڴ�����ݴ��̡�
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	virtual long CopyTo(const char* lpszFileName);

	//! \brief ��ָ���ļ������ݿ�������ǰ�ļ�
	/*!
	*\remarks �����ڴ��ļ������lpszFileName=NULL���൱�����´Ӵ��̶������ݵ��ڴ档
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	virtual long CopyFrom(const char* lpszFileName);

	//! \brief �� pFile ���ļ����ݸ��Ƶ�������
	/*!
	*\remarks �����ļ�������򿪡�
	*/
	virtual long CopyFromFile(CBasicFileObj* pFile);

	//! \brief �� pFile �и������ݵ��ڴ��ļ�
	/*!
	*\remarks pFile����򿪣��� this ����û�д򿪡�
	*/
	virtual long OpenMemFromFile(CBasicFileObj* pFile);

	//! \brief �ѱ������Ƶ��ڴ��ļ���
	/*!
	*\remarks ���������򿪣����ҺϷ���
	*/
	//virtual long CloneToMemFile(CWBasicFileObj*& pNewFile);

	//! \brief ��һ����¡�ļ�
	/*!
	*\remarks
	*/
	//virtual long OpenCloneFile(LPCTSTR lpszFileName, CWBasicFileObj*& pNewFile, DWORD dwOpenFlags);

	//only for memory files

	//! \brief ����ļ������ݻ���
	/*!
	*\remarks ֻ���ڴ��ļ������ܷ��ػ����ַ�������ļ������ؿյ�ַ
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	void* GetDataBuffer();

	//! \brief ����ļ��ĵ�ǰ���ݵ�ַ
	/*!
	*\param lCount ���úͷ��ط��ص����ݳ���
	*\remarks ֻ���ڴ��ļ������ܷ��ػ����ַ�������ļ������ؿյ�ַ���ú������ļ��ĵ�ǰλ���йأ�Ӧ�úͺ���Seek���ʹ��
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	void* GetCurDataBuffer(long& lCount);
public:
	//! \brief �������ļ�
	/*!
	*\param lpszOldName �����ļ�������(���·�������·��)
	*\param lpszNewName ��Ҫ�����������ļ���(���·�������·��)
	*\remarks �ú�����static��Ա���������Բ���Ҫ����Ϳ��Ե���
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	static long Rename(const char* lpszOldName, const char* lpszNewName);

	//! \brief ɾ���ļ�
	/*!
	*\param lpszFileName �����ļ�������(���·�������·��)
	*\remarks �ú�����static��Ա���������Բ���Ҫ����Ϳ��Ե���
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	static long Remove(const char* lpszFileName);

	//! \brief ɾ���ļ�
	/*!
	*\param lpszFileName �����ļ�������(���·�������·��)
	*\param rStatus	     �������Σ������ļ���״̬
	*\remarks �ú�����static��Ա���������Բ���Ҫ����Ϳ��Ե���
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	static long GetStatus(const char* lpszFileName, TLFileStatus& rStatus);

	//! \brief ɾ���ļ�
	/*!
	*\param lpszFileName �����ļ�������(���·�������·��)
	*\param rStatus	     ��Ҫ���õ��ļ�״̬
	*\remarks �ú�����static��Ա���������Բ���Ҫ����Ϳ��Ե���
	*\sa <a href = "sample\file_test\CWBasicFileObj_TEST.cpp">CWBasicFileObj_TEST.cpp</a>
	*/
	static long SetStatus(const char* lpszFileName, const TLFileStatus& rStatus);

protected:
	DWORD GetFileOpenType() const { return (m_dwOpenFlags & PF_FILE_MASK); }            /*!< �����ļ��򿪵��ļ����� */
	DWORD GetFileOpenFlags(DWORD dwMask) const { return (m_dwOpenFlags & dwMask); }     /*!< ȡ�򿪵����� PF_* */

	DWORD GetFileOpenFlags() const { return m_dwOpenFlags; }							/*!< ����ȫ�������� */
	void  SetFileOpenFlags(DWORD dwValue, DWORD dwMask)									/*!< �����ļ������� */
	{
		m_dwOpenFlags &= ~dwMask;
		m_dwOpenFlags |= (dwValue & dwMask);
	}

	void	ClearMember();		/*!< ��ճ�Ա���� */

	long  CopyFileContent(CBasicFileObj* pFile);		/*!< �����ļ����� */
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	DWORD			m_dwOpenFlags;		/*!< �ļ��򿪵����� PF_* */
	CBasicString	m_strFileName;		/*!< �ļ�����ȫ·���� */

	CFileBase*		m_pFileObj;		/*!< �ļ�������� no delete �ں���close() ��ɾ���ö��� */
};
//////////////////addtogroup0812
/** @} */
//////////////////addtogroup0812
__NS_BASIC_END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif 

