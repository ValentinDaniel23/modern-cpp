// Item 12: Declare overriding functions override.

#include <iostream>
#include <memory>
#include <vector>

class Widget
{
public:
    void doWork() &;  // this version of doWork applies
                      // only when *this is an lvalue
    void doWork() &&; // this version of doWork applies
                      // only when *this is an rvalue
};

class Widget1
{
public:
    using DataType = std::vector<double>;

    DataType &data() & // for lvalue Widgets,
    {
        return values;

    } // return lvalue
    DataType data() && // for rvalue Widgets,
    {
        return std::move(values);
    } // return rvalue
private:
    DataType values;
};

class Base
{
public:
    virtual void doWork(); // base class virtual function
};

class Derived : public Base
{
public:
    virtual void doWork() override;
    // overrides Base::doWork
    // ("virtual" is optional
}; // here)

void Base::doWork() {}
void Derived::doWork() {}

int main()
{
    std::unique_ptr<Base> upb =      // create base class pointer
        std::make_unique<Derived>(); // to derived class object;
                                     // see Item 21 for info on
                                     // std::make_unique

    upb->doWork(); // call doWork through base
    // class ptr; derived class
    // function is invoked

    return 0;
}

// Things to Remember
// • Declare overriding functions override.
// • Member function reference qualifiers make it possible to treat lvalue and
// rvalue objects (*this) differently.