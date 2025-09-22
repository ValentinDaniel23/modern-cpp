// Item 34: Prefer lambdas to std::bind.

#include <iostream>
#include <chrono>
#include <functional>

// typedef for a point in time (see Item 9 for syntax)
using Time = std::chrono::steady_clock::time_point;
// see Item 10 for "enum class"
enum class Sound { Beep, Siren, Whistle };
// typedef for a length of time
using Duration = std::chrono::steady_clock::duration;
// at time t, make sound s for duration d
void setAlarm(Time t, Sound s, Duration d) {
    std :: cout << "miau\n";
}

int main() {
    // The most important reason to prefer lambdas over std::bind is that lambdas are
    // more readable. Suppose, for example, we have a function to set up an audible alarm:

    // setSoundL ("L" for "lambda") is a function object allowing a
    // sound to be specified for a 30-sec alarm to go off an hour
    // after it's set
    auto setSoundL =
    [](Sound s)
    {
        // make std::chrono components available w/o qualification
        using namespace std::chrono;
        setAlarm(steady_clock::now() + hours(1), // alarm to go off
        s, // in an hour for
        seconds(30)); // 30 seconds
    };

    // We can streamline this code in C++14 by availing ourselves of the standard suffixes
    // for seconds (s), milliseconds (ms), hours (h), etc., that build on C++11’s support for
    // user-defined literals. These suffixes are implemented in the std::literals namespace,
    // so the above code can be rewritten as follows:

    auto setSoundL1 =
    [](Sound s)
    {
        using namespace std::chrono;
        using namespace std::literals; // for C++14 suffixes
        setAlarm(steady_clock::now() + 1h, // C++14, but
        s, // same meaning
        30s); // as above
    };

    {
        using namespace std::chrono;
        using namespace std::literals; // for C++14 suffixes
        using namespace std::placeholders;

        auto setSoundB = // "B" for "bind"
        std::bind(setAlarm,
        steady_clock::now() + 1h, // incorrect! see below
        _1,
        30s);

        // I’d like to highlight the call to setAlarm here as I did in the lambda, but there’s no
        // call to highlight. Readers of this code simply have to know that calling setSoundB
        // invokes setAlarm with the time and duration specified in the call to std::bind. To
        // the uninitiated, the placeholder “_1” is essentially magic, but even readers in the
        // know have to mentally map from the number in that placeholder to its position in the
        // std::bind parameter list in order to understand that the first argument in a call to
        // setSoundB is passed as the second argument to setAlarm. The type of this argument
        // is not identified in the call to std::bind, so readers have to consult the setAlarm
        // declaration to determine what kind of argument to pass to setSoundB.

        auto setSoundB1 =
            std::bind(setAlarm,
            std::bind(std::plus<>(), steady_clock::now(), 1h),
            _1,
        30s);
    }

    return 0;
}

// Things to Remember
// • Lambdas are more readable, more expressive, and may be more efficient than
// using std::bind.
// • In C++11 only, std::bind may be useful for implementing move capture or
// for binding objects with templatized function call operators.