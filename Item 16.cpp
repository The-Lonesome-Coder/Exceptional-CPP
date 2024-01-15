/**
 * Item 16 - Writing Exception-Safe Code - Part 9
*/

/**
 * - Never allow an exception to escape from a destructor or from an overloaded operator delete() or operator delete[]();
 *   write every destructor and deallocation function as though it had an exception specification of "noexcept."
 *
 * - Always use the "resource acquisition is initialization" idiom to isolate resource ownership and management.
 *
 * - In each function, take all the code that might emit an exception and do all that work safely off to the side. Only then,
 *   when you know that the real work has succeeded, should you modify the program state (and clean up) using only
 *   non-throwing operations.
*/