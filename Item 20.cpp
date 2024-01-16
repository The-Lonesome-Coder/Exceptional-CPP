#include <format>
#include <iostream>
/**
 * Item 20 - Class Mechanics
 *
 * Watch out for hidden temporaries created by implicit conversions. One good way to avoid this is to make constructors
 * explicit when possible, and avoid writing conversion operators.
 *
 * Prefer passing objects by const& instead of passing by value.
 *
 * Prefer writing "a op= b;" instead of "a = a op b;" (where op stands for any operator). It's clearer, and it's often more
 * efficient.
 *
 * If you supply a standalone version of an operator, always supply an assignment version of the same operator and prefer
 * implementing the former in terms of the latter.
 *
 * The standard requires that operators = () [] and -> must be members, and class-specific operators new, new[], delete,
 * and delete[] must be static members.
 *
 * If the function is operator>> or operator<< for stream I/O:
 * If it needs type conversions on its leftmost argument:
 * If it can be implemented using the class's public interface alone:
 *      Make it a nonmember (and friend if needed in the first two cases)
 *
 *      If it needs to behave virtually,
 *      Add a virtual member function to provide the virtual behavior, and implement it in terms of that
 *
 * Else
 *      Make it a member
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * You are doing a code review. A programmer has written the following class, which shows some poor style and has some real
 * errors. How many can you find, and how would you fix them?
*/

class Complex
{
    public:
        Complex(double real, double imaginary = 0)
            : m_real { real }, m_imaginary { imaginary } {}

        void operator + (Complex other)
        {
            m_real = m_real + other.m_real;
            m_imaginary = m_imaginary + other.m_imaginary;
        }

        void operator << (std::ostream os)
        {
            os << std::format("({}, {})", m_real, m_imaginary);
        }

        Complex operator ++ ()
        {
            ++m_real;

            return *this;
        }

        Complex operator ++ (int number)
        {
            Complex temp { *this };
            ++m_real;

            return temp;
        }

    private:
        double m_real;
        double m_imaginary;
};

/* -------------------------------------------------------------------------------------------------------------------- */

class Complex
{
    public:
        /**
         * The constructor allows an implicit conversion, therefore, use explicit to disallow such incident.
        */
        explicit Complex(double real, double imaginary = 0)
            : m_real { real }, m_imaginary { imaginary } {}

        /**
         * For efficiency, the parameter should be passed by reference to const. Also, operator + should be implemented in
         * terms of +=. operator+= is more efficient is that it operates on the left-hand object directly and returns only a
         * reference, not a temporary object.
        */
        Complex& operator += (const Complex& other)
        {
            m_real += other.m_real;
            m_imaginary += other.m_imaginary;

            return *this;
        }

        /**
         * Make it a friend function so it can be a global function but allowed to use private members.
        */
        friend std::ostream& operator << (std::ostream& os, const Complex& complex);

        /**
         * The pre-increment operator's return type should be a reference to a non-const.
        */
        Complex& operator ++ ()
        {
            ++m_real;

            return *this;
        }

        /**
         * Post-increment operator's return type should be const T.
         *
         * Postincrement should be implemented in terms of preincrement.
        */
        const Complex operator ++ (int number)
        {
            Complex temp { *this };
            ++m_real;

            return temp;
        }


    private:
        double m_real;
        double m_imaginary;
};


/**
 * Operator + should not be a member function. If operator+ is made a member function, as it is here, then it won't work as
 * naturally as your users may expect when you do decide to allow implicit conversions from other types.
 *
 * Operator + should not modify this object's value, and it should return a temporary object containing the sum.
*/
const Complex operator + (const Complex& lhs, const Complex& rhs)
{
    Complex temp { lhs };
    temp += rhs;

    return temp;
}


/**
 * Operator << should not be a member function.
 *
 * Operator << should have a return type of "ostream&" and should return a reference to the stream in order to permit
 * chaining.
*/
std::ostream& operator << (std::ostream& os, const Complex& complex)
{
    os << std::format("({}, {})", complex.m_real, complex.m_imaginary);
}