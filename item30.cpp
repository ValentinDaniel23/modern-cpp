// Item 30: Familiarize yourself with perfect forwarding failure cases.

#include <iostream>
#include <vector>
#include <cstdint>

void f(const std::vector<int>& v) {
    std :: cout << "Alright\n";
}

void f(std::size_t sz) {
    std :: cout << "Alright\n";
}

template<typename T>
void fwd(T&& param) // accept any argument
{
    f(std::forward<T>(param)); // forward it to f
}

template<typename... Ts>
void fwd(Ts&&... params) // accept any arguments
{
    f(std::forward<Ts>(params)...); // forward them to f
}

// f( expression ); // if this does one thing,
// fwd( expression ); // but this does something else, fwd fails
// // to perfectly forward expression to f

class Widget {
public:
    static const std::size_t MinVals = 28;  // if that alone, the compiler can use const propagation
                                            // and not use memory for MinVals
};

const std::size_t Widget::MinVals;

struct IPv4Header {
    std::uint32_t version:4,
    IHL:4,
    DSCP:6,
    ECN:2,
    totalLength:16;
};

int main() {

    // Suppose f is declared like this:
    // void f(const std::vector<int>& v);
    // In that case, calling f with a braced initializer compiles,
    // f({ 1, 2, 3 }); // fine, "{1, 2, 3}" implicitly
    // // converted to std::vector<int>
    // but passing the same braced initializer to fwd doesn’t compile:
    // fwd({ 1, 2, 3 }); // error! doesn't compile
    // That’s because the use of a braced initializer is a perfect forwarding failure case.

    f({1,2,3});

    // fwd({1, 2, 3}); // doesnt compile

    auto il = { 1, 2, 3 }; // il's type deduced to be
    // std::initializer_list<int>
    fwd(il); // fine, perfect-forwards il to f

    // 0 or NULL as null pointers
    // Item 8 explains that when you try to pass 0 or NULL as a null pointer to a template,
    // type deduction goes awry, deducing an integral type (typically int) instead of a
    // pointer type for the argument you pass. The result is that neither 0 nor NULL can be
    // perfect-forwarded as a null pointer. The fix is easy, however: pass nullptr instead of
    // 0 or NULL. For details, consult Item 8.

    IPv4Header h;
    // copy bitfield value; see Item 6 for info on init. form
    auto length = static_cast<std::uint16_t>(h.totalLength);
    fwd(length); // forward the copy

    return 0;
}

// Things to Remember
// • Perfect forwarding fails when template type deduction fails or when it deduces
// the wrong type.
// • The kinds of arguments that lead to perfect forwarding failure are braced initializers,
// null pointers expressed as 0 or NULL, declaration-only integral const
// static data members, template and overloaded function names, and bitfields.