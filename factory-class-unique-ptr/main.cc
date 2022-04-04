#include <iostream>
#include <map>
#include <memory>

class A;

class A_Factory {
    public:
        A_Factory() {}
        virtual std::unique_ptr<A> create_unique() = 0;
};

class A {
    public:
        A() {}
        static void registerType(int n, A_Factory *factory) {
            get_factory_instance()[n] = factory;
        }
        static std::unique_ptr<A> create_unique(int n) {
            std::unique_ptr<A> A_instance = std::move(get_factory_instance()[n]->create_unique());
            return A_instance;
        }

        virtual void setMyId(int n) {}
        virtual void I_am() { std::cout << "I am A\n"; }
        virtual ~A() {}

    protected:
        int myId;
        static std::map<int, A_Factory *> &get_factory_instance() {
            static std::map<int, A_Factory *> map_instance;
            return map_instance;
        }
};

class B : public A {
    public:
        B() {}
        void Foo() {}
        void I_am() { std::cout << "I am B" << myId << "\n"; }
        void setMyId(int n) { myId=n; }
        ~B() {}
};

class B_Factory : public A_Factory {
    public:
        B_Factory() { A::registerType(1, this); }
        std::unique_ptr<A> create_unique() {
            std::unique_ptr<A> ptr_to_B(new B);
            return ptr_to_B;
        }
};

static B_Factory b0_factory;

int main() {
    std::unique_ptr<A> b1 = std::move(A::create_unique(1));
    std::unique_ptr<A> b2 = std::move(A::create_unique(1));
    b1->setMyId(10);
    b2->setMyId(20);
    b1->I_am();
    b2->I_am();

    return 0;
 }

