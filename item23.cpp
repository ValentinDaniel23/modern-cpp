// Item 23: Understand std::move and std::forward.

#include <iostream>

// std::move and std::forward are merely functions (actually function templates)
// that perform casts. std::move unconditionally casts its argument to an rvalue, while
// std::forward performs this cast only if a particular condition is fulfilled. That’s it.
// The explanation leads to a new set of questions, but, fundamentally, that’s the complete
// story.

int main()
{
    return 0;
}