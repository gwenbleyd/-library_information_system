#pragma once
#include <iostream>
#include <string>
#include <nana/gui/widgets/date_chooser.hpp>
#include <ctime>

struct Date {
	unsigned int year;
	unsigned int mouth;
	unsigned int day;
	void equiv(nana::date::value&);
	std::string equiv();
	std::string getDate();
};

