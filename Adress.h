#pragma once
#include <iostream>

class Adress{
private:
	std::string city;
	std::string street;
	unsigned int house;
	unsigned int apartment;
public:
	Adress(std::string, std::string, unsigned int, unsigned int);
	std::string getCity();
	std::string getStreet();
	unsigned int getHouse();
	unsigned int getApartment();
	~Adress();
};

