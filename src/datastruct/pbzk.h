#ifndef BASIC_PBZK_H
#define BASIC_PBZK_H

#define PBZK_END_STRING "IsEnd"
class CPBZK
{
public:
	CPBZK();
	virtual ~CPBZK();

	void ReadPBZKFileBuffer(const char* pBuffer, int nLength, bool bAddZiFu = true);
	void AddPBZKToMap(basiclib::CBasicStringArray& ayItems);
	//�ж��Ƿ�������д�
	bool IsContainPBZK(const char* txt, int nLength, bool bDeep = false);
	//����ֱ���滻
	void ReplacePBZK(char* txt, int nLength, char cReplace = '*', bool bDeep = true);
protected:
	//! �ж��Ƿ��зǷ��ַ�
	int CheckPBZKExist(const char* txt, int nLength, int nBeginIndex, bool bDeep = false);
	
protected:
	typedef basiclib::basic_map<basiclib::CBasicString, void*>								HashMapPBZK;
	typedef HashMapPBZK::iterator															HashMapPBZKIterator;
	HashMapPBZK m_mapPBZK;
};

#endif