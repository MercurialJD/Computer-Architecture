#include <stdlib.h>
#include <memory.h>
#include "vector.h"

/* Constructor, set CAPACITY and ELEMENT_SIZE to VECTOR, alloc memory for DATA */
int vector_init(Vector* vector, size_t capacity, size_t element_size) {

    /* Check if vector is NULL, which is illegal */
    if(vector == NULL) {
        return VECTOR_ERROR;
    }

    /* Check if element_size is zero, if so, set parameters to zero */
    if(element_size == 0) {
        vector->capacity = 0;       /* Set parameters */
        vector->element_size = 0;
        vector->size = 0;
        vector->data = VECTOR_UNINITIALIZED;
        return VECTOR_ERROR;        /* Error return */
    }

    /* Capacity can't be less than VECTOR_MINIMUM_CAPACITY */
    vector->capacity = (capacity >= VECTOR_MINIMUM_CAPACITY) ? capacity : VECTOR_MINIMUM_CAPACITY;
    vector->element_size = element_size;
    vector->size = 0;

    /* Try to allocate memory */
    vector->data = malloc(vector->capacity * vector->element_size);

    /* Check if memory allocated successfully */
    if(vector->data != NULL) {
        return VECTOR_SUCCESS;
    } else {
        return VECTOR_ERROR;
    }
}

/* Copy Constructor, destination should be an **UNINITIALIZED** vector.
   You should copy the memory, instead of changing the pointers only. */
int vector_copy(Vector* destination, Vector* source) {\

    /* Variable for memmove test */
    void *memmove_test;

    /* Check if vectors are NULL, which is illegal */
    if(destination == NULL || source == NULL) {
        return VECTOR_ERROR;
    }

    /* Check if source is initialized */
    if(source->data == VECTOR_UNINITIALIZED) {
        return VECTOR_ERROR;
    }

    /* Check if element_size is zero */
    if(source->element_size == 0) {
        destination->element_size = 0;
        return VECTOR_ERROR;
    }

    /* Copy basic parameters */
    destination->capacity = source->capacity;
    destination->element_size = source->element_size;
    destination->size = source->size;

    /*Allocate memory for destination and check if it's available */
    destination->data = malloc(destination->capacity * destination->element_size);
    if(destination->data == NULL) {
        return VECTOR_ERROR;
    }

    /* Copy the memory */
    memmove_test = memmove(destination->data, source->data, source->size * source->element_size);

    /* Check if copy succeed */
    if(memmove_test != NULL) {
        return VECTOR_SUCCESS;
    } else {
        return VECTOR_ERROR;
    }
}

/* Destructor, please free all memory allocated */
int vector_destroy(Vector* vector) {

    /* Check if vector is legal */
    if(vector == NULL || vector->element_size == 0 || vector->data == VECTOR_UNINITIALIZED) {
        return VECTOR_ERROR;
    }

    /* Set variables to zero */
    vector->capacity = 0;
    vector->element_size = 0;
    vector->size = 0;

    /* Free memory */
    if(vector->data != NULL) {
        free(vector->data);
        vector->data = VECTOR_UNINITIALIZED;
        return VECTOR_SUCCESS;
    } else {                    /* No need to free */
        vector->data = VECTOR_UNINITIALIZED;
        return VECTOR_ERROR;
    }
}

/* Insertion */
int vector_push_back(Vector* vector, void* element) {

    /* Check if vector is NULL, which is illegal and will cause vector->size Error */
    if(vector != NULL)
        return vector_insert(vector, vector->size, element);
    else
        return VECTOR_ERROR;
}

/* Another insertion */
int vector_push_front(Vector* vector, void* element) {
    return vector_insert(vector, 0, element);
}

