// Item 26: Avoid overloading on universal references.

#include <iostream>
#include <string>
#include <set>
#include <chrono>

namespace item26 {
    std::multiset<std::string> names;

    void log(const std::chrono::time_point<std::chrono::system_clock> &t, const std :: string &&s) {
        std::time_t tt = std::chrono::system_clock::to_time_t(t);
        std::cout << s << ": " << std::ctime(&tt);
    }

    void logAndAdd(const std::string& name)
    {
        auto now = std::chrono::system_clock::now();
        log(now, "logAndAdd const string");
        names.emplace(name);
    }

    template<typename T>
    void logAndAdd(T&& name)   // optimized
    {
        auto now = std::chrono::system_clock::now();
        log(now, "logAndAdd universal ref");
        names.emplace(std::forward<T>(name));
    }

    // void logAndAdd(int idx) // new overload
    // {
    //     auto now = std::chrono::system_clock::now();
    //     log(now, "logAndAdd");
    //     names.emplace(nameFromIdx(idx));
    // }
}

// This is why combining
// overloading and universal references is almost always a bad idea: the universal reference
// overload vacuums up far more argument types than the developer doing the
// overloading generally expects.

int main()
{
    std::string petName("Darla");
    item26::logAndAdd(petName); // pass lvalue std::string
    item26::logAndAdd(std::string("Persephone")); // pass rvalue std::string
    item26::logAndAdd("Patty Dog"); // pass string literal

    item26::logAndAdd(petName); // as before, copy
                                // lvalue into multiset
    item26::logAndAdd(std::string("Persephone"));   // move rvalue instead
                                                    // of copying it
    item26::logAndAdd("Patty Dog"); // create std::string
                                    // in multiset instead
                                    // of copying a temporary
                                    // std::string

    const std::string s("Tralalero, Tralala");
    item26::logAndAdd(s);

    return 0;
}

// Things to Remember
// • Overloading on universal references almost always leads to the universal reference
// overload being called more frequently than expected.
// • Perfect-forwarding constructors are especially problematic, because they’re
// typically better matches than copy constructors for non-const lvalues, and
// they can hijack derived class calls to base class copy and move constructors.