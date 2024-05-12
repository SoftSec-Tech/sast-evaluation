typedef unsigned long long size_t;
extern "C" {
extern void *memcpy(void *dest, const void *src, size_t count);
}

template <typename T> class A {
public:
  // 【核心检查】// 构造函数未对其成员 data 进行初始化
  A() {}

  // 【合规检查 MISRA_CPP2008:Rule 12-1-3】//可通过基本类型的单一参数调用
  //  的构造函数没声明为 explicit
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
  // 【合规检查 MISRA_CPP2008:Rule 18-4-1】不允许使用动态堆内存分配
  return new A<T>(val); // 实则调用operator new分配
}

void testNoLeak(int val) {
  auto pa = custom_new(val); // 分配内存
  // 【合规检查 MISRA_CPP2008:Rule 3-9-2】应该使用指示大小和符号性的
  //  typedef代替基本数值类型。
  int flag = sizeof(int) - sizeof(val); // flag == 0
  // 【合规检查 MISRA_C2012:Rule 8.13】指针应尽可能指向常量限定类型。指针变量 r
  // 指向一个非常数类型，但是没有修改其指向的对象，考虑在指向类型中添加 const
  // 限定符。
  auto r = manage_res(*pa, flag); // flag == 0 时 manage_res()对a释放了内存
} // 无内存泄漏

void testLeak(int val) {
  A<int> *pa = custom_new(val);   // 分配内存
  int flag = sizeof(int);         // flag != 0
  auto r = manage_res(*pa, flag); // flag != 0 时 manage_res()未释放a的内存

  A<int> a2;
  // 【核心检查】// 误对指针取 sizeof 导致逻辑错误
  // 【合规检查 MISRA_CPP2008:Rule 0-1-7】// 函数返回值未被使用
  memcpy(&a2, pa, sizeof(pa));
} // 【核心检查】 //【合规检查 CERT_C:MEM31-C】// pa内存泄漏

A<int> *retPtr(A<int> *p, int unknown) {
  if (unknown == 10) {
    return nullptr;
  } else {
    // 【合规检查 MISRA_C2012:Rule 15.5】 函数结束时应只有一个退出点。
    return p;
  }
}

// 【合规检查 AUTOSAR_CPP14:Rule A7-1-1】非 const 变量 p 的值未被修改。
//  考虑为变量类型添加 const 修饰符。
void derefParam(A<int> *p) { A<int> x = *p; }

void testRet(A<int> *x, int y) {
  // 【核心检查】// 可能返回空指针的变量，后续被解引用了，导致程序崩溃
  auto p = retPtr(x, y);
  derefParam(p); // 在跨函数调用中解引用了p
}
