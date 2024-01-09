#include <list>
#include <string>
#include <string_view>
#include <vector>
/**
 * Item 06 - Temporary Object
*/

/**
 * How can you spot and avoid temporary object?
 *
 * Please see the following code, how many can you identify, and how should the programmer fix them?
*/

class Employee
{
    public:
        std::string m_address;
};

std::string FindAddress(std::list<Employee> employees, std::string name)
{
    for (std::list<Employee>::iterator i = employees.begin(); i != employees.end(); i++)
    {
        if (*i == name)
        {
            return i->m_address;
        }
    }

    return "";
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * The parameters should be passed by const&. For string, should be passed by std::string_view.
 *
 * std::list pass-by-value forces the compiler to make complete copies of both objects, which can be expensive.
 * std::string_view, on the other hand, is cheap to be passed by value.
*/
std::string FindAddress(const std::list<Employee>& employees, std::string_view name)
{
    /**
     * For functions / loops calls iterator that do not modify the original object, use cbegin() / cend() instead of
     * begin() / end() is better, they prevent accidental modification.
     *
     * cbegin() / cend() returns a temporary object that must be constructed and destroyed, because the value will not
     * change, recomputing it on every loop iteration is inefficient and unaesthetic. The value should only be computed once.
     *
     * i++ (post-increment) is usually less efficient than preincrement because it has to remember and return its original
     * value. For consistency, always implement post-increment in terms of pre-increment, otherwise your users will get
     * surprising results.
    */
    for (std::list<Employee>::const_iterator i = employees.cbegin(), end = employees.cend(); i != end; ++i)
    {
        /**
         * For this code to work, Employee class most likely has a conversion to string or a conversion constructor taking
         * a std::string. Both cases create a temporary object, invoking either operator == for std::string or operator ==
         * for Employees.
        */
        if (*i == name)
        {
            return i->m_address;
        }
    }

    return "";
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * After C++11, the following code is much preferable.
 *
 * 1. Prefer vector over list.
 * 2. Use std::string_view on std::string object (C++17).
 * 3. Use range-for for iterating through the entire container.
*/

std::string FindAddress(const std::vector<Employee> employees, std::string_view address)
{
    for (auto& i : employees)
    {
        if (i == address)
        {
            return i;
        }
    }

    return "";
}