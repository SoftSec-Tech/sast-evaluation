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

// 自定义内存分配函数2
template <typename T> void custom_new_by_param(T val, A<T> **p) {
  auto a = new A<T>(val); // 实则调用operator new分配，并通过二级指针p返回
  *p = a;
}

void testLeak2(int val) {
  A<int> *pa = nullptr;

  custom_new_by_param(val, &pa); // 分配内存
} // pa内存泄漏 // 缺陷

void testLeak3(int val) {
  A<int> *pa = nullptr;
  custom_new_by_param(val, &pa); // 分配内存给pa

  int flag = sizeof(int); // flag != 0
  // flag != 0 时 manage_res()未释放pa的内存，同时pa传递给返回值r
  auto r = manage_res(*pa, flag);
} // pa内存泄漏 // 缺陷
