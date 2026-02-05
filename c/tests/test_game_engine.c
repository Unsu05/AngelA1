#include <check.h>
#include <stdlib.h>
#include "game_engine.h"

START_TEST(test_game_engine_stub_currently_fails)
{
    GameEngine *eng = NULL;
    Status s = game_engine_create("../assets/starter.ini", &eng);

    ck_assert_msg(s == OK, "Stub phase: expected this to fail until game engine is implemented.");
}
END_TEST

Suite *game_engine_suite(void)
{
    Suite *s = suite_create("GameEngine");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_game_engine_stub_currently_fails);

    suite_add_tcase(s, tc_core);
    return s;
}
