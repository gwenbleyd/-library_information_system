#include "Date.h"

void Date::equiv(nana::date::value right){
    year = right.year;
    mouth = right.month;
    day = right.day;
}

std::string Date::getDate() {
    std::string str = std::to_string(year) + "-" + std::to_string(mouth) + "-" + std::to_string(day);
    return str;
}