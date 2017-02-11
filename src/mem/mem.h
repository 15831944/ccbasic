/***********************************************************************************************
// �ļ���:     mem.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 11:22:09
// ��������:   ���ļ����ȫ�ֵ��ڴ���������Ҫ�����С�ڴ�ķ���Ч��
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_MEM_H
#define BASIC_MEM_H

#include <stdlib.h>
#include <stdint.h>

__NS_BASIC_START

typedef uint32_t (*gethandleid_func)();
_BASIC_DLL_API_C  _BASIC_DLL_API void BindGetHandleIDFunc(gethandleid_func func);

//! �����СΪsize���ڴ�
    /*!
	 *\param size ��Ҫ������ڴ��С
	 *\return �ɹ����ط�����ڴ�ָ�룬ʧ�ܷ���NULL
	 *\remarks �˺���������ڴ������BasicDeallocate�������ͷ�
	 *\sa BasicDeallocate()
	 */
_BASIC_DLL_API_C  _BASIC_DLL_API void* BasicAllocate(size_t size);


//! ���·����ڴ��С,ԭ�������ݱ���
  /*! 
  *\param p ԭʼ������ָ��
  *\param size ����Ҫ����ĳ���
  *\return �ɹ�������ָ���ַ��ɾ��ԭ����ָ�룬�Ƿ񷵻�NULL��ԭ�������ݱ���
  */
_BASIC_DLL_API_C  _BASIC_DLL_API void* BasicReallocate(void* p, size_t size);

//! �ͷ���BasicAllocate����������ڴ�
   /*!
    *\param p ��Ҫ�ͷŵ��ڴ�ָ��
	*\param size �ڴ�ָ��ĳ��ȣ����δ֪����ô���������ݵ�Ŀ��������ͷ�Ч��
	*\return ��
	*\remarks ���ṩ�ڴ�ָ���С��ʱ�򣬿��Լ����ͷŵ��ٶ�
	*\sa BasicAllocate()
	*/
_BASIC_DLL_API_C  _BASIC_DLL_API void BasicDeallocate(void* p);
_BASIC_DLL_API_C  _BASIC_DLL_API char* BasicStrdup(const char* p);

//!ȡ�ڴ���������Ϣ,�˲����ٶȿ�
	/*!
	 *\param nAllocateCount �ܷ������
	 *\param nDeallocateCount ���ͷŴ���
	 *\param nSumAllocate   �ܷ�����ڴ��С(�������ڴ��ж��������)
	 *\param nSumDeallocate ���ͷŵ��ڴ��С(�������ڴ��ж��������)	 
	 *\return ��
	 */
_BASIC_DLL_API_C  _BASIC_DLL_API void  BasicGetOperationInfo(
			size_t& nAllocateCount, 
			size_t& nDeallocateCount,
			size_t& nUseMemory,
			size_t& nAllocateSize,
			size_t& nDeAllocateSize
			);

//!�����ڴ�����ģʽ,����������ʱ���ʼ��
#define MemRunMemCheck_RunFast				0x00000000
#define MemRunMemCheck_RunSizeCheck			0x00000001
#define MemRunMemCheck_RunTongJi			0x00000002
#define MemRunMemCheck_RunCheckMem			0x00000004		
_BASIC_DLL_API_C  void _BASIC_DLL_API BasicSetMemRunMemCheck(uint32_t nMode = 0, int nMin = 0, int nMax = 0);
_BASIC_DLL_API_C  void _BASIC_DLL_API DumpRunMemCheck();

//!���֮ǰ���ڴ�����¼
_BASIC_DLL_API_C  void _BASIC_DLL_API BasicEmptyMemAll();

//!�����ǰ�ķ�����Ϣ
_BASIC_DLL_API_C  void _BASIC_DLL_API BasicShowCurrentMemInfo();
_BASIC_DLL_API_C  long _BASIC_DLL_API BasicGetHandleIDMemInfo(uint32_t nHandleID);

/////////////////////////////////////////////////////////////////////////////////////
//�����ڴ�
//  
//! �����СΪsize�Ĺ����ڴ�
/*!
*\param size ��Ҫ������ڴ��С = 0 ���Ѿ����ڵĹ����ڴ棬�����ڷ���NULL
*       != 0 ����size��С���ڴ档����Ѿ����� ����NULL 
*\param lpszName ��������� =NULL ������ֻ��һ������ʹ�á�!=NULL ���̼乲��
*\return �ɹ����ط�����ڴ�ָ�룬ʧ�ܷ���NULL
*\remarks �˺���������ڴ������BasicDeleteShareMem �������ͷ�
*         ʹ�ú�������Ĺ����ڴ棬���Ժ���ͨ�ڴ�һ��ʹ�ã������ڽ����й���
*\sa BasicDeleteShareMem()
*/
_BASIC_DLL_API_C  _BASIC_DLL_API void* BasicAllocShareMem(size_t size, LPCTSTR lpszName);

//! �ͷ���BasicAlloShareMem����������ڴ�
/*!
*\param p ��Ҫ�ͷŵ��ڴ�ָ��
*\return ��
*\remarks �������� p ������ BasicAlloShareMem ����ģ�����ֱ�ӷ��أ������κδ���
*\sa BasicAlloShareMem()
*/
_BASIC_DLL_API_C  void   _BASIC_DLL_API BasicDeleteShareMem(void* p);

__NS_BASIC_END
#endif 
