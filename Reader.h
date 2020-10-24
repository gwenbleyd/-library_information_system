#pragma once
#include "Date.h"

class Reader{
private:
	std::string name;
	std::string surname;
	Date date;
	std::string phone;
	unsigned int adress;
public:
	Reader(std::string, std::string, std::string, Date, unsigned int);
	std::string getName();
	std::string getSurname();
	std::string getPhone();
	std::string getDate();
	unsigned int getAdress();
	~Reader();
};

