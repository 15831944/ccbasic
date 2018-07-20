﻿#include <stdlib.h>
#include <map>
#include <basic.h>
#include "net/nettest.h"
//#include "misc/fastdelegatetest.h"
//#include "exception/stackwalkcheck.h"
#include "util/containerexttest.h"
#include "thread/threadtest.h"
#include "coroutine/coroutinetest.h"
#include "../scbasic/encode/rsaencode.h"
#include "comm/functionxiaolvtest.h"

//computer func call per ms
basiclib::SpinLock mutexSpinLock;
void TestCallFunc(int i){
    basiclib::CSpinLockFuncNoSameThreadSafe lock(&mutexSpinLock);
    lock.Lock();
    lock.UnLock();
}
void TestCallSameFunc(int i){
    basiclib::CSpinLockFunc lock(&mutexSpinLock);
    lock.Lock();
    lock.UnLock();
}

basiclib::CBasicObject* pCreate(){
    return new basiclib::CBasicObject();
}
void pFreeCreate(basiclib::CBasicObject* p){
    delete p;
}

basiclib::CMutex mutexData;
void TestMutexCallFunc(int i){
    basiclib::CSingleLock lock(&mutexData);
    lock.Lock();
}

int main(int argc, char* argv[])
{
	if (!IsSupportBasiclib()){
		printf("Not SupportBasiclib!!!");
		getchar();
		return 0;
	}
    BasicGetModuleTitle();
	srand(time(NULL) + basiclib::BasicGetTickTime());
    {
        clock_t begin = clock();
        for(int i = 0; i < TIMES_FAST; i++){
            TestCallFunc(i);
        }
        printf("this computer funccall add %.4f/ms\n", (double)TIMES_FAST / (clock() - begin));
    }
    {
        clock_t begin = clock();
        for(int i = 0; i < TIMES_FAST; i++){
            TestCallSameFunc(i);
        }
        printf("this computer funccall add %.4f/ms\n", (double)TIMES_FAST / (clock() - begin));
    }
    {
        clock_t begin = clock();
        for(int i = 0; i < TIMES_FAST / 10; i++){
            TestMutexCallFunc(i);
        }
        printf("this computer mutex %.4f/ms\n", (double)TIMES_FAST / 10 / (clock() - begin));
    }
    {
        clock_t begin = clock();
        for(int i = 0; i < TIMES_FAST / 10; i++){
            pFreeCreate(pCreate());
        }
        printf("this computer malloc add %.4f/ms\n", (double)(TIMES_FAST / 10) / (clock() - begin));
    }

    //StartCoroutineTest();
	//TestCoroutine();
	//TestThread();
	//TestStackWalk();
	//TestFastDelegate();
	//TestContainExt();
	//NetServerTest();
	//TestFunctionXiaolvTest();

    {
        byte szBuf[1024] = { 0 };
        for(int i = 0; i < 1024; i++){
            szBuf[i] = rand() % 256;
        }
        byte szBuf2[2048] = { 0 };
        byte szBuf3[2048] = { 0 };
        CSCBasicRSA rsa;
        string strPri;// = "30820274020100300D06092A864886F70D01010105000482025E3082025A02010002818100AB9A16DBEB33AEAE447D08024F3C1B9E4B135F49FA01D6B2CE2A099C3F4E259E5F9B0C5522568242A20DCDC462E7BDE3E940866834EBE4B3920B7D4A29B77EA25AB48509D5CD29BF3D23F022E6B35C101C0A89B69B7C9D413DC3CF0C698F9A9ADEF0B55F5D89A1715834955F05B99D3CD328A36B59E7169767E438293F3E56990201110281800BC6CF5F67FC04755F0D99A5CE38D4B80A2BFF0291284875AEC6A64E7CD0A83A8E1730885A331D0492A6982915D8B02B3FB19ACAEA88A8CB118D5666F8D2DE06026AD980779D4602B7E99A567360DC57F27E8082F29807A506819BF155F21B0B3928F2CFB6E246ADBC8CF76E0926EBB4480A9CA0B2CDDB72EE777001E19ED5A3024100BE133A3FE40CB6D12863E3AE88D3DBEDD1ECAB3E044247A3B8C5B757F8B421CE1E519FE52ED218C61946B417059ACFC36EE956FBF519C61D5BD49C0CBBC074AF024100E71EA0556BF152342F2AFA6415C9D0D7A133B2723600C1A693E6A51C67EF134DC8DCB115338D39A315C33996E879F5366F126225375ED2CAC267CE4A3E2A4B370240597275C3B69C92444F3E10CA9ABE0D249F05F63B4D4C5DF2B14DFBED29BE2E06A4DB1E117F71ED8A663F63CE9939CB2ECAC828EF09EE02E0A3AF587E7678AF6102406CC31E4650EA0891072348A791C862475AEB26CC55A600C6DC304DB303BBCCD94F76E9EBDC06393DAFE36665221B4637BBCC6A6BDDD245141F3FE89B688C5FA10241008016A17615345B02C7B4D77449EEC5E9901F58028E61E363AF0B89BF9E5EFE760FAD0D66BE5F48693D03D9A801D2BDC0839406CF716F5C8FF1ACEBCA86C7E465";
        string strPub;// = "30819D300D06092A864886F70D010101050003818B0030818702818100AB9A16DBEB33AEAE447D08024F3C1B9E4B135F49FA01D6B2CE2A099C3F4E259E5F9B0C5522568242A20DCDC462E7BDE3E940866834EBE4B3920B7D4A29B77EA25AB48509D5CD29BF3D23F022E6B35C101C0A89B69B7C9D413DC3CF0C698F9A9ADEF0B55F5D89A1715834955F05B99D3CD328A36B59E7169767E438293F3E5699020111";
        rsa.GenerateRSAKey(1024, strPri, strPub);
        rsa.SetPrivateKey(strPri.c_str());
        rsa.SetPublicKey(strPub.c_str());
        int nTotalLength = rsa.Encrypt(szBuf, 1024, szBuf2, 2048);
        int nRetLength = rsa.Decrypt(szBuf2, nTotalLength, szBuf3, 2048);
        ASSERT(nRetLength == 1024);
        ASSERT(memcmp(szBuf3, szBuf, nRetLength) == 0);

        nTotalLength = rsa.Sign((char*)"12345", 5, szBuf2, 2048);
        if(!rsa.Verify((char*)szBuf2, nTotalLength, (char*)"12345", 5)){
            ASSERT(0);
        }
    }

    {
        byte szBuf[1024] = { 0 };
        for(int i = 0; i < 1024; i++){
            szBuf[i] = rand() % 256;
        }
        byte szBuf2[2048] = { 0 };
        byte szBuf3[2048] = { 0 };
        CSCBasicRSA rsa;
        rsa.GenerateRSAKey(1024, (char*)"e:/private.a", (char*)"e:/public.a");
        rsa.SetPrivateFileName("e:/private.a");
        rsa.SetPublicFileName("e:/public.a");
        int nTotalLength = rsa.Encrypt(szBuf, 1024, szBuf2, 2048);
        int nRetLength = rsa.Decrypt(szBuf2, nTotalLength, szBuf3, 2048);
        ASSERT(nRetLength == 1024);
        ASSERT(memcmp(szBuf3, szBuf, nRetLength) == 0);

        nTotalLength = rsa.Sign((char*)"12345", 5, szBuf2, 2048);
        if(!rsa.Verify((char*)szBuf2, nTotalLength, (char*)"12345", 5)){
            ASSERT(0);
        }
    }

	getchar();
	return 0;
}

