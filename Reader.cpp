#include "Reader.h"
Reader::Reader(std::string _name, std::string _surname, std::string _phone, Date _date, unsigned int _adress){
	name = std::move(_name);
	surname = std::move(_surname);
	phone = std::move(_phone);
	date = std::move(_date);
	adress = _adress;
}

std::string Reader::getName()
{
	return name;
}

std::string Reader::getSurname()
{
	return surname;
}

std::string Reader::getPhone()
{
	return phone;
}

std::string Reader::getDate()
{
	return date.getDate();
}

unsigned int Reader::getAdress()
{
	return adress;
}

Reader::~Reader() = default;
