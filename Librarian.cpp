#include "Librarian.h"

Librarian::Librarian(std::string _name, std::string _surname, std::string _phone, unsigned int _adress){
	name = std::move(_name);
	surname = std::move(_surname);
	phone = std::move(_phone);
	adress = _adress;
}

std::string Librarian::getName(){
	return name;
}

std::string Librarian::getSurname()
{
	return surname;
}

std::string Librarian::getPhone()
{
	return phone;
}

unsigned int Librarian::getAdress()
{
	return adress;
}

Librarian::~Librarian() = default;
