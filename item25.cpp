// Item 25: Use std::move on rvalue references, std::forward on universal references.

#include <iostream>
#include <memory>

namespace item25 {

    class Widget {
    public:
        template<typename T>
        void setName(T&& newName) {     // universal reference
            name = std::move(newName);  // compiles, but is
        }                               // bad, bad, bad!

    private:
        class SomeDataStructure {};
        std::string name;
        std::shared_ptr<SomeDataStructure> p;
    };

    class Matrix{
    public:
            Matrix& operator+=(const Matrix& rhs) {
                return *this;
            }
    };
    class Matrix1{
    public:
            Matrix1& operator+=(const Matrix1& rhs) {
                return *this;
            }
    };

    Matrix operator+(Matrix&& lhs, const Matrix& rhs) {
        lhs += rhs;
        return std::move(lhs);  // move lhs into
                                // return value
    }

    Matrix1 operator+(Matrix1&& lhs, const Matrix1& rhs) {
        lhs += rhs;
        return lhs; // copy lhs into
    }               // return value

    // DONT FORGET!!! Rvalue cand bind to CONST Lvalue reference

    class Widget1 {};

    Widget1 makeWidget() {
        Widget1 w; // local variable
        return w; // "copy" w into return value
    }
    // Both conditions are fulfilled here, and you can trust me when I tell you that for this
    // code, every decent C++ compiler will employ the RVO to avoid copying w. That
    // means that the “copying” version of makeWidget doesn’t, in fact, copy anything.

    Widget1 makeWidget1() // Moving version of makeWidget
    {
        Widget1 w;
        return std::move(w);   // move w into return value
    }                          // (don't do this!)
}

template<typename T>
void f(T &&x)
{
    std :: cout << &x << '\n';  // same address
}

int main()
{
    int x{10};

    std :: cout << &x << '\n';
    f(x);   // same address
    f(std::move(x));    // same address
    f(5);   // different address

    return 0;
}

// Things to Remember
// • Apply std::move to rvalue references and std::forward to universal references
// the last time each is used.
// • Do the same thing for rvalue references and universal references being
// returned from functions that return by value.
// • Never apply std::move or std::forward to local objects if they would otherwise
// be eligible for the return value optimization.