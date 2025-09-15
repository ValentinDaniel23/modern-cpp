// Item 11: Prefer deleted functions to private undefined ones.

#include <iostream>
#include <string>

// CPP 11
template <class charT, class traits = std::char_traits<charT>>
class basic_ios : public std::ios_base
{
public:
    basic_ios(const basic_ios &) = delete;
    basic_ios &operator=(const basic_ios &) = delete;
};

class Widget
{
public:
    template <typename T>
    void processPointer(T *ptr)
    {
        std ::cout << *ptr << '\n';
    }
};

// The C++ standard states that explicit template specializations must be at namespace scope, that is, outside the class.
template <>
void Widget::processPointer<void>(void *) = delete;

template <>
void Widget::processPointer<char>(char *) = delete;

int main()
{
    Widget widget;

    int a = 10;
    float b = 21.5f;
    double c = 32.65;
    void *d = &a;
    char e = 'a';

    widget.processPointer(&a);
    widget.processPointer(&b);
    widget.processPointer(&c);
    // widget.processPointer(d);
    // widget.processPointer(e);

    // useless print
    std ::cout << *(int *)d << ' ' << e << '\n';

    return 0;
}

// Things to Remember
// • Prefer deleted functions to private undefined ones.
// • Any function may be deleted, including non-member functions and template
// instantiations.