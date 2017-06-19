#include "../inc/basic.h"

__NS_BASIC_START
// Static Member Initializers
//
char CBasicBase64::base64_pad = '=';
// The 7-bit alphabet used to encode binary information
BYTE CBasicBase64::m_sBase64Alphabet[] = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char CBasicBase64::m_sBase64Decode[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
	-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
	-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicBase64::CBasicBase64()
{
	
}

CBasicBase64::~CBasicBase64()
{
}

long CBasicBase64::Encode(BYTE* szEncoding, int nSize, CBasicSmartBuffer& buf)
{
	long nMaxSize = Encode(szEncoding, nSize, NULL, 0);
	if (nMaxSize < 0)
		return nMaxSize;

	long nOldLen = 0;
	buf.AppendData(NULL, nMaxSize);
	BYTE* pOut = (BYTE *)buf.GetDataBuffer(nOldLen);
	pOut += nOldLen;

	nMaxSize = Encode(szEncoding, nSize, pOut, nMaxSize);
	if (nMaxSize > 0)
	{
		buf.SetDataLength(nOldLen + nMaxSize);
		//buf.AppendData((const char*)pOut, nMaxSize);
		return nMaxSize;
	}
	return nMaxSize;
}

long CBasicBase64::Encode(BYTE* szEncoding, int nSize, BYTE* pOut, int nMaxSize)
{
	if(pOut == NULL)
	{
		return (nSize+2) /3 * 4;
	}

	//保存数据的空间不够
	if(nMaxSize < (nSize+2) /3 * 4)
	{
		return -1;
	}
	
	const BYTE* current = szEncoding;
	BYTE* p = pOut;

	while (nSize > 2) { /* keep going until we have less than 24 bits */
		*p++ = GetEncodeAlphabet(current[0] >> 2);
		*p++ = GetEncodeAlphabet(((current[0] & 0x03) << 4) + (current[1] >> 4));
		*p++ = GetEncodeAlphabet(((current[1] & 0x0f) << 2) + (current[2] >> 6));
		*p++ = GetEncodeAlphabet(current[2] & 0x3f);

		current += 3;
		nSize -= 3; /* we just handle 3 octets of data */
	}

	/* now deal with the tail end of things */
	if (nSize != 0) {
		*p++ = GetEncodeAlphabet(current[0] >> 2);
		if (nSize > 1) {
			*p++ = GetEncodeAlphabet(((current[0] & 0x03) << 4) + (current[1] >> 4));
			*p++ = GetEncodeAlphabet((current[1] & 0x0f) << 2);
			*p++ = GetPadding();
		} else {
			*p++ = GetEncodeAlphabet((current[0] & 0x03) << 4);
			*p++ = GetPadding();
			*p++ = GetPadding();
		}
	}

	return (p - pOut);
}

long CBasicBase64::Decode(BYTE* szDecoding, int nSize, CBasicSmartBuffer& buf)
{
	long nMaxSize = Decode(szDecoding, nSize, NULL, 0);
	if (nMaxSize < 0)
		return nMaxSize;

	long nOldLen = 0;
	buf.AppendData(NULL, nMaxSize);
	BYTE* pOut = (BYTE *)buf.GetDataBuffer(nOldLen);
	pOut += nOldLen;

	nMaxSize = Decode(szDecoding, nSize, pOut, nMaxSize);
	if (nMaxSize > 0)
	{
		buf.SetDataLength(nOldLen + nMaxSize);
		//buf.AppendData((const char*)pOut, nMaxSize);
		return nMaxSize;
	}
	return nMaxSize;
}

long CBasicBase64::Decode(BYTE* szDecoding, int nSize, BYTE* pOut, int nMaxSize)
{
	if(pOut == NULL)
	{
		return nSize + 1;		
	}
	if(nMaxSize < nSize + 1)
	{
		return -1;
	}
	const char *current = (const char*)szDecoding;
	int ch, i = 0, j = 0, k;
	int nLen = nSize;

	BYTE* pDecodeBuffer = pOut;

	/* run through the whole string, converting as we go */
	while ((ch = *current++) != '\0' && nLen-- > 0) {
		if (ch == GetPadding()) break;

	    /* When Base64 gets POSTed, all pluses are interpreted as spaces.
		   This line changes them back.  It's not exaCBasicy the Base64 spec,
		   but it is completely compatible with it (the spec says that
		   spaces are invalid).  This will also save many people considerable
		   headache.  - Turadg Aleahmad <turadg@wise.berkeley.edu>
	    */

		if (ch == ' ') ch = '+'; 

		ch = GetDecodeAlphabet(ch);
		if (ch < 0) continue;

		switch(i % 4) {
		case 0:
			pDecodeBuffer[j] = ch << 2;
			break;
		case 1:
			pDecodeBuffer[j++] |= ch >> 4;
			pDecodeBuffer[j] = (ch & 0x0f) << 4;
			break;
		case 2:
			pDecodeBuffer[j++] |= ch >>2;
			pDecodeBuffer[j] = (ch & 0x03) << 6;
			break;
		case 3:
			pDecodeBuffer[j++] |= ch;
			break;
		}
		i++;
	}

	k = j;
	/* mop things up if we ended on a boundary */
	if (ch == GetPadding()) {
		switch(i % 4) {
		case 1:
			delete pDecodeBuffer;
			pDecodeBuffer = NULL;
			return 0;
		case 2:
			k++;
		case 3:
			pDecodeBuffer[k++] = 0;
		}
	}
	nMaxSize = j;
	pDecodeBuffer[j] = '\0';
	return j;
}




char CBasicBase64Ex::base64_pad = '=';

BYTE CBasicBase64Ex::m_sBase64Alphabet[] = 
{ 'o', 'P', 'b', 's', 'G', '4', 'E', 'v', 'U', '8', 'g', 'y', 'd',
   '0', '2', 'B', '3', 'q', '6', 'f', 'I', 'V', 'W', 'X', 'Y', 'Z',
   'a', 'C', 'c', 'M', 'e', 'T', 'K', 'h', 'x', 'n', 'w', 'z', 'm',
   'j', 'A', 'p', 'R', 'r', 'D', 't', 'u', 'H', 'k', 'i', 'L', 'l',
   'N', '1', 'O', '9', 'F', '5', 'S', '7', 'J', 'Q', '+', '/', '\0'
 };

char CBasicBase64Ex::m_sBase64Decode[] = {
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
 13, 53, 14, 16, 5, 57, 18, 59, 9, 55, -1, -1, -1, -1, -1, -1,
 -1,  40,  15,  27,  44,  6,  56,  4,  47,  20,  60, 32, 50, 29, 52, 54,
 1, 61, 42, 58, 31, 8, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
 -1, 26, 2, 28, 12, 30, 19, 10, 33, 49, 39, 48, 51, 38, 35, 0,
 41, 17, 43, 3, 45, 46, 7, 36, 34, 11, 37, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

CBasicBase64Ex::CBasicBase64Ex()
{
}

CBasicBase64Ex::~CBasicBase64Ex()
{
}

__NS_BASIC_END

