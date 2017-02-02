#include "rsaencode.h"

#include "hex.h"
#include "files.h"

CSCBasicRSA::CSCBasicRSA(const char* pSeed)
{
	m_rng.IncorporateEntropy((byte*)pSeed, strlen(pSeed));
}

CSCBasicRSA::~CSCBasicRSA()
{

}

void CSCBasicRSA::GenerateRSAKey(unsigned int keyLength, const char *privFilename, const char *pubFilename)
{
	CryptoPP::RSAES_OAEP_SHA_Decryptor priv(m_rng, keyLength);
	CryptoPP::HexEncoder privFile(new CryptoPP::FileSink(privFilename));
	priv.DEREncode(privFile);
	privFile.MessageEnd();

	CryptoPP::RSAES_OAEP_SHA_Encryptor pub(priv);
	CryptoPP::HexEncoder pubFile(new CryptoPP::FileSink(pubFilename));
	pub.DEREncode(pubFile);
	pubFile.MessageEnd();
}
void CSCBasicRSA::GenerateRSAKey(unsigned int keyLength, string& strPub, string& strPri)
{
	CryptoPP::RSAES_OAEP_SHA_Decryptor priv(m_rng, keyLength);
	CryptoPP::HexEncoder privFile(new CryptoPP::StringSink(strPri));
	priv.DEREncode(privFile);
	privFile.MessageEnd();

	CryptoPP::RSAES_OAEP_SHA_Encryptor pub(priv);
	CryptoPP::HexEncoder pubFile(new CryptoPP::StringSink(strPub));
	pub.DEREncode(pubFile);
	pubFile.MessageEnd();
}

//����publicfile
void CSCBasicRSA::SetPublicFileName(const char* pPubFileName)
{
	m_pubEncode.AccessKey().Load(CryptoPP::FileSource(pPubFileName, true, new CryptoPP::HexDecoder));
	m_pubDecode.AccessKey().Load(CryptoPP::FileSource(pPubFileName, true, new CryptoPP::HexDecoder));
}
void CSCBasicRSA::SetPublicKey(const char* pData)
{
	m_pubEncode.AccessKey().Load(CryptoPP::StringSource(pData, true, new CryptoPP::HexDecoder));
	m_pubDecode.AccessKey().Load(CryptoPP::StringSource(pData, true, new CryptoPP::HexDecoder));
}
void CSCBasicRSA::SetPrivateFileName(const char* pPrivateFileName)
{
	m_priDecode.AccessKey().Load(CryptoPP::FileSource(pPrivateFileName, true, new CryptoPP::HexDecoder));
	m_priEncode.AccessKey().Load(CryptoPP::FileSource(pPrivateFileName, true, new CryptoPP::HexDecoder));
}
void CSCBasicRSA::SetPrivateKey(const char* pData)
{
	m_priDecode.AccessKey().Load(CryptoPP::StringSource(pData, true, new CryptoPP::HexDecoder));
	m_priEncode.AccessKey().Load(CryptoPP::StringSource(pData, true, new CryptoPP::HexDecoder));
}

//! ���������������ݳ���
size_t CSCBasicRSA::CalcEncryptNeedLength(int nLength)
{
	size_t fixedLen = m_pubEncode.FixedMaxPlaintextLength();
	return (nLength / fixedLen + 1) * m_pubEncode.FixedCiphertextLength();
}

size_t CSCBasicRSA::Encrypt(const char* pEncode, int nLength, byte* pOutput, int nOutputLength)
{
	size_t putLen = 0;
	size_t fixedLen = m_pubEncode.FixedMaxPlaintextLength();
	for (size_t i = 0; i < nLength; i += fixedLen)
	{
		size_t len = fixedLen < (nLength - i) ? fixedLen : (nLength - i);
		CryptoPP::ArraySink *dstArr = new CryptoPP::ArraySink(pOutput + putLen, nOutputLength - putLen);
		CryptoPP::ArraySource source((byte*)(pEncode + i), len, true, new CryptoPP::PK_EncryptorFilter(m_rng, m_pubEncode, dstArr));
		putLen += dstArr->TotalPutLength();
	}
	return putLen;
}

size_t CSCBasicRSA::Decrypt(const char* pDecode, int nLength, byte* pOutput, int nOutputLength)
{
	size_t putLen = 0;
	size_t fixedLen = m_priDecode.FixedCiphertextLength();
	for (size_t i = 0; i < nLength; i += fixedLen)
	{
		size_t len = fixedLen < (nLength - i) ? fixedLen : (nLength - i);
		CryptoPP::ArraySink *dstArr = new CryptoPP::ArraySink(pOutput + putLen, nOutputLength - putLen);
		CryptoPP::ArraySource((byte*)(pDecode + i), len, true, new CryptoPP::PK_DecryptorFilter(m_rng, m_priDecode, dstArr));
		putLen += dstArr->TotalPutLength();
	}
	return putLen;
}
size_t CSCBasicRSA::Sign(const char* pEncode, int nLength, byte* pOutput, int nOutputLength)
{
	size_t putLen = 0;
	size_t fixedLen = m_priEncode.MaxSignatureLength();
	if (nLength > fixedLen)
		return 0;
	size_t len = nLength;
	CryptoPP::ArraySink *dstArr = new CryptoPP::ArraySink(pOutput + putLen, nOutputLength - putLen);
	CryptoPP::ArraySource((byte*)pEncode, len, true, new CryptoPP::SignerFilter(m_rng, m_priEncode, dstArr));
	putLen += dstArr->TotalPutLength();
	return putLen;
}

bool CSCBasicRSA::Verify(const char* pDecode, int nLength, const char* pVerify, int nVerifyLength)
{
	bool bSuccess = false;
	size_t fixedLen = m_pubDecode.SignatureLength();
	if (nLength != fixedLen)
		return false;
	CryptoPP::VerifierFilter *pVerifierFilter = new CryptoPP::VerifierFilter(m_pubDecode);
	pVerifierFilter->Put((byte*)pDecode, nLength);
	CryptoPP::ArraySource((byte*)pVerify, nVerifyLength, true, pVerifierFilter);
	return pVerifierFilter->GetLastResult();
}