/// Item 18: Use std::unique_ptr for exclusive-ownership resource management.

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <locale>
#include <memory>
#include <stdexcept>

using namespace std;

class Investment
{
};
class Stock : public Investment
{
};
class Bond : public Investment
{
};
class RealEstate : public Investment
{
};

// template <typename... Ts>
// auto makeInvestment(Ts &&...params) // C++14
// {
//     auto delInvmt = [](Investment *pInvestment) // this is now
//     {                                           // inside
//         makeLogEntry(pInvestment);              // makedelete
//         pInvestment;                            // Investment
//     };
//     std::unique_ptr<Investment, decltype(delInvmt)> // as
//         pInv(nullptr, delInvmt);                    // before
//     if ( … )                                        // as before
//     {
//         pInv.reset(new Stock(std::forward<Ts>(params)...));
//     }
//     else if ( … ) // as before
//     {
//         pInv.reset(new Bond(std::forward<Ts>(params)...));
//     }
//     else if ( … ) // as before
//     {
//         pInv.reset(new RealEstate(std::forward<Ts>(params)...));
//     }
//     return pInv; // as before
// }

// helper class for runtime polymorphism demo below
struct B
{
    virtual ~B() = default;

    virtual void bar() { std::cout << "B::bar\n"; }
};

struct D : B
{
    D() { std::cout << "D::D\n"; }
    ~D() { std::cout << "D::~D\n"; }

    void bar() override { std::cout << "D::bar\n"; }
};

// a function consuming a unique_ptr can take it by value or by rvalue reference
std::unique_ptr<D> pass_through(std::unique_ptr<D> p)
{
    p->bar();
    return p;
}

// helper function for the custom deleter demo below
void close_file(std::FILE *fp)
{
    std::fclose(fp);
}

// unique_ptr-based linked list demo
struct List
{
    struct Node
    {
        int data;
        std::unique_ptr<Node> next;
    };

    std::unique_ptr<Node> head;

    ~List()
    {
        // destroy list nodes sequentially in a loop, the default destructor
        // would have invoked its “next”'s destructor recursively, which would
        // cause stack overflow for sufficiently large lists.
        while (head)
        {
            auto next = std::move(head->next);
            head = std::move(next);
        }
    }

    void push(int data)
    {
        head = std::unique_ptr<Node>(new Node{data, std::move(head)});
    }
};

int main()
{
    {
        // auto pInvestment = makeInvestment(argumets);
    } // destroy *pInvestment

    {
        // auto pInvestment = makeInvestment();
        // auto delInvmt = [](Investment *pInvestment) // custom
        // {                                           // deleter
        //     makeLogEntry(pInvestment);              // (a lambda
        //     delete pInvestment;                     // expression)
        // };
    }

    std ::unique_ptr<Investment> a(new Investment());

    std ::shared_ptr<Investment> b = std::move(a);

    unique_ptr<Investment> p(new Investment());
    p.reset();            // releases the currently owned object
    p.reset(new Stock()); // releases current object and takes ownership of a new one

    std::cout << "1) Unique ownership semantics demo\n";
    {
        // Create a (uniquely owned) resource
        std::unique_ptr<D> p = std::make_unique<D>();

        // Transfer ownership to “pass_through”,
        // which in turn transfers ownership back through the return value
        std::unique_ptr<D> q = pass_through(std::move(p));

        // “p” is now in a moved-from 'empty' state, equal to nullptr
        assert(!p);
    }

    std::cout << "\n"
                 "2) Runtime polymorphism demo\n";
    {
        // Create a derived resource and point to it via base type
        std::unique_ptr<B> p = std::make_unique<D>();

        // Dynamic dispatch works as expected
        p->bar();
    }

    std::cout << "\n"
                 "3) Custom deleter demo\n";
    std::ofstream("demo.txt") << 'x'; // prepare the file to read
    {
        using unique_file_t = std::unique_ptr<std::FILE, decltype(&close_file)>;
        unique_file_t fp(std::fopen("demo.txt", "r"), &close_file);
        if (fp)
            std::cout << char(std::fgetc(fp.get())) << '\n';
    } // “close_file()” called here (if “fp” is not null)

    std::cout << "\n"
                 "4) Custom lambda expression deleter and exception safety demo\n";
    try
    {
        std::unique_ptr<D, void (*)(D *)> p(new D, [](D *ptr)
                                            {
            std::cout << "destroying from a custom deleter...\n";
            delete ptr; });

        throw std::runtime_error(""); // “p” would leak here if it were a plain pointer
    }
    catch (const std::exception &)
    {
        std::cout << "Caught exception\n";
    }

    std::cout << "\n"
                 "5) Array form of unique_ptr demo\n";
    {
        std::unique_ptr<D[]> p(new D[3]);
    } // “D::~D()” is called 3 times

    std::cout << "\n"
                 "6) Linked list demo\n";
    {
        List wall;
        const int enough{1'000'000};
        for (int beer = 0; beer != enough; ++beer)
            wall.push(beer);

        std::cout.imbue(std::locale("en_US.UTF-8"));
        std::cout << enough << " bottles of beer on the wall...\n";
    } // destroys all the beers

    return 0;
}

// Things to Remember
// • std::unique_ptr is a small, fast, move-only smart pointer for managing
// resources with exclusive-ownership semantics.
// • By default, resource destruction takes place via delete, but custom deleters
// can be specified. Stateful deleters and function pointers as deleters increase the
// size of std::unique_ptr objects.
// • Converting a std::unique_ptr to a std::shared_ptr is easy.