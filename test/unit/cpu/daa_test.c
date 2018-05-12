#include "unit/attounit.h"
#include "cpu.h"
#include "memory.h"
#include "cpu_test_helpers.h"

TEST_SUITE(instruction_daa)
BEFORE_EACH() {
  setup_cpu_test_env();
}
AFTER_EACH() {}

// Translate a binary number into its 2 digit BCD equivalent
int bcd_encode(int val) {
  int lo_digit = (val % 10) & 0x0F;
  int hi_digit = (val / 10) & 0x0F;

  return (hi_digit << 4) | (lo_digit);
}

int bcd_decode(int val) {
  int lo_digit = val & 0x0F;
  int hi_digit = (val >> 4) & 0x0F;

  return hi_digit*10 + lo_digit;
}

TEST_CASE(daa_1_plus_1) {
  write_byte(0, 0x80); // ADD B
  write_byte(1, 0x27); // DAA
  cpu->A = bcd_encode(1);
  cpu->B = bcd_encode(1);

  step_cpu();
  step_cpu();

  ASSERT_EQUAL(bcd_decode(cpu->A), 2);
  ASSERT_FALSE(get_flag(FLAG_A));
  ASSERT_FALSE(get_flag(FLAG_C));
  ASSERT_EQUAL(cpu->PC, 2);
}

TEST_CASE(daa_9_plus_1) {
  write_byte(0, 0x80); // ADD B
  write_byte(1, 0x27); // DAA
  cpu->A = bcd_encode(9);
  cpu->B = bcd_encode(1);

  step_cpu();
  step_cpu();

  ASSERT_EQUAL(bcd_decode(cpu->A), 10);
  ASSERT_TRUE(get_flag(FLAG_A));
  ASSERT_FALSE(get_flag(FLAG_C));
  ASSERT_EQUAL(cpu->PC, 2);
}

TEST_CASE(daa_9_plus_10) {
  write_byte(0, 0x80); // ADD B
  write_byte(1, 0x27); // DAA
  cpu->A = bcd_encode(9);
  cpu->B = bcd_encode(10);

  step_cpu();
  step_cpu();

  ASSERT_EQUAL(bcd_decode(cpu->A), 19);
  ASSERT_FALSE(get_flag(FLAG_A));
  ASSERT_FALSE(get_flag(FLAG_C));
  ASSERT_EQUAL(cpu->PC, 2);
}

TEST_CASE(daa_22_plus_39) {
  write_byte(0, 0x80); // ADD B
  write_byte(1, 0x27); // DAA
  cpu->A = bcd_encode(22);
  cpu->B = bcd_encode(39);

  step_cpu();
  step_cpu();

  ASSERT_EQUAL(bcd_decode(cpu->A), 61);
  ASSERT_TRUE(get_flag(FLAG_A));
  ASSERT_FALSE(get_flag(FLAG_C));
  ASSERT_EQUAL(cpu->PC, 2);
}

TEST_CASE(daa_95_plus_20) {
  write_byte(0, 0x80); // ADD B
  write_byte(1, 0x27); // DAA
  cpu->A = bcd_encode(95);
  cpu->B = bcd_encode(20);

  step_cpu();
  step_cpu();

  ASSERT_EQUAL(bcd_decode(cpu->A), 15);
  ASSERT_FALSE(get_flag(FLAG_A));
  ASSERT_TRUE(get_flag(FLAG_C));
  ASSERT_EQUAL(cpu->PC, 2);
}

/*
 * Steps in decimal subtraction on the 8080
 *
 * 1) Set the carry bit to 1 indicating no borrow
 * 2) Load the accumulator with 0x99 representing 99 in decimal
 * 3) Add 0 to the accumulator with carry producing either 0x99 or 0x9A and
 *    resetting the carry bit
 * 4) Subtract the subtrahend digits from the accumulator producing either 99's
 *    or 100's complement
 * 5) Add the minuend digits to the accumulator
 * 6) Use DAA to make sure the result in the accumulator is in decimal format
 *    and to indicate a borrow in the carry bit if one occurred
 * 7) If there are more digits to subtract, got to step 2, otherwise, stop
 */
TEST_CASE(daa_20_minus_3) {
  // 1
  set_flag(FLAG_C, 1);

  // 2
  cpu->A = 0x99;
  cpu->B = 0;

  // 3
  write_byte(0, 0x88); // ADC B
  step_cpu();

  ASSERT_EQUAL(cpu->A, 0x9A);
  ASSERT_FALSE(get_flag(FLAG_C));

  // 4
  cpu->B = 3;
  write_byte(1, 0x90); // SUB B

  step_cpu();

  ASSERT_EQUAL(bcd_decode(cpu->A), 97);

  // 5
  cpu->B = bcd_encode(20);
  write_byte(2, 0x80); // ADD B
  step_cpu();

  // 6
  write_byte(3, 0x27); // DAA
  step_cpu();

  ASSERT_EQUAL(bcd_decode(cpu->A), 17);
  ASSERT_TRUE(get_flag(FLAG_C));
}