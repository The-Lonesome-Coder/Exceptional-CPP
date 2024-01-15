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
 * Stack copy constructor does not call the StackImpl copy constructor.
 *
 * The worst that can happen here is that a T constructor could fail, in which case the StackImpl destructor will correctly
 * destroy exactly as many objects as were successfully created and then deallocate the raw memory.
*/

template <class T>
Stack<T>::Stack(const Stack& other)
    : StackImpl<T> { other.m_used }
{
    while (m_used < other.m_used)
    {
        construct(m_v + m_used, other.m_v[m_used]);
        ++m_used;
    }
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Copy assignment.
 *
 * Copy-and-swap idiom.
*/

template <class T>
Stack<T>& Stack<T>::operator = (const Stack& other)
{
    Stack temp { other };
    swap(temp);

    return *this;
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * count().
 *
 * count() is still the easiest member function to write.
*/

template <class T>
std::size_t Stack<T>::count() const
{
    return m_used;
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * push().
 *
 * push() needs a little more attention.
*/

template <class T>
void Stack<T>::push(const T& element)
{
    /**
     * If we don't have enough room for the new element, we trigger a reallocation.
    */
    if (m_used == m_size)
    {
        /**
         * We simply construct a temporary Stack object, push the new element onto that, and finally swap out our original
         * guts to it to ensure they're disposed of in a tidy fashion.
         *
         * If the construction of temp fails, our state is unchanged and no resources have been leaked, so that's fine.
         *
         * If any part of the loading of temp's contents (including the new object's copy construction) fails by throwing an
         * exception, temp is properly cleaned up when its destructor is called as temp goes out of scope.
         *
         * In no case do we alter our state until all the work has already been completed successfully.
        */
        Stack temp { m_size * 2 + 1 };

        while (temp.count() < m_used)
        {
            temp.push(m_v[ temp.count() ]);
        }

        temp.push(element);
        swap(temp);
    }
    else
    {
        /**
         * If we already have room for the new object, we attempt to construct it. If the construction succeeds, we update
         * our m_used count.
        */
        construct(m_v + m_used, element);
        ++m_used;
    }
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * top() and pop() do not change.
*/

template <class T>
const T& Stack<T>::top() const
{
    if (m_used == 0)
    {
        throw("Empty Stack");
    }

    return m_v[m_used - 1];
}


template <class T>
T& Stack<T>::top()
{
    return const_cast<T&>(std::as_const(*this).top());
}


template <class T>
void Stack<T>::pop()
{
    if (m_used == 0)
    {
        throw("Popping from empty stack.");
    }
    else
    {
        --m_used;
    }
}