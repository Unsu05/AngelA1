#include <check.h>
#include <stdlib.h>
#include "player.h"

START_TEST(test_player_create_and_getters)
{
    Player *p = NULL;
    ck_assert_int_eq(player_create(7, 2, 3, &p), OK);
    ck_assert_ptr_nonnull(p);
    ck_assert_int_eq(player_get_room(p), 7);

    int x = 0;
    int y = 0;
    ck_assert_int_eq(player_get_position(p, &x, &y), OK);
    ck_assert_int_eq(x, 2);
    ck_assert_int_eq(y, 3);

    player_destroy(p);
}
END_TEST

START_TEST(test_player_null_safety)
{
    int x = 0;
    int y = 0;
    ck_assert_int_eq(player_create(0, 0, 0, NULL), INVALID_ARGUMENT);
    ck_assert_int_eq(player_get_room(NULL), -1);
    ck_assert_int_eq(player_get_position(NULL, &x, &y), INVALID_ARGUMENT);
    ck_assert_int_eq(player_set_position(NULL, 1, 1), INVALID_ARGUMENT);
    ck_assert_int_eq(player_move_to_room(NULL, 1), INVALID_ARGUMENT);
    ck_assert_int_eq(player_reset_to_start(NULL, 1, 1, 1), INVALID_ARGUMENT);
    player_destroy(NULL);
}
END_TEST

Suite *player_suite(void)
{
    Suite *s = suite_create("Player");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_player_create_and_getters);
    tcase_add_test(tc_core, test_player_null_safety);

    suite_add_tcase(s, tc_core);
    return s;
}
