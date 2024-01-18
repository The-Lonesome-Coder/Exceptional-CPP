#include <string>
#include <string_view>
/**
 * Item 23 - Class Relationships - Part 2
 *
 * Avoid public virtual functions; prefer using the Template Method pattern instead.
 *
 * For widely used classes, prefer to use the compiler-firewall idiom (Pimpl Idiom) to hide implementation details. Use an
 * opaque pointer (a pointer to a declared but undefined class) declared as "struct XxxxImpl; XxxxImpl* m_p_impl;" to store
 * private members.
 *
 * Prefer cohesion. Always endeavor to give each piece of code—each module, each class, each function—a single, well-defined
 * responsibility.
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Design patterns are an important tool in writing reusable code. Do you recognize the patterns used in this Item? If so,
 * can you improve them?
*/

class Record;
class PrimaryKey;

class GenericTableAlgorithm
{
    public:
        GenericTableAlgorithm(std::string_view table);
        virtual ~GenericTableAlgorithm();

        bool process();

    private:
        virtual bool filter(const Record& record);
        virtual bool processRow(const PrimaryKey& key) = 0;

        class GenericTableAlgorithmImpl *m_p_impl;
};

class MyAlgorithm : public GenericTableAlgorithm
{
    /* Override filter() and processRow() to implement a specific operation */
};

int main()
{
    MyAlgorithm algorithm { "Customer" };
    algorithm.process();
}

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * This is a good design and implements a well-known design pattern. Which pattern is this? Why is it useful here?
 *
 * This is the Template Method pattern. It's useful because we can generalize a common way of doing something that always
 * follows the same steps. Only the details may differ and can be supplied by a derived class. Further, a derived class may
 * choose to reapply the same Template Method approach—that is, it may override the virtual function as a wrapper around a
 * new virtual function—so different steps can be filled in at different levels in the class hierarchy.
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * Without changing the fundamental design, critique the way this design was executed. What might you have done differently?
 * What is the purpose of the m_p_impl member?
 *
 * This design uses bool as return codes, with apparently no other way (status codes or exceptions) of reporting failures.
 * Depending on the requirements, this may be fine, but it's something to note. The m_p_impl member nicely hides the
 * implementation behind an opaque pointer. The struct that m_p_impl points to will contain the private member functions and
 * member variables so that any change to them will not require client code to recompile.
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * This design can, in fact, be substantially improved. What are GenericTableAlgorithm's responsibilities? If more than one,
 * how could they be better encapsulated? Explain how your answer affects the class's reusability, especially its
 * extensibility.
 *
 * GenericTableAlgorithm can be substantially improved because it currently holds two jobs. Just as humans get stressed when
 * they have to hold two jobs — because that means they're loaded up with extra and competing responsibilities — so too this
 * class could benefit from adjusting its focus. In the original version, GenericTableAlgorithm is burdened with two
 * different and unrelated responsibilities that can be effectively separated, because the two responsibilities are to
 * support entirely different audiences. In short, they are:
 *
 * 1. Client code USES the (suitably specialized) generic algorithm.
 * 2. GenericTableAlgorithm USES the specialized concrete "details" class to specialize its operation for a specific case or
 *    usage.
*/

/**
 * Responsibility #1:
 * Providing a public interface that encapsulates common functionality as a template method. This has nothing to do with
 * inheritance relationships, and can be nicely isolated to stand on its own in a better-focused class. The target audience
 * is external users of GenericTableAlgorithm.
*/

class GTAClient;

class GenericTableAlgorithm
{
    public:
        GenericTableAlgorithm(std::string_view table, GTAClient& worker);

        ~GenericTableAlgorithm();

        bool process();


    private:
        class GenericTableAlgorithmImpl* m_p_impl;
};


/**
 * Responsibility #2:
 * Providing an abstract interface for extensibility. This is an implementation detail of GenericTableAlgorithm that has
 * nothing to do with its external clients, and can be nicely separated out into a better-focused abstract protocol class.
 * The target audience is writers of concrete "implementation detail" classes which work with (and extend)
 * GenericTableAlgorithm.
*/

class GTAClient
{
    public:
        virtual ~GTAClient() = 0;

        virtual bool filter(const Record& record);
        virtual bool processRow(const PrimaryKey& key);
};
