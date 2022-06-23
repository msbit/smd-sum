#ifndef __CSTDIO_OPT__
#define __CSTDIO_OPT__

#include <optional>

std::optional<FILE *> fopen_opt(const char *, const char *);
std::optional<unsigned int> fseek_opt(FILE *, long, int);
std::optional<unsigned long> ftell_opt(FILE *);

#endif
