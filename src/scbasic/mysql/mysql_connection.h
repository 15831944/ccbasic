#ifndef _MYSQL_CONNECTION_H_
#define _MYSQL_CONNECTION_H_

#include <basic.h>
#include "mysqlconnector.h"

class CLocalConnect;
class CLocalResultSet : public basiclib::CBasicObject
{
public:
	CLocalResultSet();
	virtual ~CLocalResultSet();

	//! ��ȡ����
	my_ulonglong GetRowsNumber();
	//! ��ȡ�����
	unsigned int GetFieldNumber();

	//! ��ȡ��һ��
	bool NextRow();
	//! ��ȡ����
	bool getBoolean(uint32_t columnIndex);
	bool getBoolean(const char* pColName);

	int32_t getInt(uint32_t columnIndex);
	int32_t getInt(const char* pColName);

	uint32_t getUInt(uint32_t columnIndex);
	uint32_t getUInt(const char* pColName);

	int64_t getInt64(uint32_t columnIndex);
	int64_t getInt64(const char* pColName);

	uint64_t getUInt64(uint32_t columnIndex);
	uint64_t getUInt64(const char* pColName);

	double getDouble(uint32_t columnIndex);
	double getDouble(const char* pColName);

	const char* getString(uint32_t columnIndex);
	const char* getString(const char* pColName);
protected:
	//! ��ʼ������Ϣ
	void Init();
	//! ����������ȡindex
	uint32_t GetColumeIndexByName(const char* pColName);
protected:
	typedef basiclib::basic_map<basiclib::CBasicString, int>	MapColNameToIndex;
	typedef MapColNameToIndex::iterator							MapColNameToIndexIterator;

	MYSQL_RES*			m_pRS;
	MYSQL_ROW			m_rowData;
	MapColNameToIndex	m_mapColName;

	friend class CLocalConnect;
};

class CMySQL_ConnectionPool;
class CMySQL_Connection;
class CLocalConnect : public basiclib::CBasicObject
{
public:
	CLocalConnect(CMySQL_Connection* pConn);
	virtual ~CLocalConnect();

	//! ��
	void SetConnection(CMySQL_ConnectionPool* pPool, CMySQL_Connection* pConn);

	//! ��ѯ���ݿ�
	bool SelectMySQL(const char *q, CLocalResultSet& rs);
	//! �������ݿ�, ����Ӱ������
	int QueryMySQL(const char *q);

	//! ����AUTO_INCREMENT�����ɵ�ID
	my_ulonglong GetInsertID();
protected:
	CMySQL_ConnectionPool*	m_pPool;
	CMySQL_Connection*		m_pConn;

};

class CMySQL_Connection : public basiclib::CBasicObject
{
public:
	CMySQL_Connection();
	virtual ~CMySQL_Connection();

	//! ��ʼ�����ݿ�
	bool InitMySQL(CMySQLConnector::OnMySQLConnNotify pNotify, CMySQLConnector_NetWapper* pWapper, Net_UInt nMaxPacketSize = MYSQL_DEFAULT_MAX_PACKET_SIZE);
	//! ���ñ��뷽ʽ
	int SetCharacterSet(const char *csname);
	//! ��������ѡ��
	int SetOptions(enum mysql_option option,const void *arg);
	int SetOptions4(enum mysql_option option, const void *arg1, const void *arg2);
	//! �������ݿ�
	bool ConnectToSQL(const char *host,
		const char *user,
		const char *passwd,
		const char *db,
		unsigned int port = 3306,
		const char *unix_socket = nullptr,
		unsigned long clientflag = 0);

	//! �ж��Ƿ����ӿ���PingMySQLServer����һ��
	int ReconnectMySQL();
protected:
	void MySQLNotifyType(CMySQLConnectorNotifyType notifyType, const char* pNotify);
protected:
	//! ping��������֤��������
	int PingMySQLServer();
protected:
	CMySQLConnector	m_conn;

	friend class CLocalConnect;
};

typedef fastdelegate::FastDelegate0<CMySQLConnector_NetWapper*> OnCreateMySQLNetWapper;
class CMySQL_ConnectionPool : public basiclib::CBasicObject
{
public:
	CMySQL_ConnectionPool();
	virtual ~CMySQL_ConnectionPool();

	//! ��һ�α����ʼ��
	bool Init(basiclib::CBasicString& url, basiclib::CBasicString& user, basiclib::CBasicString& password, int nInitSize, int maxSize, 
		CMySQLConnector::OnMySQLConnNotify pNotify, OnCreateMySQLNetWapper pCreate, Net_UInt nMaxPacketSize = MYSQL_DEFAULT_MAX_PACKET_SIZE);
	//! ��ʼ��֮����ܻ�ȡ����
	bool GetConnection(CLocalConnect& localConnect); //������ݿ�����
	//! ��ȡ���ӣ��ȵ���Ϊֹ
	bool GetConnectionWaitForOne(CLocalConnect& localConnect, int nMaxTimes = 10000);
	//! �������ݿ����ӳ�
	void DestoryBasicConnPool();

	//ִ��ontimer����,ȷ��ÿ�����Ӳ��ᱻ�Ͽ�
	void HandleOnTimer();
private:
	void ReleaseConnection(CMySQL_Connection *conn); //�����ݿ����ӷŻص����ӳص�������

	void InitConnection(int iInitialSize); //��ʼ�����ݿ����ӳ�
	CMySQL_Connection*	CreateConnection(); //����һ������
	//! ��ȡһ������
	CMySQL_Connection* GetConnectionFromList();

private:
	typedef basiclib::basic_vector<CMySQL_Connection*>	VTConnect;

	VTConnect						m_connException;
	VTConnect						m_connectSafeList;
	VTConnect						m_createList;


	basiclib::CBasicString	m_strUrl;
	basiclib::CBasicString	m_strUsername;
	basiclib::CBasicString	m_strPassword;
	unsigned int			m_nMaxSize; //���ӳ��ж����������ݿ�������

	CMySQLConnector::OnMySQLConnNotify	m_pNotify;
	OnCreateMySQLNetWapper				m_create;
	Net_UInt							m_nMaxPacketSize;
	CMySQLConnector_NetWapper* pWapper, 

	friend class CLocalConnect;
};


#endif // _MYSQL_CONNECTION_H_
