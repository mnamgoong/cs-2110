/*
 * CS 2110 Homework 10 Spring 2023
 * Author: Michelle Namgoong
 */

/* we need this for uintptr_t */
#include <stdint.h>
/* we need this for memcpy/memset */
#include <string.h>
/* we need this to print out stuff*/
#include <stdio.h>
/* we need this for the metadata_t struct and my_malloc_err enum definitions */
#include "my_malloc.h"

/* Function Headers
 * Here is a place to put all of your function headers
 * Remember to declare them as static
 */

/* Our freelist structure - our freelist is represented as a singly linked list
 * the freelist is sorted by address;
 */
metadata_t *address_list;

/* Set on every invocation of my_malloc()/my_free()/my_realloc()/
 * my_calloc() to indicate success or the type of failure. See
 * the definition of the my_malloc_err enum in my_malloc.h for details.
 * Similar to errno(3).
 */
enum my_malloc_err my_malloc_errno;



// -------------------- PART 1: Helper functions --------------------

/* The following prototypes represent useful helper functions that you may want
 * to use when writing your malloc functions. You do not have to implement them
 * first, but we recommend reading over their documentation and prototypes;
 * having a good idea of the kinds of helpers you can use will make it easier to
 * plan your code.
 *
 * None of these functions will be graded individually. However, implementing
 * and using these will make programming easier. We have provided ungraded test
 * cases these functions that you may check your implementations against.
 */


/* HELPER FUNCTION: find_right
 * Given a pointer to a free block, this function searches the freelist for another block to the right of the provided block.
 * If there is a free block that is directly next to the provided block on its right side,
 * then return a pointer to the start of the right-side block.
 * Otherwise, return null.
 * This function may be useful when implementing my_free().
 */
metadata_t *find_right(metadata_t *freed_block) {
    metadata_t *curr = address_list;

    while (curr != NULL && ((uintptr_t) curr != (uintptr_t) freed_block)) {
        curr = curr->next;
    }

    if (((uintptr_t) (uint8_t*) (curr + 1) + curr->size) == (uintptr_t) curr->next) {
        return curr->next;
    } else {
        return NULL;
    }
}

/* GIVEN HELPER FUNCTION: find_left
 * This function is provided for you by the TAs. You do not need to use it, but it may be helpful to you.
 * This function is the same as find_right, but for the other side of the newly freed block.
 * This function will be useful for my_free(), but it is also useful for my_malloc(), since whenever you sbrk a new block,
 * you need to merge it with the block at the back of the freelist if the blocks are next to each other in memory.
 */

metadata_t *find_left(metadata_t *freed_block) {
    metadata_t *curr = address_list;

    while (curr && ((uintptr_t) freed_block > (uintptr_t) curr)) {
        if ((uintptr_t) ((uint8_t*) (curr + 1) + curr->size) == (uintptr_t) freed_block) {
            return curr;
        }
        curr = curr->next;
    }
    
    return NULL;
}

/* HELPER FUNCTION: merge
 * This function should take two pointers to blocks and merge them together.
 * The most important step is to increase the total size of the left block to include the size of the right block.
 * You should also copy the right block's next pointer to the left block's next pointer. If both blocks are initially in the freelist, this will remove the right block from the list.
 * This function will be useful for both my_malloc() (when you have to merge sbrk'd blocks) and my_free().
 */
 void merge(metadata_t *left, metadata_t *right) {
    left->size += TOTAL_METADATA_SIZE + right->size;
    left->next = right->next;
 }

