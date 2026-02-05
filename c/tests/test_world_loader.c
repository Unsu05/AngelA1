#include <check.h>
#include "world_loader.h"

START_TEST(test_loader_stub_currently_fails)
{
    Graph *g = NULL;
    Room *first = NULL;
    int count = 0;
    Charset cs = {0};

    Status s = loader_load_world("../assets/starter.ini", &g, &first, &count, &cs);

    ck_assert_msg(s == OK, "Stub phase: expected this to fail until world loader is implemented.");
}
END_TEST

Suite *world_loader_suite(void)
{
    Suite *s = suite_create("WorldLoader");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_loader_stub_currently_fails);

    suite_add_tcase(s, tc_core);
    return s;
}
