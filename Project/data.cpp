#include <iostream>
#include <stdio.h>
using namespace std;

class Year{
    static const int min = 1880;
    static const int max = 2020;
    
    public:
        class invalid{};
        Year(int x): y{x} { if (x < min || x > max) throw invalid{};};
        int year(){ return y;};
    private:
        int y;
};

enum class Month{
    jan = 1,feb,mar,apr,may,jun,jul,aug,sep,oct,nov,dec
};

Month operator++(Month& m){
    m = (m==Month::dec)? Month::jan: Month(int(m)+1);
    return m;
};

class Date
    {
    private:
        int d {23};
        Month m {5};
        Year y {1991};
        bool isvalid();
    public:
        Date(int , int , int);
        Date(int); // constructor with 2 default values
        Date(); // default constructor but with values attached to the members themselves and available to all other constructors
        //class invalid{};
        int month() const{ return int(m);}; // the const tells the compiler that this is non-modifying; it can be called 
                                            //  for a constant object.
        Year year(){ return y;};
        int day(){ return d;};

       
    };

    Date::Date(int yy, int mm, int dd):y{yy}, m{Month(mm)}, d{dd}{ 
        };
    Date::Date(int yy): y{yy} {};


int main(){

    Date adham{1991,5,23};  
    Month m = Month::jan;
    ++m;
    cout << adham.year().year() << int(m);
}