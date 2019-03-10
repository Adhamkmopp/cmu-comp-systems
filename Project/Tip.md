### C++

The << operator is type sensitive; it will break on whitespace if the type is string. It will CAST according to type. How do you read x number of sequences then?

"an object is some memory that holds a value associated with a type"
"a type is a set of possible values and allowed operations"
"a class is a user-defined type"
"the interface is the part of the class the user accesses directly, the implementation is is labeled private"

constexpr double pi = 3.14159; // ready at compilation time
const int radius; // read and defined once
constexpr double area = pi *radius; // ERROR: radius is not known yet

Literals are "magic constants" and should be given descriptive names instead like the asshole suggested.

Function Declarations & Definitions : include declarations in the main thingamajig but place definitions elsewhere; it's neater.

```c++

vector <int> random = {1,2,3,5,6,7,8}

```

A program that implements a grammar is called a parser

Extern is useless. Don't use it. Ever.

Headers: function declarations

Pass-by-const-reference: pass pointer, do not modify 

A pointer is a variable that holds a memory address. A reference has the same memory address as the item it references.

A reference is just another name for the same variable. I imagine it is only a way of forcing the compiler NOT to copy any values into registers when calling a function, but to keep working with the same register value, or memory address as the callee.

static members are common to all objects
Constructor:

An initializer that guarantees that an object should and must be initialized with some value. The example below includes two constructors, one inline and one defined outside the class. The other constructor incoporates both an initialization member list (by adding ':' followed byt the arguments) and initialization within the function body. 

enum class is new to C++11 due to the fact that variable names in plain enumerators pollute the namespace by exporting these values to the enumeration. In former, class access operators are required.

```c++

Class Date{
    public:
        Date (int, int, int);
        Date(){
            day = 23;
            month = 05;
            year = 1991;
        }
    private:
        int day;
        int month;
        int year;
}

Date::Date(int d, int m, int y): day(d), month(m){ 
    year = y;
    if (year < 1881) throw invalid_data{};
    
    };


Date notme(24,05,1991);
Date Adham; // default constructor
```

Static variables:
```c++

#include <stdio.h>

void func() {
	static int x = 0; 
	/* x is initialized only once across five calls of func() and
	  the variable will get incremented five 
	  times after these calls. The final value of x will be 5. */
	x++;
	printf("%d\n", x); // outputs the value of x
}

int main() { //int argc, char *argv[] inside the main is optional in the particular program
	func(); // prints 1
	func(); // prints 2
	func(); // prints 3
	func(); // prints 4
	func(); // prints 5
	return 0;
}
```