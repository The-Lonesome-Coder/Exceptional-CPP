#include <algorithm>
#include <list>
#include <string>
#include <string_view>
#include <vector>
/**
 * Item 07 - Using the Standard Library
*/

class Employee;

/**
 * Effective reuse is an important part of good software engineering.
 *
 * How many pitfalls in Item 6 could have been avoided if only the programmer had used a standard library algorithm instead
 * of handcrafting the loop?
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

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * With no other changes, simply using std::find() could have avoided two temporaries.
 *
 * Reuse code—especially standard library code—instead of handcrafting your own. It's faster, easier, and safer.
*/

std::string FindAddress(const std::vector<Employee> employees, std::string_view address)
{
    if (auto target { std::find(employees.cbegin(), employees.cend(), address) }; target != employees.cend())
    {
        return target;
    }

    return "";
}