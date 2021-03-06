#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <optional>

#include "cstdio-opt.h"
#include "rom-t.h"

rom_t::rom_t(const char *filename) : filename(filename) {}

rom_t::~rom_t() {
  if (fp) {
    fclose(*fp);
  }
}

std::optional<FILE *> rom_t::open() {
  if (!(fp = msbit::fopen(filename, "r"))) {
    perror(filename);
  }

  return fp;
}

std::optional<long> rom_t::get_size() {
  if (!msbit::fseek(*fp, 0, SEEK_END)) {
    perror(filename);
    return {};
  }

  if (!(size = msbit::ftell(*fp))) {
    perror(filename);
  }

  return size;
}

std::optional<uint32_t> rom_t::get_rom_end() {
  if (*size < 0x1a8) {
    fprintf(stderr, "%s: size 0x%lx is less than 0x1a8\n", filename, *size);
    return {};
  }

  if (!msbit::fseek(*fp, 0x1a4, SEEK_SET)) {
    perror(filename);
    return {};
  }

  if (fread(&rom_end, 1, 4, *fp) < 4) {
    perror(filename);
    return {};
  }

  rom_end = ntohl(rom_end) + 1;
  if (rom_end > *size) {
    fprintf(stderr, "%s: rom end 0x%x is beyond size 0x%lx\n", filename,
            rom_end, *size);
    return {};
  }

  return rom_end;
}

std::optional<uint16_t> rom_t::get_sum() {
  sum = 0;
  for (uint32_t i = 0x200; i < rom_end; i += 2) {
    if (!msbit::fseek(*fp, i, SEEK_SET)) {
      perror(filename);
      return {};
    }

    uint16_t word;
    if (fread(&word, 1, 2, *fp) < 2) {
      perror(filename);
      return {};
    }

    word = ntohs(word);
    sum += word;
  }

  return sum;
}
