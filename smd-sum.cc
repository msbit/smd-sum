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

    printf("%s: 0x%04x\n", argv[i], *sum);
  }

  return 0;
}
