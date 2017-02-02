/***********************************************************************************************
// �ļ���:     tlgzip.h
// ������:     ������
// Email:      zqcai@w.cn
// ����ʱ��:   2012-2-22 23:05:01
// ��������:   ֧��gzip��ʽ���ݵ�ѹ����ѹ
// �汾��Ϣ:   1.0V
************************************************************************************************/
#ifndef BASIC_TLGZIP_H
#define BASIC_TLGZIP_H

__NS_BASIC_START

/*!***********************************************************************
 * \function BASIC_gcompress
 * \param	dest[out] �����Ŀ���ַ
 * \param	destLen[in/out]	in�����Ŀ���ĳ��ȡ�outѹ�������ݵĳ���
 * \param	source[in] ��Ҫѹ�������ݿ�
 * \param	level[in] ѹ���̶�(1-9)
 * \param	bufOut[out] ���
 * \remrk	dest�ĳ�������Ϊ��
			(sourceLen + 12) * 1.1 + 18
************************************************************************/
int basic_gcompress(unsigned char *dest, unsigned long *destLen, const unsigned char *source, unsigned long sourceLen, int level);

/*!***********************************************************************
* \function BASIC_guncompress
* \param	dest[out] �����Ŀ���ַ
* \param	destLen[in/out]	in�����Ŀ���ĳ��ȡ�out��ѹ�����ݵĳ���
* \param	source[in] ��Ҫ��ѹ�������ݿ�
* \param	bufOut[out] ���
* \remrk	��dest����ΪNULLʱ��destLen�����Ϊ��ѹ��ĳ���
************************************************************************/
int basic_guncompress(unsigned char *dest, unsigned long *destLen, const unsigned char *source, unsigned long sourceLen);

__NS_BASIC_END

#endif 


