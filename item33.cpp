// Item 33: Use decltype on auto&& parameters to std::forward them.

#include <iostream>

void func(double x) {
    std :: cout << x << '\n';
}

double normalize(int x) {
    return static_cast<double> (x) / 2;
}

int main() {
    // One of the most exciting features of C++14 is generic lambdas—lambdas that use
    // auto in their parameter specifications. The implementation of this feature is straightforward:
    // operator() in the lambda’s closure class is a template. Given this lambda,
    // for example,

    auto f = [](auto x){ return func(normalize(x)); };

    // the closure class’s function call operator looks like this:
    // class SomeCompilerGeneratedClassName {
    // public:
    //     template<typename T> // see Item 3 for
    //     auto operator()(T x) const // auto return type
    //     { return func(normalize(x)); }
    //          // other closure class
    //     }; // functionality

    f(21);

    // auto f = [](auto&& x)
    // { return func(normalize(std::forward<???>(x))); };

    // Item 28 explains that if an lvalue argument is passed to a universal reference parameter,
    // the type of that parameter becomes an lvalue reference. If an rvalue is passed, the
    // parameter becomes an rvalue reference. This means that in our lambda, we can
    // determine whether the argument passed was an lvalue or an rvalue by inspecting the
    // type of the parameter x. decltype gives us a way to do that (see Item 3). If an lvalue
    // was passed in, decltype(x) will produce a type that’s an lvalue reference. If an
    // rvalue was passed, decltype(x) will produce an rvalue reference type.

    auto f1 =
    [](auto&& param)
    {
        return
        func(normalize(std::forward<decltype(param)>(param)));
    };

    // From there, it’s just a hop, skip, and six dots to a perfect-forwarding lambda that
    // accepts not just a single parameter, but any number of parameters, because C++14
    // lambdas can also be variadic:

    auto f2 =
    [](auto&&... params)
    {
        return
        func(normalize(std::forward<decltype(params)>(params)...));
    };

    return 0;
}

// Things to Remember
// • Use decltype on auto&& parameters to std::forward them.