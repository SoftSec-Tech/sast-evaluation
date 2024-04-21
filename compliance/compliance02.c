struct pthread_mutex_t {};

extern int pthread_mutex_unlock(struct pthread_mutex_t *__mutex);
extern int pthread_mutex_lock(struct pthread_mutex_t *__mutex);

struct pthread_mutex_t Ra;
struct pthread_mutex_t Rb;
int test2(int b) {
  pthread_mutex_lock(&Ra); // Ra 加锁

  // 未违反 MISRA C 2012 Rule 22.16
  pthread_mutex_unlock(&Ra); // Ra 解锁

  pthread_mutex_lock(&Rb); // Rb 加锁
  if (b > 5) {
    pthread_mutex_unlock(&Rb); // 仅在 b > 5 时 Rb 解锁
  }

  // 违反 MISRA C 2012 Rule 22.16, b <= 5 时 Rb 未被解锁
  return 0;
}
