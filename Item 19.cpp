#include <iostream>
#include <string>
/**
 * Item 19 - Code Complexity - Part 2
 *
 * Providing the strong exception-safety guarantee often (but not always) requires you to trade off performance.
 *
 * If a function has multiple unrelated side effects, it cannot always be made strongly exception-safe. If not, it can be
 * done only by splitting the function into several functions, each of whose side effects can be performed atomically.
 *
 * Not all functions need to be strongly exception-safe. Both the original code and Attempt #1 satisfy the basic guarantee.
 * For many clients, Attempt #1 is sufficient and minimizes the opportunity for side effects to occur in the exceptional
 * situations.
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Is the function from Item 18 exception-safe (works properly in the presence of exceptions) and exception-neutral
 * (propagates all exceptions to the caller)?
*/

class Employee
{
    public:
        std::string title() const;
        std::int32_t salary() const;
        std::string first() const;
        std::string last() const;
};

std::string evaluate(Employee employee)
{
    if (employee.title() == "CEO" || employee.salary() > 100'000)
    {
        std::cout << employee.first() << " " << employee.last() << " is overpaid." << std::endl;
    }

    return employee.first() + " " + employee.last();
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * As written, this function satisfies the basic guarantee: In the presence of exceptions, the function does not leak
 * resources. This function does not satisfy the strong guarantee. The strong guarantee says that if the function fails due
 * to an exception, program state must not be changed. However, has two distinct side effects:
 *
 * - An "...overpaid..." message is emitted to cout.
 * - A name string is returned.
 *
 * To meet the strong guarantee, either both effects are completed or an exception is thrown and neither effect is performed.
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Attempt #1
 *
 * Replace std::endl with a newline character in order to get the entire string into one operator << call.
*/

std::string evaluate(Employee employee)
{
    std::string result { employee.first() + " " + employee.last() };

    if (employee.title() == "CEO" || employee.salary() > 100'000)
    {
        std::string message { result + " is overpaid.\n" };
        std::cout << message;
    }

    return result;
}