template <typename T> class A {
public:
  A(T i) : data(i) {}
  T data;
};

template <typename T> A<T> *manage_res(A<T> &a, T val) {
  A<T> *pA1 = &a;
  A<T> *pA2 = pA1;
  if (val != 0) {
    // val != 0 时，入参a最终通过指针类型返回值返回，此处没有释放
    pA2->data = val;
    return pA2;
  }
  // val == 0 时，此处释放 pA2 相当于释放了引入入参a
  delete pA2;
  return nullptr;
}

// 自定义内存分配函数
template <typename T> auto custom_new(T val) {
  return new A<T>(val); // 实则调用operator new分配
}

void testNoLeak(int val) {
  auto pa = custom_new(val);            // 分配内存
  int flag = sizeof(int) - sizeof(val); // flag == 0
  auto r = manage_res(*pa, flag); // flag == 0 时 manage_res()对a释放了内存
} // 无内存泄漏 // 无缺陷

void testLeak(int val) {
  auto pa = custom_new(val);      // 分配内存
  int flag = sizeof(int);         // flag != 0
  auto r = manage_res(*pa, flag); // flag != 0 时 manage_res()未释放a的内存
} // pa出现内存泄漏 // 缺陷
