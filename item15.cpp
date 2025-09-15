// Item 15 : Use constexpr whenever possible.

#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

// • constexpr functions can be used in contexts that demand compile-time constants.
// If the values of the arguments you pass to a constexpr function in such a
// context are known during compilation, the result will be computed during
// compilation. If any of the arguments’ values is not known during compilation,
// your code will be rejected.
// • When a constexpr function is called with one or more values that are not
// known during compilation, it acts like a normal function, computing its result at
// runtime. This means you don’t need two functions to perform the same operation,
// one for compile-time constants and one for all other values. The constexpr
// function does it all.

// In C++11, constexpr functions may contain no more than a single executable statement: a return.
constexpr int pow11(int base, int exp) noexcept // that never throws
{
    return (exp == 0 ? 1 : base * pow11(base, exp - 1));
}

constexpr int pow14(int base, int exp) noexcept // C++14
{
    auto result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

// constexpr functions are limited to taking and returning literal types, which essentially means
// types that can have values determined during compilation.In C++ 11, all built - in types except
// void qualify, but user - defined types may be literal, too, because constructors and other
// member functions may be constexpr :
class Point11
{
public:
    constexpr Point11(double xVal = 0, double yVal = 0) noexcept
        : x(xVal), y(yVal)
    {
    }
    constexpr double xValue() const noexcept { return x; }
    constexpr double yValue() const noexcept { return y; }
    void setX(double newX) noexcept { x = newX; }
    void setY(double newY) noexcept { y = newY; }

private:
    double x, y;
};

// In C++ 11, two restrictions prevent Point’s member functions setX and setY from being declared constexpr.First,
// they modify the object they operate on, and in C++ 11, constexpr member functions are implicitly const.Second,
// they have void return types, and void isn’t a literal type in C++ 11. Both these restrictions are lifted in C++ 14,
// so in C++ 14, even Point’s setters can be constexpr :

class Point14
{
public:
    constexpr Point14(double xVal = 0, double yVal = 0) noexcept
        : x(xVal), y(yVal)
    {
    }
    constexpr double xValue() const noexcept { return x; }
    constexpr double yValue() const noexcept { return y; }
    constexpr void setX(double newX) noexcept { x = newX; } // C++14
    constexpr void setY(double newY) noexcept { y = newY; } // C++14

private:
    double x, y;
};

constexpr Point14 midpoint(const Point14 &p1, const Point14 &p2) noexcept
{
    return {(p1.xValue() + p2.xValue()) / 2, (p1.yValue() + p2.yValue()) / 2};
}

constexpr Point14 reflection(const Point14 &p) noexcept
{
    Point14 result;
    result.setX(-p.xValue());
    result.setY(-p.yValue());
    return result;
}

int main()
{
    int sz = 8; // non-constexpr variable

    // constexpr auto arraySize1 = sz; // error! sz's value not
    //                                 // known at compilation

    // std::array<int, sz> data1;      // error! same problem
    constexpr auto arraySize2 = 10; // fine, 10 is a
                                    // compile-time constant
    const int sz1 = sz;             // error for data2
    const int sz2 = 5;              // fine for data2

    std::array<int, arraySize2> data2; // fine, arraySize2
                                       // is constexpr

    // Note that const doesn’t offer the same guarantee as constexpr, because const
    //                                                                    objects need not be initialized with values known during compilation : int sz; // as before
    // const auto arraySize = sz; // const copy of sz                                                                                                                // fine, arraySize is
    //
    // std::array<int, arraySize> data; // error! arraySize's value
    //                                  // not known at compilation

    constexpr auto numConds = 5;          // # of conditions
    std::array<int, pow11(3, 3)> results; // results has

    std ::cout << pow14(2, 10) << '\n';
    constexpr Point14 p1(9.4, 27.7); // fine, "runs" constexpr
                                     // ctor during compilation
    constexpr Point14 p2(28.8, 5.3); // also fine

    constexpr auto mid = midpoint(p1, p2);
    constexpr auto reflectedMid = reflection(mid);

    std ::cout << mid.xValue() << ' ' << mid.yValue() << '\n';

    return 0;
}

// Things to Remember
// • constexpr objects are const and are initialized with values known during
//             compilation.
// • constexpr functions can produce compile -
//         time results when called with
//             arguments whose values are known during compilation.
// • constexpr objects and functions may be used in a wider range of contexts
//                 than non -
//         constexpr objects and functions.
// • constexpr is part of an object’s or
//     function’s interface.