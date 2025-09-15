#include <iostream>

void f(int v[2])
{
    for (int i = 0; i < 10; i++)
        std ::cout << v[i] << ' ' << v + i << '\n';
    std ::cout << v << '\n';
}

template <typename T, size_t N>
constexpr size_t arraySize(T (&)[N]) noexcept
{
    return N;
}

int main()
{
    int y = 20, z = 100;
    const int *x = &y;

    y = 10;
    x = &z;

    std::cout << *x << '\n';

    int w[] = {4374, 1345, 43223, 3453, 745, 74};
    int v[] = {1, 2, 3, 4};

    // f(v);

    std ::cout << arraySize(w) << ' ' << arraySize(v) << '\n';

    auto &&rv = 27;

    std ::cout << rv << '\n';

    rv = 100;

    std ::cout << rv << '\n';

    rv = y;

    std ::cout << rv << '\n';

    for (int i{}; i < 3; i++)
        std ::cout << i << ' ';
    std ::cout << '\n';

    for (int i{}; i < 3; ++i)
        std ::cout << i << ' ';
    std ::cout << '\n';

    return 0;
}