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

### Dynamic memory

Dynamic memory is allocated using operator new. new is followed by a data type specifier andm if a sequence of more than one element is required, the number of these within brackets []. It returns a pointer to the beginning of the nez block of memory allocated.

```cpp
int * foo;
foo = new int [5];
```

The other method is known as `nothrow`, and what happens when it is used is that when a memory allocation fails, instead of throwing a `bad_alloc` exception or terminating the program, the pointer returned by new is a null pointer, and the program continues its execution normally. 

```cpp
foo = new (nothrow) int [5];
```

In most cases, memory allocated dynamically is only needed during specific periods of time withing a program; once it is no longer needed, it can be freed so that the memory becomes available again for other requests of dynamic memory. This is the purpose of operator `delete`, whose syntax is:

```cpp
delete pointer;
delete[] pointer;
```

```cpp
// rememb-o-matic
#include <iostream>
#include <new>
using namespace std;

int main ()
{
  int i,n;
  int * p;
  cout << "How many numbers would you like to type? ";
  cin >> i;
  p= new (nothrow) int[i];
  if (p == nullptr)
    cout << "Error: memory could not be allocated";
  else
  {
    for (n=0; n<i; n++)
    {
      cout << "Enter number: ";
      cin >> p[n];
    }
    cout << "You have entered: ";
    for (n=0; n<i; n++)
      cout << p[n] << ", ";
    delete[] p;
  }
  return 0;
}
```

Notice how the value within the brackets in the new statement is a variable value entered by the user (i), not a constant expression:

```cpp
p = new (nothrow) int[i];
```

C++ integrates the operators new and delete for allocating dynamic memory. But these were not available in the C language; instead, it used a library solution, with the functions malloc, calloc, realloc, and free, defined in the header `<cstdlib>` (known as `<stdlib.h>` in C)

### Data structures

```cpp
struct product {
  int weight;
  double price;
} ;

product apple;
product banana, melon;
```

```cpp
struct product {
  int weight;
  double price;
} apple, banana, melon;
```

The arrow operator (->) is a dereference operator that is used exclusively with pointers to objects that have members. This operator serves to access the member of an object directly from its address. For example:

`pmovie->title`

is equivalent to:

`(*pmovie).title`

Something different:

`*pmovie.title`

which is equivalent to:

`*(pmovie.title)`


