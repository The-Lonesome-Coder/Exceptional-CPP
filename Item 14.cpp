#include <cstddef>
#include <utility>
/**
 * Item 14 - Writing Exception-Safe Code - Part 7
*/

/**
 * Imagine that the ???? comment in StackImpl stood for public.
 *
 * Implement all the member functions of the following version of Stack, which is to be implemented in terms of StackImpl by
 * using a StackImpl member object.
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

template <class T>
class Stack
{
    public:
        Stack(std::size_t size = 0);
        ~Stack() = default;

        Stack(const Stack& other);
        Stack& operator = (const Stack& other);

        std::size_t count() const;
        void push(const T& element);
        T& top();
        void pop();

    private:
        StackImpl<T> m_impl;
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

template <class T>
Stack<T>::Stack(std::size_t size)
    : m_impl { size } { }


template <class T>
Stack<T>::Stack(const Stack& other)
    : m_impl { other.m_impl.m_used }
{
    while (m_impl.m_used < other.m_impl.m_used)
    {
        construct(m_impl.m_v + m_impl.m_used, other.m_impl.m_v[m_impl.m_used]);
        ++m_impl.m_used;
    }
}


template <class T>
Stack<T>& Stack<T>::operator = (const Stack& other)
{
    Stack temp { other };
    m_impl.swap(temp.m_impl);

    return *this;
}


template <class T>
std::size_t Stack<T>::count() const
{
    return m_impl.m_used;
}


template <class T>
void Stack<T>::push(const T& element)
{
    if (m_impl.m_used == m_impl.m_size)
    {
        Stack temp { m_impl.m_size * 2 + 1 };

        while (temp.count() < m_impl.m_used)
        {
            temp.push(m_impl.m_v[temp.count()]);
        }

        temp.push(element);
        m_impl.swap(temp.m_impl);
    }
    else
    {
        construct(m_impl.m_v + m_impl.m_used, element);
        ++m_impl.m_used;
    }
}


template <class T>
T& Stack<T>::top()
{
    if (m_impl.m_used == 0)
    {
        throw("Empty Stack");
    }

    return m_impl.m_v[m_impl.m_used - 1];
}


template <class T>
void Stack<T>::pop()
{
    if (m_impl.m_used == 0)
    {
        throw("Pop from an empty stack.");
    }
    else
    {
        --m_impl.m_used;
        destroy(m_impl.m_v + m_impl.m_used);
    }
}