#pragma once

#include <iostream>
#include <mysql/mysql.h>
#include "Book.h"
#include "Author.h"
#include "Adress.h"
#include "Reader.h"
#include "Librarian.h"

class Bookkeeping{
private:
	std::string host;
	std::string username;
	std::string password;
	unsigned int port;
	std::string schema;
	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;
	
public:
	Bookkeeping(const std::string, const std::string, const std::string, const unsigned int, const std::string);
	void connect();
	std::string getHost();
	std::string getUsername();
	unsigned int getPort();
	std::string getSchema();
	void addBook(std::string, std::string, std::string, std::string, std::string, int, int, int, nana::date::value, nana::date::value);
	void addReader(std::string, std::string, std::string, nana::date::value, std::string, std::string, unsigned int, unsigned int);
	void addLibrarian(std::string, std::string, std::string, std::string, std::string, unsigned int, unsigned int);
	void giveBook();
	void viewBooks();
	void close();
	~Bookkeeping();
};

