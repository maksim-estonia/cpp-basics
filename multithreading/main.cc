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