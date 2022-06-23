#ifndef __CSTDIO_OPT__
#define __CSTDIO_OPT__

#include <optional>

namespace msbit {
std::optional<FILE *> fopen(const char *, const char *);
std::optional<unsigned int> fseek(FILE *, long, int);
std::optional<unsigned long> ftell(FILE *);
} // namespace msbit

#endif
