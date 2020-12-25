
/****** Copyright @ Mercurial ******/

#include <stdio.h>
#include <memory.h>
#include "vector.h"

#define CHECKVALUE(test_val, correct_val, test_point) { \
    if(test_val == correct_val) { \
        printf("Passed! Test point %d.\n", test_point); \
        passed++; \
    } else { \
        printf("Error! Test point %d. Pay attention since you didn't pass this.\n", test_point); \
        wrong++; \
    } \
}

#define TESTBREAK printf("-------------------------\n");

bool char_less(const void *a, const void *b){
    return *(char*)a < *(char*)b;
}

bool double_less(const void *a, const void *b){
    return *(double*)a < *(double*)b;
}

typedef struct Struct {
    int id;
    int num;
} Struct;

int main() {

    /* Declare and initialize some vectors*/
    int test_points = 0;
    int passed = 0, wrong = 0;
    Vector char_vec, double_vec, struct_vec, error_vec;
    Vector char_vec_copy, double_vec_copy, error_vec_copy;
    vector_init(&char_vec, 0, sizeof(char));
    vector_init(&double_vec, 1, sizeof(double));

    {
        size_t step = 2;
        char *answer = "a b c i j k l m n o x y z";
        Iterator present, begin, end;

        printf("Tests for char_vec and char_vec_copy operation results and return values:\n");
        TESTBREAK

        /* Ops for char_vec*/
        /* Test point 0 */
        CHECKVALUE(char_vec.capacity, VECTOR_MINIMUM_CAPACITY, test_points++)

        /* Test points 1-10 */
        char char_val = 'a';
        size_t count;
        for (count = 0; count < 5; ++count) {
            CHECKVALUE(vector_push_back(&char_vec, &char_val), VECTOR_SUCCESS, test_points++)
            CHECKVALUE(*(char *)vector_back(&char_vec), char_val, test_points++)
            ++char_val;
        }
        TESTBREAK

        /* Test points 11-13 */
        CHECKVALUE(vector_size(&char_vec), 5, test_points++)
        CHECKVALUE(char_vec.capacity, 8, test_points++)
        CHECKVALUE(char_vec.element_size, sizeof(char), test_points++)
        TESTBREAK

        /* Test points 14-23 */
        char_val = 'z';
        for (count = 0; count < 5; ++count) {
            CHECKVALUE(vector_push_front(&char_vec, &char_val), VECTOR_SUCCESS, test_points++)
            CHECKVALUE(*(char *)vector_front(&char_vec), char_val, test_points++)
            --char_val;
        }
        TESTBREAK

        /* Test points 24-26 */
        CHECKVALUE(vector_size(&char_vec), 10, test_points++)
        CHECKVALUE(char_vec.capacity, 16, test_points++)
        CHECKVALUE(char_vec.element_size, sizeof(char), test_points++)
        TESTBREAK

        /* Test points 27-46 */
        char_val = 'i';
        for (count = 0; count < 10; ++count) {
            CHECKVALUE(vector_insert(&char_vec, step, &char_val), VECTOR_SUCCESS, test_points++)
            CHECKVALUE(VECTOR_GET_AS(char, &char_vec, step), char_val, test_points++)
            ++char_val;
            ++step;
        }
        TESTBREAK

        /* Test points 47-49 */
        CHECKVALUE(vector_size(&char_vec), 20, test_points++)
        CHECKVALUE(char_vec.capacity, 32, test_points++)
        CHECKVALUE(char_vec.element_size, sizeof(char), test_points++)
        TESTBREAK

        /* Test points 50-53 */
        for (count = 0; count < 2; ++count) {
            CHECKVALUE(vector_pop_front(&char_vec), VECTOR_SUCCESS, test_points++)
            CHECKVALUE(vector_pop_back(&char_vec), VECTOR_SUCCESS, test_points++)
        }
        TESTBREAK

        /* Test points 54-56 */
        CHECKVALUE(vector_size(&char_vec), 16, test_points++)
        CHECKVALUE(char_vec.capacity, 32, test_points++)
        CHECKVALUE(char_vec.element_size, sizeof(char), test_points++)
        TESTBREAK

        /* Test points 57-59 */
        for (count = 0; count < 3; ++count) {
            CHECKVALUE(vector_erase(&char_vec, 7), VECTOR_SUCCESS, test_points++)
        }
        TESTBREAK

        /* Test points 60-62 */
        CHECKVALUE(vector_size(&char_vec), 13, test_points++)
        CHECKVALUE(char_vec.capacity, 32, test_points++)
        CHECKVALUE(char_vec.element_size, sizeof(char), test_points++)
        TESTBREAK

        /* Test point 63-88 */
        vector_sort(&char_vec, &char_less);
        present = vector_begin(&char_vec);
        end = vector_end(&char_vec);
        answer = "a b c i j k l m n o x y z";
        for (count = 0; count < (strlen(answer) + 1) / 2 && !iterator_equals(&present, &end); ++count, iterator_increment(&present)) {
            CHECKVALUE(VECTOR_GET_AS(char, &char_vec, count), answer[count * 2], test_points++)
            CHECKVALUE(ITERATOR_GET_AS(char, &present), answer[count * 2], test_points++)
        }
        TESTBREAK

        /* Test points 89-91 */
        CHECKVALUE(vector_size(&char_vec), 13, test_points++)
        CHECKVALUE(char_vec.capacity, 32, test_points++)
        CHECKVALUE(char_vec.element_size, sizeof(char), test_points++)
        TESTBREAK

        /* Test points 92-117 */
        vector_copy(&char_vec_copy, &char_vec);
        begin = vector_begin(&char_vec_copy);
        iterator_decrement(&begin);
        present = vector_end(&char_vec_copy);
        iterator_decrement(&present);
        for (count = 0; count < (strlen(answer) + 1) / 2 && !iterator_equals(&present, &begin); ++count, iterator_decrement(&present)) {
            CHECKVALUE(VECTOR_GET_AS(char, &char_vec_copy, count), answer[count * 2], test_points++)
            CHECKVALUE(ITERATOR_GET_AS(char, &present), answer[strlen(answer) - 1 - 2 * count], test_points++)
        }
        TESTBREAK

        /* Test points 118-119 */
        CHECKVALUE(vector_destroy(&char_vec), VECTOR_SUCCESS, test_points++)
        CHECKVALUE(vector_destroy(&char_vec_copy), VECTOR_SUCCESS, test_points++)
        TESTBREAK
    }



    {
        size_t step = 2;
        double answer[13] = {-5.00, -4.00, -3.00, 3.00, 4.00, 5.00, 100.00, 101.00, 102.00, 103.00, 104.00, 105.00, 106.00};
        Iterator present, begin, end;

        printf("Tests for double_vec and double_vec_copy operation results and return values:\n");
        TESTBREAK

        /* Ops for double_vec*/
        /* Test points 120-129 */
        double double_val = -5.0;
        size_t count;
        for (count = 0; count < 5; ++count) {
            CHECKVALUE(vector_push_back(&double_vec, &double_val), VECTOR_SUCCESS, test_points++)
            CHECKVALUE(*(double *)vector_back(&double_vec), double_val, test_points++)
            ++double_val;
        }
        TESTBREAK

        /* Test points 130-132 */
        CHECKVALUE(vector_size(&double_vec), 5, test_points++)
        CHECKVALUE(double_vec.capacity, 8, test_points++)
        CHECKVALUE(double_vec.element_size, sizeof(double), test_points++)
        TESTBREAK

        /* Test points 133-142 */
        double_val = 5.0;
        for (count = 0; count < 5; ++count) {
            CHECKVALUE(vector_push_front(&double_vec, &double_val), VECTOR_SUCCESS, test_points++)
            CHECKVALUE(*(double *)vector_front(&double_vec), double_val, test_points++)
            --double_val;
        }
        TESTBREAK

        /* Test points 143-145 */
        CHECKVALUE(vector_size(&double_vec), 10, test_points++)
        CHECKVALUE(double_vec.capacity, 16, test_points++)
        CHECKVALUE(double_vec.element_size, sizeof(double), test_points++)
        TESTBREAK

        /* Test points 146-165 */
        double_val = 100.0;
        for (count = 0; count < 10; ++count) {
            CHECKVALUE(vector_insert(&double_vec, step, &double_val), VECTOR_SUCCESS, test_points++)
            CHECKVALUE(VECTOR_GET_AS(double, &double_vec, step), double_val, test_points++)
            ++double_val;
            ++step;
        }
        TESTBREAK

        /* Test points 166-168 */
        CHECKVALUE(vector_size(&double_vec), 20, test_points++)
        CHECKVALUE(double_vec.capacity, 32, test_points++)
        CHECKVALUE(double_vec.element_size, sizeof(double), test_points++)
        TESTBREAK

        /* Test points 169-172 */
        for (count = 0; count < 2; ++count) {
            CHECKVALUE(vector_pop_front(&double_vec), VECTOR_SUCCESS, test_points++)
            CHECKVALUE(vector_pop_back(&double_vec), VECTOR_SUCCESS, test_points++)
        }
        TESTBREAK

        /* Test points 173-175 */
        CHECKVALUE(vector_size(&double_vec), 16, test_points++)
        CHECKVALUE(double_vec.capacity, 32, test_points++)
        CHECKVALUE(double_vec.element_size, sizeof(double), test_points++)
        TESTBREAK

        /* Test points 176-178 */
        for (count = 0; count < 3; ++count) {
            CHECKVALUE(vector_erase(&double_vec, 7), VECTOR_SUCCESS, test_points++)
        }
        TESTBREAK

        /* Test points 179-181 */
        CHECKVALUE(vector_size(&double_vec), 13, test_points++)
        CHECKVALUE(double_vec.capacity, 32, test_points++)
        CHECKVALUE(double_vec.element_size, sizeof(double), test_points++)
        TESTBREAK

        /* Test point 182-207 */
        vector_sort(&double_vec, &double_less);
        present = vector_begin(&double_vec);
        end = vector_end(&double_vec);
        for (count = 0; count < sizeof(answer) / sizeof(double) && !iterator_equals(&present, &end); ++count, iterator_increment(&present)) {
            CHECKVALUE(VECTOR_GET_AS(double, &double_vec, count), answer[count], test_points++)
            CHECKVALUE(ITERATOR_GET_AS(double, &present), answer[count], test_points++)
        }
        TESTBREAK

        /* Test points 208-210 */
        CHECKVALUE(vector_size(&double_vec), 13, test_points++)
        CHECKVALUE(double_vec.capacity, 32, test_points++)
        CHECKVALUE(double_vec.element_size, sizeof(double), test_points++)
        TESTBREAK

        /* Test points 211-236 */
        vector_copy(&double_vec_copy, &double_vec);
        begin = vector_begin(&double_vec_copy);
        iterator_decrement(&begin);
        present = vector_end(&double_vec_copy);
        iterator_decrement(&present);
        for (count = 0; count < sizeof(answer) / sizeof(double) && !iterator_equals(&present, &begin); ++count, iterator_decrement(&present)) {
            CHECKVALUE(VECTOR_GET_AS(double, &double_vec_copy, count), answer[count], test_points++)
            CHECKVALUE(ITERATOR_GET_AS(double, &present), answer[sizeof(answer) / sizeof(double) - 1 - count], test_points++)
        }
        TESTBREAK

        /* Test points 237-238 */
        CHECKVALUE(vector_destroy(&double_vec), VECTOR_SUCCESS, test_points++)
        CHECKVALUE(vector_destroy(&double_vec_copy), VECTOR_SUCCESS, test_points++)
        TESTBREAK
    }



    {
        Struct st;
        int count;
        Iterator present, end;

        printf("Tests for struct_vec operation results and return values:\n");
        TESTBREAK

        /* Ops for struct_vec*/
        /* Test points 239-249 */
        CHECKVALUE(vector_init(&struct_vec, 0, sizeof(Struct)), VECTOR_SUCCESS, test_points++)
        for (count = 0;  count < 10; ++count) {
            st.id = count, st.num = st.id + 10;
            CHECKVALUE(vector_push_back(&struct_vec, &st), VECTOR_SUCCESS, test_points++)
        }
        TESTBREAK

        /* Test points 250-252 */
        CHECKVALUE(vector_size(&struct_vec), 10, test_points++)
        CHECKVALUE(struct_vec.capacity, 16, test_points++)
        CHECKVALUE(struct_vec.element_size, sizeof(Struct), test_points++)
        TESTBREAK

        /* Test points 253-256 */
        CHECKVALUE(vector_resize(&struct_vec, 100), VECTOR_SUCCESS, test_points++)
        CHECKVALUE(vector_size(&struct_vec), 100, test_points++)
        CHECKVALUE(struct_vec.capacity, 128, test_points++)
        CHECKVALUE(struct_vec.element_size, sizeof(Struct), test_points++)
        TESTBREAK

        /* Test points 257-261 */
        CHECKVALUE(vector_resize(&struct_vec, 0), VECTOR_SUCCESS, test_points++)
        CHECKVALUE(vector_is_empty(&struct_vec), true, test_points++)
        CHECKVALUE(vector_size(&struct_vec), 0, test_points++)
        CHECKVALUE(struct_vec.capacity, 128, test_points++)
        CHECKVALUE(struct_vec.element_size, sizeof(Struct), test_points++)
        TESTBREAK

        /* Test points 262-282 */
        CHECKVALUE(vector_resize(&struct_vec, 10), VECTOR_SUCCESS, test_points++)
        count = 0;
        present = vector_begin(&struct_vec);
        end = vector_end(&struct_vec);
        while(!iterator_equals(&present, &end) && count < 10) {
            Struct* str = iterator_get(&present);
            CHECKVALUE(str->id, count, test_points++)
            CHECKVALUE(str->num, count + 10, test_points++)
            iterator_increment(&present);
            ++count;
        }
        TESTBREAK

        /* Test point 283 */
        CHECKVALUE(vector_destroy(&struct_vec), VECTOR_SUCCESS, test_points++)
        TESTBREAK
    }



    {
        int test_val = 0;
        Iterator iter_test = {NULL, 0}, iter_get_1, iter_get_2;
        printf("Tests for exception handling:\n");
        TESTBREAK

        /* Ops for struct_vec*/
        /* Test points 284-285 */
        CHECKVALUE(vector_init(NULL, 1, sizeof(int)), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_init(&error_vec, 1, 0), VECTOR_ERROR, test_points++)
        TESTBREAK

        /* Test points 286-302 */
        CHECKVALUE(vector_copy(NULL, &error_vec), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_copy(&error_vec_copy, NULL), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_copy(NULL, NULL), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_destroy(NULL), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_push_back(NULL, &test_val), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_push_front(NULL, &test_val), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_insert(NULL, 0, &test_val), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_insert(NULL, 0, NULL), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_assign(NULL, 0, &test_val), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_assign(NULL, 0, NULL), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_get(NULL, 0), NULL, test_points++)
        CHECKVALUE(vector_pop_back(NULL), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_pop_front(NULL), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_erase(NULL, 0), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_clear(NULL), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_is_empty(NULL), false, test_points++)
        CHECKVALUE(vector_resize(NULL, 10), VECTOR_ERROR, test_points++)
        TESTBREAK

        /* Test point 303-318 */
        iter_get_1 = vector_begin(&error_vec);
        CHECKVALUE(iter_get_1.pointer, iter_test.pointer, test_points++)
        CHECKVALUE(iter_get_1.element_size, iter_test.element_size, test_points++)
        iter_get_1 = vector_end(&error_vec);
        CHECKVALUE(iter_get_1.pointer, iter_test.pointer, test_points++)
        CHECKVALUE(iter_get_1.element_size, iter_test.element_size, test_points++)
        iter_get_1 = vector_iterator(&error_vec, 1000);
        CHECKVALUE(iter_get_1.pointer, iter_test.pointer, test_points++)
        CHECKVALUE(iter_get_1.element_size, iter_test.element_size, test_points++)
        iter_get_1 = vector_begin(NULL);
        CHECKVALUE(iter_get_1.pointer, iter_test.pointer, test_points++)
        CHECKVALUE(iter_get_1.element_size, iter_test.element_size, test_points++)
        iter_get_1 = vector_end(NULL);
        CHECKVALUE(iter_get_1.pointer, iter_test.pointer, test_points++)
        CHECKVALUE(iter_get_1.element_size, iter_test.element_size, test_points++)
        iter_get_1 = vector_iterator(NULL, 1000);
        iterator_increment(&iter_get_1);      /* Should not crash */
        iterator_increment(NULL);
        iterator_decrement(&iter_get_1);
        iterator_decrement(NULL);
        CHECKVALUE(iter_get_1.pointer, iter_test.pointer, test_points++)
        CHECKVALUE(iter_get_1.element_size, iter_test.element_size, test_points++)
        CHECKVALUE(iterator_get(NULL), NULL, test_points++)
        CHECKVALUE(iterator_equals(NULL, &iter_get_1), false, test_points++)
        CHECKVALUE(iterator_equals(&iter_get_1, NULL), false, test_points++)
        CHECKVALUE(iterator_equals(&iter_get_1, &iter_get_2), false, test_points++)
        TESTBREAK

        /* Should not crash */
        vector_sort(NULL, NULL);
        vector_sort(NULL, char_less);
        vector_sort(&error_vec, NULL);

        /* Test points 319-330 */
        CHECKVALUE(vector_init(&error_vec, 1, sizeof(int)), VECTOR_SUCCESS, test_points++)
        CHECKVALUE(vector_copy(NULL, &error_vec), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_push_back(&error_vec, NULL), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_push_front(&error_vec, NULL), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_insert(&error_vec, 0, NULL), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_insert(&error_vec, 1000, &test_val), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_assign(&error_vec, 0, NULL), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_assign(&error_vec, 1000, &test_val), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_pop_back(&error_vec), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_pop_front(&error_vec), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_erase(&error_vec, 1000), VECTOR_ERROR, test_points++)
        CHECKVALUE(vector_get(&error_vec, 1000), NULL, test_points++)
        TESTBREAK

        /* Test points 331-334*/
        iter_get_2 = vector_iterator(&error_vec, 1000);
        CHECKVALUE(iter_get_2.pointer, iter_test.pointer, test_points++)
        CHECKVALUE(iter_get_2.element_size, iter_test.element_size, test_points++)
        iterator_increment(&iter_get_2);      /* Should not crash */
        iterator_increment(NULL);
        iterator_decrement(&iter_get_2);
        iterator_decrement(NULL);
        CHECKVALUE(iter_get_2.pointer, iter_test.pointer, test_points++)
        CHECKVALUE(iter_get_2.element_size, iter_test.element_size, test_points++)
        TESTBREAK

        /* Test point 335 */
        CHECKVALUE(vector_destroy(&error_vec), VECTOR_SUCCESS, test_points++)
        TESTBREAK
    }

    printf("Test finished. %d Passed. %d Failed.\n", passed, wrong);
    printf("You should think about using valgrind to detect memory leaks.\n\n");
    printf("/****** Copyright @ Mercurial ******/\n\n");
    return 0;
}
