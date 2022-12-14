#include "core.h"

suseconds_t ft_min(suseconds_t a, suseconds_t b) {
  return (a < b ? a : b);
}

suseconds_t ft_max(suseconds_t a, suseconds_t b) {
  return (a > b ? a : b);
}
