/***********************************************************************************************
// �ļ���:     tlzipfile.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012-2-22 23:02:48
// ��������:   ֧��zip��ʽ���ݵĶ�д
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_TLZIPFILE_H
#define BASIC_TLZIPFILE_H

#include "../../inc/basic.h"

__NS_BASIC_START

typedef unsigned int	uint32;
typedef unsigned short	uint16;

const uint32 LOCALHEADERMAGIC    = 0x04034b50;
const uint32 CENTRALHEADERMAGIC  = 0x02014b50;
const uint32 ENDHEADERMAGIC      = 0x06054b50;

// zip�ṹ�Ķ���
#pragma pack(1)
struct	zip_file_data_block
{
	uint32		header_magic;	// LOCALHEADERMAGIC
	uint16		version_unzip;	// ��ѹ�����pkware�汾
	uint16		bit_flag;		// ȫ�ַ�ʽλ��� ?
	uint16		zip_type;		// ѹ����ʽ
	uint16		modify_time;	// dos��ʽ������޸�ʱ��
	uint16		modify_date;	// dos��ʽ������޸�����
	uint32		crc;			// crc32У��
	uint32		size_zipped;	// ѹ����ߴ�
	uint32		size_unzipped;	// δѹ���ߴ�
	uint16		size_filename;	// �ļ�������
	uint16		size_extend;	// ��չ��¼����
	// �ļ���
	// ��չ
	// ����

	zip_file_data_block()
	{
		memset(this, 0, sizeof(zip_file_data_block));
		header_magic = LOCALHEADERMAGIC;
		version_unzip = 0x0A;
	}
};

struct	zip_file_dir_block
{
	uint32		header_magic;			// CENTRALHEADERMAGIC
	uint16		version_zip;			// ѹ��ʹ�õ�pkware�汾
	uint16		version_unzip;			// ��ѹ�����pkware�汾
	uint16		bit_flag;				// ȫ�ַ�ʽλ���
	uint16		zip_type;				// ѹ����ʽ
	uint16		modify_time;			// dos��ʽ������޸�ʱ��
	uint16		modify_date;			// dos��ʽ������޸�����
	uint32		crc;					// crc32У��
	uint32		size_zipped;			// ѹ����ߴ�
	uint32		size_unzipped;			// δѹ���ߴ�
	uint16		size_filename;			// �ļ�������
	uint16		size_extend;			// ��չ��¼����
	uint16		size_comment;			// �ļ�ע�ͳ���
	uint16		disk_num_start;			// ���̿�ʼ��
	uint16		property_internal_file;	// �ڲ��ļ�����
	uint32		property_outer_file;	// �ⲿ�ļ�����
	uint32		local_header_offset;	// �ֲ�ͷ��ƫ����
	// �ļ���
	// ��չ�ֶ�
	// �ļ�ע��
	zip_file_dir_block()
	{
		memset(this, 0, sizeof(zip_file_dir_block));
		header_magic = CENTRALHEADERMAGIC;
		version_zip		= 0x14;
		version_unzip	= 0x0A;;
	}
};

struct _BASIC_DLL_API zip_file_end
{
	uint32		header_magic;				// ENDHEADERMAGIC
	uint16		disk_num_cur;				// ��ǰ���̱��
	uint16		disk_num_dir_block_start;	// Ŀ¼����ʼ���̱��
	uint16		file_num_cur_disk;			// �������ϼ�¼����
	uint16		file_num_total;				// Ŀ¼���м�¼����
	uint32		size_dir;					// Ŀ¼���ߴ��С
	uint32		offset_dir_to_first_disk;	// Ŀ¼���Ե�һ�Ŵ��̵�ƫ����
	uint16		size_comment;				// zip�ļ�ע�ͳ���

	// ע��
	zip_file_end()
	{
		memset(this, 0, sizeof(zip_file_end));
		header_magic = ENDHEADERMAGIC;
	}
};
#pragma pack()


const uint32	property_outer_file_dir		= 16;
const uint32	property_outer_file_file	= 32;

//----------------------------------8<---------------------------------
// ��������������ȡд��ѹ���ļ����ڴ��б�������ݽṹ

// �����ṹ
struct zip_base_file_info
{
	char_string	file_name;		// zip�ļ��е��ļ���
	char_string	comment;		// ע��
	time_t	time_lastmodify;	// ����޸�ʱ��
	uint16	zip_type;			// ѹ����ʽ

	zip_base_file_info()
	{
		time_lastmodify = zip_type = 0;
	}

	virtual ~zip_base_file_info(){}
	virtual bool add_file(zip_base_file_info* file)
	{
		return false;
	}
	struct is_equal_filename
	{
		is_equal_filename(const char* filename, uint16 len) : __filename(filename), __len(len){}
		bool operator()(zip_base_file_info* info)
		{
			const char* nakename = strrchr(info->file_name.c_str(), '/');
			if (nakename)
				++ nakename;
			else
				nakename = info->file_name.c_str();
			return strncmp(nakename, __filename, __len) == 0 ;
		}
		const char* __filename;
		uint16		__len;
	};

};

//typedef list<zip_base_file_info*>	file_container;
typedef basic_map<char_string, zip_base_file_info*>		file_container;

// ��zip�ļ��ж������ļ���Ϣ
struct zip_file_info : public zip_base_file_info
{
	uint32	size_zipped;		// ѹ�����ļ���С
	uint32	size_unzipped;		// δѹ�����ļ���С
	uint32	offset_data_block;	// ���ݿ�ƫ��
	uint32	offset_dir_block;	// ������ƫ��
	uint32	crc;				// crcУ���

	zip_file_info()
	{
		size_zipped = size_unzipped = offset_data_block = offset_dir_block = 0;
	}
	virtual ~zip_file_info(){}
};

// Ŀ¼��Ϣ
struct zip_dir_info : public zip_base_file_info
{
	file_container	sub_files;	// �ļ��б�

	virtual ~zip_dir_info()
	{
		for(file_container::iterator iter = sub_files.begin(); iter != sub_files.end(); ++ iter)
		{
			BASIC_DeleteObject(iter->second);
		}
		sub_files.clear();
	}

	virtual bool add_file(zip_base_file_info* file)
	{
		//sub_files.push_back(file);
		const char* nakename = strrchr(file->file_name.c_str(), '/');
		if (NULL == nakename)
			nakename = file->file_name.c_str();
		else
			++ nakename;

		sub_files[nakename] = file;
		return true;
	}
};

class zipfile_exception : public exception
{
public:
	enum zip_error_code
	{
		noError,
		generic,
		streamEnd,
		needDict,
		errNo,
		streamError,
		dataError,
		memError,
		bufError,
		versionError,
		badFuncParam,
		badZipFile,
		badCrc,
	};
	zipfile_exception(zip_error_code code);
	virtual ~zipfile_exception() throw();
	virtual const char* what() const throw();

protected:
	virtual void _Doraise() const;
	zip_error_code	__errcode;
	char_string		__error;
};

#define	__ERROR_ZIPFILE		0x00001000
#define ZIPFILE_ERROR	(0x80000000 | _ERROR_FILE | __ERROR_ZIPFILE)

#define BASIC_ZIPFILE_GENERIC		(ZIPFILE_ERROR|0x0001)
#define BASIC_ZIPFILE_STREAMEND	(ZIPFILE_ERROR|0x0002)
#define BASIC_ZIPFILE_NEEDDICT		(ZIPFILE_ERROR|0x0003)
#define BASIC_ZIPFILE_ERRNO		(ZIPFILE_ERROR|0x0004)
#define BASIC_ZIPFILE_STREAMERROR	(ZIPFILE_ERROR|0x0005)
#define BASIC_ZIPFILE_DATAERROR	(ZIPFILE_ERROR|0x0006)
#define BASIC_ZIPFILE_MEMERROR		(ZIPFILE_ERROR|0x0007)
#define BASIC_ZIPFILE_BUFERROR		(ZIPFILE_ERROR|0x0008)
#define BASIC_ZIPFILE_VERSIONERROR	(ZIPFILE_ERROR|0x0009)
#define BASIC_ZIPFILE_BADFUNCPARAM	(ZIPFILE_ERROR|0x000A)
#define BASIC_ZIPFILE_BADZIPFILE	(ZIPFILE_ERROR|0x000B)
#define BASIC_ZIPFILE_BADCRC		(ZIPFILE_ERROR|0x000C)


struct _BASIC_DLL_API TLPackFileStatus
{
	time_t			m_mtime;
	uint32			m_zipped;		/*!< ѹ�����С */
	uint32			m_unzipped;		/*!< ѹ��ǰ��С */
	BYTE			m_attribute;     /*!< �ļ������ԡ���ö�� TLFileAttribute ��ֵ */
	char_string		m_comment;
	char_string		m_filename;
	uint32			m_crc;		    /*!< crcУ��� */

	TLPackFileStatus();
	bool IsDirectory() const;
	bool IsFile() const;
};