Expression | What is evaluated | Equivalent
---------|----------|---------
 a.b | Member b of object a | C1
 a->b | Member b of object pointed to by a | (*a).b
 *a.b | Value pointed to by member b of object a | *(a.b)

 ### Other data types

 #### Type aliases

 ```cpp
 typedef char C;
 typedef unsigned int WORD;
 typedef char * pChar;
 typedef char field [50];
 ```

 A second syntax to define aliases:

 ```cpp
 using C = char;
 using WORD = unsigned int;
 using pChar = char *;
 using field = char [50];
 ```

 Type aliases can be used to reduce the length of long or confusing names, but they are most useful as tools to abstract programs from the underlying types they use. For example, by using an alias of int to refer to a particular kind of parameter instead of using int directly, it allows for the type to be easily replaced by long (or some other type) in a later version, without having to change every instance where it is used.

 #### Unions

 Unions allow one portion of memory to be accessed as different data types. Its declaration and use is similar to the one of structures, but its functionality is totally different.

 This creates a new union type, identified by type_name, in which all its member elements occupy the same physical space in memory. The size of this type is the one of the largest member element. For example:

 ```cpp
 union mytypes_t {
   char c;
   int i;
   float f;
 } mytypes;
 ```

 Each of these members is of a different data type. But since all of them are referring to the same location in memory, the modification of one of the members will affect the value of all of them. It is not possible to store different values in them in any way that each is independent of the others.

 One of the uses of a union is to be able to access a value either in its entirety or as an array or structure of smaller elements. For example:

 ```cpp
 union mix_t {
   int l;
   struct {
     short hi;
     short lo;
   } s;
   char c[4];
 } mix;
 ```

 ### Anonymous unions

 When unions are members of a class (or structure), they can be declared with no name. In this case, they become anonymous unions, and its members are directly accessible from objects by their member names.

 Structure with regular union:

 ```cpp
 struct book1_t {
   char title[50];
   char author[50];
   union {
     float dollars;
     int yen;
   } price;
 } book1;
 ```

 Structure with anonymous union:

 ```cpp
 struct book2_t {
   char title[50];
   char author[50];
   union {
     float dollars;
     int yen;
   };
 } book2;
 ```

 For first example:

 ```
 book1.price.dollars
 book1.price.yen
 ```

 For second example:

 ```
 book2.dollars
 book2.yen
 ```

 ### Enumerated types (enum)

 ```cpp
 enum colors_t {black, blue, green, cyan, red, purple, yellow, white};
 ```

 Values of enumerated types declared with enum are implicitly convertible to an integer type. In fact, the elements of such an enum are always assigned an integer numerical equivalent internally, to which they can be implicitly converted to. If it is not specified otherwise, the integer value equivalent of the first possible value is 0.

 A specific integer value can be specified for any of the possible values in the enumerated type. And if the constant value that follows it is itself not given its own value, it is automatically assumed to be the same value plus one. For example:

 ```cpp
 enum months_t { january = 1, february, march, april,
                  may, june, july}
 ```

 ### Enumerated types with enum class

 It is possible to create real enum types that are neither implicitly convertible to int and that neither have enumerator values of type int, but of the enum type itself, thus preserving type safety. They are declared with enum class (or enum struct) instead of just enum:

 ```cpp
 enum class Colors {black, blue, green, cyan};
 ```

 Each of the enumerator values of an enum class type needs to be scoped in its type

 ```cpp
 Colors mycolor;

 mycolor = Colors::blue;
 if (mycolor == Colors::green)
    mycolor = Colors::red;
 ```

 ### Classes (I)

 Classes are an expanded concept of data structures: like data structures, they can contain data members, but they can also contain functions as members.

 An object is an instantiation of a class.

 An access specifies is one of the following three keywords: `private`, `public` and `protected`. These specifiers modify the access rights for the members that follow them:

 - `private` members of a class are accessible only from within other members of the same class
 - `protected` members are accessible from other members of the same class, but also from members of their derived classes.
 - Finally, `public` members are accessible from anywhere where the object is visible.

By default, all members of a class declared with the `class` keyword have private access for all its members. Therefore, any member that is declared before any other access specifier has private access automatically. For example:

```cpp
class Rectangle 
{
  int width, height;
  public:
    void set_values (int, int);
    int area (void);
} rect;
```

```cpp
// classes example
#include <iostream>
using namespace std;

class Rectangle {
    int width, height;
  public:
    void set_values (int,int);
    int area() {return width*height;}
};

void Rectangle::set_values (int x, int y) {
  width = x;
  height = y;
}

int main () {
  Rectangle rect;
  rect.set_values (3,4);
  cout << "area: " << rect.area();
  return 0;
}
```

The scope operator (::) specifies the class to which the member being defined belongs, granting exactly the same scope properties as if this function definition was directly included within the class definition. 

### Constructors

A class can include a special function called its constructor, which is automatically called whenever a new object of this class is created, allowing the class to initialize member variables or allocate storage.

```cpp
// example: class constructor
#include <iostream>
using namespace std;

class Rectangle {
    int width, height;
  public:
    Rectangle (int,int);
    int area () {return (width*height);}
};

Rectangle::Rectangle (int a, int b) {
  width = a;
  height = b;
}

int main () {
  Rectangle rect (3,4);
  Rectangle rectb (5,6);
  cout << "rect area: " << rect.area() << endl;
  cout << "rectb area: " << rectb.area() << endl;
  return 0;
}
```

### Overloading constructors

### Uniform initialization

```cpp
Circle foo (10.0);   // functional form
Circle bar = 20.0;   // assignment init.
Circle baz {30.0};   // uniform init.
Circle qux = {40.0}; // POD-like
```

