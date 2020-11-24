#include "Date.h"

void Date::equiv(nana::date::value &right){
    year = std::move(right.year);
    mouth = std::move(right.month);
    day = std::move(right.day);
}

std::string Date::equiv() {
    struct tm *right;
    const time_t timer = time(NULL);
    right = localtime(&timer);
    year = right->tm_year + 1900;
    mouth = right->tm_mon + 1;
    day = right->tm_mday;
    return getDate();
}


std::string Date::getDate() {
    std::string str = std::to_string(year) + "-" + std::to_string(mouth) + "-" + std::to_string(day);
    return str;
}