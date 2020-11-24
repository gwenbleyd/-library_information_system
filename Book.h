#pragma once
#include "Date.h"

class Book{
private:
	std::string title;
	std::string publishing_house;
	Date date;
public:
	Book(std::string _title, std::string _ph, Date _date);
	std::string getTitle();
	std::string getPH();
	std::string getDate();
	~Book();
};

