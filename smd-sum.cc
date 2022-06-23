#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "rom-t.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <filename>\n", argv[0]);
    return 1;
  }

  for (auto i = 1; i < argc; i++) {
    auto rom = rom_t(argv[i]);
    if (!rom.open()) {
      continue;
    }

    if (!rom.get_size()) {
      continue;
    }

    if (!rom.get_rom_end()) {
      continue;
    }

    auto sum = rom.get_sum();
    if (!sum) {
      continue;
    }

    printf("%s: 0x%04x\n", argv[i], *sum);
  }

  return 0;
}
