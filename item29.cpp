// Item 29: Assume that move operations are not present, not cheap, and not used.

#include <iostream>

// Assuming that Widget is a type
// where moving is faster than copying, moving a std::array of Widget will be faster
// than copying the same std::array. So std::array certainly offers move support.
// Yet both moving and copying a std::array have linear-time computational complexity,
// because each element in the container must be copied or moved. This is far
// from the “moving a container is now as cheap as assigning a couple of pointers”
// claim that one sometimes hears.

// On the other hand, std::string offers constant-time moves and linear-time copies.
// That makes it sound like moving is faster than copying, but that may not be the case.
// Many string implementations employ the small string optimization (SSO). With the
// SSO, “small” strings (e.g., those with a capacity of no more than 15 characters) are
// stored in a buffer within the std::string object; no heap-allocated storage is used.
// Moving small strings using an SSO-based implementation is no faster than copying
// them, because the copy-only-a-pointer trick that generally underlies the performance
// advantage of moves over copies isn’t applicable.

// The motivation for the SSO is extensive evidence that short strings are the norm for
// many applications. Using an internal buffer to store the contents of such strings eliminates
// the need to dynamically allocate memory for them, and that’s typically an efficiency
// win. An implication of the win, however, is that moves are no faster than
// copies, though one could just as well take a glass-half-full approach and say that for
// such strings, copying is no slower than moving.

// A consequence is
// that even if a type offers move operations that are more efficient than the corresponding
// copy operations, and even if, at a particular point in the code, a move operation
// would generally be appropriate (e.g., if the source object is an rvalue), compilers
// might still be forced to invoke a copy operation because the corresponding move
// operation isn’t declared noexcept.

int main() {

    return 0;
}

// Things to Remember
// • Assume that move operations are not present, not cheap, and not used.
// • In code with known types or support for move semantics, there is no need for
// assumptions.