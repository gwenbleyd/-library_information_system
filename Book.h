#pragma once
#include "Date.h"

class Book{
private:
	unsigned int id;
	std::string title;
	std::string publishing_house;
	Date date;
public:
	Book(unsigned int _id, std::string _title, std::string _ph, Date _date);
	unsigned int getId();
	std::string getTitle();
	std::string getPH();
	std::string getDate();
	~Book();
};

