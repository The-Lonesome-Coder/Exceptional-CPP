#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>
/**
 * Item 01 - Iterators
 *
 * Every programmer who uses the standard library has to be aware of these common and not-so-common iterator mistakes:
 *
 * 1. Valid values: Is the iterator dereferenceable?
 * 2. Valid lifetimes: Is the iterator still valid when it's being used? Or has it been invalidated by some operation since
 *    we obtained it?
 * 3. Valid ranges: Is a pair of iterators a valid range?
 * 4. Illegal builtin manipulation.
*/

class Date;
Date TodaysDate();


int main()
{
    /**
     * Fine.
     *
     * Declare and define a vector of class "Date".
    */
    std::vector<Date> e;


    /**
     * Fine.
     *
     * Calls "std::copy( InputIt first, InputIt last, OutputIt d_first )", which copies all elements in the range
     * [ first, last ).
     *
     * std::istream_iterator<> is a single-pass input iterator that reads successive objects of type T from the
     * std::basic_istream object for which it was constructed, by calling the appropriate operator >>.
     *
     * std::istream_iterator<Date> { std::cin }: calls "istream_iterator( istream_type& stream )" constructor, which
     * initializes the iterator with std::cin, stores the address of "stream" in a data member, and performs the first
     * read from the input stream.
     *
     * std::istream_iterator<Date> { }: calls "istream_iterator()" constructor, a default constructor that constructs
     * the end-of-stream iterator.
     *
     * "std::back_inserter( Container& c )" constructs a std::back_insert_iterator for the container c, with the type
     * deduced from the type of the argument.
     *
     * Copy the elements [ first, last ) into the vector backwardly. In this case, it reads the successive objects from
     * the beginning to the end of the stream, and inserts them into the vector backwardly.
    */
    std::copy( std::istream_iterator<Date> { std::cin },
               std::istream_iterator<Date> { },
               std::back_inserter(e) );


    /**
     * May be illegal. "last" may be e.end(), which is not a dereferenceable iterator (one-past-the-end iterator).
     *
     * Calls "std::find( InputIt first, InputIt last, const T& value )"", which searches for an element equal to "value"
     * in the range [ first, last ). Returns the first iterator satisfy "*it == value", or "last" if there is no such
     * iterator.
     *
     * Declare and define two std::vector<Date>::iterator, which points to the satisfied value, or e.end() if there is
     * no such iterator. Then dereference "last".
    */
    std::vector<Date>::iterator first { std::find( e.begin(), e.end(), "01/01/95" ) };
    std::vector<Date>::iterator last { std::find( e.begin(), e.end(), "12/31/95" ) };
    *last = "12/30/95";


    /**
     * May be illegal. [ first, last ) may not be a valid range. For instance, if  "01/01/95" is not found in "e" but
     * "12/31/95" is, then "first" will be after "last".
     *
     * Calls "std::copy( InputIt first, InputIt last, OutputIt d_first )".
     *
     * std::ostream_iterator<> is a single-pass LegacyOutputIterator that writes successive objects of type T into the
     * std::basic_ostream object for which it was constructed, using operator <<. Optional delimiter string is written .
     * to the output stream after every write operation.
     *
     * std::ostream_iterator<Date> { std::cout, "\n" }: calls "ostream_iterator( ostream_type& stream, const CharT* delim )"
     * constructor, which uses "delim" as the delimiter.
     *
     * Copy [ first, last ) into std::ostream_iterator<>, which basically prints out the elements in [ first, last ).
    */
    std::copy( first, last, std::ostream_iterator<Date> { std::cout, "\n" } );


    /**
     * Likely to be illegal. std::vector::iterator is often simple a Date*, and C++ does not allow you to modify temporaries
     * of built-in type. Even if we write the following as "e.insert( e.end() - 1, TodaysDate() )", you might get another
     * error, which is: if "e" is empty, any attempt to take "the iterator before e.end()" will not be valid.
     *
     * Calls "std::vector<T, Allocator>::insert( const_iterator pos, const T& value )", which inserts "value" before "pos".
     *
     * Insert the returned value of "TodaysDate()" into the position before "--e.end()".
    */
    e.insert( --e.end(), TodaysDate() );


    /**
     * May be illegal. "first" and "last" may not be valid iterators any more. A vector grows in "chunks" so that it won't
     * have to reallocate its buffer every time you insert something into it. Here, as a result of the e.insert() operation,
     * the vector may or may not grow. We must consider any existing iterators into that container to be invalidated.
    */
    std::copy( first, last, std::ostream_iterator<Date> { std::cout, "\n" } );
}


/**
 * Solution.
*/
// A not-so-useful class for code-testing
class Date
{
    public:
        Date() { }

        Date(std::string_view date)
            : m_date { date } { }

        Date& operator = (Date& another)
        {
            return another;
        }

        Date& operator = (std::string_view another)
        {
            m_date = another;

            return *this;
        }

        std::string getDate() const
        {
            return m_date;
        }

        bool operator == (Date& another)
        {
            return m_date == another.m_date;
        }

        bool operator == (std::string_view another)
        {
            return m_date == another;
        }

    friend std::istream& operator >> (std::istream& is, Date& date);
    friend std::ostream& operator << (std::ostream& os, const Date& date);

    private:
        std::string m_date;
};


// Intput operator for "Date" class
std::istream& operator >> (std::istream& is, Date& date)
{
    is >> date.m_date;

    return is;
}


// Output operator for "Date" class
std::ostream& operator << (std::ostream& os, const Date& date)
{
    os << date.m_date;

    return os;
}


// The day I write this program , I'm too tired to figure out how to use <chrono>, maybe later
Date todaysDate()
{
    return Date { "01/03/2024" };
}


int main()
{
    std::vector<Date> e;

    // Why use iterator when overloading is more convenient?
    Date date;
    std::cin >> date;
    e.emplace_back(date);

    std::vector<Date>::iterator first { std::find( e.begin(), e.end(), "01/01/95" ) };
    std::vector<Date>::iterator last { std::find( e.begin(), e.end(), "12/31/95" ) };

    // Check whether the iterator is one-pass-the-end
    if (last != e.end())
    {
        *last = "12/30/95";
    }

    // Range-for will be better (C++11) for printing out the elements
    for (auto element : e)
    {
        std::cout << element << ' ';
    }

    std::cout << '\n';

    // To insert an element in the end of a vector, use emplace_back()
    e.emplace_back(todaysDate());

    // Range-for will be better (C++11) for printing out the elements
    for (auto element : e)
    {
        std::cout << element << ' ';
    }
}