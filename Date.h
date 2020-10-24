#pragma once
#include <iostream>
#include <string>
#include <nana/gui/widgets/date_chooser.hpp>

struct Date {
	unsigned int year;
	unsigned int mouth;
	unsigned int day;
	void equiv(nana::date::value);
	std::string getDate();
};

