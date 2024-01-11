#include <cstddef>
/**
 * Item 08 - Writing Exception-Safe Code - Part 1
*/

/**
 * Make Stack exception-safe and exception-neutral (no catch statement).
*/

template <class T>
class Stack
{
    public:
        Stack();
        ~Stack() noexcept;

    private:
        T* m_v;
        std::size_t m_size;
        std::size_t m_used;

};

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * The below function is fully exception-safe and exception-neutral.
 *
 * 1. Exception-neutral - we don't catch anything, so if the new throws, then the exception is correctly propagated up to
 *                        or caller as required. If a function is not going to handle an exception, it should allow the
 *                        exception to propagate up to a caller who can handle it.
 *
 * 2. It does not leak. If the operator new[] allocation call exited by throwing a bad_alloc() exception, then no memory was
 *    was allocated to begin with, so there cannot be a leak. If one of the T constructors threw, then any T objects that
 *    were fully constructed were properly destroyed and, finally, operator delete[]() was automatically called to release
 *    the memory.
 *
 * 3. It's in a consistent state whether or not any part of the new throws. If the new throws, we propagate the exception
 *    out of our own constructor, and, by definition, "exiting a constructor by means of an exception" means our Stack
 *    proto-object never actually got to become a completely constructed object at all.  Its lifetime never started, so its
 *    state is meaningless because the object never existed.
*/

template <class T>
Stack<T>::Stack()
    : m_v { new T[m_size] }, m_size { 10 }, m_used { 0 } { }


/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * delete[] will never throw, as the standard requires:
 *
 *      void operator delete[] (void*) noexcept;
 *      void operator delete[] (void*, std::size_t) noexcept;
 *
 * The only thing that could possibly throw is one of the T::~T calls, and we're arbitrarily going to have Stack require
 * that T::~T may not throw. To make a long story short, we just can't implement the Stack destructor with complete exception
 * safety if T::~T can throwTo make a long story short, we just can't implement the Stack destructor with complete exception
 * safety if T::~T can throw.
 *
 * Never allow an exception to escape from a destructor or from an overloaded operator delete() or operator delete[]().
 * Write every destructor and deallocation function as though it had an exception specification of "noexcept". More on this
 * as we go on, this is an important theme.

*/

template <class T>
Stack<T>::~Stack()
{
    delete[] m_v;       // This can't throw
}