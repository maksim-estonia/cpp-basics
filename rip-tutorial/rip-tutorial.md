## Atomic Types

### Multi-threaded Access

An atomic type can be used to safely read and write to a memory location shared between two threads.

A BAD example that is likely to cause a data race:

```cpp
#include <thread>
#include <iostream>


//function will add all values including and between 'a' and 'b' to 'result'
void add(int a, int b, int * result) {
    for (int i = a; i <= b; i++) {
        *result += i;
    }
}

int main() {
    //a primitive data type has no thread safety
    int shared = 0;

    //create a thread that may run parallel to the 'main' thread
    //the thread will run the function 'add' defined above with paramters a = 1, b = 100, result = &shared
    //analogous to 'add(1,100, &shared);'
    std::thread addingThread(add, 1, 100, &shared);

    //attempt to print the value of 'shared' to console
    //main will keep repeating this until the addingThread becomes joinable
    while (!addingThread.joinable()) {
        //this may cause undefined behavior or print a corrupted value
        //if the addingThread tries to write to 'shared' while the main thread is reading it
        std::cout << shared << std::endl;  
    }


    //rejoin the thread at the end of execution for cleaning purposes
    addingThread.join();
    
    return 0;
}
```

An example with tread safety

```cpp
#include <atomic>
#include <thread>
#include <iostream>


    //function will add all values including and between 'a' and 'b' to 'result'
void add(int a, int b, std::atomic<int> * result) {
    for (int i = a; i <= b; i++) {
        //atomically add 'i' to result
        result->fetch_add(i);
    }
}

int main() {
    //atomic template used to store non-atomic objects
    std::atomic<int> shared = 0;

    //create a thread that may run parallel to the 'main' thread
    //the thread will run the function 'add' defined above with paramters a = 1, b = 100, result = &shared
    //analogous to 'add(1,100, &shared);'
    std::thread addingThread(add, 1, 10000, &shared);

    //print the value of 'shared' to console
    //main will keep repeating this until the addingThread becomes joinable
    while (!addingThread.joinable()) {
        //safe way to read the value of shared atomically for thread safe read
        std::cout << shared.load() << std::endl;  
    }


    //rejoin the thread at the end of execution for cleaning purposes
    addingThread.join();
    
    return 0;
}
```

The above example is safe because all `store()` and `load()` operations of the `atomic` data type protect the encapsulated `int` from simultaneous access.

## auto

The keyword `auto` is a typename that represents an automatically-deduced type.

### Basic auto sample

- Especially convenient when dealing with long type names:

    ```cpp
    std::map< std::string, std::shared_ptr< Widget > > table;
    // C++98
    std::map< std::string, std::shared_ptr< Widget > >::iterator i = table.find( "42" );
    // C++11/14/17
    auto j = table.find( "42" );
    ```

- range-based for loops:

    ```cpp
    vector<int> v = {0, 1, 2, 3, 4, 5};
    for(auto n: v)
        std::cout << n << ' ';
    ```

- with lambdas

    ```cpp
    auto f = [](){ std::cout << "lambda\n"; };
    f(); 
    ```

### Generic lamda (C++14)

```cpp
auto print = [](const auto& arg) { std::cout << arg << std::endl; };

print(42);
print("hello world");
```

## C++ Containers

![images/rip-tutorial/containers.png](/images/rip-tutorial/containers.png)

## C++ Streams

### Reading a file

A proper way to read a text file line-by-line till the end is usually not clear from `ifstream` documentation. Let's consider some common mistakes done by beginner C++ programmers, and a proper way to read the file.

#### Lines without whitespace characters

For the sake of simplicity, let's assume that each line in the file contains no whitespace symbols.

`ifstream` has `operator bool()`, which returns true when a stream has no errors and is ready to read. Moreover, `ifstream::operator >>` returns a reference to the stream itself, so we can read and check for EOF (as well as for errors) in one line with very elegant syntax:

```cpp
std::ifstream ifs("1.txt");
std::string s;
while(ifs >> s) {
    std::cout << s << std::endl;
}
```

#### Lines with whitespace characters

`ifstream::operator >>` reads the stream until any whitespace character occurs, so the above code will print the words from a line on seperate lines. To read everything till the end of the line, use `std::getline` instead of `ifstream::operator >>`. `getline` returns reference to the the thread it worked with, so the same syntax is available:

