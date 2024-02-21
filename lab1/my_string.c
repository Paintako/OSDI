int strcmp(const char *s1, const char *s2) {
  while (*s1 && *s1 == *s2) {
    s1++;
    s2++;
  }
  // if s1 is greater than or less than s2, return the difference
  // if s1 is equal to s2, return 0
  return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

int strlen(const char *s) {
  int len = 0;
  while (*s++) {
    len++;
  }
  return len;
}