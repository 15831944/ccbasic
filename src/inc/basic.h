/***********************************************************************************************
// �ļ���:     basic.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012/2/17 8:18:40
// ��������:   BASIC���ͷ�ļ�
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_BASIC_H
#define BASIC_BASIC_H

//ϵͳ��������
#include "basic_def.h"

#ifdef __BASICWINDOWS
#ifndef _WIN32_WINNT
//Ϊ��֧��IPv6,��������xp�Ĵ���
#define _WIN32_WINNT	0x0600
#define NTDDI_VERSION	NTDDI_VISTA
#endif
#include <winsock2.h>		  // ������ windows.h ǰ��
#include <WS2tcpip.h>
#endif

//
//�������Ͷ���
#include "../types/types.h"
#include "../types/basicobj.h"
#include <stdint.h>
//

//Linuxϵͳ��ض���
#if	(defined(__LINUX) || defined(__MAC) || defined(__ANDROID))
#include "../types/linux.h"
#include <pthread.h>
#include <limits.h>
#endif

//
//������
#include "error.h"
//
//
//��
#include "../misc/fastdelegate.h"
#include "../misc/fastdelegatebind.h"
//�̶߳���
#include "../thread/thread.h"
//
//
//ͬ��������
#include "../thread/mt.h"
//�¼�����
//#include "../thread/basicpoll.h"
//
//��̬ʵ��
#include "../misc/singleton.h"
//
//DEBUG�궨��
#include "../debug/debug.h"
//
//�쳣����,ʹ�ÿ�Ԫstacktrace
#include "../exception/call_stack.hpp"
#include "../exception/exception.h"
//
#include "../util/functor.h"
//�ڴ����
#include "../mem/mem.h"
#include "../mem/smartbuffer.h"
#include "../mem/tlstaticbuffer.h"
//
//�����������
#include "../util/strutil/strutil.h"
#include "../util/strutil/charset.h"
#include "../datastruct/basic_string.h"
#include "../datastruct/stringex.h"
#include "../datastruct/extern.h"


//����ָ��
#include "../misc/tlrefptr.h"
//��չ����������
#include "../datastruct/key_value.h"

// ����
#include "../util/container.h"
#include "../util/containerext.h"

//�����ֿ�ʵ��
#include "../datastruct/pbzk.h"
//
//ϵͳ��Ϣ
#include "../sys/syserror.h"
#include "../sys/sysinfo.h"
//
//
//�ļ�����
#include "../file/filedefine.h"
#include "../file/filenotify.h"
#include "../file/fileobj.h"
#include "../file/fileman.h"
#include "../file/inifile.h"

//xtra
#include "../algorithm/xtra.h"

//CRC16
//#include "../algorithm/crc16.h"
#include "../algorithm/crc32.h"
//base64
#include "../algorithm/base64.h"
//
//#include "../algorithm/algorithm.h"
#include "../algorithm/md5.h"
//#include "../algorithm/aes/aes.h"
#include "../algorithm/sha1/sha1.h"
//Time
#include "../time/tltime.h"
#include "../time/ontimer.h"
#include "../time/timeutil.h"
//#include "../time/scheduletime.h"
//#include "../time/basicTimeDWord.h"
//��־
#include "../log/log.h"
//
//����ѧ������صĺ���
//#include "../algorithm/tlmath.h"
#include <math.h>

//̰������
//#include "../misc/lazy_init.h"

#include "../coroutine/coroutineplus.h"
#include "../mem/bitstream.h"

//
//RC5
//#include "../algorithm/rc5/rc5.h"

// ѹ��
#include "../algorithm/zip/tlgzip.h"
#include "../algorithm/zip/tlzipfile.h"
#include "../algorithm/zip/zlib.h"

// XML
//#include "../util/xml/xmldom.h"
//#include "../util/xml/tldomparser.h"
//#include "../util/xml/xmloutput.h"

//������ʽ
#if !defined(__MAC) //regex not implement
#include "../util/regex/cgnuregexp.h"
#endif

//dll����
#include "../dll/loaddll.h"

//sqlite֧��
#include "../sqlite/sqlite3.h"
#include "../sqlite/sqlite3db.h"

//net
#include "../net/net_client.h"
#include "../net/net_server.h"

//��ȡ�汾��
_BASIC_DLL_API basiclib::CBasicString& GetBasicLibVersion();
//�ж�basic���Ƿ����
_BASIC_DLL_API bool IsSupportBasiclib();

#endif 
