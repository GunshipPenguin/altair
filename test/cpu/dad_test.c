#include "attounit.h"
#include "cpu.h"
#include "memory.h"
#include "cpu_test_helpers.h"

TEST_SUITE(instruction_dad);
BEFORE_EACH() {
  setup_cpu_test_env();
}
AFTER_EACH() {}

// Individual opcode tests
TEST_CASE(dad_b) {
  write_byte(0, 0x09); // DAD B
  cpu->B = 0x00; cpu->C = 0x05; // BC contains 5
  cpu->H = 0xFF; cpu->L = 0xFD; // HL contains -3

  step_cpu();

  ASSERT_EQUAL(cpu->B, 0x00);
  ASSERT_EQUAL(cpu->C, 0x02);
  ASSERT_EQUAL(cpu->PC, 1);
}

TEST_CASE(dad_d) {
  write_byte(0, 0x19); // DAD D
  cpu->D = 0x00; cpu->E = 0x05; // DE contains 5
  cpu->H = 0xFF; cpu->L = 0xFD; // HL contains -3

  step_cpu();

  ASSERT_EQUAL(cpu->D, 0x00);
  ASSERT_EQUAL(cpu->E, 0x02);
  ASSERT_EQUAL(cpu->PC, 1);
}

TEST_CASE(dad_h) {
  write_byte(0, 0x29); // DAD H
  cpu->H = 0xFF; cpu->L = 0xFD; // HL contains -3

  step_cpu();

  ASSERT_EQUAL(cpu->H, 0xFF);
  ASSERT_EQUAL(cpu->L, 0xFA);
  ASSERT_EQUAL(cpu->PC, 1);
}

TEST_CASE(dad_sp) {
  write_byte(0, 0x39); // DAD SP
  cpu->SP = 0x05; // SP contains 5
  cpu->H = 0xFF; cpu->L = 0xFD; // HL contains -3

  step_cpu();

  ASSERT_EQUAL(cpu->SP, 0x0002);
  ASSERT_EQUAL(cpu->PC, 1);
}

// Bit flag tests
TEST_CASE(dad_sets_c_flag) {
  write_byte(0, 0x19); // DAD D
  cpu->D = 0x00; cpu->E = 0x05; // DE contains 5
  cpu->H = 0xFF; cpu->L = 0xFD; // HL contains -3
  set_flag(FLAG_C, 0);

  step_cpu();

  ASSERT_TRUE(get_flag(FLAG_C));
  ASSERT_EQUAL(cpu->PC, 1);
}

TEST_CASE(dad_resets_c_flag) {
  write_byte(0, 0x19); // DAD D
  cpu->D = 0x00; cpu->E = 0x05; // DE contains 5
  cpu->H = 0x00; cpu->L = 0x01; // HL contains 1
  set_flag(FLAG_C, 1);

  step_cpu();

  ASSERT_FALSE(get_flag(FLAG_C));
  ASSERT_EQUAL(cpu->PC, 1);
}