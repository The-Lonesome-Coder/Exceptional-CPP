/**
 * Item 22 - Class Relationships - Part 1
 *
 * Never use public inheritance except to model true Liskov IS-A and WORKS-LIKE-A. All overridden member functions must
 * require no more and promise no less.
 *
 * Never inherit publicly to reuse code (in the base class); inherit publicly in order to be reused (by code that uses base
 * objects polymorphically).
 *
 * When modeling "is implemented in terms of," always prefer membership/containment, not inheritance. Use private inheritance
 * only when inheritance is absolutely necessary—that is, when you need access to protected members or you need to override a
 * virtual function. Never use public inheritance for code reuse.
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * A networking application has two kinds of communications sessions, each with its own message protocol. The two protocols
 * have similarities (some computations and even some messages are the same), so the programmer has come up with the
 * following design to encapsulate the common computations and messages in a BasicProtocol class.
 *
 * Comment on this design. Is there anything you would change? If so, why?
*/

/**
 * To sum up:
 *
 * BasicProtocol is clearly not designed to be derived from, its virtual destructor is unnecessary (indeed, misleading) and
 * should be eliminated.
 *
 * BasicProtocol should probably be renamed to something less misleading, such as MessageCreator or MessageHelper.
*/

/**
 * It provides no virtual functions, which means it is not intended to be used polymorphically, which is a strong hint
 * against public inheritance.
 *
 * It has no protected functions or members. This means there is no "derivation interface", which is a strong hint against
 * any inheritance at all, either public or private.
 *
 * It encapsulates common work, but as described, it does not seem to actually perform its own transmissions as the derived
 * classes do. This means that a BasicProtocol object does not WORK-LIKE-A derived protocol object, nor is it USABLE-AS-A
 * derived protocol object. Public inheritance should be used to model one thing and one thing only—a true interface IS-A
 * relationship that obeys the Liskov Substitution Principle.
*/
class BasicProtocol /* : possible base classes */
{
    public:
        BasicProtocol();
        virtual ~BasicProtocol();

        bool BasicMsgA( /*...*/ );
        bool BasicMsgB( /*...*/ );
        bool BasicMsgC( /*...*/ );
};

/**
 * This is clearly describing an "is implemented in terms of " relationship, which, in C++, is spelled either "private
 * inheritance" or "membership."
 *
 * The derived classes use BasicProtocol's public interface only. This means that they do not benefit from being derived
 * classes, and could as easily perform their work using a separate helper object of type BasicProtocol.
*/
class Protocol1 : public BasicProtocol
{
    public:
        Protocol1();
        ~Protocol1();

        bool DoMsg1( /*...*/ );
        bool DoMsg2( /*...*/ );
        bool DoMsg3( /*...*/ );
        bool DoMsg4( /*...*/ );
};

class Protocol2 : public BasicProtocol
{
    public:
        Protocol2();
        ~Protocol2();

        bool DoMsg1( /*...*/ );
        bool DoMsg2( /*...*/ );
        bool DoMsg3( /*...*/ );
        bool DoMsg4( /*...*/ );
        bool DoMsg5( /*...*/ );
};