### Member initialization in constructors

### Classes (II)

### Overloading operators

Operators are overloaded by means of operator functions, which are regular function with special names: their name begins by the `operator` keyword follewed by the operator sign that is overloaded.

```cpp
// overloading operators example
#include <iostream>
using namespace std;

class CVector {
  public:
    int x,y;
    CVector () {};
    CVector (int a,int b) : x(a), y(b) {}
    CVector operator + (const CVector&);
};

CVector CVector::operator+ (const CVector& param) {
  CVector temp;
  temp.x = x + param.x;
  temp.y = y + param.y;
  return temp;
}

int main () {
  CVector foo (3,1);
  CVector bar (1,2);
  CVector result;
  result = foo + bar;
  cout << result.x << ',' << result.y << '\n';
  return 0;
}
```

The function operator+ of class CVector overloads the addition operator (+) for that type. Once declared, this function can be called either implicitly using the operator, or explicitly using its functional name.

```cpp
c = a + b;
c = a.operator+ (b);
```

Notice that some operators may be overloaded in two forms: either as a member function or as a non-member function: the first case has been used in the example above for operator+. But some operators can also be overloaded as non-member functions; in this case, the operator function takes an object of the proper class as first argument.

```cpp
// non-member operator overloads
#include <iostream>
using namespace std;

class CVector {
  public:
    int x,y;
    CVector () {}
    CVector (int a, int b) : x(a), y(b) {}
};


CVector operator+ (const CVector& lhs, const CVector& rhs) {
  CVector temp;
  temp.x = lhs.x + rhs.x;
  temp.y = lhs.y + rhs.y;
  return temp;
}

int main () {
  CVector foo (3,1);
  CVector bar (1,2);
  CVector result;
  result = foo + bar;
  cout << result.x << ',' << result.y << '\n';
  return 0;
}
```

### The keyword this

```cpp
#include <iostream>
using namespace std;

class Dummy {
  public:
    bool isitme (Dummy& param);
};

bool Dummy::isitme (Dummy& param)
{
  if (&param == this) return true;
  else return false;
}

int main () {
  Dummy a;
  Dummy* b = &a;
  if ( b->isitme(a) )
    cout << "yes, &a is b\n";
  return 0;
}
```

### Static members

A class can contain static members, either data or functions.

A static data member of a class is also known as a "class variable", because there is only one common variable for all the objects of that same class, sharing the same value: ei, its value is not different from one object of this class to another.

```cpp
// static members in classes
#include <iostream>
using namespace std;

class Dummy {
  public:
    static int n;
    Dummy () { n++; };
};

int Dummy::n=0;

int main () {
  Dummy a;
  Dummy b[5];
  cout << a.n << '\n';
  Dummy * c = new Dummy;
  cout << Dummy::n << '\n';
  delete c;
  return 0;
}
```

In factm static members have the same properties as non-member variables but they enjoy class scope. For that reason, and to avoid them to be declared several times, they cannot initialized directly in the class, but they need to be initialized somewhere outside it.

Because is is a common variable for all the objects of the same class, it can be referred to as a member of any object of that class or even directly by the class name (of couse this is only valid for static members).

```cpp
cout << a.n;
cout << Dummy::n;
```

Classes can also have static member functions. These represent the same: members of a class that are common to all object of that class, acting exactly as non-member functions but being accessed like members of the class. 

### Const member functions

```cpp
const MyClass myobject;
```

The access to its data members from outside the class is restricted to read-only, as if all its data members were const for those accessing them from outside the class. Note though, that the constructor is still called and is allowed to initialize and modify these data members.

```cpp
// constructor on const object
#include <iostream>
using namespace std;

class MyClass {
  public:
    int x;
    MyClass(int val) : x(val) {}
    int get() {return x;}
};

int main() {
  const MyClass foo(10);
// foo.x = 20;            // not valid: x cannot be modified
  cout << foo.x << '\n';  // ok: data member x can be read
  return 0;
}
```

