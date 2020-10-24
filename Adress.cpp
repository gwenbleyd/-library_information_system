#include "Adress.h"

Adress::Adress(std::string _city, std::string _street, unsigned int _house, unsigned int _apartment){
	city = std::move(_city);
	street = std::move(_street);
	house = _house;
	apartment = _apartment;
}

std::string Adress::getCity()
{
	return city;
}

std::string Adress::getStreet()
{
	return street;
}

unsigned int Adress::getHouse()
{
	return house;
}

unsigned int Adress::getApartment()
{
	return apartment;
}

Adress::~Adress() = default;
