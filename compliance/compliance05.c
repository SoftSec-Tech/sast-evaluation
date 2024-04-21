typedef float float32_t;
typedef double float64_t;

void testf64a(float64_t f64a) {}

void test(float32_t f32a) {
  // 违反 MISRA C 2012 Rule 10.8
  testf64a((float64_t)(f32a + f32a));
}
