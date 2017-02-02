#ifndef SCBASIC_RSA_H
#define SCBASIC_RSA_H

#include <basic.h>
#include "randpool.h"
#include "rsa.h"

#define DEFAULT_SCBASICRSA_SEED	"seed_scbasic"
class CSCBasicRSA : public basiclib::CBasicObject
{
public:
	CSCBasicRSA(const char* pSeed = DEFAULT_SCBASICRSA_SEED);
	virtual ~CSCBasicRSA();

	void GenerateRSAKey(unsigned int keyLength, const char *privFilename, const char *pubFilename);
	void GenerateRSAKey(unsigned int keyLength, string& strPub, string& strPri);

	//! ����public&private
	void SetPublicFileName(const char* pPubFileName);
	void SetPublicKey(const char* pData);
	void SetPrivateFileName(const char* pPrivateFileName);
	void SetPrivateKey(const char* pData);

	//! ���������������ݳ���
	size_t CalcEncryptNeedLength(int nLength);

	size_t Encrypt(const char* pEncode, int nLength, byte* pOutput, int nOutputLength);
	size_t Decrypt(const char* pDecode, int nLength, byte* pOutput, int nOutputLength);

	//! ����ܳ���max signlength, 0����ʧ��
	size_t Sign(const char* pEncode, int nLength, byte* pOutput, int nOutputLength);
	bool Verify(const char* pDecode, int nLength, const char* pVerify, int nVerifyLength);
protected:
	CryptoPP::RandomPool					m_rng;
	CryptoPP::RSAES_OAEP_SHA_Encryptor		m_pubEncode;
	CryptoPP::RSASSA_PKCS1v15_SHA_Verifier	m_pubDecode;
	CryptoPP::RSAES_OAEP_SHA_Decryptor		m_priDecode;
	CryptoPP::RSASSA_PKCS1v15_SHA_Signer	m_priEncode;
};

#endif