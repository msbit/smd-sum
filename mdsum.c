#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int result = 0;
  if (argc < 2) {
    fprintf(stderr, "usage: %s <filename>\n", argv[0]);
    result = 1;
    goto defer_none;
  }

  FILE *rom = fopen(argv[1], "r");
  if (rom == NULL) {
    perror("fopen");
    result = 1;
    goto defer_none;
  }

  if (fseek(rom, 0, SEEK_END) != 0) {
    perror("fseek 1");
    result = 1;
    goto defer_close_rom;
  }

  int size = ftell(rom);
  if (size == -1) {
    perror("ftell");
    result = 1;
    goto defer_close_rom;
  }

  if (size < 0x1a8) {
    fprintf(stderr, "error: size 0x%x is less than 0x1a8\n", size);
    result = 1;
    goto defer_close_rom;
  }

  if (fseek(rom, 0x1a4, SEEK_SET) != 0) {
    perror("fseek 2");
    result = 1;
    goto defer_close_rom;
  }

  int32_t rom_end;
  if (fread(&rom_end, 1, 4, rom) < 4) {
    perror("fread");
    result = 1;
    goto defer_close_rom;
  }

  rom_end = ntohl(rom_end) + 1;
  if (rom_end > size) {
    fprintf(stderr, "error: rom end 0x%x is beyond size 0x%x\n", rom_end, size);
    result = 1;
    goto defer_close_rom;
  }

  uint16_t sum = 0;
  for (int32_t i = 0x200; i < rom_end; i += 2) {
    uint16_t word;
    fseek(rom, i, SEEK_SET);
    fread(&word, 1, 2, rom);
    word = ntohs(word);
    sum += word;
  }

  printf("0x%04x\n", sum);

defer_close_rom:
  fclose(rom);

defer_none:
  return result;
}