class _BASIC_DLL_API CBasicPackFileVisitor
{
public:
	//! breif �������false,��ֹͣ����
	virtual bool	Visit(const TLPackFileStatus& status) = 0;
};

#pragma warning (push)
#pragma warning (disable: 4251)

class _BASIC_DLL_API CBasicZipFile : public CBasicFileObj
{
public:
	CBasicZipFile(void);
	virtual ~CBasicZipFile(void);
public:
	// �ļ�����
	//! \brief ��һ��zip�ļ����������������Ӧ���Ѿ���ȡ���е��ļ�����
	virtual long	OpenZipFile(const char* lpszFileName, DWORD dwOpenFlags);
	//! \brief �ر�
	virtual void	Close();
	//! \breif д��
	virtual long	Flush();

	//!���ļ��Ĳ���
	//! \brief �����ڴ��ļ���ʽ��CWBasicFileObj����
	CBasicFileObj*	GetPackFile(const char* filename);
	//! \brief �õ��ļ���Ϣ
	bool	GetPackFileStatus(const char* filename, TLPackFileStatus& status);
	//! \brief �ͷ�CWBasicFileObj����,fileobj��GetFile�õ�
	void	ReleasePackFile(CBasicFileObj* fileobj);


	//! \brief ����һ���ļ�,���dirΪ�գ���ֱ��ȡfileobj���ļ���.
	// ������ļ����Ѿ��������滻�Ѿ����ڵ��ļ���
	bool	AddPackFile(CBasicStaticBuffer* buffer, const char* filename , const char* comment = NULL);