/* HELPER FUNCTION: split_block
 * This function should take a pointer to a large block and a requested size, split the block in two, and return a pointer to the new block (the right part of the split).
 * Remember that you must make the right side have the user-requested size when splitting. The left side of the split should have the remaining data.
 * We recommend doing the following steps:
 * 1. Compute the total amount of memory that the new block will take up (both metadata and user data).
 * 2. Using the new block's total size with the address and size of the old block, compute the address of the start of the new block.
 * 3. Shrink the size of the old/left block to account for the lost size. This block should stay in the freelist.
 * 4. Set the size of the new/right block and return it. This block should not go in the freelist.
 * This function will be useful for my_malloc(), particularly when the best-fit block is big enough to be split.
 */
 metadata_t *split_block(metadata_t *block, size_t size) {
    size_t new_size = TOTAL_METADATA_SIZE + size;
    metadata_t *new_block = (metadata_t*) ((uint8_t*) (block + 1) + block->size - new_size);
    
    block->size -= new_size;
    new_block->size = size;

    return new_block;
 }

/* HELPER FUNCTION: add_to_addr_list
 * This function should add a block to freelist.
 * Remember that the freelist must be sorted by address. You can compare the addresses of blocks by comparing the metadata_t pointers like numbers (do not dereference them).
 * Don't forget about the case where the freelist is empty. Remember what you learned from Homework 9.
 * This function will be useful for my_malloc() (mainly for adding in sbrk blocks) and my_free().
 */
 void add_to_addr_list(metadata_t *block) {
    if (address_list == NULL || (uintptr_t) block < (uintptr_t) address_list) { // freelist is empty
        block->next = address_list;
        address_list = block;
    } else {
        metadata_t *curr = address_list;
        while (curr->next != NULL && (uintptr_t) curr->next < (uintptr_t) block) {
            curr = curr->next;
        }
        block->next = curr->next;
        curr->next = block;
    }
}

/* GIVEN HELPER FUNCTION: remove_from_addr_list
 * This function is provided for you by the TAs. You are not required to use it or our implementation of it, but it may be helpful to you.
 * This function should remove a block from the freelist.
 * Simply search through the freelist, looking for a node whose address matches the provided block's address.
 * This function will be useful for my_malloc(), particularly when the best-fit block is not big enough to be split.
 */
 void remove_from_addr_list(metadata_t *block) {
    metadata_t *curr = address_list;
    if (!curr) {
        return;
    } else if (curr == block) {
        address_list = curr->next;
    }

    metadata_t *next;
    while ((next = curr->next) && (uintptr_t) block > (uintptr_t) next) {
        curr = next;
    }
    if (next == block) {
        curr->next = next->next;
    }
}
/* HELPER FUNCTION: find_best_fit
 * This function should find and return a pointer to the best-fit block. See the PDF for the best-fit criteria.
 * Remember that if you find the perfectly sized block, you should return it immediately.
 * You should not return an imperfectly sized block until you have searched the entire list for a potential perfect block.
 */
 metadata_t *find_best_fit(size_t size) {
    metadata_t *curr = address_list;
    metadata_t *best_fit = NULL;

    while (curr != NULL) {
        if (curr->size == size) { 
            return curr; // perfectly sized block
        } else if ((best_fit == NULL || curr->size < best_fit->size) && curr->size > size) {
            best_fit = curr; // block bigger than curr or previous best_fit
        } else {
            curr = curr->next;
        }
    }
    
    return best_fit;
 }




// ------------------------- PART 2: Malloc functions -------------------------

/* Before starting each of these functions, you should:
 * 1. Understand what the function should do, what it should return, and what the freelist should look like after it finishes
 * 2. Develop a high-level plan for how to implement it; maybe sketch out pseudocode
 * 3. Check if the parameters have any special cases that need to be handled (when they're NULL, 0, etc.)
 * 4. Consider what edge cases the implementation needs to handle
 * 5. Think about any helper functions above that might be useful, and implement them if you haven't already
 */


/* MALLOC
 * See PDF for documentation
 */
