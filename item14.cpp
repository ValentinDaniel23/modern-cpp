// Item 14: Declare functions noexcept if they won’t emit exceptions.

#include <iostream>
#include <vector>

// it permits compilers to generate better object code

int f(int x) throw();  // no exceptions from f: C++98 style
int f(int x) noexcept; // no exceptions from f: C++11 style

// C++98: throw()
// Declares that the function does not throw exceptions.
// If an exception is thrown anyway, the call stack is always unwound to the caller before the program terminates.
// The compiler must keep the stack in an "unwindable" state and destroy objects in reverse construction order → less room for optimization.

// C++11: noexcept
// Declares that the function does not throw exceptions.
// If an exception is thrown anyway, the stack may not be unwound at all before program termination.
// The compiler is allowed to skip stack unwinding preparation and can optimize more aggressively (less overhead, more efficient code).

// RetType function(params) noexcept; // most optimizable
// RetType function(params) throw();  // less optimizable
// RetType function(params);          // less optimizable

int main()
{
    // In C++11, std::vector prefers move operations over copy for efficiency during reallocation.
    // If a move constructor can throw (not marked noexcept), std::vector cannot guarantee container invariants if an exception occurs during reallocation.
    // As a result, most implementations (e.g., GCC, Clang) call std::terminate instead of falling back to copying.
    // Declaring a move constructor as noexcept signals to the standard library that moves are safe, allowing std::vector to move elements without risking termination.Safe practice: always make move constructors noexcept if they won’t throw, especially for types used in STL containers.

    // C++11 treats destructors and delete as safe by default—they won’t throw—and you almost never need to manually declare
    // noexcept for them. This makes code safer and allows better optimizations.

    return 0;
}

// Things to Remember
// • noexcept is part of a function’s interface, and that means that callers may
// depend on it.
// • noexcept functions are more optimizable than non-noexcept functions.
// • noexcept is particularly valuable for the move operations, swap, memory
// deallocation functions, and destructors.
// • Most functions are exception-neutral rather than noexcept.