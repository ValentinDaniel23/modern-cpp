// Item 27: Familiarize yourself with alternatives to overloading on universal references.

#include <iostream>
#include <set>
#include <chrono>

namespace item27 {
    std::multiset<std::string> names;

    void log(const std::chrono::time_point<std::chrono::system_clock> &t, const std :: string &&s) {
        const std::time_t tt = std::chrono::system_clock::to_time_t(t);
        std::cout << s << ": " << std::ctime(&tt);
    }

    // With that taken care of, we can shift our attention to the function being called,
    // logAndAddImpl. There are two overloads, and the first is applicable only to nonintegral
    // types (i.e., to types where std::is_integral<typename std::remove_ref
    // erence<T>::type> is false):

    // In this design, the types std::true_type and std::false_type are “tags” whose
    // only purpose is to force overload resolution to go the way we want.

    template<typename T> // non-integral
    void logAndAddImpl(T&& name, std::false_type) // argument:
    { // add it to
            const auto &now = std::chrono::system_clock::now(); // global data
            log(now, "logAndAdd"); // structure
            names.emplace(std::forward<T>(name));
    }

    std::string nameFromIdx(int idx);

    void logAndAddImpl(int idx, std::true_type) // integral
    {                                           // argument: look
                                                // up name and
    }                                           // call logAndAdd
                                                // with it

    template<typename T>
    void logAndAdd(T&& name)
    {
        logAndAddImpl(std::forward<T>(name),
        std::is_integral<T>()); // not quite correct
    }

    template<typename T>
    void logAndAdd1(T&& name)
        {
            logAndAddImpl(
                std::forward<T>(name),
                std::is_integral<typename std::remove_reference<T>::type>()
            );
        }

    // CPP14
    template<typename T>
    void logAndAdd2(T&& name)
        {
            logAndAddImpl(
                std::forward<T>(name),
                std::is_integral<std::remove_reference_t<T>>()
            );
        }


    // std::enable_if gives you a way to force compilers to behave as if a particular template
    // didn’t exist. Such templates are said to be disabled. By default, all templates are
    // enabled, but a template using std::enable_if is enabled only if the condition specified
    // by std::enable_if is satisfied. In our case, we’d like to enable the Person
    // perfect-forwarding constructor only if the type being passed isn’t Person. If the type
    // being passed is Person, we want to disable the perfect-forwarding constructor (i.e.,
    // cause compilers to ignore it), because that will cause the class’s copy or move constructor
    // to handle the call, which is what we want when a Person object is initialized
    // with another Person.

    // SFINAE
    // !std::is_same<Person, typename std::decay<T>::type>::value
    // i.e., Person is not the same type as T, ignoring any references or cv-qualifiers.

    // That trait is std::decay.
    // std::decay<T>::type is the same as T, except that references and cv-qualifiers (i.e.,
    // const or volatile qualifiers) are removed. (I’m fudging the truth here, because
    // std::decay, as its name suggests, also turns array and function types into pointers
    // (see Item 1), but for purposes of this discussion, std::decay behaves as I’ve described.)

    class Person {
    public:
        template<
            typename T,
            typename = std::enable_if_t<
                            !std::is_base_of<Person,
                                std::decay_t<T>
                            >::value
                        >
        >
        explicit Person(T&& n);
    };

    class SpecialPerson: public Person {
    public:
        SpecialPerson(const SpecialPerson& rhs) // copy ctor; calls
        : Person(rhs) // base class
        {} // forwarding ctor!
        SpecialPerson(SpecialPerson&& rhs) // move ctor; calls
        : Person(std::move(rhs)) // base class
        {} // forwarding ctor!
    };

    // We now realize that we don’t want to enable the templatized constructor
    // for any argument type other than Person, we want to enable it for any
    // argument type other than Person or a type derived from Person. Pesky inheritance!

    // It’s called std::is_base_of.
    // std::is_base_of<T1, T2>::value is true if T2 is derived from T1. Types are considered
    // to be derived from themselves, so std::is_base_of<T, T>::value is true.

    // We haven’t yet seen how to apply it to distinguish integral
    // and non-integral arguments. That was, after all, our original goal; the constructor
    // ambiguity problem was just something we got dragged into along the way.
    // All we need to do—and I really do mean that this is everything—is (1) add a Person
    // constructor overload to handle integral arguments and (2) further constrain the
    // templatized constructor so that it’s disabled for such arguments. Pour these ingredients
    // into the pot with everything else we’ve discussed, simmer over a low flame, and
    // savor the aroma of success:

    class Person1 {
    public:
        template<
        typename T,
        typename = std::enable_if_t<
                !std::is_base_of<Person1, std::decay_t<T>>::value
                &&
                !std::is_integral<std::remove_reference_t<T>>::value
                // &&  std::is_constructible<std::string, T>::value     // yeah, insane
            >
        >
        explicit Person1(T&& n) // ctor for std::strings and
        : name(std::forward<T>(n)) // args convertible to
        {   // std::strings
            static_assert(
                std::is_constructible<std::string, T>::value,
                "Parameter n can't be used to construct a std::string"
            );
        }

        explicit Person1(int idx) // ctor for integral args
        : name(nameFromIdx(idx))
        {}

        // copy and move ctors, etc.
        private:
        std::string name;
    };

    // Voilà! A thing of beauty! Well, okay, the beauty is perhaps most pronounced for
    // those with something of a template metaprogramming fetish, but the fact remains
    // that this approach not only gets the job done, it does it with unique aplomb. Because
    // it uses perfect forwarding, it offers maximal efficiency, and because it controls the
    // combination of universal references and overloading rather than forbidding it, this
    // technique can be applied in circumstances (such as constructors) where overloading
    // is unavoidable.

    // The std::is_constructible
    // type trait performs a compile-time test to determine whether an object of one type
    // can be constructed from an object (or set of objects) of a different type (or set of
    // types), so the assertion is easy to write:
}

int main() {
    item27::logAndAdd2("YOU");

    // item27::Person1 p(u"Hello");    // error

    return 0;
}

// Things to Remember
// • Alternatives to the combination of universal references and overloading
// include the use of distinct function names, passing parameters by lvaluereference-
// to-const, passing parameters by value, and using tag dispatch.
// • Constraining templates via std::enable_if permits the use of universal references
// and overloading together, but it controls the conditions under which
// compilers may use the universal reference overloads.
// • Universal reference parameters often have efficiency advantages, but they typically
// have usability disadvantages.