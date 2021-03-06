#include <algorithm>
#include "../inc/basic.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
__NS_BASIC_START

CBasicKey2Value::CBasicKey2Value(){
}

bool CBasicKey2Value::HasValue(const char* lpszKey, CBasicString& strValue){
    if(lpszKey != NULL && lpszKey[0] != '\0'){
        CBasicString strKey = lpszKey;
        strKey.MakeLower();
        auto iter = m_map.find(strKey);
        if(iter != m_map.end()){
            strValue = iter->second;
            return true;
        }
    }
    return false;
}
CBasicString CBasicKey2Value::GetValue(const char* lpszKey) const{
    if(lpszKey != NULL && lpszKey[0] != '\0'){
        CBasicString strKey = lpszKey;
        strKey.MakeLower();
        auto iter = m_map.find(strKey);
        if(iter != m_map.end()){
            return iter->second;
        }
    }
    return "";
}

const char* CBasicKey2Value::GetValueRef(const char* lpszKey) const{
    if(lpszKey != NULL && lpszKey[0] != '\0'){
        CBasicString strKey = lpszKey;
        strKey.MakeLower();
        auto iter = m_map.find(strKey);
        if(iter != m_map.end()){
            return iter->second.c_str();
        }
    }
    return nullptr;
}

void CBasicKey2Value::SetValue(const char* lpszKey, const char* lpszValue){
    if(lpszKey != NULL && lpszKey[0] != '\0' && lpszValue != NULL){
        CBasicString strKey = lpszKey;
        strKey.MakeLower();
        m_map[strKey] = lpszValue;
    }
}

void CBasicKey2Value::SetValue(const char* lpszKey, long lValue){
    char cTemp[32];
    sprintf(cTemp, "%d", (uint32_t)lValue);
    SetValue(lpszKey, cTemp);
}

bool CBasicKey2Value::RemoveKey(const char* lpszKey){
    if(lpszKey != NULL && lpszKey[0] != '\0'){
        CBasicString strKey = lpszKey;
        strKey.MakeLower();
        return m_map.erase(strKey);
    }
    return false;
}

void CBasicKey2Value::ParseTextURL(const char* pszBuffer, int nLen){
    ParseText(pszBuffer, "=&", nLen);
}

void CBasicKey2Value::ParseTextLine(const char* pszBuffer, int nLen){
    ParseText(pszBuffer, "=\n", nLen);
}

void CBasicKey2Value::ParseText(const char* pszBuffer, const char* lpszTok, int nLen){
    if(nLen < 0){
        nLen = strlen(pszBuffer);
    }
    Basic_ParseParamString(pszBuffer, nLen, lpszTok, [&](const char* key, const char* value)->void{
        if(key && value){
            SetRawValue(key, value);
        }
    });
}

void CBasicKey2Value::SetRawValue(const char* lpszKey, const char* lpszValue){
    if(lpszKey != NULL && lpszKey[0] != '\0' && lpszValue != NULL){
        CBasicString strKey = lpszKey;
        SetValueAt(strKey, lpszValue);
    }
}

void CBasicKey2Value::SetValueAt(CBasicString& strKey, const char* lpszValue){
    m_map[strKey] = lpszValue;
}

const char* CBasicKey2Value::CombineToString(basiclib::CBasicSmartBuffer& sbBuf, const char* lpszTok){
    char  m_cTok1;		//Key和Value之间的分隔符
    char  m_cTok2;		//行之间的分隔符
    if(lpszTok == NULL){
        m_cTok1 = '=';
        m_cTok2 = '\n';
    }
    else if(lpszTok[1] == '\0'){
        m_cTok1 = lpszTok[0];
        m_cTok2 = '\n';
    }
    else{
        m_cTok1 = lpszTok[0];
        m_cTok2 = lpszTok[1];
    }
    ForEach([&](const char* lpKey, const char* lpValue)->void{
        sbBuf.AppendString(lpKey);
        sbBuf.AppendData((const char*)&m_cTok1, sizeof(char));
        sbBuf.AppendString(lpValue);
        sbBuf.AppendData((const char*)&m_cTok2, sizeof(char));
    });
    sbBuf.AppendData("\0\0", 2);
    return sbBuf.GetDataBuffer();
}
//////////////////////////////////////////////////////////////////////////

__NS_BASIC_END
