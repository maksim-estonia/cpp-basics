#include <iostream>
#include <exception>

void foo3() { std::exception e; throw(e); }

void foo2() { foo3(); }

void foo() { foo2(); }


int main()
{
    try
    {
       foo();
    }
    catch (std::exception & e)
    {
        std::cout << "caught in main()\n";
    }
}