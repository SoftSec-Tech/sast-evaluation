typedef _Bool bool_t;

bool_t bla;
char cha;

void test3() {
  // 字符类型用作布尔值
  cha && bla; // 违反 MISRA_C2012:Rule 10.1
}
