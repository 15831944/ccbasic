/***********************************************************************************************
// �ļ���:     basicobj.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 8:41:24
// ��������:   �������Ϳ��ļ�
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_BASICOBJ_H
#define BASIC_BASICOBJ_H

//���ļ�����������ࡢ�ṹ�������ȵ�������������ϵ
///////////////////////////////////////////////////////////////////////////
__NS_BASIC_START
class CBasicObject;
///////////////////////////////////////////////////////////////////////////
//
/*! \class CBasicObject
 * \brief CBasicObject class.
 *
 * basiclib�����ඨ��.
 */

class _BASIC_DLL_API CBasicObject
{
public:
	CBasicObject();
	virtual ~CBasicObject();

	// Diagnostic allocations
	void* operator new(size_t nSize);
	void* operator new(size_t nSize, void* pPoint);
	void operator delete(void* p);
	void operator delete(void* p, void* pPont);
};
__NS_BASIC_END

#endif

