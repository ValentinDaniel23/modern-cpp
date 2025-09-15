// Item 19: Use std::shared_ptr for shared-ownership resource management.

#include <iostream>
#include <memory>
#include <vector>

// • std::shared_ptrs are twice the size of a raw pointer, because they internally
// contain a raw pointer to the resource as well as a raw pointer to the resource’s
// reference count.

// • Memory for the reference count must be dynamically allocated.

// • Increments and decrements of the reference count must be atomic.

// auto loggingDel = [](Widget *pw) // custom deleter
// {                                // (as in Item 18)
//     makeLogEntry(pw);
//     delete pw;
// };
// std::unique_ptr<                 // deleter type is
//     Widget, decltype(loggingDel) // part of ptr type
//     >
//     upw(new Widget, loggingDel);
// std::shared_ptr<Widget>          // deleter type is not
//     spw(new Widget, loggingDel); // part of ptr type

// The control block contains, in addition to the
// reference count, a copy of the custom deleter, if one has been specified. If a custom
// allocator was specified, the control block contains a copy of that, too. The control
// block may also contain additional data, including, as Item 21 explains, a secondary
// reference count known as the weak count.

// • std::make_shared (see Item 21) always creates a control block. It manufactures
// a new object to point to, so there is certainly no control block for that
// object at the time std::make_shared is called.

// • A control block is created when a std::shared_ptr is constructed from a
// unique-ownership pointer (i.e., a std::unique_ptr or std::auto_ptr).
// Unique-ownership pointers don’t use control blocks, so there should be no control
// block for the pointed-to object. (As part of its construction, the
// std::shared_ptr assumes ownership of the pointed-to object, so the uniqueownership
// pointer is set to null.)

// • When a std::shared_ptr constructor is called with a raw pointer, it creates a
// control block. If you wanted to create a std::shared_ptr from an object that
// already had a control block, you’d presumably pass a std::shared_ptr or a
// std::weak_ptr (see Item 20) as a constructor argument, not a raw pointer.
// std::shared_ptr constructors taking std::shared_ptrs or std::weak_ptrs
// as constructor arguments don’t create new control blocks, because they can rely
// on the smart pointers passed to them to point to any necessary control blocks.

class A
{
    int x, y;

public:
    A(int x, int y) : x{x}, y{y}
    {
        std ::cout << "constructor\n";
    }

    virtual ~A()
    {
        std ::cout << "destructor\n";
    }
};

class Widget;

std::vector<std::shared_ptr<Widget>> processedWidgets;

class Widget : public std::enable_shared_from_this<Widget>
{
public:
    void process();
};

void Widget::process()
{
    // add std::shared_ptr to current object to processedWidgets
    processedWidgets.emplace_back(shared_from_this());
}

// To prevent clients from calling member functions that invoke shared_from_this
// before a std::shared_ptr points to the object, classes inheriting from
// std::enable_shared_from_this often declare their constructors private and
// have clients create objects by calling factory functions that return std::
// shared_ptrs. Widget, for example, could look like this:

// class Widget : public std::enable_shared_from_this<Widget>
// {
// public:
//     // factory function that perfect-forwards args
//     // to a private ctor
//     template <typename... Ts>
//     static std::shared_ptr<Widget> create(Ts &&...params);
//     … void process(); // as before
// … private :
// …                    // ctors
// };

// Having read about dynamically allocated control blocks, arbitrarily large deleters and
// allocators, virtual function machinery, and atomic reference count manipulations,
// your enthusiasm for std::shared_ptrs may have waned somewhat. That’s fine.

// Under typical conditions, where the default deleter and default allocator are used and where
// the std::shared_ptr is created by std::make_shared, the control block is only
// about three words in size, and its allocation is essentially free. (It’s incorporated into
// the memory allocation for the object being pointed to.

int main()
{
    std ::unique_ptr<A> o1(new A{5, 6});
    std ::unique_ptr<A> o2(new A{2, 4});

    std ::vector<std ::unique_ptr<A>> v;

    v.emplace_back(std ::move(o1));
    v.emplace_back(std ::move(o2));

    if (o1 == nullptr)
    {
        std ::cout << "take the L\n";
    }

    std ::shared_ptr<A> o3(new A{5, 6});
    std ::shared_ptr<A> o4(new A{2, 4});

    std ::vector<std ::shared_ptr<A>> v1{o3, o4};

    std ::shared_ptr<A> o5{std::move(o1)};

    std::cout << o3.use_count() << '\n';
    std::cout << o4.use_count() << '\n';
    std::cout << o5.use_count() << '\n';

    // auto pw = new Widget;                         // pw is raw ptr
    // std::shared_ptr<Widget> spw1(pw, loggingDel); // create control
    //                                               // block for *pw
    // std::shared_ptr<Widget> spw2(pw, loggingDel); // create 2nd
    //                                               // control block
    //                                               // for *pw!

    std ::shared_ptr<int> pp(new int(100));

    std ::cout << *pp << '\n';

    std ::shared_ptr<std ::vector<int>> vv(new std::vector<int>);

    vv->push_back(10);
    std ::cout << vv->at(0) << '\n';

    return 0;
}

// Things to Remember
// • std::shared_ptrs offer convenience approaching that of garbage collection
// for the shared lifetime management of arbitrary resources.
// • Compared to std::unique_ptr, std::shared_ptr objects are typically
// twice as big, incur overhead for control blocks, and require atomic reference
// count manipulations.
// • Default resource destruction is via delete, but custom deleters are supported.
// The type of the deleter has no effect on the type of the std::shared_ptr.
// • Avoid creating std::shared_ptrs from variables of raw pointer type.
