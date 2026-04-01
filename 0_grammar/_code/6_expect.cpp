#include <cstdio>
#include <stdio.h>

#ifdef __clang__
#define __LIKELY(x) __builtin_expect((x), (true))
#elif defined(__GNUC__)
// GCC
#define __LIKELY(x) __builtin_expect((x), (true))
#elif defined(_MSC_VER)
// Microsoft Visual C++
#define __LIKELY(x) __assume(x)
#else
// Other compilers
#endif

int main() {
  int x = 10;
  int y = 20;

  if (__LIKELY(x > y)) {
    printf("x is greater than y\n");
  } else {
    printf("x is not greater than y\n");
  }

  return 0;
}