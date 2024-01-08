#include <algorithm>
#include <cstddef>
#include <iterator>
/**
 * Item 05 - Maximally Reusable Generic Containers - Part 2
*/

/**
 * What is the following solution doing, and why? Explain each constructor and operator.
 * Does this design or code have any flaws?
*/

template <typename T, std::size_t size>
class FixedVector
{
    public:
        using iterator          = T*;
        using const_iterator    = const T*;

        FixedVector() {}

        /**
         * This is not a copy constructor,. A copy constructor specifically constructs from another object of exactly the
         * same tye - including the same template arguments.
        */
        template <typename U, std::size_t u_size>
        FixedVector(const FixedVector<U, u_size>& other)
        {
            std::copy(other.begin(), other.begin() + std::min(size, u_size), begin());
        }

        /**
         * This is not a copy assignment operator. Same reason as the above not-a-copy-constructor.
        */
        template <typename U, std::size_t u_size>
        FixedVector<T, size>& operator = (const FixedVector<U, u_size>& other)
        {
            std::copy(other.begin(), other.begin() + std::min(size, u_size), begin());

            return *this;
        }

        iterator begin() { return m_v; }
        iterator end() { return m_v + size; }

        const_iterator begin() const { return m_v; }
        const_iterator end() const { return m_v + size; }


    private:
        T m_v[size];
};


/**
 * There are two major usability consideration:
 *
 * 1. Support varying types.
 *    While FixedVector should remain a homogeneous container, sometimes it makes sense to construct or assign from another
 *    FixedVector that actually contains different objects. As long as the source objects are assignable to our type of
 *    object, this should be allowed.
 *
 * 2. Support varying sizes.
 *    Similarly, clients may want to construct or assign from FixedVector with different sizes.
*/
FixedVector<char, 4> v1;
FixedVector<int, 4> w1;         // Templated construction
w1 = v1;                        // Templated assignment


FixedVector<char, 6> v2;
FixedVector<int, 4> w2(v2);     // Initialize using 4 values
w2 = v2;                        // Assigns using 4 values


/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * The Standard Library Approach
*/

template <typename T, std::size_t size>
class AnotherFixedVector
{
    public:
        using iterator          = T*;
        using const_iterator    = const T*;

        // Copying
        template <class RAIterator>
        AnotherFixedVector(RAIterator first, RAIterator last)
        {
            std::copy(first, first + std::min(size, static_cast<std::size_t>(last - first)), begin());
        }

        // Assignment - we can't templatized assignment to take an iterator range. Instead, we can provide a named function.
        template <<class Iter>
        AnotherFixedVector<T, size>& assign(Iter first, Iter last)
        {
            std::copy(first, first + std::min(size, static_cast<std::size_t>(last - first)), begin());

            return *this;
        }

        iterator begin() { return m_v; }
        iterator end() { return m_v + size; }

        const_iterator begin() const { return m_v; }
        const_iterator end() const { return m_v + size; }


    private:
        T m_v[size];
};


/**
 * Now, for copying and assigning
*/
AnotherFixedVector<char, 6> v3;
AnotherFixedVector<int, 4> w3(v3.begin(), v3.end());

w3.assign(v3.begin(), v3.end());


/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Does this design or code have any flaws?
 *
 * Perhaps. Look at the operator = definition in FixedVector, if one of the T assignments fails during the std::copy()
 * operation, the object will be in an inconsistent state.Some of the contents of out FixedVector object will be what they
 * were before failed assignment, but other parts of the contents will already have been updated.
 *
 * Normally, the right way to solve this issue is to supply and atomic and noexcept swap() function, then simply use the
 * canonical for of operator =, which simply uses the create-a-temporary-and-swap-idiom. However, there is no way to write
 * an atomic noexcept swap() that exchanges the internals of two FixedVector objects. This is because the FixedVector
 * internals are stored as a simple array that can't be copied in one atomic step.
 *
 * There is another good solution.
*/

template <typename T, std::size_t size>
class SafeFixedVector
{
    public:
        using iterator          = T*;
        using const_iterator    = const T*;

        SafeFixedVector()
            : m_v { new T[size] } {}

        ~SafeFixedVector()
        {
            delete[] m_v;
        }

        template <typename U, std::size_t u_size>
        SafeFixedVector(const SafeFixedVector<U, u_size>& other)
            : m_v { new T[size] }
        {
            try
            {
                std::copy(other.begin(), other.begin() + std::min(size, u_size), begin());
            }
            catch ( ... )
            {
                delete[] m_v;
                throw;
            }
        }

        SafeFixedVector(const SafeFixedVector<T, size>& other)
            : m_v { new T[size] }
        {
            try
            {
                std::copy(other.begin(), other.begin() + std::min(size, u_size), begin());
            }
            catch ( ... )
            {
                delete[] m_v;
                throw;
            }
        }

        void swap(SafeFixedVector<T, size>& other) noexcept
        {
            std::swap(m_v, other.m_v);
        }

        template <typename U, std::size_t u_size>
        SafeFixedVector<T, size>& operator = (const SafeFixedVector<U, u_size>& other)
        {
            SafeFixedVector<T, size> temp { other };
            swap(temp);

            return *this;
        }

        SafeFixedVector<T, size>& operator = (const SafeFixedVector<T, size>& other)
        {
            SafeFixedVector<T, size> temp { other };
            swap(temp);

            return *this;
        }

        iterator begin() { return m_v; }
        iterator end() { return m_v + size; }

        const_iterator begin() const { return m_v; }
        const_iterator end() const { return m_v + size; }


    private:
        T* m_v;
};