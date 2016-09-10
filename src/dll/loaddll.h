/***********************************************************************************************
// �ļ���:     loaddll.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012-2-21 23:25:28
// ��������:   ���ö�̬��ӿڣ���װһЩAPI����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_LOADDLL_H
#define BASIC_LOADDLL_H

#pragma once
__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//����
////////////////////////////////////////////////////////////////////////////////////////
class CBasicLoadDll : public CBasicObject
{
public:
	CBasicLoadDll();
	CBasicLoadDll(const char* lpszLibFileName);
	~CBasicLoadDll();
public:
	long LoadLibrary(const char* lpszLibFileName);
	long FreeLibrary();
	void* GetProcAddress(const char* lpszProcName);
protected:
	basiclib::CBasicString	m_strLoadFileName;
	void*					m_hModule;
};
////////////////////////////////////////////////////////////////////////////////////////
__NS_BASIC_END

#endif 
