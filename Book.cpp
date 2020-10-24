#include "Book.h"

Book::Book(unsigned int _id, std::string _title, std::string _ph, Date _date){
    id = _id;
    title = std::move(_title);
    publishing_house = std::move(_ph);
    date = std::move(_date);
}

unsigned int Book::getId(){
    return id;
}

std::string Book::getTitle()
{
    return title;
}

std::string Book::getPH()
{
    return publishing_house;
}

std::string Book::getDate()
{
    return date.getDate();
}

Book::~Book() = default;

