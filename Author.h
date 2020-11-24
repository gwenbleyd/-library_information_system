#pragma once
#include "Date.h"

class Author{
private:
	std::string name;
	std::string surname;
	std::string country;
	Date date;
public:
	Author(std::string, std::string, std::string, Date);
	std::string getName();
	std::string getSurname();
	std::string getCountry();
	std::string getDate();
	~Author();
};

