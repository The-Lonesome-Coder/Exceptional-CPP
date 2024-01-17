#include <complex>
#include <iostream>
/**
 * Item 21 - Overriding Virtual Functions
 *
 * Make base class destructors virtual (unless you are certain that no one will ever attempt to delete a derived object
 * through a pointer to base).
 *
 * When providing a function with the same name as an inherited function, be sure to bring the inherited functions into scope
 * with a using-declaration if you don't want to hide them.
 *
 * Never change the default parameters of overridden inherited functions.
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * What did the programmer probably expect the program to print, and what is the actual result?
*/

class Base
{
    public:
        virtual void f(int number);
        virtual void f(double number);
        virtual void g(int number = 10);
};

void Base::f(int number)
{
    std::cout << "Base::f(int number)" << std::endl;
}

void Base::f(double number)
{
    std::cout << "Base::f(double number)" << std::endl;
}

void Base::g(int number)
{
    std::cout << number << std::endl;
}


class Derived : public Base
{
    public:
        void f(std::complex<double> number);
        void g(int number = 20) override;
};

/**
 * This is not an overload. This distinction is very important, because it means that Base::f(int) and Base::f(double) are
 * not visible in the scope of Derived. Usually, however, the hiding is inadvertent and surprising, and the correct way to
 * bring the names into the scope of Derived is to write the using-declaration "using Base::f;".
*/
void Derived::f(std::complex<double> number)
{
    std::cout << "Derived::f(std::complex<double> number)" << std::endl;
}

/**
 * This overrides Base::g() but changes the default parameter.  Unless you're really out to confuse people, don't change the
 * default parameters of the inherited functions you override.
*/
void Derived::g(int number)
{
    std::cout << "Derived::g(int number)" << std::endl;
}

int main()
{
    Base b;
    Derived d;
    Base* p_b = new Derived;

    /**
     * This calls Base:::f(double number), as expected.
    */
    b.f(1.0);

    /**
     * This calls Derived::f(std::complex<double> number). Derived doesn't declare "using Base::f;" to bring the Base
     * functions named f into scope, and so, clearly, Base::f(int number) and Base::f(double number) can't be called. They
     * are not present in the same scope as Derived::f(std::complex<double> number) so as to participate in overloading.
    */
    d.f(1.0);

    /**
     * Even though the Base* pb is pointing to a Derived object, this calls Base::f(double number), because overload
     * resolution is done on the static type (Base), not the dynamic type (Derived). For the same reason, the call
     * "pb->f(complex<double>(1.0));" would not compile, because there is no satisfactory function in the Base interface.
    */
    p_b->f(1.0);

    /**
     * This prints "10", because it simply invokes Base::g( int ) whose parameter defaults to the value 10.
    */
    b.g();

    /**
     * This prints "Derived::g() 20", because it simply invokes Derived::g( int ) whose parameter defaults to the value 20.
    */
    d.g();

    /**
     * This prints "Derived::g() 10". Default parameters are taken from the static type (Base) of the object, hence the
     * default value of 10 is taken. However, the function happens to be virtual, so the function actually called is based on
     * the dynamic type (here Derived) of the object.
    */
    p_b->g();

    /**
     * delete p_b is unsafe. deleting via a pointer-to-base without a virtual destructor is evil, pure and simple, and
     * corruption is the best thing you can hope for, because the wrong destructor will get called and operator delete() will
     * be invoked with the wrong object size.
    */
    delete p_b;
}