void *my_malloc(size_t size) {
    my_malloc_errno = NO_ERROR;

    // Reminder of how to do malloc:
    // 1. Make sure the size is not too small or too big.
    if (TOTAL_METADATA_SIZE + size > SBRK_SIZE) {
        my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
        return NULL;
    }
    
    if (size == 0) {
        return NULL;
    }

    // 2. Search for a best-fit block. See the PDF for information about what to check.
    metadata_t *best_fit = find_best_fit(size);
    // 3. If a block was not found:
    while (best_fit == NULL) { 
        // 3.a. Call sbrk to get a new block.
        metadata_t *new_block = (metadata_t *) my_sbrk(SBRK_SIZE);
        // 3.b. If sbrk fails (which means it returns -1), return NULL.
        if (new_block == (metadata_t *) -1) {
            my_malloc_errno = OUT_OF_MEMORY;
            return NULL;
        } else {
            // 3.c. If sbrk succeeds, add the new block to the freelist. If the new block is next to another block in the freelist, merge them.
            new_block->next = NULL;
            new_block->size = SBRK_SIZE - TOTAL_METADATA_SIZE;
            add_to_addr_list(new_block);

            metadata_t *left = find_left(new_block);
            if (left != NULL) {
                merge(left, new_block);
            }
        }
        // 3.d. Go to step 2.
        best_fit = find_best_fit(size);
    }

    // 4. If the block is too small to be split (see PDF for info regarding this), then remove the block from the freelist and return a pointer to the block's user section.
    if (best_fit->size < (TOTAL_METADATA_SIZE + size + 1)) {
        remove_from_addr_list(best_fit);
        return best_fit + 1;
    } else { // 5. If the block is big enough to split:
        // 5.a. Split the block into a left side and a right side. The right side should be the perfect size for the user's requested data.
        // 5.b. Keep the left side in the freelist.
        // 5.c. Return a pointer to the user section of the right side block.
        return split_block(best_fit, size) + 1;
    }
}

/* FREE
 * See PDF for documentation
 */
void my_free(void *ptr) {
    my_malloc_errno = NO_ERROR;

    // Reminder for how to do free:
    // 1. Since ptr points to the start of the user block, obtain a pointer to the metadata for the freed block.
    if (ptr == NULL) {
        return;
    }

    metadata_t *freed_block = (metadata_t *) ptr - 1;

    // 3. Once the freed block has been merged (if needed), add the freed block back to the freelist.
    add_to_addr_list(freed_block);

    // 2. Look for blocks in the freelist that are positioned immediately before or after the freed block.
    metadata_t *curr = address_list;
    while (curr != NULL) {
        metadata_t *right = find_right(curr);
        // 2.a. If a block is found before or after the freed block, then merge the blocks.
        if (right != NULL) { 
            merge(curr, right);
        } else {
            curr = curr->next;
        }
    }
    
    // 4. Alternatively, you can do step 3 before step 2. Add the freed block back to the freelist,
    // then search through the freelist for consecutive blocks that need to be merged.
}

/* REALLOC
 * See PDF for documentation
 */
void *my_realloc(void *ptr, size_t size) {
    my_malloc_errno = NO_ERROR;

    // Reminder of how to do realloc:
    // 1. If ptr is NULL, then only call my_malloc(size). If the size is 0, then only call my_free(ptr).
    if (ptr == NULL) {
        return my_malloc(size);
    }

    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    // 2. Call my_malloc to allocate the requested number of bytes. If this fails, immediately return NULL and do not free the old allocation.
    void *new_block = my_malloc(size);
    if (new_block == NULL) {
        return NULL;
    }

    metadata_t *block = (metadata_t *) ptr - 1;
    // 3. Copy the data from the old allocation to the new allocation. We recommend using memcpy to do this. Be careful not to read or write out-of-bounds!
    memcpy(new_block, ptr, block->size);

    // 4. Free the old allocation and return the new allocation.
    my_free(ptr);
    return new_block;
}

/* CALLOC
 * See PDF for documentation
 */
void *my_calloc(size_t nmemb, size_t size) {
    my_malloc_errno = NO_ERROR;

    // Reminder for how to do calloc:
    // 1. Use my_malloc to allocate the appropriate amount of size.
    void *new_block = my_malloc(nmemb * size);
    if (new_block == NULL) {
        return NULL;
    }

    // 2. Clear all of the bytes that were allocated. We recommend using memset to do this.
    memset(new_block, 0, nmemb * size);

    return new_block;
}