/* Insert ELEMENT to INDEX, move all elements which index > INDEX right by one time */
int vector_insert(Vector* vector, size_t index, void* element) {

    /* Check if input is legal */
    if(vector == NULL || element == NULL || vector->element_size == 0 || index > vector->size || vector->data == VECTOR_UNINITIALIZED) {
        return VECTOR_ERROR;
    }

    /* Check if vector->data is (nearly) full */
    if(vector->size >= vector->capacity - 1) {

        /* Variables for memmove test and new capacity */
        void *test1, *test2, *new_mem;
        size_t new_capacity = vector->capacity;

        /* Cauculate the minimum new capacity */
        while(new_capacity <= vector->size + 1) {
            new_capacity *= VECTOR_GROWTH_FACTOR;
        }

        /* Temp new_mem and check */
        new_mem = malloc(new_capacity * vector->element_size);
        if(new_mem == NULL) {
            return VECTOR_ERROR;
        }

        /* Divide the data and leave room for new element */
        test1 = memmove(new_mem, vector->data, index * vector->element_size);     /* Copy data fraction 1 */
        test2 = memmove((char *)new_mem + (index + 1) * vector->element_size, (char *)(vector->data) + index * vector->element_size, (vector->size - index) * vector->element_size);     /* Copy data fraction 2 */
        if(test1 == NULL || test2 == NULL) {
            return VECTOR_ERROR;
        }

        /* Copy, free, change capacity */
        free(vector->data);
        vector->data = new_mem;
        vector->capacity = new_capacity;
    } else {

        /* Variable for memmove test */
        void *test;

        /* Move existing data to new_mem */
        void *new_mem = malloc((vector->size + 1) * vector->element_size);
        void *test1 = memmove(new_mem, vector->data, index * vector->element_size);     /* Copy data fraction 1 */
        void *test2 = memmove((char *)new_mem + (index + 1) * vector->element_size, (char *)(vector->data) + index * vector->element_size, (vector->size - index) * vector->element_size); /* Copy data fraction 2 */
        if(test1 == NULL || test2 == NULL) {
            return VECTOR_ERROR;
        }

        /* Move back data (leave one free room) */
        test = memmove(vector->data, new_mem, (vector->size + 1) * vector->element_size);
        free(new_mem);

        /* Test memmove */
        if(test == NULL) {
            return VECTOR_ERROR;
        }
    }

    /* Inc size, assign element and return */
    (vector->size)++;
    return vector_assign(vector, index, element);
}

/* Just assign VECTOR[INDEX] = ELEMENT, you should not move any elements */
int vector_assign(Vector* vector, size_t index, void* element) {

    /* Variable for memmove test */
    void *memmove_test;

    /* Check whether vector and index are valid */
    if(vector == NULL || element == NULL || vector->element_size == 0 || index >= vector->size || vector->data == VECTOR_UNINITIALIZED) {
        return VECTOR_ERROR;
    }

    /* Copy data */
    memmove_test = memmove((char *)(vector->data) + index * vector->element_size, element, vector->element_size);
    if(memmove_test == NULL) {       /* Test memmove */
        return VECTOR_ERROR;
    } else {
        return VECTOR_SUCCESS;
    }
}

/* Deletion */
/* Delete the right-most element */
int vector_pop_back(Vector* vector) {

    /* Check if vector is illegal */
    if(vector == NULL || vector->element_size == 0) {
        return VECTOR_ERROR;
    }

    /* Special operation for size == 0 */
    if(vector->size == 0) {
        return VECTOR_ERROR;
    } else {
        return vector_erase(vector, vector->size - 1);
    }
}

/* Delete element at INDEX = 0, move all other elements left by one */
int vector_pop_front(Vector* vector) {
    return vector_erase(vector, 0);
}