```cpp
while(std::getline(ifs, s)) {
    std::cout << s << std::endl;
}
```

#### Reading a file into a buffer at once

Finally, let's read the file from the beginning till the end without stopping at any character, including white-spaces and newlines. If we know the exact file size of upper bound of the length is acceptable, we can resize the string and then read:

```cpp
s.resize(100);
std::copy(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(),
    s.begin());
```

Otherwise, we need to insert each character to the end of the string, so `std::back_inserter` is what we need:

```cpp
std::copy(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(),
    std::back_inserter(s));
```

Alternatively, it is possible to initialize a collection with stream data, using a constructor with iterator range arguments:

```cpp
std::vector v(std::istreambuf_iterator<char>(ifs),
    std::istreambuf_iterator<char>());
```

Note that these examples are also applicable if `ifs` is opened as binary file:

```cpp
std::ifstream ifs("1.txt", std::ios::binary);
```

#### Copying streams

A file may be copied to another file with streams and iterators:

```cpp
std::ofstream ofs("out.file");
std::copy(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(),
    std::ostream_iterator<char>(ofs));
ofs.close();
```

or redirected any other type of stream with a compatible interface. For example Boost.Asio network stream:

```cpp
boost::asio::ip::tcp::iostream stream;
stream.connect("example.com", "http");
std::copy(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(),
    std::ostream_iterator<char>(stream));
stream.close();
```

### String streams

`std::ostringstream` is a class whose objects look like an output stream (this is, you can write to them via `operator<<`), but actually store the writing results, and provide them in the form of a stream.

Consider the following short code:

```cpp
#include <sstream>
#include <string>                                                                                                                          

using namespace std;

int main()
{
    ostringstream ss;
    ss << "the answer to everything is " << 42;
    const string result = ss.str(); 
}  
```

This is mainly useful when we have a class for which stream serialization has been defined, and for which we want a string form. For example, suppose we have some class

```cpp
class foo 
{   
    // All sort of stuff here.
};  

ostream &operator<<(ostream &os, const foo &f);
```

To get the string representation of a `foo` object.

```
foo f;
```

we could use

```cpp
ostringstream ss; 
ss << f;
const string result = ss.str();    
```

Then `result` contains the string representation of the `foo` object.

## C++11 Memory Model

The memory model introduced in C++11 defines two new portable ways to synchronize access to memory in multi-threaded environment: atomic operations and fences.

### Atomic operations

It is now possible to read and write to given memory location by the use of atomic load and atomic store operations. For convenience these are wrapped in the `std::atomic<t>` template class. This class wraps a value of type `t` but this time loads and stores to the object are atomic.

## Classes/Structures

Note that the only difference between the `struct` and `class` keywords is that by default, the member variables, member functions, and base classes of a `struct` are `public`, while in a `class` they are `private`. C++ programmers tend to call it a class if it has constructors and destructors, and the ability to enforce its own invariants; or a struct if it's just a simple collection of values, but the C++ language itself makes no distinction.

### Basics

A class is a user-defined type. A class is introduced with the `class`, `struct` or `union` keyword. 

A class is a collection of class members, which can be:

- member variables (also called "fields")
- member functions (also called "methods")
- member types or typedefs ("nested classes")
- member templates (of any kind: variable, function, class or alias template)

## Const Correctness

`const` correctness is a very useful troubleshooting tool, as it allows the programmer to quickly determine which functions might be inadvertently modifying code. It also prevents unintetional errors from compiling properly and going unnoticed.

It is much easier to design a class for `const` correctness, than it is to later add `const` correctness to a pre-existing class. If possible, design any class that can be `const` correct so that it is `const` correct, to save yourself and others the hassle of later modifying it.

Note that this can also be applied to volatile correctness if necessary, with the same rules as for `const` correctness, but this is also used much less often.

### The basics

`const` correctness is the practice of designing code so that only code that needs to modify an instance is able to modify an instance (i.e. has write access), and conversely, that any code that doesn't need to modify an instance is unable to do so (i.e. only has read access). This prevents the instance from being modified unintentionally, making code less errorprone, and documents whether the code is intended to change the instance's state or not. It also allows instances to be treated as `const` whenever they don't need to be modified, or defined as `const` if they don't need to be changed after initialisation, without losing any functionality.

