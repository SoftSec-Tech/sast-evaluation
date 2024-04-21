#define bool_t int
#define true 1
#define false 0

typedef unsigned short uint16_t;

void f(bool_t b, uint16_t *p) {
  if (b) {
    *p = 3U; // 仅在 b 为 true 时候初始化
  }
}

void testBad(void) {
  uint16_t u;
  f(false, &u); // f() 在第一个参数为 false 时候未初始化 u

  // 违反 MISRA C-2012 Rule 9.1
  if (u == 3U) {
  }
}

void testGood(void) {
  uint16_t u;
  f(true, &u); // f() 在第一个参数为 true 时候已经初始化 u

  // 不违反 MISRA C-2012 Rule 9.1
  if (u == 3U) {
  }
}
