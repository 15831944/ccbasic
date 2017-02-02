/***********************************************************************************************
// �ļ���:     exception.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 12:20:31
// ��������:   �����쳣�������
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_EXCEPTION_H
#define BASIC_EXCEPTION_H

///////////////////////////////////////////////////////////////////
__NS_BASIC_START


class CBasicException;
/*!\class CBasicException
 * \brief CBasicException class.
 *
 * �����쳣�������
 *
 * Ӧ�ó������ʱ������ģ�飬�Ϳ�ʵ�ֳ����쳣����
 * Ĭ�Ϸ�ʽ�������쳣���Զ�����������¼����־�ļ�
 * ���û��޸ı�����ʽ��ͨ������ȫ�ֺ���BasicSetExceptionModeʵ��
 *
 * �쳣��¼�ļ�:
 * Windows: ���ɵ���ǰ����Ŀ¼�µ�exceptionĿ¼����������dbgYYYYMMDD.txt
 * Linux: ���ɵ�/tmp�£��������򣺽�����.�쳣�߳�ID.����ID
*/

class CBasicException : public basiclib::CBasicObject
{
public:
	CBasicException();
	~CBasicException();

	//
	//�����˳�ʱ����
	virtual void BeforeQuit();
	//
	//����������
	virtual void Restart() = 0;

	friend void SetExceptionMode(int nMode);
};


//BasicSetExceptionModeѡ��
#define BASIC_EXCEPTION_DISABLE			0x10000000	//!< ��ֹ�쳣����
#define BASIC_EXCEPTION_NORESTART		0x00000001	//!< �����쳣�󣬲���Ҫ����
#define BASIC_EXCEPTION_NOLOG			0x00000002	//!< ����¼�쳣��־
#define BASIC_DAEMONMODE				0x00000010	//!< ����Ϊ��̨����ģʽ
#define BASIC_WINDOWSRUNDAEMON    		0x00000020  //!< windows����ģʽ
#define BASIC_EXCEPTION_KILLEXIST		0x20000000	//!< ����ʱɱ�����н���
#define BASIC_EXCEPTION_DUMP			0x80000000	//!< ��ջDUMP���
#define BIT_POS_DUMP					31

/*! \fn void SetExceptionMode(int nMode);
 * \brief  �����쳣����ģʽ
 * \param  nMode �쳣����ģʽ BASIC_EXCEPTION_*
*/
void BasicSetExceptionMode(int nMode = 0, int nInstance = 0);
//

/*! \fn void BasicClearException();
* \brief  �˳�ǰȡ���쳣����ģʽ
*/
void BasicClearException();

/*! \fn BOOL BasicIsDaemonMode();
* \brief �ж��Ƿ��̨����ģʽ
*/
BOOL BasicIsDaemonMode();

/*! \fn void BasicRestart();
* \brief  ��������
*/
void BasicRestart();

/*! \fn void BasicRegisteShutDown();
* \brief  ע���˳�����
*/
typedef fastdelegate::FastDelegate0<long> GlobalShutdownFunc;
void BasicRegisteShutDown(GlobalShutdownFunc funcShutdown);

#if	!(defined(__LINUX) || defined(__MAC) || defined(__ANDROID))
typedef fastdelegate::FastDelegate1<PEXCEPTION_POINTERS> GlobalExceptionFunc;
//!ע���쳣�ص���������
void BasicRegisteExceptionFunction(GlobalExceptionFunc funcException);
#endif


__NS_BASIC_END

#endif 
