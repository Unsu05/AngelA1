#include <check.h>
#include <stdlib.h>
#include "room.h"

// Test that we can create and destroy a room without crashing
START_TEST(test_room_lifecycle)
{
    // 1. Create
    Room *r = room_create(1, "Dungeon", 10, 10);
    ck_assert_ptr_nonnull(r);
    ck_assert_str_eq(r->name, "Dungeon");
    ck_assert_int_eq(room_get_width(r), 10);

    // 2. Destroy
    room_destroy(r);
    // If this crashes, valgrind will yell at us
}
END_TEST

// Test that treasures are copied correctly (Deep Copy check)
START_TEST(test_room_treasure_copy)
{
    Room *r = room_create(1, "Vault", 5, 5);
    
    // Create a stack-allocated treasure (simulating transient data)
    Treasure t = {0};
    t.id = 99;
    t.x = 2; 
    t.y = 2;
    t.name = "Gold Bar"; // String literal, but we want to ensure Room copies it

    // Place it
    Status s = room_place_treasure(r, &t);
    ck_assert_int_eq(s, OK);

    // Verify it's there
    int found_id = room_get_treasure_at(r, 2, 2);
    ck_assert_int_eq(found_id, 99);

    // Verify Deep Copy: The room's treasure name should match
    // but the POINTER address should be different (it should be a copy)
    // Note: This accesses internal fields, which is allowed in white-box testing
    ck_assert_str_eq(r->treasures[0].name, "Gold Bar");
    ck_assert_ptr_ne(r->treasures[0].name, t.name); 

    room_destroy(r);
}
END_TEST

// Boilerplate to run the tests
Suite *room_suite(void)
{
    Suite *s = suite_create("Room");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_room_lifecycle);
    tcase_add_test(tc_core, test_room_treasure_copy);

    suite_add_tcase(s, tc_core);
    return s;
}