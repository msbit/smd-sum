#include <cstdint>
#include <cstdio>
#include <cstdlib>

struct rom_t {
  const char *filename;
  FILE *fp;
  int size;
  int32_t rom_end;
  uint16_t sum;

  rom_t(const char *);
  ~rom_t();

  bool open();
  bool get_size();
  bool get_rom_end();
  bool get_sum();
};

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

  if (!rom.get_sum()) {
    return 1;
  }

  printf("0x%04x\n", rom.sum);

  return 0;
}

rom_t::rom_t(const char *filename):filename(filename) {}

rom_t::~rom_t() {
  if (fp != NULL) {
    fclose(fp);
  }
}

bool rom_t::open() {
  if ((fp = fopen(filename, "r")) == NULL) {
    perror(NULL);
    return false;
  }

  return true;
}

bool rom_t::get_size() {
  if (fseek(fp, 0, SEEK_END) != 0) {
    perror(NULL);
    return false;
  }

  if ((size = ftell(fp)) == -1) {
    perror(NULL);
    return false;
  }

  return true;
}

bool rom_t::get_rom_end() {
  if (size < 0x1a8) {
    fprintf(stderr, "error: size 0x%x is less than 0x1a8\n", size);
    return false;
  }

  if (fseek(fp, 0x1a4, SEEK_SET) != 0) {
    perror(NULL);
    return false;
  }

  if (fread(&rom_end, 1, 4, fp) < 4) {
    perror(NULL);
    return false;
  }

  rom_end = ntohl(rom_end) + 1;
  if (rom_end > size) {
    fprintf(stderr, "error: rom end 0x%x is beyond size 0x%x\n", rom_end, size);
    return false;
  }

  return true;
}

bool rom_t::get_sum() {
  sum = 0;
  for (auto i = 0x200; i < rom_end; i += 2) {
    if (fseek(fp, i, SEEK_SET) != 0) {
      perror(NULL);
      return false;
    }

    uint16_t word;
    if (fread(&word, 1, 2, fp) < 2) {
      perror(NULL);
      return false;
    }

    word = ntohs(word);
    sum += word;
  }

  return true;
}
