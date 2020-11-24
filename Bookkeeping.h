#pragma once

#include <iostream>
#include <mysql/mysql.h>
#include <nana/gui/widgets/listbox.hpp>
#include "Book.h"
#include "Author.h"
#include "Adress.h"
#include "Reader.h"
#include "Librarian.h"

#define HOST "127.0.0.1"
#define USERNAME "root"
#define PASSWORD "ak260800"
#define PORT 3306
#define DB_NAME "bookkeeping"

class Bookkeeping{
private:
	unsigned int port;
	std::string host;
	std::string username;
	std::string password;
	std::string schema;
	MYSQL* conn;
	void close();
public:
	
	Bookkeeping(Bookkeeping &) = delete;
	void operator=(const Bookkeeping &) = delete;

	Bookkeeping(const std::string, const std::string, const std::string, const unsigned int, const std::string);

	void connect();
	std::string getHost();
	std::string getUsername();
	unsigned int getPort();
	std::string getSchema();
	void addBook(std::string&, std::string&, std::string&, std::string&, std::string&, int, nana::date::value&, nana::date::value&);
	void addReader(std::string&, std::string&, std::string&, nana::date::value&, std::string&, std::string&, unsigned int, unsigned int);
	void addLibrarian(std::string&, std::string&, std::string&, std::string&, std::string&, unsigned int, unsigned int);
	void actBook(unsigned int, unsigned int, std::string&, bool);
	nana::listbox::cat_proxy viewBooks(nana::listbox&);
	nana::listbox::cat_proxy search(nana::listbox& lb, std::string&, unsigned short int);
	~Bookkeeping();
};

