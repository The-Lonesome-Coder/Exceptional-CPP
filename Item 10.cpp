#include <cstddef>
#include <utility>
#include <stack>
/**
 * Item 10 - Writing Exception-Safe Code - Part 3
 *
 * If a piece of code isn't exception-safe, that's generally okay and can simply be fixed. But if a piece of code cannot be
 * made exception-safe because of its underlying design, that almost always is a signal of its poor design.
*/

/**
 * Write the final three Stack functions: Count(), Push(), and Pop(). Remember, be exception-safe and exception-neutral.
*/

template <class T>
class Stack
{
    public:
        Stack();
        ~Stack() noexcept;

        Stack(const Stack& other);
        Stack& operator = (const Stack& other);

        std::size_t count() const;
        void push(const T& element);
        const T& top() const;
        T& top();
        void pop();                            // If empty, throw exception
        bool empty() const;

    private:
        T* m_v;
        std::size_t m_size;
        std::size_t m_used;
};


template <class T>
T* newCopy(const T* source, std::size_t sourceSize, std::size_t destSize)
{
    assert(destSize >= sourceSize);

    T* dest = new T[destSize];

    try
    {
        std::copy(source, source + sourceSize, dest);
    }
    catch ( ... )
    {
        delete[] dest;
        throw;
    }

    return dest;
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * The easiest of all Stack's members to implement safely is count(), because all it does is copy a builtin that can never
 * throw.
*/

template <class T>
std::size_t Stack<T>::count() const
{
    // Safe, built-ins don't throw
    return m_used;
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * We need to apply our now-usual duty of care on push().
*/

template <class T>
void Stack<T>::push(const T& element)
{
    // Grow if necessary by some grow factor
    if (m_used == m_size)
    {
        // Pick a new size for the buffer
        std::size_t newSize { m_size * 2 + 1 };

        // Make a larger copy using newCopy()
        // If throws, our Stack's state is unchanged and the exception propagates through cleanly
        T* m_vNew { newCopy(m_v, m_size, newSize) };

        // Delete the original buffer and take ownership of the new one involves only operations that are known not to throw,
        // so the entire if block is exception-safe.
        delete[] m_v;

        m_v = m_vNew;
        m_size = newSize;
    }

    // After any required grow operation, we attempt to copy the new value before incrementing our m_used count
    // If the assignment succeeds, the Stack's state is changed to recognize the presence of the new value
    m_v[m_used] = element;
    ++m_used;
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * As specified, Pop() has two responsibilities — namely, to pop the top-most element and to return the just-popped value.
 *
 * Prefer cohesion. Always endeavor to give each piece of code—each module, each class, each function—a single, well-defined
 * responsibility.
 *
 * Therefore, the preferable method is to separate the functions of "querying the top-most value" and "popping the top-most
 * value off the stack."
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


template <class T>
bool Stack<T>::empty() const
{
    return m_used == 0;
}