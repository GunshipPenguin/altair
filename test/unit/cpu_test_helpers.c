#include <stdlib.h>
#include "i8080.h"
#include "memory.h"

/**
 * @brief Setup a small test environment for CPU tests
 *
 * This function sets up a test environment suitable for testing CPU
 * functionality.
 *
 * Specifically, this function sets up a machine with
 * - 128 bytes of memory, all initialized to 0x00
 * - Stack pointer initialized to 0x10
 * - FLAGS register initialized to 0x02
 * - All other registers (including the program counter) initialized to 0x00
 * - Zero cycles
 * - No pending interrupts
 */
struct i8080 *setup_cpu_test_env() {
  struct i8080 *cpu = malloc(sizeof(struct i8080));
  i8080_reset(cpu);
  cpu->SP = 0x10;

  // Create and zero out memory
  cpu->memsize = 128;
  cpu->memory = malloc(sizeof(char) * cpu->memsize);
  for (size_t i=0;i<128;i++) {
    i8080_write_byte(cpu, i, 0);
  }

  return cpu;
}

void teardown_cpu_test_env(struct i8080 *cpu) {
  free(cpu->memory);
  free(cpu);
}