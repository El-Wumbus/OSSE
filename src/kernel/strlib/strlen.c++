#include "strlen.hpp"

size_t strlen(const char *str) {
  size_t len = 0;
  // Iterating through the string until it reaches the null character.
  while (str[len])
    len++;
  return len;
}
