#ifndef BASIC_ERROR_H
#define BASIC_ERROR_H

//basic��Ŀ������붨��
//��16λ����Ϊģ����࣬��16λ����Ϊģ���ڲ������

//ϵͳ�����
#define _ERROR_SYS				0x00010000
//

//�ļ������
#define _ERROR_FILE				0x00020000
//

//�ڴ������
#define _ERROR_MEM				0x00030000
//
//
//�߳������
#define _ERROR_THREAD			0x00050000
#define _ERROR_THREAD_INVALID	(_ERROR_THREAD|1)		//��Ч�߳�
//

//
//��־�����
#define _ERROR_LOG				0x00060000
#define _ERROR_LOG_OPEN			(_ERROR_LOG|1)			//����־ʧ��
//

//���������
#define _ERROR_NET				0x00070000

//XML�����ļ������
#define _ERROR_XCON				0x00080000

//���л��Ĵ������
#define _ERROR_ARC				0x00090000

//���Ĵ������
#define _ERROR_TREE				0x000A0000

#endif //BASIC_ERROR_H
