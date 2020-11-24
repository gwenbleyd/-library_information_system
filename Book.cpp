#include "Book.h"

Book::Book(std::string _title, std::string _ph, Date _date){
    title = std::move(_title);
    publishing_house = std::move(_ph);
    date = std::move(_date);
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

