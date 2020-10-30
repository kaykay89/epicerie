/*  AUTEUR(S):
    1) Charles-Étienne Couture
    2) Kayla Louis
*/

#include "date.h"
#include <cstdio>
#include <iomanip>
#include <assert.h>


Date::Date(){
    year = 0;
    month = 0;
    day = 0;
}

Date::Date(const int year_, const int month_,const int day_)
    : year(year_), month(month_),day(day_) 
{  
}

Date::Date(const Date& d)
    :year(d.year), month(d.month), day(d.day)
{
}



bool Date::operator <(const Date& d) const{
    
    if(this->year!=d.year){
        return (this->year<d.year)?true:false;
    }
    if(this->month!=d.month){
        return (this->month<d.month)?true:false;
    }
    if(this->day!=d.day){
        return (this->day<d.day)?true:false;
    }
   
    return false;
}

bool Date::operator==(const Date& d) const{
    return year==d.year && month==d.month && day==d.day;
}

bool Date::operator<=(const Date& d) const{
    if(*this < d)
        return true;
    if(*this == d)
        return true;

    return false;
}

std::ostream& operator << (std::ostream& os, const Date& d){
    char chaine[40];
    sprintf(chaine, "%04d-%02d-%02d", d.year, d.month,d.day);
    os << chaine;
    return os;
}

std::istream& operator >> (std::istream& is, Date& d){
    int a=0, m=0, j=0;
    char t1=0,t2=0;
    is >> a >> t1 >> m >> t2 >> j;
    assert(t1=='-');
    assert(t2=='-');

    d.year = a; 
    d.month = m;
    d.day = j;
    return is;
}

