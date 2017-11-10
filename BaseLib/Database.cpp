#include "stdafx.h"
#include "Database.h"
#include "Def.h"
#include "Logger.h"
#include <cstdio>

Database::Database()
{
	try
	{
		driver = get_driver_instance();
		driver->threadInit();
	} catch (SQLException &e) {
		Logger::Log("# ERROR # %s (MySQL error code: %d  SQLState: %s)\n", e.what(), e.getErrorCode(), e.getSQLState());
	}
}

Database::~Database()
{
	try
	{
		driver->threadEnd();
	}
	catch (SQLException &e) {
		Logger::Log("# ERROR # %s (MySQL error code: %d  SQLState: %s)\n", e.what(), e.getErrorCode(), e.getSQLState());
	}
}

bool Database::Connect(char* host, char* database, char* user, char* password, int port)
{
	try
	{
		char _host[MAX_PATH];
		sprintf_s(_host, MAX_PATH, "tcp://%s:%d", host, port);
		m_conn = (std::unique_ptr<Connection>)(driver->connect(_host, user, password));
		if (m_conn->isValid())
		{
			m_conn->setSchema(database);
			stmt = (std::unique_ptr<Statement>)(m_conn->createStatement());
			Logger::Log("Connected to Database Server (%s:%d) [%s]\n", host, port, database);
			return true;
		}
		return false;
	}
	catch (SQLException &e) {
		Logger::Log("# ERROR # %s (MySQL error code: %d  SQLState: %s)\n", e.what(), e.getErrorCode(), e.getSQLState());
	}
	return false;
}

bool Database::ChangeDB(char* db)
{
	try {
		stmt.release();
		m_conn->setSchema(db);
		stmt = (std::unique_ptr<Statement>)(m_conn->createStatement());
		return true;
	}
	catch (SQLException &e) {
		Logger::Log("# ERROR # %s (MySQL error code: %d  SQLState: %s)\n", e.what(), e.getErrorCode(), e.getSQLState());
	}
	return false;
}

bool Database::ExecuteQuery(char* Format, ...)
{
	char szQuery[6000];
	va_list ap;
	va_start(ap, Format);
	vsprintf_s(szQuery, Format, ap);
	va_end(ap);
	SQLString query = SQLString(szQuery);

	try {
		stmt->executeUpdate(query);
		return true;
	}
	catch (SQLException &e) {
		Logger::Log("# ERROR # %s (MySQL error code: %d  SQLState: %s)\n", e.what(), e.getErrorCode(), e.getSQLState());
	}
	return false;
}

bool Database::ExecuteSelect(char* Format, ...)
{
	char szQuery[6000];
	va_list ap;
	va_start(ap, Format);
	vsprintf_s(szQuery, Format, ap);
	va_end(ap);
	SQLString query = SQLString(szQuery);

	try {
		res.reset(stmt->executeQuery(query));
		return true;
	}
	catch (SQLException &e) {
		Logger::Log("# ERROR # %s (MySQL error code: %d  SQLState: %s)\n", e.what(), e.getErrorCode(), e.getSQLState());
	}
	return false;
}

bool Database::ExecuteSp(char* Format, ...)
{
	char szQuery[6000];
	va_list ap;
	va_start(ap, Format);
	vsprintf_s(szQuery, Format, ap);
	va_end(ap);
	SQLString query = SQLString(szQuery);

	try {
		stmt->execute(query);
		return true;
	}
	catch (SQLException &e) {
		Logger::Log("# ERROR # %s (MySQL error code: %d  SQLState: %s)\n", e.what(), e.getErrorCode(), e.getSQLState());
	}
	return false;
}

void Database::GetResultSet()
{
	res.reset(stmt->getResultSet());
}

bool Database::Fetch()
{
	return res->next();
}

bool Database::GetMoreResults()
{
	return stmt->getMoreResults();
}

float Database::getFloat(const char* index)
{
	return static_cast<float>(res->getDouble(index));
}

long double Database::getDouble(const char* index)
{
	return res->getDouble(index);
}

bool Database::getBoolean(const char* index)
{
	return res->getBoolean(index);
}

int Database::getInt(const char* index)
{
	return res->getInt(index);
}

char* Database::getString(const char* index)
{
	return _strdup(((std::string)res->getString(index)).c_str());
}

size_t Database::rowsCount()
{
	return res->rowsCount();
}