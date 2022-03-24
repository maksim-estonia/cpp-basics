## Multithreading in C++

from: https://www.geeksforgeeks.org/multithreading-in-cpp/

std::thread is the thread class that represents a single thread in C++. To start a thread we simply need to create a new thread object and pass the executing code to be called (ie, a callable object) into the constructor of the object. 

### Launching thread using function pointer

```cpp
void foo(param)
{
    // do something
}

// The parameters to the function are put after the comma
std::thread thread_obj (foo, params);
```

## Launching threads using function objects

```cpp
// define the class of function object
class fun_object_class {
    // Overload () operator
    void operator()(params)
    {
        // do something
    }
}

// create thread object
std::thread thread_object(fn_object_class(), params)
```

## Waiting for threads to finish

To wait for a thread use the std::thread::join() function. This function makes the current thread wait until the thread identified by *this has finished executing.

```cpp
int main()
{
    // Start thread t1
    std::thread t1(callable);

    // wait for t1 to finish
    t1.join();

    // t1 has finished do other stuff

    ...
}
```

## A complete C++ program

A C++ program is given below. It launches two threads from the main function. Each thread is called using one of the callable objects specified above. 

```cpp
// CPP program to demonstrate multithreading
// using two different callables.
#include <iostream>
#include <thread>
using namespace std;

// a dummy function
void foo(int Z)
{
    for (int i = 0; i < Z; i++) {
        cout << "Thread using function pointer as callable\n";
    }
}

// a callable object
class thread_obj {
    public:
        void operator()(int x)
        {
            for (int i = 0; i < x; i++)
                cout << "Thread using function object as callable \n";
        }
};

int main()
{
    cout << "Threads 1 and 2 operating indepedently" << endl;

    // This thread is launched by using function pointer as callable
    thread th1(foo, 3);

    // This thread is launched by using function object as callable
    thread th2(thread_obj(), 3);

    // Wait for the threads to finish
    // Wait for thread t1 to finish
    th1.join();

    // Wait for thread t2 to finish
    th2.join();

    return 0;
    
}
```

execution:

```
g++ -pthread main.cc -o main
./main
```

![multithreading/multithreading.png](/multithreading/multithreading.png)