/* Delete element at INDEX, move all rhs elements left by one */
int vector_erase(Vector* vector, size_t index) {

    /* Variables for new_mem and memmove tests */
    void *new_mem, *test1, *test2, *test;

    /* Check whether vector and index are valid */
    if(vector == NULL || index >= vector->size || vector->element_size == 0 || vector->data == VECTOR_UNINITIALIZED) {
        return VECTOR_ERROR;
    }

    /* Move existing data to new_mem */
    new_mem = malloc((vector->size - 1) * vector->element_size);
    if(new_mem == NULL) {
        return VECTOR_ERROR;
    }

    test1 = memmove(new_mem, vector->data, index * vector->element_size);     /* Copy data fraction 1 */
    test2 = memmove((char *)new_mem + index * vector->element_size, (char *)(vector->data) + (index + 1) * vector->element_size, (vector->size - index - 1) * vector->element_size); /* Copy data fraction 2 */
    if(test1 == NULL || test2 == NULL) {        /* Test memmove */
        return VECTOR_ERROR;
    }

    /* Move back data (discard one selected one) and check */
    test = memmove(vector->data, new_mem, (vector->size - 1) * vector->element_size);
    if(test == NULL) {
        return VECTOR_ERROR;
    }

    /* Free and return */
    (vector->size)--;
    free(new_mem);
    return VECTOR_SUCCESS;
}

/* Delete all elements in the vector */
int vector_clear(Vector* vector) {

    /* Check whether vector and vector->data are valid */
    if(vector == NULL|| vector->element_size == 0 || vector->data == VECTOR_UNINITIALIZED) {
        return VECTOR_ERROR;
    }

    vector->size = 0;       /* Change size */
    return VECTOR_SUCCESS;
}

/* Lookup */
/* Returns VECTOR[INDEX] */
void* vector_get(Vector* vector, size_t index) {

    /* Check if inputs are legal */
    if(vector == NULL || index >= vector->size || vector->element_size == 0 || vector->data == VECTOR_UNINITIALIZED) {
        return NULL;
    }

    /* Return the pointer that points to vector[INDEX] */
    return ((char *)(vector->data) + index * vector->element_size);
}

/* Returns VECTOR[0] */
void* vector_front(Vector* vector) {
    return vector_get(vector, 0);
}

/* Returns VECTOR[-1] */
void* vector_back(Vector* vector) {

    /* Check if vector is NULL or vector->size is 0 */
    if(vector == NULL || vector->size == 0 || vector->element_size == 0) {
        return NULL;
    } else {
        return vector_get(vector, vector->size - 1);
    }
}

/* Information */
/* Returns size of VECTOR */
size_t vector_size(const Vector* vector) {

    /* Check if legal and return */
    if(vector == NULL || vector->element_size == 0 || vector->data == VECTOR_UNINITIALIZED) {
        return 0;
    } else {
        return vector->size;
    }
}

/* Returns TRUE if VECTOR is empty(i.e. size == 0) */
bool vector_is_empty(const Vector* vector) {

    /* Check if legal and return */
    if(vector == NULL || vector->element_size == 0 || vector->data == VECTOR_UNINITIALIZED) {
        return false;
    } else {
        return (vector->size == 0);
    }
}

/* Memory management */
int vector_resize(Vector* vector, size_t new_size) {

    /* Check if legal and return */
    if(vector == NULL || vector->element_size == 0 || vector->data == VECTOR_UNINITIALIZED) {
        return VECTOR_ERROR;
    }

    if(new_size <= vector->size) {
        vector->size = new_size;        /* Just change the size to new_size and truncate the rest */
        return VECTOR_SUCCESS;
    } else {
        if(new_size < vector->capacity) {
            vector->size = new_size;        /* Just change the size to new_size */
            return VECTOR_SUCCESS;
        } else {

            /* Variable for new_mem and memmove test, temp enlarged capacity */
            void *new_mem, *memmove_test;
            size_t new_capacity = vector->capacity;

            /* Cauculate the minimum new capacity */
            while(new_capacity <= new_size) {
                new_capacity *= VECTOR_GROWTH_FACTOR;
            }

            new_mem = malloc(new_capacity * vector->element_size);        /* new_mem */
            if(new_mem == NULL) {
                return VECTOR_ERROR;
            }

            /* Copy data */
            memmove_test = memmove(new_mem, vector->data, vector->size * vector->element_size);
            if(memmove_test == NULL) {
                return VECTOR_ERROR;
            }

            /* Exchange pointers */
            free(vector->data);
            vector->data = new_mem;

            /* Change size and capacity */
            vector->size = new_size;
            vector->capacity = new_capacity;
            return VECTOR_SUCCESS;
        }
    }
}

