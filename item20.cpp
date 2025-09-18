// Item 20: Use std::weak_ptr for std::shared_ptrlike pointers that can dangle.

#include <iostream>
#include <memory>
#include <unordered_map>
#include <cstdint>

using WidgetID = uint32_t;

// One form is std::
// weak_ptr::lock, which returns a std::shared_ptr. The std::shared_ptr is null
// if the std::weak_ptr has expired.

class Widget
{
};

std::shared_ptr<const Widget> fastLoadWidget(WidgetID id)
{
    static std::unordered_map<WidgetID,
                              std::weak_ptr<const Widget>>
        cache;
    auto objPtr = cache[id].lock(); // objPtr is std::shared_ptr
    // to cached object (or null
    // if object's not in cache)
    if (!objPtr)
    { // if not in cache,
      // objPtr = loadWidget(id); // load it
      // cache[id] = objPtr;      // cache it
    }
    return objPtr;
}

struct B;

struct A
{
    std ::shared_ptr<B> ptr;

    ~A()
    {
        std ::cout << "A is closed\n";
    }
};

struct B
{
    std ::weak_ptr<A> ptr;

    ~B()
    {
        std ::cout << "B is closed\n";
    }
};

// From an efficiency perspective, the std::weak_ptr story is essentially the same as
// that for std::shared_ptr. std::weak_ptr objects are the same size as std::
// shared_ptr objects, they make use of the same control blocks as std::
// shared_ptrs (see Item 19), and operations such as construction, destruction, and
// assignment involve atomic reference count manipulations. That probably surprises
// you, because I wrote at the beginning of this Item that std::weak_ptrs don’t participate
// in reference counting. Except that’s not quite what I wrote. What I wrote was
// that std::weak_ptrs don’t participate in the shared ownership of objects and hence
// don’t affect the pointed-to object’s reference count. There’s actually a second reference
// count in the control block, and it’s this second reference count that std::weak_ptrs
// manipulate. For details, continue on to Item 21.

int main()
{
    auto spw =                      // after spw is constructed,
        std::make_shared<Widget>(); // the pointed-to Widget's

    std::weak_ptr<Widget> wpw(spw); // wpw points to same Widget
                                    // as spw. RC remains 1

    std::shared_ptr<Widget> spw1(spw);

    std ::cout << spw.use_count() << '\n';
    std ::cout << spw1.use_count() << '\n';

    spw = nullptr;

    std ::cout << spw.use_count() << '\n';
    std ::cout << spw1.use_count() << '\n';

    if (wpw.expired())
    {
        std ::cout << "expired weak ptr\n";
    }
    // if wpw doesn't point
    // to an object…

    {
        std::shared_ptr<Widget> spw2 = wpw.lock();

        auto spw3 = wpw.lock();

        std ::cout << spw2.use_count() << ' ' << spw3.use_count() << '\n';
    }

    spw1.reset();

    try
    {
        std::shared_ptr<Widget> spw4(wpw); // if wpw's expired,
                                           // throw std::bad_weak_ptr
    }
    catch (std::bad_weak_ptr &err)
    {
        std ::cout << err.what() << '\n';
    }

    auto aptr = std ::make_shared<A>();
    auto bptr = std ::make_shared<B>();

    std ::cout << aptr.use_count() << ' ' << bptr.use_count() << '\n';

    aptr->ptr = bptr;
    bptr->ptr = aptr;

    std ::cout << aptr.use_count() << ' ' << bptr.use_count() << '\n';

    return 0;
}

// Things to Remember
// • Use std::weak_ptr for std::shared_ptr-like pointers that can dangle.
// • Potential use cases for std::weak_ptr include caching, observer lists, and the
// prevention of std::shared_ptr cycles.