```cpp
class ConstCorrectClass {
    int x;

  public:
    int getX() const { return x; } // Function is const: Doesn't modify instance.
    void setX(int i) { x = i; }    // Not const: Modifies instance.
};

// Parameter is const: Doesn't modify parameter.
int const_correct_reader(const ConstCorrectClass& c) {
    return c.getX();
}

// Parameter isn't const: Modifies parameter.
void const_correct_writer(ConstCorrectClass& c) {
    c.setX(42);
}

const ConstCorrectClass invariant; // Instance is const: Can't be modified.
ConstCorrectClass         variant; // Instance isn't const: Can be modified.

// ...

const_correct_reader(invariant); // Good.   Calling non-modifying function on const instance.
const_correct_reader(variant);   // Good.   Calling non-modifying function on modifiable instance.

const_correct_writer(variant);   // Good.   Calling modifying function on modifiable instance.
const_correct_writer(invariant); // Error.  Calling modifying function on const instance.
```

### Const Correct Class Design

In a const-correct class, all member functions which don't change logical state have `this` cv-qualified as `const`, indicating that they don't modify the object.

```cpp
// Assume class Field, with member function "void insert_value(int);"

class ConstIncorrect {
    Field fld;

    public:
        ConstIncorrect(Field& f);   // Modifies

        Field& getField();          // Might modify. Also exposes member as non-const 
                                    // reference, allowing indirect modification.
        void setField(Field& f);    // Modifies

        void doSomething(int i);    // Might modify
        void doNothing();           // Might modify
};

ConstIncorrect::ConstIncorrect(Field& f) : fld(f) {}    // Modifies
Field& ConstIncorrect::getField() { return fld; }       // Doesn't modify
void ConstIncorrect::setField(Field& f) { fld = f; }    // Modifies
void ConstIncorrect::doSomething(int i) {               // Modifies
    fld.insert_value(i);
}
void ConstIncorrect::doNothing() {}                     // Doesn't modify

class ConstCorrectCVQ {
    Field fld;

    public:
        ConstCorrectCVQ(Field& f);   // Modifies

        const Field& getField() const;  // Doesn't modify. Exposes member as 
                                        // const reference preventing indirect
                                        // modification
        void setField(Field& f);        // Modifies

        void doSomething(int i);        // Modifies
        void doNothing() const;         // Doesn't modify
};

ConstCorrectCVQ::ConstCorrectCVQ(Field& f) : fld(f) {}
Field& ConstCorrectCVQ::getField() const { return fld; }
void ConstCorrectCVQ::setField(Field& f) { fld = f; }
void ConstCorrectCVQ::doSomething(int i) {
    fld.insert_value(i);
}
void ConstCorrectCVQ::doNothing() const {}

// This won't work
// No member functions can be called on const ConstIncorrect instances
void const_correct_func(const ConstIncorrect& c) {
    Field f = c.getField();
    c.do_nothing(); //<- tries to modify here even though const on entrance
}

// But this will
// getField() and doNothing() can be called on const ConstCorrectCVQ instances
void const_correct_func(const ConstCorrectCVQ& c)
{
    Field f = c.getField();
    c.do_nothing();
}
```

### Const Correct Function Parameters

### Const Correctness as Documentation

## Design pattern implementation in C++

### Adapter Pattern

### Builder Pattern with Fluent API

### Factory Pattern

### Observer Pattern

## Enumeration

### Basic enumeration declaration

Standard enumeration allows users to declare a useful name for a set of integers. The names are collectively referred to as enumerators. An enumeration and its associated enumerators are defined as follows:

```cpp
enum myEnum
{
    enumName1,
    enumName2,
};
```

An enumeration is a type, one which is distinct from all other types. In this case, the name of this type is `myEnum`. Objects of this type are expected to assume the value of an enumerator within the enumeration.

The scope operator can be optionally used to access an enumerator within an enumeration. So `enumName1` can also be spelled `myEnum::enumName1`. 

### Enumeration in switch statements

A common use for enumerators is for switch statements and so they commonly appear in state machines. A useful feature of switch statements with enumerations is that if no default statement is included for the switch, and not all values of the enum have been utilized, the compiler will issue a warning.

```cpp
enum State {
    start,
    middle,
    end
};

...

switch(myState) {
    case start:
       ...
    case middle:
       ...
} // warning: enumeration value 'end' not handled in switch [-Wswitch]
```

