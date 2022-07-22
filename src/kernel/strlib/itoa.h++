#ifndef ITOA_HPP
#define ITOA_HPP

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

namespace unstd {
  char* itoa(int val, int base);
}
#endif