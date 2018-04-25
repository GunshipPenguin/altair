#include "attounit.h"
#include "cpu.h"
#include "memory.h"

TEST_SUITE(instruction_rrc);
BEFORE_EACH() {
  if (cpu == NULL) {
    create_cpu();
  }

  reset_cpu();
  cpu->PC = 0;

  if (memory == NULL) {
    create_memory(16);
  }

  // Zero out memory
  for (int i=0;i<16;i++) {
    write8(i, 0);
  }
}
AFTER_EACH() {}

TEST_CASE(sta) {
  write8(0, 0x32); // STA
  write8(1, 5); // d8
  cpu->A = 0x01;

  step_cpu();

  ASSERT_EQUAL(read8(5), 0x01);
}