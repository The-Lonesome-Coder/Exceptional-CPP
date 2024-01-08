#include <iostream>

#include "Item 02.cpp"
/**
 * Item 03 - Case-Insensitive Strings Part 2
*/

/**
 * 1. Is it safe to inherit CI_Char_Traits from std::char_traits<char> this way?
 *
 * Public inheritance should normally model Is-A / Works-Like-A as per the Liskov Substitution Principle (LSP). However, this
 * is one of the rare exceptions to the LSP, because CI_Char_Traits is not intended to be use polymorphically through a
 * pointer or reference to the base class std::char_traits<char>. The standard library does not use traits objects
 * polymorphically. In this case, inheritance is use merely for convenience.
 *
 * LSP does apply in another sense: it applies at compile-time, when the derived object must Work-Like-A base object in the
 * ways required by the std::basic_string template's requirements.
 *
 * In short, this inheritance is safe, because if conforms Generic LSP (any type (or template) passed as a template argument
 * should conform to the requirements listed for that argument).
*/


/* -------------------------------------------------------------------------------------------------------------------- */

CI_String s { "abc" };
std::cout << s << std::endl;

/**
 * 2. Why does the code above fail to compile?
 *
 * std::cout is actually std::basic_ostream<char, std::char_traits<char>>. operator << for std::basic_string is templated
 * and all, but it's specified only for insertion into a std::basic_ostream with the same "char type" and "traits type" as
 * std::string.
 *
 * To resolve this, define << and >> for CI_String.
*/

std::ostream& operator << (std::ostream& os, CI_String& str)
{
    os << str;

    return os;
}

std::istream& operator >> (std::istream& is, CI_String& str)
{
    is >> str;

    return is;
}


/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * 3. What about using other operators (for example +, -, +=) and mixing std::string and CI_String as arguments?
 *
 * For example:
 *
 *      std::string a { "aaa" };
 *      CI_String b { "bbb" };
 *      std::string c { a + b };
 *
 * To resolve this, define your own operator functions.
*/