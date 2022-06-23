#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int (*defer_t)(FILE*);

bool get_size(FILE *, int *, defer_t);
bool get_rom_end(FILE *, int, int32_t *, defer_t);
bool get_sum(FILE *, int32_t, uint16_t *, defer_t);

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <filename>\n", argv[0]);
    return 1;
  }

  FILE *rom = fopen(argv[1], "r");
  if (rom == NULL) {
    perror("fopen");
    return 1;
  }

  int size;
  if (!get_size(rom, &size, fclose)) {
    return 1;
  }

  int32_t rom_end;
  if (!get_rom_end(rom, size, &rom_end, fclose)) {
    return 1;
  }

  uint16_t sum;
  if (!get_sum(rom, rom_end, &sum, fclose)) {
    return 1;
  }

  printf("0x%04x\n", sum);

  return 0;
}

bool get_size(FILE *fp, int *result, defer_t on_error){
  if (fseek(fp, 0, SEEK_END) != 0) {
    perror("fseek 1");
    if (on_error) {
      on_error(fp);
    }
    return false;
  }

  *result = ftell(fp);
  if (*result == -1) {
    perror("ftell");
    if (on_error) {
      on_error(fp);
    }
    return false;
  }

  return true;
}

bool get_rom_end(FILE *fp, int size, int32_t *result, defer_t on_error) {
  if (size < 0x1a8) {
    fprintf(stderr, "error: size 0x%x is less than 0x1a8\n", size);
    if (on_error) {
      on_error(fp);
    }
    return false;
  }

  if (fseek(fp, 0x1a4, SEEK_SET) != 0) {
    perror("fseek 2");
    if (on_error) {
      on_error(fp);
    }
    return false;
  }

  if (fread(result, 1, 4, fp) < 4) {
    perror("fread");
    if (on_error) {
      on_error(fp);
    }
    return false;
  }

  *result = ntohl(*result) + 1;
  if (*result > size) {
    fprintf(stderr, "error: rom end 0x%x is beyond size 0x%x\n", *result, size);
    if (on_error) {
      on_error(fp);
    }
    return false;
  }

  return true;
}

bool get_sum(FILE *fp, int32_t rom_end, uint16_t *result, defer_t on_error) {
  *result = 0;
  for (int32_t i = 0x200; i < rom_end; i += 2) {
    uint16_t word;
    if (fseek(fp, i, SEEK_SET) != 0) {
      perror("fseek");
      if (on_error) {
        on_error(fp);
      }
      return false;
    }

    if (fread(&word, 1, 2, fp) < 2) {
      perror("fread");
      if (on_error) {
        on_error(fp);
      }
      return false;
    }

    word = ntohs(word);
    *result += word;
  }

  return true;
}
