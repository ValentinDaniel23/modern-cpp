// Item 13 : Prefer const_iterators to iterators.

#include <iostream>
#include <vector>
#include <algorithm>

// This works fine in C++14, but, sadly, not in C++11. Through an oversight during
// standardization, C++11 added the non-member functions begin and end, but it
// failed to add cbegin, cend, rbegin, rend, crbegin, and crend. C++14 rectifies that
// oversight.
template <typename C, typename V>
void findAndInsert(C &container,       // in container, find
                   const V &targetVal, // first occurrence
                   const V &insertVal) // of targetVal, then
{                                      // insert insertVal
    using std::cbegin;                 // there
    using std::cend;
    auto it = std::find(cbegin(container), // non-member cbegin
                        cend(container),   // non-member cend
                        targetVal);
    container.insert(it, insertVal);
}

// If you’re using C++11, you want to write maximally generic code, and none of the
// libraries you’re using provides the missing templates for non-member cbegin and
// friends, you can throw your own implementations together with ease. For example,
// here’s an implementation of non-member cbegin:
template <class C>
auto cbegin(const C &container) -> decltype(std::begin(container))
{
    return std::begin(container); // see explanation below
}

int main()
{
    // std ::vector<int> values{1983};
    std ::vector<int> values;

    // CPP 98
    std ::vector<int>::iterator it =
        std ::find(values.begin(), values.end(), 1983);

    values.insert(it, 1998);

    for (const int &x : values)
        std ::cout << x << ' ' << &x << '\n';

    // Const iterators in CPP 98 weren't practical

    std ::vector<int> values1{1983};

    // const iterators
    auto it1 = std ::find(values1.cbegin(), values1.cend(), 1983);

    values1.insert(it1, 1998);

    return 0;
}

// Things to Remember
// • Prefer const_iterators to iterators.
// • In maximally generic code, prefer non-member versions of begin, end,
// rbegin, etc., over their member function counterparts.