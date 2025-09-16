//Item 24: Distinguish universal references from rvalue references.

#include <iostream>

// The other meaning for “T&&” is either rvalue reference or lvalue reference. Such references
// look like rvalue references in the source code (i.e., “T&&”), but they can behave
// as if they were lvalue references (i.e., “T&”). Their dual nature permits them to bind to
// rvalues (like rvalue references) as well as lvalues (like lvalue references). Furthermore,
// they can bind to const or non-const objects, to volatile or non-volatile objects,
// even to objects that are both const and volatile. They can bind to virtually anything.
// Such unprecedentedly flexible references deserve a name of their own. I call
// them universal references.

// Because universal references are references, they must be initialized. The initializer
// for a universal reference determines whether it represents an rvalue reference or an
// lvalue reference. If the initializer is an rvalue, the universal reference corresponds to
// an rvalue reference. If the initializer is an lvalue, the universal reference corresponds
// to an lvalue reference.

// Even the simple presence of a const qualifier is enough to disqualify a reference from
// being universal:
template<typename T>
void f(const T&& param); // param is an rvalue reference


void f(auto&& x) {
    std :: cout << x << '\n';
}

int main()
{
    // void f(Widget&& param); // rvalue reference
    // Widget&& var1 = Widget(); // rvalue reference
    // auto&& var2 = var1; // not rvalue reference
    // template<typename T>
    // void f(std::vector<T>&& param); // rvalue reference
    // template<typename T>
    // void f(T&& param); // not rvalue reference

    f(10);

    return 0;
}

// Things to Remember
// • If a function template parameter has type T&& for a deduced type T, or if an
// object is declared using auto&&, the parameter or object is a universal reference.
// • If the form of the type declaration isn’t precisely type&&, or if type deduction
// does not occur, type&& denotes an rvalue reference.
// • Universal references correspond to rvalue references if they’re initialized with
// rvalues. They correspond to lvalue references if they’re initialized with lvalues.