#include <cstddef>
/**
 * Item 04 - Maximally Reusable Generic Containers - Part 1
*/

/**
 * How flexible can you make this simple container class?
 *
 * How can you best implement copy construction and copy assignment for the following fixed-length vector class?
 * How can you provide maximum usability for construction and assignment?
*/

template <typename T, std::size_t size>
class FixedVector
{
    public:
        using iterator          = T*;
        using const_iterator    = const T*;

        iterator begin() { return m_v; }
        iterator end() { return m_v + size; }

        const_iterator begin() const { return m_v; }
        const_iterator end() const { return m_v + size; }


    private:
        T m_v[size];
};


/**
 * The solution is in Item 05.
*/