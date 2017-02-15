#ifndef SCBASIC_CCFRAME_LOG_H
#define SCBASIC_CCFRAME_LOG_H

#include "../../scbaisc_head.h"

//!�¼���¼
/*!
* д��Ĭ����־�ļ�
*\remarks ��־�ļ�Ĭ��·����ͨ��BasicSetLogEventMode����
*\remarks �ɸ�ʽ������
*/
_SCBASIC_EXTERNC  _SCBASIC_DLL_API void CCFrameSCBasicLogEventV(const char* pszLog, ...);
_SCBASIC_EXTERNC  _SCBASIC_DLL_API void CCFrameSCBasicLogEventErrorV(const char* pszLog, ...);

//!�¼���¼
/*!
* д��Ĭ����־�ļ�
*\remarks ��־�ļ�Ĭ��·����ͨ��BasicSetLogEventMode����
*/
_SCBASIC_EXTERNC  _SCBASIC_DLL_API void CCFrameSCBasicLogEvent(const char* pszLog);
_SCBASIC_EXTERNC  _SCBASIC_DLL_API void CCFrameSCBasicLogEventError(const char* pszLog);
//////////////////////////////////////////////////////////////////////////////////////////////
//
//BasicSetLogEventModeѡ��
#define	LOG_BY_DAY		0x00010000		//!< �����¼������������ƵĴ�С��һ����ܻ��ж���ļ���
#define	LOG_BY_SIZE		0x00020000		//!< ����С��¼���������ƵĴ�С���½�һ���ļ���
#define	LOG_BY_ONEFILE	0x00040000		//!< ʼ��ֻ��һ���ļ���������죬��ɾ��ǰһ������ݣ�������С���ƣ�ɾ��ԭ�����ļ���
#define	LOG_BY_BUFFER	0x00080000		//!< ��־������д�̣��ȱ����ڻ��������棬��ʱд�̡����Ч�ʡ�
#define	LOG_BY_OPEN		0x01000000		//!< ��־�ļ����ִ�״̬�����رա�
#define	LOG_BY_NOLIMIT	0x02000000		//!< �������ļ���С
#define LOG_BY_SAMENAME	0x04000000		//!< �����������

//
#define LOG_SIZE_LIMIT	0x0000ffff		//!< ��־�ļ���С����	��λ��MB
#define LOG_NAME_DAY_S	"S%DAY%"


#define LOG_ERROR_NAME_EMPTY		-1	//!< �ļ���Ϊ��
#define LOG_ERROR_OPEN_FILE			-2	//!< ���ļ�ʧ��
#define LOG_ERROR_FULL				-3	//!< ��־��¼ͨ���Ѿ����ˡ�
//
//! ����Ĭ�ϵ��¼���¼ģʽ
/*
* ���ú�ȫ����Ч����Ӧ�ó�������ʱ����һ�μ���
*
*\param		nOption:	��¼ģʽLOG_*
*\param		pszLogFile:	��־�ļ���,�����·�����ᶯ̬����
*
*\return		= 0 Ĭ�ϵ���־���
*				<0 ����
*\remarks  ֻ����������Ĭ�ϵ�ͨ������ͨ��ID = 0 ����������ã�ʹ���ļ��� basiclib.log
*/
_SCBASIC_EXTERNC  _SCBASIC_DLL_API uint32_t CCFrameSCBasicSetDefaultLogEventMode(long nOption, const char* pszLogFile);
_SCBASIC_EXTERNC  _SCBASIC_DLL_API uint32_t CCFrameSCBasicSetDefaultLogEventErrorMode(long nOption, const char* pszLogFile);
//

//
//! �����¼���¼ģʽ
/*
* ���ú�ȫ����Ч����Ӧ�ó�������ʱ����һ�μ���
*
*\param		nOption:	��¼ģʽLOG_*
*\param		pszLogFile:	��־�ļ���,�����·�����ᶯ̬����
*
*\return		>0 ��־��������ڴ��ݸ�BasicLogEvent
*				<0 ����
*/
_SCBASIC_EXTERNC  _SCBASIC_DLL_API uint32_t CCFrameSCBasicSetLogEventMode(long nOption, const char* pszLogFile);
//

//! �ر���־��¼
/*
* �ڳ����˳�ǰ���߹ر�ĳ����־��¼��ʱ�����
*
*\param		lLogChannel:	��־��������� BasicSetLogEventMode �ķ���ֵ ���С�� 0 ���ر����е�
*
*\return		>0 ��־��������ڴ��ݸ�BasicLogEvent
*				<0 ����
*/
_SCBASIC_EXTERNC  _SCBASIC_DLL_API uint32_t CCFrameSCBasicCloseLogEvent(long lLogChannel);
//

//
//!�¼���¼��д����־�ļ���
/*
*\remarks ��־�ļ�·����ͨ��BasicSetLogEventMode����
*\remarks �ɸ�ʽ������
*/
_SCBASIC_EXTERNC _SCBASIC_DLL_API void CCFrameSCBasicLogEventV(long lLogChannel, const char* pszLog, ...);
//
//
//
//


//
//!�¼���¼��д����־�ļ���
/*
*\remarks ��־�ļ�·����ͨ��BasicSetLogEventMode����
*/
_SCBASIC_EXTERNC _SCBASIC_DLL_API void BasicLogEvent(long lLogChannel, const char* pszLog);
//
//
//


#endif