## Exceptions

### Best practice: throw by value, catch by const reference

In general, it is considered good practice to throw by value (rather than by pointer), but catch by (const) reference.

```cpp
try {
    // throw new std::runtime_error("Error!");   // Don't do this!
    // This creates an exception object
    // on the heap and would require you to catch the
    // pointer and manage the memory yourself. This can
    // cause memory leaks!
    
    throw std::runtime_error("Error!");
} catch (const std::runtime_error& e) {
    std::cout << e.what() << std::endl;
}
```

One reason why catching by reference is a good practice is that it eliminates the need to reconstruct the object when being passed to the catch block (or when propagating through to other catch blocks). 

### Catching exceptions

A `try/catch` block is used to catch exceptions. The code in the `try` section is the code that may throw an exception, and the code in the `catch` clause handles the exception.

```cpp
#include <iostream>
#include <string>
#include <stdexcept>

int main() {
  std::string str("foo");
  
  try {
      str.at(10); // access element, may throw std::out_of_range
  } catch (const std::out_of_range& e) {
      // what() is inherited from std::exception and contains an explanatory message
      std::cout << e.what();
  }
}
```

Multiple `catch` clauses may be used to handle multiple exception types. If multiple `catch` clauses are present, the exception handling mechanism tries to match them in order of their appearance in the code:

```cpp
std::string str("foo");
  
try {
    str.reserve(2); // reserve extra capacity, may throw std::length_error
    str.at(10); // access element, may throw std::out_of_range
} catch (const std::length_error& e) {
    std::cout << e.what();
} catch (const std::out_of_range& e) {
    std::cout << e.what();
}
```

Exception classes which are derived from a common base class can be caught with a single `catch` clause for the common base class. The above example can replace the two `catch` clauses for `std::length_error` and `std::out_of_range` with a single cause for `std::exception`:

```cpp
std::string str("foo");
  
try {
    str.reserve(2); // reserve extra capacity, may throw std::length_error
    str.at(10); // access element, may throw std::out_of_range
} catch (const std::exception& e) {
    std::cout << e.what();
}
```

Another possibility is the catch-all handler, which will catch any thrown object:

```cpp
try {
    throw 10;
} catch (...) {
    std::cout << "caught an exception";
}
```

### Function Try Block for regular function

```cpp
void function_with_try_block() 
{
    try
    {
        // try block body
    } 
    catch (...) 
    { 
        // catch block body
    }
}
```

## Explicit type conversion

An expression can be explicitly converted or cast to type `T` using `dynamic_cast<T>`, `static_cast<T>`, `reinterpret_cast<T>`, or `const_cast<T>`, depending on what type of cast is intended.

C++ also supports function-style cast notation, `T(expr)`, and C-style cast notation, `(T)expr`.

- Casting to an lvalue reference type, as in `dynamic_cast<Derived&>(base)`, yields an lvalue. Therefore, when you want to do something with the same object but treat it as a different type, you would cast to an lvalue reference type.
- Casting to an rvalue reference type, as in `static_cast<string&&>(s)`, yields an rvalue. 
- Casting to a non-reference type, as in `(int)x`, yields a prvalue, which may be thought of as a copy of the value being cast, but with a different type from the original.

### Base to derived conversion

### Casting away constness

### Conversion between pointer and integer

### C-style casting

## File IO

C++ file IO is done via streams. The key abstractions are:

- `std::istream` for reading text
- `std::ostream` for writing text
- `std::streambuf` for reading or writing characters

More on streams: <iostream> library

### Cheching end of file inside a loop function, bad practice?

`eof` returns `true` only after reading the end of file. It does not indicate that the next read will be the end of stream.

```cpp
while (!f.eof())
{
  // Everything is OK

  f >> buffer;

  // What if *only* now the eof / fail bit is set?

  /* Use `buffer` */
}
```

You could correctly write:

```cpp
while (!f.eof()) 
{  
  f >> buffer >> std::ws;

  if (f.fail())
    break;

  /* Use `buffer` */
}
```

but

```cpp
while (f >> buffer)
{
  /* Use `buffer` */
}
```

is simpler and less error prone.

Further references:

- `std::ws`: discards leading whitespace from an input stream
- `std::basic_ios::fail`: returns `true` if an error has occurred on the associated stream

### Closing a file