	//! \brief ����һ���ļ�,���dirΪ�գ���ֱ��ȡfileobj���ļ���.
	// ������ļ����Ѿ��������滻�Ѿ����ڵ��ļ���
	// ��ʹ��CWBasicStaticBuffer�İ汾
	bool	AddPackFile(const void* pBuffer, size_t length, const char* filename , const char* comment = NULL);

	//! \brief ɾ��һ���ļ�
	bool	DeletePackFile(const char* filepath);
	//! \brief �����ļ���
	void	VisitPackFile(CBasicPackFileVisitor* pVisitor, const char* filespec, bool bSubdir = true);

	//! zip����
	// ����zip�ļ��Ĳ���
	//! \brief ���ӱ���Ŀ¼�ϵ��ļ���ѹ������filename�����Ŀ¼��Ŀ¼�µ��ļ�Ҳ����ѹ����
	bool	AddPackFile(const char* filename, const char* path = 0, const char* comment = 0);
	bool	AddPathToFile(const char* path);
	//! \breif ��ѹ��Ŀ¼
	bool	ExtractTo(const char* path);
	//! \brief �����ļ�ע��
	bool	SetComment(const char* comment);
	//! \brief ��ȡע��
	const char*	GetComment() const;

	//! \brief ����ѹ����level
	void	SetZipLevel(int level);

	uint16	GetFileNumTotal() const {return m_end.file_num_total;}
protected:
	virtual long	buildZipIndex();
	CBasicFileObj*		getFileObj(const zip_file_info* info);
	bool			extractToFile(const char* dir, const zip_base_file_info* info);
	bool			setFileTime(const char* filename, time_t mtime);
		
	zip_file_info*	__addfile(CBasicStaticBuffer* dataBuffer, file_container* dir, CBasicSmartBuffer& bufIndex, zip_file_info* fileinfo, const char* filepath);
	zip_dir_info*	__addpath(file_container* dir, CBasicSmartBuffer& bufIndex, zip_dir_info* dirinfo, const char* filepath);
	bool			__adddir(const char* dirname, CBasicSmartBuffer& bufIndex, char_string path, file_container* dir);
	void			removeIndexFromDirBuffer(CBasicSmartBuffer& bufIndex, zip_file_info* fileinfo);
	bool			repairFile();

	static	zip_base_file_info*	getFileInfo(const char* filename, file_container* dir);
	static	zip_base_file_info* deleteFileInfo(const char* file, file_container* dir);
	static	void		getFileStatus(const zip_base_file_info* info, TLPackFileStatus& status);
	static	bool		addFile(const char *filename, zip_base_file_info* file, file_container* dir);
	static  char_string getRootPath(const char* fileame);
	static  const char* getFileName(const char* filename);
	static  const char* stepPath(const char* filename);



	class VisitorSpecFile;
	class ZipExtractor;
	class ShortenOffset;
	friend class ZipExtractor;
protected:
	file_container	m_Files;
	char_string		m_strComment;
	int				m_ziplevel;
	zip_file_end	m_end;
	bool			m_bNeedRepair;
};

#pragma warning (pop)

__NS_BASIC_END

#endif 

