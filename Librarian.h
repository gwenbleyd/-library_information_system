#pragma once
#include <iostream>

class Librarian{
private:
	std::string name;
	std::string surname;
	std::string phone;
	unsigned int adress;
public:
	Librarian(std::string, std::string, std::string, unsigned int);
	std::string getName();
	std::string getSurname();
	std::string getPhone();
	unsigned int getAdress();
	~Librarian();
};

