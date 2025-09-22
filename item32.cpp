// Item 32: Use init capture to move objects into closures.

#include <iostream>
#include <memory>
#include <vector>
#include <functional>

namespace item32 {
    // Here’s how you can use init capture to move a std::unique_ptr into a closure:
    class Widget { // some useful type
        public:
            bool isValidated() const;
            bool isProcessed() const;
            bool isArchived() const;
        private:
            };

    // Remember that a lambda expression is simply a way to cause a class to be generated
    // and an object of that type to be created. There is nothing you can do with a lambda
    // that you can’t do by hand. The example C++14 code we just saw, for example, can be
    // written in C++11 like this:

    class IsValAndArch {        // is validated
    public:                     // and archived
        using DataType = std::unique_ptr<Widget>;
        explicit IsValAndArch(DataType&& ptr) : pw(std::move(ptr)) {}
        bool operator()() const {
            return pw->isValidated() && pw->isArchived();
        }
    private:
        DataType pw;
    };

    auto func = IsValAndArch(std::make_unique<Widget>());

    // That’s more work than writing the lambda, but it doesn’t change the fact that if you
    // want a class in C++11 that supports move-initialization of its data members, the only
    // thing between you and your desire is a bit of time with your keyboard.
}

int main() {

    auto pw = std::make_unique<item32::Widget>(); // create Widget; see
    // Item 21 for info on
    // std::make_unique
    // configure *pw

    // CPP14
    auto func = [pw = std::move(pw)]{
        return pw->isValidated() && pw->isArchived();
    };

    auto func1 = [pw = std::make_unique<item32::Widget>()]{
        return pw->isValidated() && pw->isArchived();
    };

    // Suppose you’d like to create a local std::vector, put an appropriate set of values
    // into it, then move it into a closure. In C++14, this is easy:
    std :: vector<double> data{1, 2, 3};
    auto func3 = [data =  std :: move(data)] {
        if (data.empty()) {
            std :: cout << "empty\n";
        } else {
            std :: cout << "not empty\n";
        }
    };

    func3();
    func3();

    if (data.empty())
        std :: cout << "empty\n";

    // CPP11 equivalent
    std :: vector<double> data1{1, 2, 3};

    auto func4 =
    std::bind( // C++11 emulation
    [](const std::vector<double>& data)
    { std :: cout << "yo\n"; },
        std::move(data)
    );

    // A bind object contains copies of all the arguments passed to std::bind. For each
    // lvalue argument, the corresponding object in the bind object is copy constructed. For
    // each rvalue, it’s move constructed. In this example, the second argument is an rvalue
    // (the result of std::move—see Item 23), so data is move constructed into the bind
    // object. This move construction is the crux of move capture emulation, because moving
    // an rvalue into a bind object is how we work around the inability to move an
    // rvalue into a C++11 closure.

    // By default, the operator() member function inside the closure class generated from
    // a lambda is const. That has the effect of rendering all data members in the closure
    // const within the body of the lambda. The move-constructed copy of data inside the
    // bind object is not const, however, so to prevent that copy of data from being modified
    // inside the lambda, the lambda’s parameter is declared reference-to-const. If the
    // lambda were declared mutable, operator() in its closure class would not be
    // declared const, and it would be appropriate to omit const in the lambda’s parameter
    // declaration

    auto func5 =
    std::bind( // C++11 emulation
    [](const std::vector<double>& data) mutable
    { std :: cout << "yo\n"; },
        std::move(data)
    );

    return 0;
}

// Things to Remember
// • Use C++14’s init capture to move objects into closures.
// • In C++11, emulate init capture via hand-written classes or std::bind.