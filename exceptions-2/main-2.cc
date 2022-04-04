// https://stackoverflow.com/questions/34727228/way-to-throw-exceptions-out-of-several-function-calls-all-the-way-back-to-main

#include <iostream>
#include <exception>

void foo3() { std::exception e; throw(e); }

void foo2() { try { foo3(); } catch (std::bad_exception){} }

void foo() { foo2(); }


int main()
{
    try
    {
       foo();
    }
    catch (std::exception & e)
    {
        std::cout << "caught in main()";
    }
}