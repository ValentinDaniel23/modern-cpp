// Item 10 : Prefer scoped enums to unscoped enums.

#include <iostream>
#include <tuple>
#include <cstdint>

enum Status : std::uint8_t
{
    good = 0,
    failed = 1,
    incomplete = 65,
    corrupt = 66,
};

// enum Responses; // error
enum class Message;

int function(size_t)
{
    std ::cout << "fine\n";

    return 0;
}

// CPP 11
// template <typename E>
// constexpr typename std::underlying_type<E>::type
// toUType(E enumerator) noexcept
// {
//     return static_cast<typename std::underlying_type<E>::type>(enumerator);
// }

// // CPP 14
// template <typename E>
// constexpr std::underlying_type_t<E>
// toUType(E enumerator) noexcept
// {
//     return static_cast<std::underlying_type_t<E>>(enumerator);
// }

// // CPP 14 even sleeker
template <typename E>
constexpr auto
toUType(E enumerator) noexcept
{
    return static_cast<std::underlying_type_t<E>>(enumerator);
}

int main()
{
    enum Food
    {
        meat,
        eggs,
        cheese
    };

    // error - scoped enum
    // auto meat = false;

    enum class Color
    {
        red,
        blue,
        green
    };

    // fine - unscoped enum by the name class (cpp 11)

    auto blue = false;

    // auto color = red;    error
    auto color1 = Color::red;
    auto food = meat;
    auto food1 = Food::meat;

    // The reduction in namespace pollution offered by scoped enums is reason enough to
    // prefer them over their unscoped siblings, but scoped enums have a second compelling
    // advantage: their enumerators are much more strongly typed. Enumerators for unscoped
    // enums implicitly convert to integral types (and, from there, to floating-point
    // types)

    function(static_cast<size_t>(color1));
    function(food1);

    std ::cout << blue << ' ' << food << ' ' << corrupt << '\n';

    // useful unscoped enum
    enum UserInfoFields
    {
        uiName,
        uiEmail,
        uiReputation
    };

    using UserInfo = std ::tuple<std ::string, std ::string, std ::size_t>;

    UserInfo uInfo;
    // auto val = std ::get<1>(uInfo);  //  ugly
    // std :: get needs size_t

    auto val = std ::get<uiEmail>(uInfo);

    std ::cout << val << '\n';

    enum class UserInfoFields1
    {
        uiName,
        uiEmail,
        uiReputation
    };

    auto val1 = std::get<toUType(UserInfoFields1::uiEmail)>(uInfo);

    return 0;
}

// Things to Remember
// • C++98-style enums are now known as unscoped enums.
// • Enumerators of scoped enums are visible only within the enum. They convert
// to other types only with a cast.
// • Both scoped and unscoped enums support specification of the underlying type.
// The default underlying type for scoped enums is int. Unscoped enums have no
// default underlying type.
// • Scoped enums may always be forward-declared. Unscoped enums may be
// forward-declared only if their declaration specifies an underlying type.