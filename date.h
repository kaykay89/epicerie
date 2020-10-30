/*  AUTEUR(S):
    1) Charles-Étienne Couture COUC13049004
    2) Kayla Louis
*/

#if !defined(__DATE_H__)
#define __DATE_H__

#include <iostream>

class Date{
  public:
    Date();
    Date(int year_,int month_,int day_);
    Date(const Date& date);
    bool operator <(const Date& date) const;
    bool operator==(const Date& date) const;
    bool operator<=(const Date& date) const;

  private:
    int year,month,day; 
  friend std::ostream& operator << (std::ostream&, const Date& date);
  friend std::istream& operator >> (std::istream&, Date& date);
};

#endif
