/**
 * Item 15 - Writing Exception-Safe Code - Part 8
*/

/**
 * 1. Which technique is better—using StackImpl as a private base class, or as a member object?
 *
 * Both methods give essentially the same effect and nicely separate the two concerns of memory management and object
 * construction/destruction. When deciding between private inheritance and containment, my rule of thumb is always to prefer
 * the latter and use inheritance only when absolutely necessary. Both techniques mean "is implemented in terms of," and
 * containment forces a better separation of concerns because the using class is a normal client with access to only the
 * used class's public interface.
 *
 * Use private inheritance instead of containment only when absolutely necessary, which means when:
 * - You need access to the class's protected members
 * - You need to override a virtual function
 * - The object needs to be constructed before other base sub-objects
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * 2. How reusable are the last two versions of Stack? What requirements do they put on T, the contained type?
 * (In other words, what kinds of T can our latest Stack accept? The fewer the requirements are, the more reusable Stack
 * will be.)
 *
 * These latest Stacks decouple memory management from contained object construction and destruction.
 *
 * The new Stacks construct and destroy individual objects in place as needed, instead of creating default T objects in the
 * entire buffer and then assigning them as needed. This second difference turns out to have significant benefits: better
 * efficiency and reduced requirements on T, the contained type.
*/

/* -------------------------------------------------------------------------------------------------------------------- */

/**
 * 3. Should Stack provide exception specifications on its functions?
 *
 * No, because we, the authors of Stack, don't know enough, and we still probably wouldn't want to even if we did know enough.
 * The same is true in principle for any generic container.
*/