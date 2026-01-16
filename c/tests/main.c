#include <check.h>
#include <stdlib.h>

Suite *name_of_suite_here(void);
//more suites

int main(void)
{
    Suite *suites[] = {
        name_of_suite_here(),
        //more suites
        NULL
    };

    SRunner *runner = srunner_create(suites[0]);
    for (int i = 1; suites[i] != NULL; ++i) {
        srunner_add_suite(runner, suites[i]);
    }

    srunner_run_all(runner, CK_NORMAL);
    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
