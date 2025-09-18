// Item 16 : Make const member functions thread safe.
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <cmath>

using namespace std;

class Polynomial
{
public:
    using RootsType = std::vector<double>;
    RootsType roots() const
    {
        std::lock_guard<std::mutex> g(m); // lock mutex
        if (!rootsAreValid)
        { // if cache not valid
          // compute/store roots
            rootsAreValid = true;
        }
        return rootVals;
    } // unlock mutex
private:
    mutable std::mutex m;
    mutable bool rootsAreValid{false};
    mutable RootsType rootVals{};
};

class Point
{ // 2D point
public:
    double distanceFromOrigin() const noexcept // see Item 14
    {                                          // for noexcept
        ++callCount;                           // atomic increment
        return std::sqrt((x * x) + (y * y));
    }

private:
    mutable std::atomic<unsigned> callCount{0};
    double x, y;
};

// Like std::mutexes, std::atomics are move-only types, so the existence of call
// Count in Point means that Point is also move-only.

int expensiveComputation1()
{
    int x = 0;
    for (int i = 0; i < 1e6; i++)
        x++;

    return x;
}

int expensiveComputation2()
{
    int x = 0;
    for (int i = 0; i < 1e6; i++)
        x++;

    return x;
}

class Widget
{
public:
    int magicValue() const
    {
        std::lock_guard<std::mutex> guard(m); // lock m
        if (cacheValid)
            return cachedValue;
        else
        {
            auto val1 = expensiveComputation1();
            auto val2 = expensiveComputation2();
            cachedValue = val1 + val2;
            cacheValid = true;
            return cachedValue;
        }
    } // unlock m
private:
    mutable std::mutex m;
    mutable int cachedValue;        // no longer atomic
    mutable bool cacheValid{false}; // no longer atomic
};

int main()
{
    std::cout << __cplusplus << endl;

    return 0;
}
