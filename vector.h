#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdint.h>

/// @file vector.h

struct vector
{
    // Current number of entries in the vector.
    uint32_t len;
    // Current capacity of the vector.
    uint32_t cap;
    // Size of each entry.
    size_t memb_size;
    // Backing memory of the vector.
    void *arr;
};

/**
 * Initialize a struct vector.
 * @param vec The vector to initialize.
 * @param memb_size How large each entry in the vector will be.
 * @param nmemb How many entries to expect in the vector.
 * @return 0 if success, -1 otherwise.
 */
int vector_init(struct vector *vec, size_t memb_size, uint32_t nmemb);
/**
 * Insert the given data into the vector.
 * NOTE: May realloc vector if insufficient space is available.
 * Copies vec->memb_size bytes from memb parameter.
 * @param vec The vector to insert into.
 * @param memb The entry to insert.
 * @return 0 if success, -1 otherwise.
 */
int vector_insert(struct vector *vec, void *memb);
/**
 * Free and clean up the given vector.
 * @param vec The vector to clean up.
 * @return 0 if success, -1 otherwise.
 */
void vector_destroy(struct vector *vec);

/**
 * Create a for loop to iterate over the members of a struct vector.
 * Creates two variables, iterv_t iterv and uint32_t iterv_idx.
 *  @param iterv_t The pointer type to iterate over the vector as. Expected that sizeof(*iterv_t) == vec.memb_size.
 *  @param iterv   The name of the iterator variable for the loop.
 *  @param vec     The struct vector to iterate over.
 */
#define FOR_EACH(iterv_t, iterv, vec) iterv_t iterv = (vec)->arr; for (uint32_t iterv##_idx=0; (iterv##_idx * (vec)->memb_size) < (vec)->len; iterv##_idx++, iterv++)

#endif

