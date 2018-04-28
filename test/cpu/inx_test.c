#include "attounit.h"
#include "cpu.h"
#include "memory.h"
#include "cpu_test_helpers.h"

TEST_SUITE(instruction_inx);
BEFORE_EACH() {
  setup_cpu_test_env();
}
AFTER_EACH() {}

TEST_CASE(inx_b) {
  write_byte(0, 0x03); // INX B
  cpu->B = 0x00; cpu->C = 0xFF;

  step_cpu();

  ASSERT_EQUAL(cpu->B, 0x01);
  ASSERT_EQUAL(cpu->C, 0x00);
  ASSERT_EQUAL(cpu->PC, 1);
}

TEST_CASE(inx_d) {
  write_byte(0, 0x13); // INX D
  cpu->D = 0x00; cpu->E = 0xFF;

  step_cpu();

  ASSERT_EQUAL(cpu->D, 0x01);
  ASSERT_EQUAL(cpu->E, 0x00);
  ASSERT_EQUAL(cpu->PC, 1);
}

TEST_CASE(inx_h) {
  write_byte(0, 0x23); // INX H
  cpu->H = 0x00; cpu->L = 0xFF;

  step_cpu();

  ASSERT_EQUAL(cpu->H, 0x01);
  ASSERT_EQUAL(cpu->L, 0x00);
  ASSERT_EQUAL(cpu->PC, 1);
}

TEST_CASE(inx_sp) {
  write_byte(0, 0x33); // INX SP
  cpu->SP = 0x00;

  step_cpu();

  ASSERT_EQUAL(cpu->SP, 0x01);
  ASSERT_EQUAL(cpu->PC, 1);
}