#ifndef __ROM_T__
#define __ROM_T__

#include <optional>

struct rom_t {
  rom_t(const char *);
  ~rom_t();

  std::optional<FILE *> open();
  std::optional<long> get_size();
  std::optional<uint32_t> get_rom_end();
  std::optional<uint16_t> get_sum();

private:
  const char *filename;
  std::optional<FILE *> fp;
  std::optional<unsigned long> size;
  uint32_t rom_end;
  uint16_t sum;
};

#endif
