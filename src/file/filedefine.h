/***********************************************************************************************
// �ļ���:     filedefine.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 12:42:04
// ��������:   �����ļ�ϵͳ��һЩ�ṹ�ʹ�����Ϣ
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_FILEDEFINE_H
#define BASIC_FILEDEFINE_H

#pragma once
#pragma warning(disable : 4996)
#pragma	pack(1)
__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//����
struct TLFileStatusW;

/////////////////////////////////////////////////////////////////////////////////////////////
//! �ļ���״̬��Ϣ
/*!
*  ���� CWBasicFileObj::GetStatus ����ȡ���ļ�״̬��Ϣ
*/
struct TLFileStatusW
{
	time_t m_ctime;          /*!< �ļ��Ĵ������� */
	time_t m_mtime;          /*!< �ļ�������޸����� */
	time_t m_atime;          /*!< �ļ������������� */
	long   m_size;           /*!< �ļ����ȣ��ֽڣ� */
	BYTE   m_attribute;      /*!< �ļ������ԡ���ö�� TLFileAttribute ��ֵ */
	BYTE   m_padding;        /*!< �ṹ���� */
	TCHAR  m_szFullName[MAX_PATH]; /*!< �ļ���ȫ·�� */

	TLFileStatusW()
	{
		memset(this, 0, sizeof(TLFileStatusW));
	}

};

struct TLFileStatus
{
	time_t m_ctime;          /*!< �ļ��Ĵ������� */
	time_t m_mtime;          /*!< �ļ�������޸����� */
	time_t m_atime;          /*!< �ļ������������� */
	long   m_size;           /*!< �ļ����ȣ��ֽڣ� */
	BYTE   m_attribute;      /*!< �ļ������ԡ���ö�� TLFileAttribute ��ֵ */
	BYTE   m_padding;        /*!< �ṹ���� */
	char  m_szFullName[MAX_PATH]; /*!< �ļ���ȫ·�� */

	TLFileStatus()
	{
		memset(this, 0, sizeof(TLFileStatus));
	}

};

//! �ļ�������
enum TLFileAttribute 
{
	_basic_fa_normal =    0x00,		/*!< ���� */
	_basic_fa_readOnly =  0x01,		/*!< ֻ�� */
	_basic_fa_hidden =    0x02,		/*!< ���� */
	_basic_fa_sys    =    0x04,		/*!< ϵͳ */
	_basic_fa_volume =    0x08,		/*!<  */
	_basic_fa_directory = 0x10,		/*!< Ŀ¼ */
	_basic_fa_archive =   0x20		/*!< �鵵 */
};

//seek
#define BASIC_FILE_BEGIN           0				//�ļ�ͷ
#define BASIC_FILE_CURRENT         1				//��ǰλ��
#define BASIC_FILE_END             2				//�ļ�β

//////////////////////////////////////////////////////////////////////////////
//���ش�����
#define FILE_ERROR							(0xE0000000 | _ERROR_FILE)	    //�ļ�����
#define FATFILE_ERROR						(0xF0000000 | _ERROR_FILE)		//FATFILE�ļ�����

#define BASIC_FILE_OK						0								//�ɹ���û�д���

#define BASIC_FILE_GENERIC_ERROR			(FILE_ERROR | 0x0001)			//һ���Դ��󣬼�δ֪�Ĵ���
#define BASIC_FILE_NOT_FOUND				(FILE_ERROR | 0x0002)			//�ļ�������
#define BASIC_FILE_BAD_PATH					(FILE_ERROR | 0x0003)			//·�����Ϸ�
#define BASIC_FILE_TOO_MANY_OPEN			(FILE_ERROR | 0x0004)			//�򿪵��ļ�̫��
#define BASIC_FILE_ACCESS_DENIED			(FILE_ERROR | 0x0005)			//��ֹ����
#define BASIC_FILE_INVALID					(FILE_ERROR | 0x0006)			//�ļ����Ϸ�
#define BASIC_FILE_REMOVE_CUR_DIR			(FILE_ERROR | 0x0007)			//ɾ����ǰ��Ŀ¼
#define BASIC_FILE_DIR_FULL					(FILE_ERROR | 0x0008)			//Ŀ¼����
#define BASIC_FILE_BAD_SEEK					(FILE_ERROR | 0x0009)			//�ƶ��ļ�ָ��ʧ��
#define BASIC_FILE_HARD_IO					(FILE_ERROR | 0x000a)			//IO ����
#define BASIC_FILE_SHARING_VIOLATION		(FILE_ERROR | 0x000b)			//Υ������
#define BASIC_FILE_LOCK_VIOLATION			(FILE_ERROR | 0x000c)			//Υ������
#define BASIC_FILE_DISK_FULL				(FILE_ERROR | 0x000d)			//������
#define BASIC_FILE_END_OF_FILE				(FILE_ERROR | 0x000e)			//�ļ�����
#define BASIC_FILE_BAD_NET_PATH				(FILE_ERROR | 0x000f)			//����·�����Ϸ�
#define BASIC_FILE_ALREADY_EXISTS			(FILE_ERROR | 0x0010)			//�ļ��Ѵ���
#define BASIC_FILE_NET_ACCESS_DENIED		(FILE_ERROR | 0x0011)			//������ʴ���
#define BASIC_FILE_WRITE_FAULT				(FILE_ERROR | 0x0012)			//д�ļ�����
#define BASIC_FILE_NET_WRITE_FAULT			(FILE_ERROR | 0x0013)			//д�����ļ�����
#define BASIC_FILE_NOT_SUPPORT				(FILE_ERROR | 0x0014)			//��֧��
#define BASIC_FILE_TOO_LARGE				(FILE_ERROR | 0x0015)			//�ļ�̫��ֻ֧�� 2G 
#define BASIC_FILE_ALREADY_OEPN				(FILE_ERROR | 0x0016)			//�ļ��Ѿ���
#define BASIC_FILE_NOT_OEPN					(FILE_ERROR | 0x0017)			//�ļ�û�д�
#define BASIC_FILE_WRITE_DENIED				(FILE_ERROR | 0x0018)			//�ļ���ֹд����ֻ����ʽ�򿪣�����д�͸ı��ļ�����
#define BASIC_FILE_SETLEN_DENIED			(FILE_ERROR | 0x0019)			//�ļ���ֹ�ı䳤�ȡ��ڴ��ļ�����PF_MEM_ONLY��ʽ�򿪡�
#define BASIC_FILE_NO_BUFFER				(FILE_ERROR | 0x001a)			//û�кϷ��Ļ�����
#define BASIC_FILE_MKDIR_ERROR				(FILE_ERROR | 0x001b)			//����Ŀ¼ʧ��
#define BASIC_FILE_NO_MEMORY				(FILE_ERROR | 0x001c)			//�ڴ治��

#define BASIC_FILE_BAD_PARAM				(FILE_ERROR | 0x001d)			//�������ò�������
#define BASIC_FILE_BAD_RESOURCE				(FILE_ERROR | 0x001e)			//����Դ��صĴ���
#define BASIC_FILE_BAD_PROCESS				(FILE_ERROR | 0x001f)			//�ͽ��̡��������á��ӽ�����صĴ���
#define BASIC_FILE_NO_EXEC					(FILE_ERROR | 0x0020)			//�޷�ִ��
#define BASIC_FILE_BAD_PIPE					(FILE_ERROR | 0x0021)			//�͹ܵ���صĴ���

__NS_BASIC_END
//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif 
