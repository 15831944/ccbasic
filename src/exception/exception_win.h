#ifndef _WIN_EXCEPTION_H
#define _WIN_EXCEPTION_H

#include "exception.h"
#include <winsvc.h>
#pragma once

__NS_BASIC_START

enum BasicType  // Stolen from CVCONST.H in the DIA 2.0 SDK
{
    btNoType = 0,
    btVoid = 1,
    btChar = 2,
    btWChar = 3,
    btInt = 6,
    btUInt = 7,
    btFloat = 8,
    btBCD = 9,
    btBool = 10,
    btLong = 13,
    btULong = 14,
    btCurrency = 25,
    btDate = 26,
    btVariant = 27,
    btComplex = 28,
    btBit = 29,
    btBSTR = 30,
    btHresult = 31
};

//
//Ӧ�ó����쳣���������� for Windows
//
// �쳣����ԭ��ͨ��ϵͳ����SetUnhandledExceptionFilter����������쳣
// ��ջ���ݺͷ��ű����ͨ��dbghelp APIʵ��
// ����DEBUG�棬�ɽ�����ǰ·���ж�Ӧ��pdb�ļ����ű�
// 
//
class CBasicWinExceptionReport :public CBasicException
{
public:
    
    CBasicWinExceptionReport();
    ~CBasicWinExceptionReport();
    
	virtual void Restart();

	void SetLogFileName( PTSTR pszLogFileName );


    // entry point where control comes on an unhandled exception
    static LONG WINAPI WheatyUnhandledExceptionFilter(
                                PEXCEPTION_POINTERS pExceptionInfo );

    private:

	// where report info is extracted and generated 
    static void GenerateExceptionReport( PEXCEPTION_POINTERS pExceptionInfo, HANDLE hFile);

    // Helper functions
    static char* GetExceptionString( DWORD dwCode );

	static int __cdecl PrintOut(HANDLE hFile, const char* format, ...);

    // Variables used by the class
    static TCHAR m_szLogFileName[MAX_PATH];
	static TCHAR m_szDumpFileName[MAX_PATH];
    static LPTOP_LEVEL_EXCEPTION_FILTER m_previousFilter;
    static HANDLE m_hProcess;
protected:
	//
	// ����dump�ļ�
	//
	static long WriteDumpFile( struct _EXCEPTION_POINTERS *pExceptionInfo );

	static BOOL m_bRestart;
	static BOOL m_bLog;
    static BOOL m_bToClose;

	friend void BasicSetExceptionMode(int nMode, int nInstance);
	friend void BasicClearException();
};


extern CBasicWinExceptionReport* g_WheatyExceptionReport; //  global instance of class

__NS_BASIC_END
#endif //_WIN_EXCEPTION_H
