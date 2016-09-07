/***********************************************************************************************
// �ļ���:     charset_win.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 11:19:56
// ��������:   ����ת����
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_CHARSET_WIN_H
#define BASIC_CHARSET_WIN_H

#pragma once

#ifdef __BASICWINDOWS

#include <string>

using namespace std;

__NS_BASIC_START
namespace __private  
{
	// ���ַ������ࡣ�����ֽ��ַ���ת��Ϊ���ֽ��ַ���
	class	WideBytes
	{
	public:
		WideBytes(const char* str, int len, unsigned int nCodePage);
		~WideBytes();
		operator wchar_t* () const;
		int Length() const;
		operator bool() const;
	private:
		wchar_t*	__wchars;
		int			__length;
	};


	// ��ָ��������ַ���ת��Ϊ���ַ���
	class	MultiBytes
	{
	public:
		MultiBytes(WideBytes& str, unsigned int nCodePage);
		MultiBytes(const wchar_t* str, int len, unsigned int nCodePage);
		~MultiBytes();
		int Length() const;
		operator const char* () const;
		operator bool() const;
	private:
		char*	__chars;
		int		__length;
	};
}

__NS_BASIC_END

#endif	// __BASICWINDOWS
#endif 
