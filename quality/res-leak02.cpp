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

// 自定义内存分配函数1
template <typename T> auto custom_new(T val) {
  return new A<T>(val); // 实则调用operator new分配
}

// 自定义内存分配函数2
template <typename T> void custom_new_by_param(T val, A<T> **p) {
  auto a = new A<T>(val); // 实则调用operator new分配，并通过二级指针p返回
  *p = a;
}

// 自定义内存释放函数
template <typename T> void release_res(A<T> *p) {
  auto *p2 = p;
  delete p2;
}

void test1(int val) {
  auto pa = custom_new(val);            // 分配内存
  int flag = sizeof(int) - sizeof(val); // flag == 0
  auto r = manage_res(*pa, flag); // flag == 0 时 manage_res()对a释放了内存
} // 无内存泄漏 // 无缺陷

void test2(int val) {
  auto pa = custom_new(val);      // 分配内存
  int flag = sizeof(int);         // flag != 0
  auto r = manage_res(*pa, flag); // flag != 0 时 manage_res()未释放a的内存
  A<int> **r2 = &r;
  delete *r2;
} // 无内存泄漏 // 无缺陷

void test3(int val) {
  auto pa = custom_new(val);      // 分配内存
  int flag = sizeof(int);         // flag != 0
  auto r = manage_res(*pa, flag); // flag != 0 时 manage_res()未释放a的内存
  if (r) {
    delete r;
  }
} // 无内存泄漏 // 无缺陷

void test4(int val) {
  int i = 3;
  A<int> *pa = nullptr;
  if (i > 0) {
    custom_new_by_param(val, &pa); // 分配内存
  }
  int flag = sizeof(int);         // flag != 0
  auto r = manage_res(*pa, flag); // flag != 0 时 manage_res()未释放a的内存

  A<int> *r2 = r;
  release_res(r2);
} // 无内存泄漏 // 无缺陷

A<int> *ga = nullptr;
void test5(int val) {
  int i = 3;
  A<int> *pa = nullptr;
  if (i > 0) {
    custom_new_by_param(val, &pa); // 分配内存
  }
  int flag = sizeof(int);         // flag != 0
  auto r = manage_res(*pa, flag); // flag != 0 时 manage_res()未释放a的内存

  ga = r;
} // 无内存泄漏 // 无缺陷

void test6() {
  if (ga) {
    release_res(ga);
  }
}

void test7() {
  test5(5);
  test6();
}

A<int> *ga2 = nullptr;
void test8(int val) {
  auto pa = custom_new(val);      // 分配内存
  int flag = sizeof(int);         // flag != 0
  auto r = manage_res(*pa, flag); // flag != 0 时 manage_res()未释放a的内存
  ga2 = r;
} // 无内存泄漏 // 无缺陷

void test9() {
  test8(5);
  if (ga2) {
    release_res(ga2);
  }
}
