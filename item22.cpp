// Item 22: When using the Pimpl Idiom, define special member functions in the implementation file.

#include <iostream>
#include <memory>

// That’s the technique whereby you replace the
// data members of a class with a pointer to an implementation class (or struct), put the
// data members that used to be in the primary class into the implementation class, and
// access those data members indirectly through the pointer.

// CPP98
class Widget
{ // still in header "widget.h"
public:
    Widget();
    ~Widget(); // dtor is needed—see below
private:
    struct Impl; // declare implementation struct
    Impl *pImpl; // and pointer to it
};

// #include "widget.h" // in impl. file "widget.cpp"
// #include "gadget.h"
// #include <string>
// #include <vector>
// struct Widget::Impl
// {                             // definition of Widget::Impl
//     std::string name;         // with data members formerly
//     std::vector<double> data; // in Widget
//     Gadget g1, g2, g3;
// };
// Widget::Widget()      // allocate data members for
//     : pImpl(new Impl) // this Widget object
// {
// }
// Widget::~Widget() // destroy data members for
// {
//     delete pImpl;
// } // this object

// CPP11
class Widget1
{ // in "widget.h"
public:
    Widget1();

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl; // use smart pointer
}; // instead of raw pointer

// #include "widget.h" // in "widget.cpp"
// #include "gadget.h"
// #include <string>
// #include <vector>
// struct Widget::Impl
// { // as before
//     std::string name;
//     std::vector<double> data;
//     Gadget g1, g2, g3;
// };
// Widget::Widget()                      // per Item 21, create
//     : pImpl(std::make_unique<Impl>()) // std::unique_ptr
// {} // via std::make_unique

int main()
{
    return 0;
}

// Things to Remember
// • The Pimpl Idiom decreases build times by reducing compilation dependencies
// between class clients and class implementations.
// • For std::unique_ptr pImpl pointers, declare special member functions in
// the class header, but implement them in the implementation file. Do this even
// if the default function implementations are acceptable.
// • The above advice applies to std::unique_ptr, but not to std::shared_ptr.