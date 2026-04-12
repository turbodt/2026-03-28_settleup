#include <spm.h>
#include <test-shared.h>


SPM_LIST_HEADER(size_t, Size, size);


static int test_001(void) {
    int err;
    SpmListSize numbers = {0};
    spm_list_size_init(&numbers, 10);
    ASSERT_EQ(0, spm_list_size_get_count(&numbers));

    err = spm_list_size_insert_at(&numbers, 0, 10);
    ASSERT_FALSE(err);
    ASSERT_EQ(10, spm_list_size_get_count(&numbers));

    for (size_t i = 0; i < spm_list_size_get_count(&numbers); i++) {
        spm_list_size_set(&numbers, i, i);
    }

    for (size_t i = 0; i < spm_list_size_get_count(&numbers); i++) {
        ASSERT_EQ(spm_list_size_get(&numbers, i), (size_t) i);
    }

    err = spm_list_size_insert_at(&numbers, 3, 18);
    ASSERT_FALSE(err);
    ASSERT_EQ(28, spm_list_size_get_count(&numbers));
    for (size_t i = 0; i < 3; i++) {
        ASSERT_EQ(spm_list_size_get(&numbers, i), (size_t) i);
    }
    for (size_t i = 21; i < 28; i++) {
        ASSERT_EQ(spm_list_size_get(&numbers, i), (size_t) i-18);
    }


    err = spm_list_size_remove_at(&numbers, 5, 7);
    ASSERT_FALSE(err);
    ASSERT_EQ(21, spm_list_size_get_count(&numbers));
    for (size_t i = 0; i < 3; i++) {
        ASSERT_EQ(spm_list_size_get(&numbers, i), (size_t) i);
    }
    for (size_t i = 14; i < 21; i++) {
        ASSERT_EQ(spm_list_size_get(&numbers, i), (size_t) i-11);
    }

    spm_list_size_clear(&numbers);
    return 0;
}


static int test_002(void) {
    int err;
    SpmListSize numbers = {0};
    spm_list_size_init(&numbers, 2);

    for (int i = 0; i < 100; i++) {
        err = spm_list_size_insert_at(&numbers, spm_list_size_get_count(&numbers), 1);
        ASSERT_FALSE(err);
        spm_list_size_set(&numbers, i, i);
    }
    ASSERT_EQ(100, spm_list_size_get_count(&numbers));

    spm_list_size_remove_at(&numbers, 0, 10);
    ASSERT_EQ(90, spm_list_size_get_count(&numbers));
    ASSERT_EQ(10, spm_list_size_get(&numbers, 0));

    spm_list_size_remove_at(&numbers, spm_list_size_get_count(&numbers) - 10, 10);
    ASSERT_EQ(80, spm_list_size_get_count(&numbers));

    spm_list_size_clear(&numbers);
    return 0;
}


static Test tests[] = {
    {.name="List size_t 001", .call=&test_001},
    {.name="List size_t 002", .call=&test_002},
    {.name="", .call=NULL},
};


static TestSuite test_suite = (TestSuite) {
    .name = "List: Basic operations with size_t",
    .tests = tests,
};


TestSuite const * test_suite_list_get_001(void) {
    return &test_suite;
};
