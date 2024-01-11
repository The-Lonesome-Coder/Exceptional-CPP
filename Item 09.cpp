#include <algorithm>
#include <cassert>
#include <cstddef>
/**
 * Item 08 - Writing Exception-Safe Code - Part 2
 *
 * In each function, take all the code that might emit an exception and do all that work safely off to the side. Only then,
 * when you know that the real work has succeeded, should you modify the program state (and clean up) using only non-throwing
 * operations.
*/

/**
 * Write the Stack copy constructor and copy assignment operator so that both are demonstrably exception-safe and
 * exception-neutral.
*/

template <class T>
class Stack
{
    public:
        Stack();
        ~Stack() noexcept;

        Stack(const Stack& other);
        Stack& operator = (const Stack& other);

    private:
        T* m_v;
        std::size_t m_size;
        std::size_t m_used;

};

template <class T>
Stack<T>::Stack()
    : m_v { new T[m_size] }, m_size { 10 }, m_used { 0 } { }

template <class T>
Stack<T>::~Stack()
{
    delete[] m_v;
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * To implement the copy constructor and the copy assignment operator, let's use a common helper function - newCopy(), to
 * manage allocating and growing memory.
*/

template <class T>
T* newCopy(const T* source, std::size_t sourceSize, std::size_t destSize)
{
    assert(destSize >= sourceSize);

    /**
     * The new allocation might throw std::bad_alloc or the T::T's may throw anything. In either case, nothing is allocated
     * and we simply allow the exception to propagate. This is both leak-free and exceptional-neutral.
    */
    T* dest = new T[destSize];

    try
    {
        /**
         * It assign all the existing values using T::operator =. If any of the assignments fail, we catch the exception,
         * free the allocated memory and rethrow the original exception. This is again both leak-free and exception-neutral.
         * However, there's an important subtlety here: T::operator=() must guarantee that if it does throw, then the
         * assigned-to T object must be destructible.
        */
        std::copy(source, source + sourceSize, dest);
    }
    catch ( ... )
    {
        delete[] dest;      // This can't throw
        throw;
    }

    /**
     * If the allocation and copy both succeed, then we return the pointer to the new buffer and relinquish ownership.
     * The return simply copies the pointer value, which cannot throw.
    */
    return dest;
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * With the newCopy() in hand, the Stack copy constructor is easy to write.
*/

template <class T>
Stack<T>::Stack(const Stack<T>& other)
    : m_v { newCopy(other.m_v, other.m_size, other.m_used) }, m_size { other.m_size }, m_used { other.m_used } { }

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Copy assignment.
*/

template <class T>
Stack<T>& Stack<T>::operator = (const Stack& other)
{
    if (this != &other)
    {
        T* newObject { newCopy(other.m_v, other.m_size, other.m_used) };

        delete[] m_v;

        m_v = newObject;
        m_size = other.m_size;
        m_used = other.m_used;
    }

    return *this;
}