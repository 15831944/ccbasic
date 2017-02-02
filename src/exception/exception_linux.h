#ifndef __EXCEPTION_LINUX_H_
#define __EXCEPTION_LINUX_H_

#define ALLOCTO_STACK_MEMORY  8192	//�쳣��ջ�Ĵ�С

__NS_BASIC_START

class CBasicExceptionLinux : public CBasicException
{
public:
	CBasicExceptionLinux();
	~CBasicExceptionLinux();

	virtual void BeforeQuit();
	virtual void Restart();

	void SetDaemonMode(int nInstance = 0);

protected:
	void CatchSignal();

protected:
	char m_pszLockFile[_MAX_PATH];

	//����һ���ڴ���Ϊ�쳣����Ķ�ջ
	void *	m_pAllocStack;

	char** 	m_pszCmdline;	//�����в���
	int	m_nParaNum;	//�����в�������
};

__NS_BASIC_END

#endif //__EXCEPTION_LINUX_H_
