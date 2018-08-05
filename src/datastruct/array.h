/***********************************************************************************************
// �ļ���:     array.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 9:14:11
// ��������:   ʹ��stl��vectorʵ��MFC CArray�Ľӿ�
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_ARRAY_H
#define BASIC_ARRAY_H

#pragma once

#include "../inc/basic_def.h"
#include <vector>
using namespace std;

__NS_BASIC_START

#pragma warning (push)
#pragma warning (disable: 4251)
#pragma warning (disable: 4275)

template<class TYPE, class ARG_TYPE = const TYPE&>
class CArray : public vector<TYPE, DEFAULT_ALLOCATOR<TYPE> >{
public:
    typedef typename vector<TYPE, DEFAULT_ALLOCATOR<TYPE> >::const_pointer const_pointer;
    typedef typename vector<TYPE, DEFAULT_ALLOCATOR<TYPE> >::pointer pointer;
    typedef CArray<TYPE, ARG_TYPE> _Self;
    // Construction
    CArray();

    // Attributes
    //! ȡ��Array��С
    int GetSize() const;
    //! ȡ��Array���Ŀ����±�
    int GetUpperBound() const;
    //! ���������С
    void SetSize(int nNewSize);

    // Operations
    // Clean up
    //! Ϊ�˼���MFC CArray������ʵ��ʲô��������
    void FreeExtra();
    //! �������
    void RemoveAll();

    // Accessing elements
    //!ȡ��nIndexλ�õĶ���ֵ
    TYPE GetAt(int nIndex) const;
    //!����nIndexλ�õ�ֵ
    void SetAt(int nIndex, ARG_TYPE newElement);
    //! ȡ��nIndexλ�ö����ֵ
    TYPE& ElementAt(int nIndex);

    // Direct Access to the element data(may return NULL)
    //! ת��Ϊָ���б�
    const TYPE* GetData() const;
    //! ת��Ϊָ���б�
    TYPE* GetData();

    // Potentially growing the array
    //! ����nIndexλ�õ�ֵ������õ�ַ���������С������չ������
    void SetAtGrow(int nIndex, ARG_TYPE newElement);
    //! β��׷��һ������
    int Add(ARG_TYPE newElement);
    //! β��׷��src�еĶ���
    int Append(const CArray& src);
    //! ��src��ֵ
    void Copy(const CArray& src);

    // Operations that move elements arround
    //! ��nIndexλ�ò������
    void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
    //! ���������Ƴ�nIndexλ�õĶ���
    void RemoveAt(int nIndex, int nCount = 1);
    //! ��nIndexλ�ò������pNewArray�е�ֵ��
    void InsertAt(int nStartIndex, CArray* pNewArray);
public:
    ~CArray();
};
#pragma warning (pop)
/////////////////////////////////////////////////////////////////////
// CArray<TYPE>	inline functions
template<class TYPE, class ARG_TYPE>
inline int CArray<TYPE, ARG_TYPE>::GetSize() const{
    return _Self::size();
}

template<class TYPE, class ARG_TYPE>
inline int CArray<TYPE, ARG_TYPE>::GetUpperBound() const{
    return _Self::size() - 1;
}

template<class TYPE, class ARG_TYPE>
inline void CArray<TYPE, ARG_TYPE>::RemoveAll(){
    _Self::clear();
}

template<class TYPE, class ARG_TYPE>
inline TYPE CArray<TYPE, ARG_TYPE>::GetAt(int nIndex) const{
    ASSERT(nIndex >= 0 && nIndex < (int)_Self::size());
    return _Self::at(nIndex);
}

template<class TYPE, class ARG_TYPE>
inline void CArray<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE newElement){
    ASSERT(nIndex >= 0 && nIndex < (int)_Self::size());
    _Self::at(nIndex) = newElement;
}

template<class TYPE, class ARG_TYPE>
inline TYPE& CArray<TYPE, ARG_TYPE>::ElementAt(int nIndex){
    ASSERT(nIndex >= 0 && nIndex < (int)_Self::size());
    return _Self::at(nIndex);
}

template<class TYPE, class ARG_TYPE>
inline const TYPE* CArray<TYPE, ARG_TYPE>::GetData() const{
    const_pointer ptr = &_Self::at(0);
    return ptr;
}

template<class TYPE, class ARG_TYPE>
inline TYPE* CArray<TYPE, ARG_TYPE>::GetData(){
    pointer ptr = &_Self::at(0);
    return ptr;
}

template<class TYPE, class ARG_TYPE>
inline int CArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement){
    int nIndex = _Self::size();
    _Self::push_back(newElement);
    return nIndex;
}

////////////////////////////////////////////////////////
// CArray<TYPE> out-of-line functions
template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::CArray(){
}

template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::~CArray(){
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::SetSize(int nNewSize){
    _Self::resize(nNewSize);
}

template<class TYPE, class ARG_TYPE>
int CArray<TYPE, ARG_TYPE>::Append(const CArray& src){
    int nIndex = GetSize();
    _Self::insert(_Self::end(), src.begin(), src.end());
    return nIndex;
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::Copy(const CArray& src){
    _Self::assign(src.begin(), src.end());
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::FreeExtra(){
    // not support
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::SetAtGrow(int nIndex, ARG_TYPE newElement){
    if(nIndex >= GetSize())
        _Self::resize(nIndex + 1);
    _Self::at(nIndex) = newElement;
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::InsertAt(int nIndex, ARG_TYPE newElement, int nCount /*=1*/){
    _Self::insert(_Self::begin() + nIndex, nCount, newElement);
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::RemoveAt(int nIndex, int nCount /*= 1*/){
    _Self::erase(_Self::begin() + nIndex, _Self::begin() + nIndex + nCount);
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::InsertAt(int nStartIndex, CArray* pNewArray){
    _Self::insert(_Self::begin() + nStartIndex, pNewArray->begin(), pNewArray->end());
}

__NS_BASIC_END
#endif 