Explicitly closing a file is rarely necessary in C++, as a file stream will automatically close its associated file in its destructor. However, you should try to limit the lifetime of a file stream object, so that it does not keep the file handle open longer than necessary. For example, this can be done by putting all file operations into an own scope ({}):

```cpp
std::string const prepared_data = prepare_data();
{
    // Open a file for writing.
    std::ofstream output("foo.txt");

    // Write data.
    output << prepared_data;
}  // The ofstream will go out of scope here.
   // Its destructor will take care of closing the file properly.
```

Calling `close()` explicitly is only necessary if you want to reuse the same `fstream` object later, but don't want to keep the file open in between:

```cpp
// Open the file "foo.txt" for the first time.
std::ofstream output("foo.txt");

// Get some data to write from somewhere.
std::string const prepared_data = prepare_data();

// Write data to the file "foo.txt".
output << prepared_data;

// Close the file "foo.txt".
output.close();

// Preparing data might take a long time. Therefore, we don't open the output file stream
// before we actually can write some data to it.
std::string const more_prepared_data = prepare_complex_data();

// Open the file "foo.txt" for the second time once we are ready for writing.
output.open("foo.txt");

// Write the data to the file "foo.txt".
output << more_prepared_data;

// Close the file "foo.txt" once again.
output.close();
```

### Copying a file

```cpp
std::ifstream  src("source_filename", std::ios::binary);
std::ofstream  dst("dest_filename",   std::ios::binary);
dst << src.rdbuf();
```

With C++17 the standard way to copy a file is including the `<filestream>` header and using `copy_file`: 

```cpp
std::fileystem::copy_file("source_filename", "dest_filename");
```

### Flushing a stream

File streams are buffered by default, as are many other types of streams. This means that writes to the stream may not cause the underlying file to change immediately. In order to force all buffered writes to take place immediately, you can flush the stream. You can do this either directly by invoking the `flush()` method or through the `std::flush` stream manipulator.

```cpp
std::ofstream os("foo.txt");
os << "Hello World!" << std::flush;

char data[3] = "Foo";
os.write(data, 3);
os.flush();
```

There is a stream manipulator `std::endl` that combines writing a newline with flushing the stream:

```cpp
// Both following lines do the same thing
os << "Hello World!\n" << std::flush;
os << "Hello world!" << std::endl;
```

Buffering can improve the performance of writing to stream. Therefore, applications that do a lot of writing should avoid flushing unnecessarily. Contrary, if IO is done infrequently, applications should consider flushing frequently in order to avoid data getting stuck in the stream object.

### Opening a file

Opening a file is done in the same way for all 3 file streams (`ifstream`, `ofstream` and `fstream`). 

You can open the file directly in the constructor

```cpp
std::ifstream ifs("foo.txt");  // ifstream: Opens file "foo.txt" for reading only.

std::ofstream ofs("foo.txt");  // ofstream: Opens file "foo.txt" for writing only.

std::fstream iofs("foo.txt");  // fstream:  Opens file "foo.txt" for reading and writing.
```

Alternatively, you can use the file stream's member function `open()`:

```cpp
std::ifstream ifs;
ifs.open("bar.txt");           // ifstream: Opens file "bar.txt" for reading only.

std::ofstream ofs;
ofs.open("bar.txt");           // ofstream: Opens file "bar.txt" for writing only.

std::fstream iofs;
iofs.open("bar.txt");          // fstream:  Opens file "bar.txt" for reading and writing.
```

You should always check if a file has been opened successfully (even when writing). Failures can include: the file doesn't exist, file hasn't the right access rights, file is already in use, disk errors occured, drive disconnected...

```cpp
// Try to read the file 'foo.txt'.
std::ifstream ifs("fooo.txt");  // Note the typo; the file can't be opened.

// Check if the file has been opened successfully.
if (!ifs.is_open()) {
    // The file hasn't been opened; take appropriate actions here.
    throw CustomException(ifs, "File could not be opened");
}
```

When file path contains backlashes (for example, on Windows system) you should properly escape them:

```cpp
// Open the file 'c:\folder\foo.txt' on Windows.
std::ifstream ifs("c:\\folder\\foo.txt"); // using escaped backslashes
```

Or use raw literal:

```cpp
// Open the file 'c:\folder\foo.txt' on Windows.
std::ifstream ifs(R"(c:\folder\foo.txt)"); // using raw literal
```

