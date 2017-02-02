#include "../inc/basic.h"

CCQLite3DB::CCQLite3DB()
{
	m_pDB = NULL;
}
CCQLite3DB::~CCQLite3DB()
{
	Close();
}

bool CCQLite3DB::Open(const char *szFileName, const char* pPwd, int nLength)
{
	int nRet = sqlite3_open(szFileName, &m_pDB);
	if (nRet != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(m_pDB);
		return false;
	}
	if (pPwd)
	{
		nRet = SetOpenPWD(pPwd, nLength);
		if (nRet != SQLITE_OK)
		{
			m_strLastError = sqlite3_errmsg(m_pDB);
			return false;
		}

		CCQLite3DBTable tableDB;
		if (!GetDataToTable("select count(*) FROM sqlite_master WHERE type='table'", &tableDB))
		{
			//ʹ��û������Ĵ�
			Close();
			return Open(szFileName, NULL, 0);
		}
	}
	return true;
}

bool CCQLite3DB::Close()
{
	if (m_pDB)
	{
		if (sqlite3_close(m_pDB) == SQLITE_OK)
		{
			m_pDB = 0; //һ���ر����ݿ�ָ�룬Ҫ��Ϊ0����ֹ��ιرճ���  
		}
		else
		{
			ASSERT(0);
			m_strLastError = "Unable to close database";
			return false;
		}
	}
	return true;
}

//������
int CCQLite3DB::SetOpenPWD(const char* pPwd, int nLength)
{
	return sqlite3_key(m_pDB, pPwd, nLength);
}
//��������
int CCQLite3DB::SetPWD(const char* pPwd, int nLength)
{
	return sqlite3_rekey(m_pDB, pPwd, nLength);
}

bool CCQLite3DB::GetDataToTable(const char *szSQL, CCQLite3DBTable* pTable)
{
	if (!CheckDB())
	{
		return false;
	}

	char* szError = 0;
	char** paszResults = 0;
	int nRet;
	int nRows(0);
	int nCols(0);

	nRet = sqlite3_get_table(m_pDB, szSQL, &paszResults, &nRows, &nCols, &szError);

	if (nRet == SQLITE_OK && pTable)
	{
		pTable->InitTable(paszResults, nRows, nCols);
	}
	else
	{
		m_strLastError = szError;
		return false;
	}
	return true;
}
int CCQLite3DB::ExecSQL(const char *szSQL)
{
	if (!CheckDB())
	{
		return -1;
	}

	char* szError = 0;

	int nRet = sqlite3_exec(m_pDB, szSQL, 0, 0, &szError);

	if (nRet == SQLITE_OK)
	{
		return sqlite3_changes(m_pDB);  //�������ִ��Ӱ�������  
	}
	else
	{
		m_strLastError = szError;
		sqlite3_free(szError);
		return -1;
	}
}
bool CCQLite3DB::CheckDB()
{
	if (!m_pDB)
	{
		ASSERT(0);
		m_strLastError = "Database not open";
		return false;
	}
	return true;
}

sqlite3_stmt* CCQLite3DB::Compile(const char *szSQL)
{
	if (!CheckDB())
	{
		return NULL;
	}

	const char *szTail = 0;
	sqlite3_stmt *pStmt;

	int nRet = sqlite3_prepare(m_pDB, szSQL, -1, &pStmt, &szTail);

	if (nRet != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(m_pDB);
		return NULL;
	}
	return pStmt;
}
////////////////////////////////////////////////////////////////////////////////
bool CCQLite3DB::ExecQuery(const char *szSQL, CCQLite3DBQuery* pQuery)
{
	if (!CheckDB())
	{
		return false;
	}

	//����һ��ָ���������ʱ�����洢�����ݸ�CCQLite3DBQuery��  
	//�����ʱsqlite3_stmt*������Զ���ʧ�����ֻ��һ�ݱ�����CCQLite3DBQuery��  
	sqlite3_stmt *pStmt = Compile(szSQL);
	if (NULL == pStmt)
	{
		return false;
	}

	int nRet = sqlite3_step(pStmt);

	if (nRet == SQLITE_DONE) //���������ѯû�з��ؽ��  
	{
		pQuery->InitQuery(m_pDB, pStmt, true);
	}
	else if (nRet == SQLITE_ROW) //�����ѯ��������һ�м�¼  
	{
		pQuery->InitQuery(m_pDB, pStmt, false);
	}
	else
	{
		nRet = sqlite3_finalize(pStmt);
		m_strLastError = sqlite3_errmsg(m_pDB);
		return false;
	}
	return true;
}

