#ifndef _DATABASE_H
#define _DATABASE_H

#pragma once

#define CPPCONN_LIB_BUILD 1

#include "Base.h"
#include <vector>
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "mysqlcppconn-static.lib")

using namespace sql;

class Database
{
public:
	Database();
	~Database();
	bool Connect(char* host, char* database, char* user, char* password, int port);
	bool ChangeDB(char* db);
	bool ExecuteQuery(char* Format, ...);
	bool ExecuteSelect(char* Format, ...);
	bool ExecuteSp(char* Format, ...);
	void GetResultSet();
	bool Fetch();
	bool GetMoreResults();
	float getFloat(const char* index);
	long double getDouble(const char* index);
	bool getBoolean(const char* index);
	int getInt(const char* index);
	char* getString(const char* index);
	size_t rowsCount();

	Driver* driver;
	std::unique_ptr<Connection> m_conn;
	std::unique_ptr<Statement> stmt;
	std::unique_ptr<ResultSet> res;
};

#endif