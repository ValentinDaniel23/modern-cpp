// Item 31: Avoid default capture modes.

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using FilterContainer = std::vector<std::function<bool(int)>>;

FilterContainer filters;

int computeSomeValue1() {
    return 10;
}

int computeSomeValue2() {
    return 5;
}

int computeDivisor(int a, int b) {
    return a / b;
}

void addDivisorFilter()
{
    auto calc1 = computeSomeValue1();
    auto calc2 = computeSomeValue2();
    auto divisor = computeDivisor(calc1, calc2);
    filters.emplace_back(                               // danger!
        [&](int value) { return value % divisor == 0; } // ref to
    );                                                  // divisor
}                                                       // will
                                                        // dangle!

void addDivisorFilter1()
{
    static auto calc1 = computeSomeValue1(); // now static
    static auto calc2 = computeSomeValue2(); // now static
    static auto divisor = // now static
    computeDivisor(calc1, calc2);
    filters.emplace_back([=](int value) // captures nothing!
    {
        return value % divisor == 0;
    } // refers to above static
    );
    ++divisor; // modify divisor
}

// This code is a problem waiting to happen. The lambda refers to the local variable
// divisor, but that variable ceases to exist when addDivisorFilter returns. That’s
// immediately after filters.emplace_back returns, so the function that’s added to
// filters is essentially dead on arrival. Using that filter yields undefined behavior
// from virtually the moment it’s created.

// By the way, the ability to use auto in C++14 lambda parameter specifications means
// that the code above can be simplified in C++14. The ContElemT typedef can be eliminated,
// and the if condition can be revised as follows:

// std::all_of(begin(container), end(container),
//     [&](const auto& value) { return value % divisor == 0; })

// One way to solve our problem with divisor would be a default by-value capture
// mode. That is, we could add the lambda to filters as follows:
// filters.emplace_back(
//     [=](int value) { return value % divisor == 0; }
//     );

// This suffices for this example, but, in general, default by-value capture isn’t the antidangling
// elixir you might imagine. The problem is that if you capture a pointer by
// value, you copy the pointer into the closures arising from the lambda, but you don’t
// prevent code outside the lambda from deleteing the pointer and causing your
// copies to dangle.

class Widget {
public:
    void addFilter() const; // add an entry to filters
private:
    int divisor; // used in Widget's filter
};

void Widget::addFilter() const
{
    filters.emplace_back(
    [=](int value) { return value % divisor == 0; }
    );

    // Captures apply only to non-static local variables (including parameters) visible in
    // the scope where the lambda is created. In the body of Widget::addFilter, divisor
    // is not a local variable, it’s a data member of the Widget class. It can’t be captured. Yet
    // if the default capture mode is eliminated, the code won’t compile:
    // void Widget::addFilter() const
    // {
    //     filters.emplace_back( // error!
    //     [](int value) { return value % divisor == 0; } // divisor
    //     ); // not
    // } // available

    // Furthermore, if an attempt is made to explicitly capture divisor (either by value or
    // by reference—it doesn’t matter), the capture won’t compile, because divisor isn’t a
    // local variable or a parameter:
    // void Widget::addFilter() const
    // {
    //     filters.emplace_back(
    //     [divisor](int value) // error! no local
    //     { return value % divisor == 0; } // divisor to capture
    //     );
    // }

    // The explanation hinges on the implicit use of a raw pointer: this. Every non-static
    // member function has a this pointer, and you use that pointer every time you mention
    // a data member of the class. Inside any Widget member function, for example,
    // compilers internally replace uses of divisor with this->divisor. In the version of
    // Widget::addFilter with a default by-value capture, what’s being captured is the Widget’s
    // this pointer, not divisor. Compilers treat thecode as if it had been written as follows:

    // void Widget::addFilter() const
    // {
    //     auto currentObjectPtr = this;
    //     filters.emplace_back(
    //     [currentObjectPtr](int value)
    //     { return value % currentObjectPtr->divisor == 0; }
    //     );
    // }

    // In C++14, a better way to capture a data member is to use generalized lambda capture
    // (see Item 32):
    // void Widget::addFilter() const
    // {
    //     filters.emplace_back( // C++14:
    //     [divisor = divisor](int value) // copy divisor to closure
    //     { return value % divisor == 0; } // use the copy
    //     );
    // }
}

int main() {
    filters.emplace_back(
        [](int value) { return value % 5 == 0; }
    );

    int ret = filters[0](5);

    std :: cout << ret << '\n';

    addDivisorFilter();

    // ret = filters[1](5);
    //
    // std :: cout << ret << '\n';

    return 0;
}

// Things to Remember
// • Default by-reference capture can lead to dangling references.
// • Default by-value capture is susceptible to dangling pointers (especially this),
// and it misleadingly suggests that lambdas are self-contained.