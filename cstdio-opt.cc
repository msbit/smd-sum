#include <cstdio>

#include <optional>

std::optional<FILE *> fopen_opt(const char *path, const char *mode) {
  auto result = fopen(path, mode);
  if (result == NULL) {
    return {};
  }

  return result;
}

std::optional<unsigned int> fseek_opt(FILE *stream, long offset, int whence) {
  auto result = fseek(stream, offset, whence);
  if (result == -1) {
    return {};
  }

  return result;
}

std::optional<unsigned long> ftell_opt(FILE *stream) {
  auto result = ftell(stream);
  if (result == -1) {
    return {};
  }

  return result;
}