or use forward slashes instead:

```cpp
// Open the file 'c:\folder\foo.txt' on Windows.
std::ifstream ifs("c:/folder/foo.txt");
```

### Opening modes

When creating a file stream, you can specify an opening mode. An opening mode is basically a setting to control how the stream opens the file.

An opening mode can be provided as a second parameter to the constructor of a file stream or to its `open()` member function:

```cpp
std::ofstream os("foo.txt", std::ios::out | std::ios::trunc);

std::ifstream is;
is.open("foo.txt", std::ios::in | std::ios::binary);
```

If you don't specify an opening mode, then the following default modes are used:

- ifstream - in
- ofstream - out
- fstream - in and out

The file opening modes that you may specify by design are:


Mode | Meaning | For  | Description
---------|----------|---------  |---------
 `app`   | append   | Output    | Appends data at the end of the file
 `binary`| binary   | Input/output | Input and output is done in binary
 `in`    | B3       | Input     | Opens the file for reading
 `out`   | B3       | Output    | Opens the file for writing
 `trunc` | B3       | Input/Output | Removes contents of the file when opening
 `ate`   | B3       | Input     | Goes to the end of the file when opening




### Reading a 'struct' from a formatted text file

### Reading a file into a container

### Reading an ASCII file into a std::string

### Reading from a file

### Writing files with non-standard locale settings

### Writing to a file

## Header Files

In C++, as in C, the C++ compiler and compilation process makes use of the C preprocessor. As specified by the GNU C Preprocessor manual, a header file is defined as the following:

A header file is a file containing C declarations and macro definitions (see Macros) to be shared between several source files. 

Header files serve two purposes:

- System header files declare the interfaces to parts of the operating system. You include them in your program to supply the definitions and declarations you need to invoke system calls and libraries. 
- Your own header files contain declarations for interfaces between the source files of your program. Each time you have group of related declarations and macro definitions all or most of which are needed in several different source files, it is a good idea to create a header file for them.

## Move semantics

## Non-Static Member Functions

A non-`static` member function is a `class`/`struct`/`union` member function, which is called on a particular instance, and operates on said instance. Unlike `static` member functions, it cannot be called without specifying an instance.

A `class` or `struct` can have member functions as well as member variables. These functions have a syntax mostly similar to standalone functions, and can be defined either inside or outside the class definition; if defined outside the class definition, the function's name is prefixed with the class' name and the scope (`::`) operator.

```cpp
class CL {
  public:
    void  definedInside() {}
    void definedOutside();
};
void CL::definedOutside() {}
```

### Const Correctness

One of the primary uses for `this` cv-qualifiers is `const` correctness. This is the practice of guaranteeing that only accesses that need to modify an object are able to modify an object. This prevents unintentional modifications, making code less errorprone. It also allows any function that doesn't need to modify state to be able to take either a `const` or non-`const` object, without needing to rewrite or overload the function. 

```cpp
class ConstIncorrect {
    Field fld;

  public:
    ConstIncorrect(const Field& f) : fld(f) {}     // Modifies.

    const Field& get_field()       { return fld; } // Doesn't modify; should be const.
    void set_field(const Field& f) { fld = f; }    // Modifies.

    void do_something(int i) {                     // Modifies.
        fld.insert_value(i);
    }
    void do_nothing()        { }                   // Doesn't modify; should be const.
};

class ConstCorrect {
    Field fld;

  public:
    ConstCorrect(const Field& f) : fld(f) {}       // Not const: Modifies.

    const Field& get_field() const { return fld; } // const: Doesn't modify.
    void set_field(const Field& f) { fld = f; }    // Not const: Modifies.

    void do_something(int i) {                     // Not const: Modifies.
        fld.insert_value(i);
    }
    void do_nothing() const  { }                   // const: Doesn't modify.
};

// ...

const ConstIncorrect i_cant_do_anything(make_me_a_field());
// Now, let's read it...
Field f = i_cant_do_anything.get_field();
  // Error: Loses cv-qualifiers, get_field() isn't const.
i_cant_do_anything.do_nothing();
  // Error: Same as above.
// Oops.

const ConstCorrect but_i_can(make_me_a_field());
// Now, let's read it...
Field f = but_i_can.get_field(); // Good.
but_i_can.do_nothing();          // Good.
```

## Operator Overloading