/* Iterators */
/* Begin at 0 */
Iterator vector_begin(Vector* vector) {
    return vector_iterator(vector, 0);
}

/* End at size */
Iterator vector_end(Vector* vector) {

    /* Temp iter for illegal inputs */
    Iterator iter = {NULL, 0};

    /* Check if vector == NULL */
    if(vector == NULL)
        return iter;
    else
        return vector_iterator(vector, vector->size);
}

/* Returns an iterator points to vector[index] */
Iterator vector_iterator(Vector* vector, size_t index) {

    /* Create iterator for illegal inputs */
    Iterator iter = {NULL, 0};

    /* Check and return */
    if(vector == NULL || index > vector->size || vector->element_size == 0 || vector->data == VECTOR_UNINITIALIZED) {
        return iter;
    }

    /* Change info */
    iter.pointer = (char *)(vector->data) + index * vector->element_size;
    iter.element_size = vector->element_size;
    return iter;
}

/* Returns the element, similar to `operator *()' in C++ */
void* iterator_get(Iterator* iterator) {

    /* Check if iterator exists and if it's legal */
    if(iterator == NULL || iterator->element_size == 0) {
        return NULL;
    }

    return iterator->pointer;
}

/* Similar to `operator++()' in C++ */
void iterator_increment(Iterator* iterator) {

    /* Do nothing if iterator is NULL or input is illegal */
    if(iterator == NULL || iterator->element_size == 0) {
        return;
    }

    /* Do nothing if iterator or iterator->pointer are NULL */
    if(iterator->pointer != NULL) {
        iterator->pointer = (char *)(iterator->pointer) + 1 * iterator->element_size;
    }
}

/* Similar to `operator--()' in C++ */
void iterator_decrement(Iterator* iterator) {

    /* Do nothing if iterator is NULL */
    /* How to avoid underflow??? */
    if(iterator == NULL || iterator->element_size == 0) {
        return;
    }

    /* Do nothing if iterator->pointer are NULL */
    if(iterator->pointer != NULL) {
        iterator->pointer = (char *)(iterator->pointer) - 1 * iterator->element_size;
    }
}

/* Similar to `operator==(Iterator second)' in C++ */
bool iterator_equals(Iterator* first, Iterator* second) {

    /* Check if inputs are legal */
    if(first == NULL || second == NULL || first->pointer == NULL || second->pointer == NULL || first->element_size == 0 || second->element_size == 0) {
        return false;
    } else {
        return (first->pointer == second->pointer);       /* Compare by pointers */
    }
}

/* Sort the vector in nondecreasing according to comparision function less.
     You can implement any sorting algorithm you want such as bubble sort
	 or quick sort.	 There is no strict time or space limit. */
void vector_sort(Vector *vector, vector_less_func *less) {

    /* Variables for sort indices */
    size_t i, j;

    /* Check if inputs are legal */
    if(vector == NULL || less == NULL || vector->element_size == 0 || vector->data == VECTOR_UNINITIALIZED) {
        return;
    }

    /* Bubble sort */
    for (i = 0; i < vector->size - 1; ++i) {
        for (j = 0; j < vector->size - i - 1; ++j) {        /* Last i elements have already been sorted */
            if(less(vector_get(vector, j + 1), vector_get(vector, j)) == true) {
                void *test;
                void *tmp = malloc(1 * vector->element_size);       /* Store one of the values */
                if(tmp == NULL) {
                    return;
                }

                /* Copy and check */
                test = memmove(tmp, vector_get(vector, j + 1), vector->element_size);
                if(test == NULL) {
                    return;
                }

                /* Exchange the values */
                vector_assign(vector, j + 1, vector_get(vector, j));
                vector_assign(vector, j, tmp);
                free(tmp);
            }
        }
    }
}
