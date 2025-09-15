// Item 21: Prefer std::make_unique and std::make_shared to direct use of new.

#include <iostream>
#include <memory>

// Let’s begin by leveling the playing field for std::make_unique and std::
// make_shared. std::make_shared is part of C++11, but, sadly, std::make_
// unique isn’t. It joined the Standard Library as of C++14. If you’re using C++11,
// never fear, because a basic version of std::make_unique is easy to write yourself.
// Here, look:

// template <typename T, typename... Ts>
// std::unique_ptr<T> make_unique(Ts &&...params)
// {
//     return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
// }

class Widget
{
};

// Make_unique and Make_shared are exception safe

// A special feature of std::make_shared (compared to direct use of new) is improved
// efficiency. Using std::make_shared allows compilers to generate smaller, faster
// code that employs leaner data structures.

class ReallyBigType
{
};

int main()
{
    auto upw1(std::make_unique<Widget>());    // with make func
    std::unique_ptr<Widget> upw2(new Widget); // without make func
    auto spw1(std::make_shared<Widget>());    // with make func
    std::shared_ptr<Widget> spw2(new Widget); // without make func

    {
        auto pBigObj =                         // create very large
            std::make_shared<ReallyBigType>(); // object via
                                               // std::make_shared
        // create std::shared_ptrs and std::weak_ptrs to
        // large object, use them to work with it

        // final std::shared_ptr to object destroyed here,
        // but std::weak_ptrs to it remain

        // during this period, memory formerly occupied
        // by large object remains allocated

        // final std::weak_ptr to object destroyed here;
        // memory for control block and object is released
    }

    {
        std::shared_ptr<ReallyBigType> pBigObj(new ReallyBigType);
        // create very large
        // object via new

        // as before, create std::shared_ptrs and
        // std::weak_ptrs to object, use them with it

        // final std::shared_ptr to object destroyed here,
        // but std::weak_ptrs to it remain;
        // memory for object is deallocated

        // during this period, only memory for the
        // control block remains allocated

        // final std::weak_ptr to object destroyed here;
        // memory for control block is released
    }

    return 0;
}

// Things to Remember
// • Compared to direct use of new, make functions eliminate source code duplication,
// improve exception safety, and, for std::make_shared and std::allo
// cate_shared, generate code that’s smaller and faster.
// • Situations where use of make functions is inappropriate include the need to
// specify custom deleters and a desire to pass braced initializers.
// • For std::shared_ptrs, additional situations where make functions may be
// ill-advised include (1) classes with custom memory management and (2) systems
// with memory concerns, very large objects, and std::weak_ptrs that
// outlive the corresponding std::shared_ptrs.