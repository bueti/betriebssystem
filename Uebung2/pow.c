#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

// my_power of function
int my_pow(int n, int m);

// cUnit test stuff
void test_my_pow();
int init_suite1();
int clean_suite1(void);

/* Pointer to the file used by the tests. */
static FILE* temp_file = NULL;

/*
 * my_power of function, easy to test :)
 */
int my_pow(int n, int m) {
    int result = 0;

    for (int i = 0; i<m; i++) {
        result = result + n*n;
    }

    return result;
}

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1() {
  if (NULL == (temp_file = fopen("temp.txt", "w+"))) {
    return -1;
  } else {
    return 0;
  }
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void) {
  if (0 != fclose(temp_file)) {
    return -1;
  } else {
    temp_file = NULL;
    return 0;
  }
}

/**
 * Test Function to test my_power()
 */
void test_my_pow() {
    CU_ASSERT(my_pow(2,2) == 4);
    CU_ASSERT(my_pow(2,4) == 16);
    CU_ASSERT(my_pow(4,2) == 16);
}
/*
 * Main function, calls the necessary cUnit functions.
 */
int main() {
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry()) {
    return CU_get_error();
    }

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
    if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
    }

    /* add the tests to the suite */
    if (NULL == CU_add_test(pSuite, "my test", test_my_pow)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
