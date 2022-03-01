# C++ basics

To do:

- go over C++ basics tutorial ([link](https://www.cplusplus.com/doc/tutorial/))
- go over more advanced stuff ([link](https://www.tutorialspoint.com/cplusplus/index.htm))

## Tutorial 1: cplusplus.com

`cout` is part of the standard library, and all the elements in the standard C++ library are declared withing what is called a _namespace_ std (std::cout).

In order to refer to the elements in the std namespace a program shall either qualify each and every use of elements of the library (as we have done by prefixing `cout` with `std::`), or introduce visibility of its components. The most typical way to introduce visibility of these components is by means of _using declarations_.

```cpp
using namespace std;
```

Note: inserting the `endl` manipulator ends the line (printing a newline character and flushing the stream).


Escape code | Description 
---------|----------
 \n | newline
 \r | carriage return
 \t | tab
 \v | vertical tab
 \b | backspace
 \' | single quote (')
 \" | double quote
 \? | question mark 
 \\ | backslash

Some programmers also use a trick to include long string literals in multiple lines: in C++, a backslash (\) at the end of line is considered a line-continuation character that merges both that line and the next into a single line. Therefore the following code:

```cpp
x = "string expressed in \
two line"
```

### Conditional ternary operator

```cpp
7 == 5 ? 4 : 3  // evaluates to 3, since 7 is not equal to 5
7 == 5+2 ? 4 : 3 // evaluate to 4, since equals
a>b ? a : b     // evaluates to whichever is greaterm a or b
```

### Basic input/output

stream | description 
---------|----------
 cin  | standard input stream 
 cout | standard output stream
 cerr | standard error (output) stream
 clog | standard logging (output) stream

 ### stringstream

 The standar header `<sstream>` defines a type called `stringstream` that allows a string to be treated as a stream, and thus allowing extraction or insertion from/to strings in the same way as they are performed on `cin` and `cout`. This feature is most useful to convert strings to numerical values and vice versa. For example, in order to extract an integer from a string we can write:

 ```cpp
 string mystr ("1024");
 int myint;
 stringstream(mystr) >> myint;
 ```

 ### The do-while loop

 A very similar loop is the do-while loop, whose syntax is:

 ```cpp
 do statement while (condition);
 ```

 It behaves like a while-loop, except that condition is evaluated after the execution of statement instead of before, guaranteeing at least one execution of statement, even if condition is never fulfilled.

 ### The for loop

 ```cpp
 for (initialization; condition; increase) statement;
 ```

 ### Jump statement

 - `break`: leaves a loop, even if the condition for its end is not fulfilled. It can be used to end an infinite loop, or to force it to end before its natural end. 
 - `continue`: causes the program to skip the rest of the loop in the current iteration, as if the end of the statement block had been reached, causing it to jump to the start of the following iteration.
 - `goto`: to make an absolute jump to another point in the program. This unconditional jump ignores nesting levels, and does not cause any automatic stack unwinding. Therefore, it is a feature to use with care, and preferably within the same block of statements, especially in the presence of local variables. The destination point is identified by a _label_, which is then used as an argument for the goto statement. A _label_ is made of valid identifier followed by a colon (:). 

```cpp
// goto loop example
#include <iostream>
using namespace std;

int main ()
{
  int n=10;
mylabel:
  cout << n << ", ";
  n--;
  if (n>0) goto mylabel;
  cout << "liftoff!\n";
}
```

### The return value of main

value | description 
---------|----------
 0 | The program was successful 
 EXIT_SUCCESS | The program was succesful (same as above) (defined in header `<cstdlib.h>`)
 EXIT_FAILURE | The program failed

 ### Arguments passed by value and by reference

 When a variable is passed `by reference`, what is passed is no longer a copy, but the variable itself.

### Efficiency considerations and const references

The solution is for the function to guarantee that its reference parameters are not going to be modified by this function. This can be done by qualifying the parameters as constant:

```cpp
string concatenate (const string& a, const string& b)
{
  return a+b;
}
```

By qualifying them as `const`, the function is forbidden to modify the values of neither a nor b, but can actually access their values as references (aliases of the arguments), without having to make actual copies of the strings.

### Inline functions

Preceding a function declaration with the `inline` specifier informs the compiler that inline expansion is preferred over the usual function call mechanism for a specific function. 

### Overloaded functions

In C++, two different functions can have the same name if their parameters are different; either because they have a different number of parameters, or because any of their parameters are of a different type.

### Function templates

The function sum could be overloaded for a lot of types, and it could make sense for all of them to have the same body. For cases such as this, C++ has the ability to define functions with generic types, known as `function templates`. Defining a function template follows the same syntax as a regular function, except that it is preceded by the template keyword and a series of template parameters enclosed in angle-brackets <>

`template <template-parameters> function-declaration`

```cpp
template <class SomeType>
SomeType sum(SomeType a, SomeType b)
{
    return a+b;
}
```

For example, the sum function template defined above can be called with:

```cpp
x = sum<int>(10,20);
```

An actual example:

```cpp
// function template
#include <iostream>
using namespace std;

template <class T>
T sum (T a, T b)
{
    T result;
    result = a + b;
    return result;
}

int main()
{
    int i = 5, j = 6, k;
    double f = 2.0, g = 0.5, h;
    k = sum<int>(i,j);
    h = sum<double>(f,g);
    cout << k << '\n';
    cout << k << '\n';
    return 0;
}
```

Templates can have multiple template parameters, and the function can still use regular non-templated types. For example:

```cpp
template <class T, class U>
boolo are_equal (T a, U b)
{
    return (a == b);
}

int main ()
{
    if (are_equal(10, 10.0))
        cout << "x and y are equal\n";
    else
        cout << "x and y are not equal\n";
    return 0;
}
```

### Non-type template arguments

The template parameters can not only include types introduced by class or typename, but can also include expressions of a particual type:

```cpp
template <class T, int N>
T fixed_multiply (T val)
{
    return val * N;
}

int main()
{
    std::cout << fixed_multiply<int,2>(10) << '\n';
    std::cout << fixed_multiply<int,3>(10) << '\n';
}
```

The second argument of the `fixed_multiply` function template is of type `int`. It looks like a regular function parameters, and can actually be used just like one.

But there exists one mayor difference: the value of template parameters is determined on compile-time to generate a different instantiation of the function `fixed_multiply`, and thus the value of that argument is never passed during runtime: the two calls to `fixed_multiply` in `main` essentially call two versions of the function: one that always multiplies by two, and one that always multiplies by three. For that same reason, the second template argument needs to be a constant expression (it cannot be passed a variable).

### Namespace

Only one entity can exist with a particular name in a particular scope. This is seldom a problem for local names, since blocks tend to be relatively short, and names have particular purposes within them, such as naming a counter variable, an argument, etc...

But non-local names bring more possibilities for name collision, especially considering that libraries may declare many functions, types, and variables, neither of them local in nature, and some of them very generic.

Namespace allow us to group named entities that otherwise would have global scope into narrower scopes, giving them namespace scope. This allows organizing the elements of programs into different logical scopes referred to by names.

The syntax to declare a namepace:

```cpp
namespace identifier
{
    named_entities
}
```

Example:

```cpp
namespace myNameSpace
{
    int a, b;
}
```

These variables can be accessed within their namespace normally, with their identifier (a, b), but if accessed from outside the myNameSpace namespace they have to properly qualified with the scope operator ::. For example, to access the previous variables from outside myNamespace they should be qualified like:

```cpp
myNamespace::a
myNameSpace::b
```

Namespaces can be split: two segments of a code can be declared in the same namespace:

```cpp
namespace foo { int a; }
namespace bar { int b; }
namespace foo { int c; }
```

This declares three variables: a and c are in namespace `foo`, while b is in namespace `bar`. Namespaces can even extend across different translation units (ie, across different files of source code).

### using

The keyword `using` introduces a name into the current declarative region (such as a block), thus avoiding the need to qualify the name. 

### Namespace aliasing

Existing namespaces can be aliased with new names, with the following syntax:

```cpp
namespace new_name = current_name;
```

### Pointers

```cpp
*p++    // same as *(p++): increment pointer, and dereference unincremented address
*++p    // same as *(++p): increment pointer, and dereference incremented address
++*p    // same as ++(*p): dereference pointer, and increment the value it points to
(*p)++  // dereference pointer, and post-increment the value it points to
```

A typical -but not so simple- statement involving these operators is:

```cpp
*p++ = *q++;
```

Because `++` has a higher precedence than *, both p and q are incremented, but because both increment operators (++) are used as postfix and not prefix, the value assigned to *p is *q before both p and q are incremented. And then both are incremented. It would be roughly equivalent to:

```cpp
*p = *q;
++p;
++q;
```

### Pointers and const

Pointers can be used to access a variable by its address, and this access may include modifying the value pointed. But it is also possible to declare pointers that can access the pointed value to read it, but not to modify it. For this, it is enough with qualifying the type pointed to by the pointer as const.

```cpp
const int * p = &y;
*p = x; // error: modyfying p, which is const-qualified
```

One of the use cases of pointers to `const` elements is as function parameters: a functions that takes a pointer to non-const as parameter can modify the value passed as argument, while a function that takes a pointer to const as parameter cannot.

```cpp
// pointers as arguments:
#include <iostream>
using namespace std;

void increment_all (int* start, int* stop)
{
  int * current = start;
  while (current != stop) {
    ++(*current);  // increment value pointed
    ++current;     // increment pointer
  }
}

void print_all (const int* start, const int* stop)
{
  const int * current = start;
  while (current != stop) {
    cout << *current << '\n';
    ++current;     // increment pointer
  }
}
```

This is where the second dimension to constness is added to pointers: Pointers can also be themselves const. And this is specified by appending const to the pointed type (after the asterisk):

```cpp
int x;
        int *   const p1 = &x;  // non-const pointer to non-const int
const   int *         p2 = &x;  // non-const pointer to const int
        int *   const p3 = &x;  // const pointer to non-const int
const   int *   const p4 = &x;  // const pointer to const int
```

The `const` qualifier can either precede or follow the pointed type, with the exact same meaning:

```cpp
const int * p2a = &x;   // non-const pointer to const int
int const * p2b = &x;   // also non-const pointer to const int
```

### void pointers

The void type of pointer is a special type of pointer. In C++, void represents the absence of type. Therefore, void pointers are pointers that point to a value that has no type (and thus also an undetermined length and undetermined dereferencing properties).

This gives void pointers a great flexibility, by being able to point to any data type, from an integer value or a float to a string of characters. In exchange, they have a great limitation: the data pointed to by them cannot be directly dereferenced (which is logical, since we have no type to dereference to), and for that reason, any address in a void pointer needs to be transformed into some other pointer type that points a concrete data type before being derefenced.

One of its possible uses may be to pass generic parameters to a function.

```cpp
// increaser
#include <iostream>
using namespace std;

void increase (void* data, int psize)
{
  if ( psize == sizeof(char) )
  { char* pchar; pchar=(char*)data; ++(*pchar); }
  else if (psize == sizeof(int) )
  { int* pint; pint=(int*)data; ++(*pint); }
}
```

### Invalid pointers and null pointers

In principle, pointers are meant to point to valid addresses, such as the address of a variable or the address of an element in an array. But pointers can actually point to any address, including addresses that do not refer to any valid element. Typical examples of this are uninitialized pointers and pointers to nonexistent elements of an array.

```cpp
int * p;               // uninitialized pointer (local variable)

int myarray[10];
int * q = myarray+20;  // element out of bounds 
```

Sometimes a pointer needs to explicitly point to nowhere, and not just an invalid address.

```cpp
int * p = 0;
int * q = nullptr;
```

Do not confuse null pointers and void pointers! A null pointer is a value that any pointer can take to represent that is is pointing to "nowhere", while a void pointer is a type of pointer that can point to somewhere without a specific type.

### Pointers to functions

C++ allows operations with pointers to functions. The typical use of this is for passing a function as an argument to another function. Pointers to functions are declared with the same syntax as a regular function declaration, except that the name of the function is enclosed between parentheses () and an asterix (*) is inserted before the name:

```cpp
// pointer to functions
#include <iostream>
using namespace std;

int addition (int a, int b)
{ return (a+b); }

int subtraction (int a, int b)
{ return (a-b); }

int operation (int x, int y, int (*functocall)(int,int))
{
  int g;
  g = (*functocall)(x,y);
  return (g);
}

int main ()
{
  int m,n;
  int (*minus)(int,int) = subtraction;

  m = operation (7, 5, addition);
  n = operation (20, m, minus);
  cout <<n;
  return 0;
}
```

In the example above, minus is a pointer to a function that has two parameters of type int. It is directly initialized to point to the function subtraction.

continue tomorrow: https://www.cplusplus.com/doc/tutorial/dynamic/