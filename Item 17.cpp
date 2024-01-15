/**
 * Item 17 - Writing Exception-Safe Code - Part 10
*/

/**
 * Is the C++ standard library exception-safe?
 *
 * The short answer is: Yes.
 *
 * All iterators returned from standard containers are exception-safe and can be copied without throwing an exception.
 *
 * All standard containers must implement the basic guarantee for all operations: They are always destructible, and they are
 * always in a consistent (if not predictable) state even in the presence of exceptions.
 *
 * All standard containers must also implement the strong guarantee for all operations (with two exceptions - multi-element
 * inserts (all containers) and inserts and erases (vector and deque)).
*/