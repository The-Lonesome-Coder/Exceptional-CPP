#include <iostream>
#include <string>
/**
 * Item 02 - Case-Insensitive Strings Part 1
*/

/**
 * 1. What does "case-insensitive" mean?
 *
 * In computer science, it means treating or interpreting uppercase and lowercase letters as being the same.
*/


/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * 2. Write a ci_string class that is identical to the standard std::string class but that is case-insensitive in the same
 * way as the commonly provided extension stricmp().
 *
 * What to achieve:
 *
 *      ci_string s( "AbCdE" );
 *
 *      // case insensitive
 *      assert( s == "abcde" );
 *      assert( s == "ABCDE" );
 *
 *      // still case-preserving, of course
 *      assert( strcmp( s.c_str(), "AbCdE" ) == 0 );
 *      assert( strcmp( s.c_str(), "abcde" ) != 0 );
*/

/**
 * String is not really a class, it's an alias of a template called "basic_string<char, char_traits<char>, allocator<char>>".
 * "char_traits" defines how characters interact (including compare).
*/
class CI_Char_Traits : public std::char_traits<char>
{
    public:
        static constexpr char toupper(char c) { return std::toupper(static_cast<unsigned char>(c)); }

        static constexpr bool eq(const char c1, const char c2) { return toupper(c1) == toupper(c2); }
        static constexpr bool lt(const char c1, const char c2) { return toupper(c1) < toupper(c2); }

        static constexpr int compare(const char* s1, const char* s2, std::size_t n)
        {
            while (n-- != 0)
            {
                if (toupper(*s1) < toupper(*s2))
                {
                    return -1;
                }

                if (toupper(*s1) > toupper(*s2))
                {
                    return 1;
                }

                ++s1;
                ++s2;
            }

            return 0;
        }

        static const char* find(const char* s, std::size_t n, char a)
        {
            const auto ua { toupper(a) };

            while (n-- != 0)
            {
                if (toupper(*s) == ua)
                {
                    ++s;
                }
            }

            return nullptr;
        }
};

/**
 * Make CI_String that operates exactly like the standard string, except it uses CI_Char_Traits instead of
 * std::char_traits<char> to get its character comparison rules.
*/
using CI_String = std::basic_string<char, CI_Char_Traits>;


/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * 3. Is making case sensitivity a property of the object a good idea?
 *
 * In most cases, it's more useful to have the case sensitivity be a characteristic of the comparison operation. Sometimes
 * it's useful because you can simply compare values "naturally" without having to remember to use the case-insensitive
 * comparison every time.
*/