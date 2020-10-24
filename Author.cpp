#include "Author.h"

Author::Author(unsigned int _id, std::string _name, std::string _surname, std::string _country, Date _date){
	id = _id;
	name = std::move(_name);
	surname = std::move(_surname);
	country = std::move(_country);
	date = std::move(_date);
}

unsigned int Author::getId()
{
	return id;
}

std::string Author::getName()
{
	return name;
}

std::string Author::getSurname()
{
	return surname;
}

std::string Author::getCountry()
{
	return country;
}

std::string Author::getDate()
{
	return date.getDate();
}

Author::~Author() = default;
