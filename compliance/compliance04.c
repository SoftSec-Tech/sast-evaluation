extern char *strrchr(const char *__s, int __c);
extern void puts(char *a){};
const char *get_dirname(const char *pathname) {
  char *slash;
  slash = strrchr(pathname, '/'); // 违反 CERT_C:STR30-C
  if (slash) {
    *slash = '\0';
  }
  return pathname;
}

int main(void) {
  puts(get_dirname(__FILE__)); // 违反 CERT_C:STR30-C
  return 0;
}
