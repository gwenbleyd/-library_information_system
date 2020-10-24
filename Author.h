#pragma once
#include "Date.h"

class Author{
private:
	unsigned int id;
	std::string name;
	std::string surname;
	std::string country;
	Date date;
public:
	Author(unsigned int, std::string, std::string, std::string, Date);
	unsigned int getId();
	std::string getName();
	std::string getSurname();
	std::string getCountry();
	std::string getDate();
	~Author();
};

