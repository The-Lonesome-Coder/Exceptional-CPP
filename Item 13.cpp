#include <cstddef>
#include <utility>
/**
 * Item 13 - Writing Exception-Safe Code - Part 6
*/

/**
 * Implement all the member functions of the following version of Stack, which is to be implemented in terms of StackImpl
 * by using StackImpl as a private base class.
*/

template <class T>
class StackImpl
{
    protected:
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

template <class T>
class Stack : private StackImpl<T>
{
    public:
        Stack(std::size_t size = 0);
        ~Stack() = default;

        Stack(const Stack& other);
        Stack& operator = (const Stack& other);

        std::size_t count() const;
        void push(const T& element);
        const T& top() const;
        T& top();
        void pop();
};

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
void StackImpl<T>::swap(StackImpl& other) noexcept
{
    std::swap(m_v, other.m_v);
    std::swap(m_size, other.m_size);
    std::swap(m_used, other.m_used);
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Default constructor.
 *
 * Stack's default constructor simply calls the default constructor of StackImpl, that just sets the stack's state to empty
 * and optionally performs an initial allocation. The only operation here that might throw is the new done in StackImpl's
 * constructor, and that's unimportant when considering Stack's own exception safety.
*/

template <class T>
Stack<T>::Stack(std::size_t size)
    : StackImpl<T> { size } { }

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Destructor.
 *
 * We don't need to provide a Stack destructor. The default compiler-generated Stack destructor is fine, because it just
 * calls the StackImpl destructor to destroy any objects that were constructed and actually free the memory.
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Copy constructor.
 *
 *
*/