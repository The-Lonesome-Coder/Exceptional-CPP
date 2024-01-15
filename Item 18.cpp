#include <iostream>
#include <string>
/**
 * Item 18 - Code Complexity - Part 1
*/

/**
 * How many execution paths could there be in the following code?
*/

class Employee
{
    public:
        std::string title() const;
        std::int32_t salary() const;
        std::string first() const;
        std::string last() const;
};

std::string Evaluate(Employee employee)
{
    if (employee.title() == "CEO" || employee.salary() > 100'000)
    {
        std::cout << employee.first() << " " << employee.last() << " is overpaid." << std::endl;
    }

    return employee.first() + " " + employee.last();
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * The answer is 23.
 *
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Non-exceptional code paths:
 *
 * 1. If employee.title() == "CEO" evaluates to true, then the second part of the condition doesn't need to be evaluated (for
 *    example, e.Salary() will never be called), but the cout will be performed. With suitable overloads for ==, ||, and/or
 *    > in the if's condition, the || could actually turn out to bea function call. If it is a function call, the
 *    short-circuit evaluation rule would be suppressed and both parts of the condition would be evaluated all the time.
 *
 * 2. If employee.title() != "CEO" but employee.salary() > 100000, both parts of the condition will be evaluated and the cout
 *    will be performed.
 *
 * 3. If employee.title() != "CEO" and employee.salary() <= 100000, the cout will not be performed.
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Exceptional code paths:
 *
 * 4. The argument is passed by value, which invokes the Employee copy constructor. This copy operation might throw an
 *    exception.
 *
 * 5. The title() member function might itself throw, or it might return an object of class type by value, and that copy
 *    operation might throw.
 *
 * 6. To match a valid operator == (), the string literal may need to be converted to a temporary object of class type
 *    (probably the same as employee.title()'s return type), and that construction of the temporary might throw.
 *
 * 7.  If operator == () is a programmer-supplied function, it might throw.
 *
 * 8. Similar to #5, Salary() might itself throw, or it might return a temporary object and this construction operation might
 *    throw.
 *
 * 9. Similar to #6, a temporary object ("CEO") may need to be constructed and this construction might throw.
 *
 * 10. Similar to #7, > might be a programmer-provided function and therefore might throw.
 *
 * 11. Similar to #7 and #10, || might be a programmer-provided function and therefore might throw.
 *
 * 12. As documented in the C++ Standard, any of the five calls to operator << might throw.
 *
 * 13. Similar to #5, first() and/or last() might throw, or each might return a temporary object and those construction
 *     operations might throw.
 *
 * 14. Similar to #5, first() and/or last() might throw, or each might return a temporary object and those construction
 *     operations might throw.
 *
 * 15. Similar to #6, a temporary object (" is overpaid.") may need to be constructed and this construction might throw.
 *
 * 16. Similar to #7, << might be a programmer-provided function and therefore might throw.
*/