The member function of a const object can only be called if they are themselves specified as const members; in the example above, member get (which is not specified as const) cannot be called from foo. To specify that a member is a const member, the const keyword shall follow the function prototype, after the closing parenthesis for its parameters:

```cpp
int get() const {return x;} 
```

### Class templates

Just like we can create function templates, we can also create class templates, allowing classes to have members that use template parameters as type. For example:

```cpp
template <class T>
class mypair {
    T values [2];
  public:
    mypair (T first, T second)
    {
      values[0]=first; values[1]=second;
    }
};
```

### Template specialization

It is possible to define a different implementation for a template when a specific type is passed as template argument. This is called a template specialization.



### Special members

Special member functions are member functions that are implicitly defined as member of classes under certain cirumstances. These are six:

Member function | typical form for class C 
---------|----------
 Default constructor | C::C();
 Destructor | 	C::~C();
 Copy constructor | 	C::C (const C&);
 Copy assignment | C& operator= (const C&);
 Move constructor | 	C::C (C&&);
 Copy assignment | C& operator= (C&&);

### Default constructor

### Destructor

### Friendship and inheritance

### Friend function

In priciplem private and protected members of a class cannot be accessed from outside the same class in which they are declared. However this rule does not apply to "friends".

A non-member function can access the private and protected members of a class if it is declared by a friend of that class. That is done by including a declaration of this external function within the class, and preceding it with the keyword friend:

Typical use cases of friend functions are operations that are conducted between two different classes accessing private or protected members of both.

### Friend class

Similar to friend functions, a friend class is a class whose members have access to the private or protected members of another class

### Inheritance between classes

```cpp
// derived classes
#include <iostream>
using namespace std;

class Polygon {
  protected:
    int width, height;
  public:
    void set_values (int a, int b)
      { width=a; height=b;}
 };

class Rectangle: public Polygon {
  public:
    int area ()
      { return width * height; }
 };

class Triangle: public Polygon {
  public:
    int area ()
      { return width * height / 2; }
  };
  
int main () {
  Rectangle rect;
  Triangle trgl;
  rect.set_values (4,5);
  trgl.set_values (4,5);
  cout << rect.area() << '\n';
  cout << trgl.area() << '\n';
  return 0;
}
```

### Polymorphism

### Pointers to base class

One of the key features of class inheritance is that a pointer to a derived class is type-compatible with a pointer to its base class. Polymorphism is the art of taking advantage of this simple but powerful and versatile feature. 

```cpp
// pointers to base class
#include <iostream>
using namespace std;

class Polygon {
  protected:
    int width, height;
  public:
    void set_values (int a, int b)
      { width=a; height=b; }
};

class Rectangle: public Polygon {
  public:
    int area()
      { return width*height; }
};

class Triangle: public Polygon {
  public:
    int area()
      { return width*height/2; }
};

int main () {
  Rectangle rect;
  Triangle trgl;
  Polygon * ppoly1 = &rect;
  Polygon * ppoly2 = &trgl;
  ppoly1->set_values (4,5);
  ppoly2->set_values (4,5);
  cout << rect.area() << '\n';
  cout << trgl.area() << '\n';
  return 0;
}
```

Function main declares two pointers to Polygon (named ppoly1 and ppoly2). These are assigned the addresses of rect and trgl, respectively which are objects of type Rectangle and Triangle. Such assignments are valid, since both Rectangle and Triangle are classes derived from Polygon.

### Abstract base classes

Abstract base classes are something very similar to the Polygon class in the previous example. They are classes that can only be used as base classes, and thus are allowed to have virtual member functions without definition (known as pure virtual functions). The syntax is to replace their definition by =0.

An abstract base `Polygon` class could look like this:

```cpp
// abstract class CPolygon
class Polygon {
  protected:
    int width, height;
  public:
    void set_values (int a, int b)
      { width=a; height=b; }
    virtual int area () =0;
};
```

### Type conversions

### Implicit conversion

### Implicit conversions with classes

### Exceptions

Exceptions provide a way to react to exceptional circumstances (like runtime errors) in programs by transferring control to special functions called handlers.

