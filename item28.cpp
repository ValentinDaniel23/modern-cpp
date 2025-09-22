// Item 28: Understand reference collapsing.

#include <iostream>

// If either reference is a lvalue reference, the result is a lvalue reference.
// Otherwise (i.e., if both are rvalue references) the result is a rvalue reference.

class Widget {

};

Widget widgetFactory() {
    return Widget();
}

template<typename T>
void func(T&& param) {
    std :: cout << "Hey\n";
}

template<typename T>
T&& forward(typename std :: remove_reference<T>::type& param)
{
    return static_cast<T&&>(param);
}

template<typename T>
void f(T&& fParam)
{
    std::forward<T>(fParam);
}

// Suppose that the argument passed to f is a lvalue of type Widget. T will be deduced
// as Widget&, and the call to std::forward will instantiate as std::forward
// <Widget&>. Plugging Widget& into the std::forward implementation yields this:
// Widget& && forward(typename
// remove_reference<Widget&>::type& param)
// { return static_cast<Widget& &&>(param); }

// Reference collapsing is also applied to the return type and the cast, and the result is
// the final version of std::forward for the call:
// Widget& forward(Widget& param) // still in
// { return static_cast<Widget&>(param); } // namespace std

// Now suppose that the argument passed to f is a rvalue of type Widget. In this case,
// the deduced type for f’s type parameter T will simply be Widget. The call inside f to
// std::forward will thus be to std::forward<Widget>. Substituting Widget for T in
// the std::forward implementation gives this:

// Widget&& forward(typename
// remove_reference<Widget>::type& param)
// { return static_cast<Widget&&>(param); }

// Widget&& forward(Widget& param)
// { return static_cast<Widget&&>(param); }


// In C++14, the existence of std::remove_reference_t makes it possible to implement
// std::forward a bit more concisely:

// template<typename T> // C++14; still in
// T&& forward(remove_reference_t<T>& param) // namespace std
// {
//     return static_cast<T&&>(param);
// }


template<typename T>
class Widget1 {
public:
    typedef T&& RvalueRefToT;
};

int main() {
    Widget w; // a variable (a lvalue)
    func(w); // call func with lvalue; T deduced
                // to be Widget&
    func(widgetFactory()); // call func with rvalue; T deduced
                                // to be Widget

    std :: cout << "Hey\n";

    Widget1<int&> ww;

    return 0;
}

// Things to Remember
// • Reference collapsing occurs in four contexts: template instantiation, auto type
// generation, creation and use of typedefs and alias declarations, and
// decltype.
// • When compilers generate a reference to a reference in a reference collapsing
// context, the result becomes a single reference. If either of the original references
// is a lvalue reference, the result is a lvalue reference. Otherwise, it’s a
// rvalue reference.
// • Universal references are rvalue references in contexts where type deduction
// distinguishes lvalues from rvalues and where reference collapsing occurs.