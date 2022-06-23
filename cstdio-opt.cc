#include <cstdio>

#include <optional>

namespace msbit {
std::optional<FILE *> fopen(const char *path, const char *mode) {
  auto result = std::fopen(path, mode);
  if (result == NULL) {
    return {};
  }

  return result;
}

std::optional<unsigned int> fseek(FILE *stream, long offset, int whence) {
  auto result = std::fseek(stream, offset, whence);
  if (result == -1) {
    return {};
  }

  return result;
}

std::optional<unsigned long> ftell(FILE *stream) {
  auto result = std::ftell(stream);
  if (result == -1) {
    return {};
  }

  return result;
}
} // namespace msbit
