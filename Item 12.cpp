#include <cstddef>
#include <utility>
/**
 * Item 12 - Writing Exception-Safe Code - Part 5
*/

/**
 * 1 .Implement all three member functions of StackImpl, but not just any old way. Assume that at any time, the m_v buffer
 *    must contain exactly as many constructed T objects as there are T objects in the container, no more, no less. In
 *    particular, unused space in the m_v buffer should not contain constructed T objects.
 *
 * 2. Describe StackImpl's responsibilities. Why does it exist?
 *
 * 3. What should ???? be? How does the choice affect how StackImpl will be used? Be as specific as possible.
*/

template <class T>
class StackImpl
{
    public:
        StackImpl(std::size_t size = 0);
        ~StackImpl();

        // No copying allowed
        StackImpl(const StackImpl& other) = delete;
        StackImpl& operator = (const StackImpl& other) = delete;

        void swap(StackImpl& other) noexcept;

        T* m_v;
        std::size_t m_size;
        std::size_t m_used;
};

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Constructor.
 *
 * The constructor is fairly straightforward. We'll use operator new() to allocate the buffer as raw memory.
*/

template <class T>
StackImpl<T>::StackImpl(std::size_t size)
    : m_v { static_cast<T*>(size == 0 ? 0 : new(sizeof(T) * size)) }, m_size { size }, m_used { 0 } { }

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Destructor.
 *
 * The destructor is the easiest of the three functions to implement, with some standard helper functions.
*/

// construct()
template <class T, class U>
void construct(T* pointer, const U& value)
{
    new (pointer) T(value);
}

// destroy() destroys an object or a range
template <class T>
void destroy(T* pointer)
{
    pointer->~T();
}

template <class ForwardIterator>
void destroy(ForwardIterator first, ForwardIterator last)
{
    while (first != last)
    {
        destroy(&*(first));
        ++first;
    }
}

template <class T>
StackImpl<T>::~StackImpl()
{
    destroy(m_v, m_v + m_used);
    delete m_v;
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * swap().
 *
 * A simple but very important function. this is the function that is instrumental in making the complete Stack class so
 * elegant, especially its operator = ().
 *
 * swap() supports the strongest exception guarantee of all—namely, the nothrow guarantee; swap() is guaranteed not to throw
 * an exception under any circumstances.
*/

template <class T>
void StackImpl<T>::swap(StackImpl& other) noexcept
{
    std::swap(m_v, other.m_v);
    std::swap(m_size, other.m_size);
    std::swap(m_used, other.m_used);
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * 2. Describe StackImpl's responsibilities. Why does it exist?
 *
 *  StackImpl is responsible for simple raw memory management and final cleanup, so any class that uses it won't have to
 * worry about those details.
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * 3. What should ???? be? How does the choice affect how StackImpl will be used? Be as specific as possible.
 *
 * Using protected means that StackImpl is intended to be used as a private base class. So Stack will be "implemented in
 * terms of " StackImpl, which is what private inheritance means, and we have a clear division of responsibilities.
 *
 * Using public hints that StackImpl is intended to be used as a struct by some external client, because its data members
 * are public. So again, Stack will be "implemented in terms of " StackImpl, only this time using a HAS-A containment
 * relationship instead of private inheritance.
*/