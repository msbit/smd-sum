#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "rom-t.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <filename>\n", argv[0]);
    return 1;
  }

  auto rom = rom_t(argv[1]);
  if (!rom.open()) {
    return 1;
  }

  if (!rom.get_size()) {
    return 1;
  }

  if (!rom.get_rom_end()) {
    return 1;
  }

  auto sum = rom.get_sum();

  if (!sum) {
    return 1;
  }

  printf("0x%04x\n", *sum);

  return 0;
}