To catch exceptionsm a portion of code is placed under exception inspection. This is done by enclosing that portion of code in a try-block. When an exceptional circumstance arises within that block, an exception is thrown that transfers the control the exception handler. If no exception is thrown, the code continues normally and all handlers are ignored.

An exception is thrown by using the throw keyword from inside the try block. Exception handlers are declared with the keyword catch, which must be placed immediately after the try block.

```cpp
// exceptions
#include <iostream>
using namespace std;

int main () {
  try
  {
    throw 20;
  }
  catch (int e)
  {
    cout << "An exception occurred. Exception Nr. " << e << '\n';
  }
  return 0;
}
```

The code under exception handling is enclosed in a try block. In this example this code simply throws an exception:

```cpp
throw 20;
```

A `throw` expression accepts one parameter (in this case the integer value 20), which is passed as an argument to the exception handler.

The exception handler is declared with the catch keyword immediately after the closing brace of the try block. The syntax for catch is similar to a regular function with one parameter. The type of this parameter is very important, since the type of the argument passed by the throw expression is checked against it, and only in the case they match, the exception is caught by that handler.

If an ellipsis(...) is used as the parameter of catch, that handler will catch any exception no matter what the type of the exception thrown. This can be used as a default handler that catches all exceptions not caught by other handlers.

```cpp
try {
  // code here
}
catch (int param) { cout << "int exception"; }
catch (char param) { cout << "char exception"; }
catch (...) { cout << "default exception"; }
```

### Standard exceptions

The C++ standard library provides a base class specifically designed to declare objects to be thrown as exceptions. It is called `std::exceptions` and is defined in the `<exceptions>` header. This class has a virtual member function called `what` that returns a null-terminated character sequence (of type char *) and that can be overwritten in derived classes to contain some sort of description of the exception.

```cpp
// using standard exceptions
#include <iostream>
#include <exception>
using namespace std;

class myexception: public exception
{
  virtual const char* what() const throw()
  {
    return "My exception happened";
  }
} myex;

int main () {
  try
  {
    throw myex;
  }
  catch (exception& e)
  {
    cout << e.what() << '\n';
  }
  return 0;
}
```

We have placed a handler that catches exception objects by reference (notice the ampersand & after the type), therefore this catches also classes derived from exception, like our myex object of type myexception.

A typical examnple where standard exceptions need to be checked for is on memory allocation:

```cpp
// bad_alloc standard exception
#include <iostream>
#include <exception>
using namespace std;

int main () {
  try
  {
    int* myarray= new int[1000];
  }
  catch (exception& e)
  {
    cout << "Standard exception: " << e.what() << endl;
  }
  return 0;
}
```

### Preprocessor directives

### Input/output with files

C++ provides the following classes to perform output and input of characters to/from files:

- `ofstream`: stream class to write on files
- `ifstream`: stream class to read from files
- `fstream`: stream class to both read and write from/to files

```cpp
// basic file operations
#include <iostream>
#include <fstream>
using namespace std;

int main () {
  ofstream myfile;
  myfile.open ("example.txt");
  myfile << "Writing this to a file.\n";
  myfile.close();
  return 0;
}
```

 ios::in | Open for input operations
 ios::out | Open for output operations
 ios::binary | Open in binary mode
 ios::ate | Set the initial position at the end of the file. If this flag is not set, the initial position is the beginning of the file.
 ios::app | All output operations are performed at the end of the file, appending the content to the current content of the file.
 ios::trunc | If the file is opened for output operations and it is already existed, its previous content is deleted and replaced by the new one.

 All these flags can be combined using the bitwise operator OR (|). For example, if we want to open the file `example.bin` in binary mode to add data we could do it by the following call to member function open.

 ```cpp
 ofstream myfile;
myfile.open ("example.bin", ios::out | ios::app | ios::binary); 
 ```

 Each of the open member functions of classes `ofstream`, `ifstream`, `fstream` has a default mode that is used if the file is opened without a second argument:

 