/**
 * Item 11 - Writing Exception-Safe Code - Part 4
*/

/**
 * 1. What are the important exception-safety guarantees?
 *
 * Basic guarantee:
 * Even in the presence of exceptions thrown by T or other exceptions, Stack objects don't leak resources. Note that this
 * also implies that the container will be destructible and usable even if an exception is thrown while performing some
 * container operation. However, if an exception is thrown, the container will be in a consistent, but not necessarily
 * predictable, state. Containers that support the basic guarantee can work safely in some settings.
 *
 *
 * Strong guarantee:
 * If an operation terminates because of an exception, program state will remain unchanged. This always implies
 * commit-or-rollback semantics, including that no references or iterators into the container be invalidated if an operation
 * fails.
 *
 *
 * Nothrow guarantee:
 * The function will not emit an exception under any circumstances. Overall exception safety isn't possible unless certain
 * functions are guaranteed not to throw. In particular, we've seen that this is true for destructors; later in this
 * miniseries, we'll see that it's also needed in certain helper functions, such as Swap().
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * 2. For the Stack<T> that was just implemented, what are the requirements on T, the contained type?
 *
 * For the template as we've seen it so far, Stack requires its instantiation type to have all of the following:
 * - Default constructor (to construct the v_ buffers)
 * - Copy constructor (if Pop returns by value)
 * - Nonthrowing destructor (to be able to guarantee exception-safety)
 * - Exception-safe copy assignment (To set the values in v_, and if the copy assignment throws, then it must guarantee that
 *   the target object is still a valid T. Note that this is the only T member function that must be exception-safe in order
 *   for our Stack to be exception-safe.)
*/
