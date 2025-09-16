// Item 23: Understand std::move and std::forward.

#include <iostream>
#include <string>

// std::move and std::forward are merely functions (actually function templates)
// that perform casts. std::move unconditionally casts its argument to an rvalue, while
// std::forward performs this cast only if a particular condition is fulfilled. That’s it.
// The explanation leads to a new set of questions, but, fundamentally, that’s the complete
// story.

namespace item23 {
    // To make the story more concrete, here’s a sample implementation of std::move in
    // C++11. It’s not fully conforming to the details of the Standard, but it’s very close.
    template<typename T> // in namespace std
    typename std :: remove_reference<T>::type&& move(T&& param)
    {
        using ReturnType = typename std :: remove_reference<T>::type&&;

        return static_cast<ReturnType>(param);
    }

    // C++14
    template<typename T> // C++14; still in
    decltype(auto) move(T&& param) // namespace std
    {
        using ReturnType = std :: remove_reference_t<T>&&;
        return static_cast<ReturnType>(param);
    }


    class D
    {
    public:
        explicit D() {};

        D(const D& d) {
            std :: cout << "D copy constr\n";
        }

        D(D &&d) {
            std :: cout << "D move constr\n";
        }
    };

    class Widget {
    public:
        Widget(Widget&& rhs) : s(std::move(rhs.s)) {
            ++moveCtorCalls;
        }

    private:
        static std::size_t moveCtorCalls;
        std::string s;
    };

    // To implement the same behavior with std::forward, the code would look like this:
    class Widget1 {
    public:
        Widget1(Widget1&& rhs) : s(std::forward<std::string>(rhs.s)) {
            ++moveCtorCalls;
        }
    private:
        static std::size_t moveCtorCalls;
        std::string s;
    };
}

// There are two lessons to be drawn from this example. First, don’t declare objects
// const if you want to be able to move from them. Move requests on const objects are
// silently transformed into copy operations. Second, std::move not only doesn’t
// actually move anything, it doesn’t even guarantee that the object it’s casting will be
// eligible to be moved. The only thing you know for sure about the result of applying
// std::move to an object is that it’s an rvalue.

// But param, like all function parameters, is an lvalue. Every call to process inside
// logAndProcess will thus want to invoke the lvalue overload for process. To prevent
// this, we need a mechanism for param to be cast to an rvalue if and only if the argument
// with which param was initialized—the argument passed to logAndProcess—
// was an rvalue. This is precisely what std::forward does. That’s why std::forward
// is a conditional cast: it casts to an rvalue only if its argument was initialized with an
// rvalue.

int main()
{
    item23::D d1;
    const item23::D d2;

    item23::D d3(d1);   // copy
    item23::D d4(d2);   // copy
    item23::D d5{std :: move(d1)};  // move
    item23::D d6(std :: move(d2));  // copy

    int a = 10;

    int &&b = 15;
    b = a;

    return 0;
}

// Things to Remember
// • std::move performs an unconditional cast to an rvalue. In and of itself, it
// doesn’t move anything.
// • std::forward casts its argument to an rvalue only if that argument is bound
// to an rvalue.
// • Neither std::move nor std::forward do anything at runtime.