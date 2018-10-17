
char *strpbrk(const char *s, const char *accept);
char *strdup(const char *s);

char *strndup(const char *s, size_t n);
char *strdupa(const char *s);
char *strndupa(const char *s, size_t n);

size_t strlen(const char *s);

char *strrev(char * str);

char *strcat(char *dest, const char *src);

char *strncat(char *dest, const char *src, size_t n);

char *strchr(const char *s, int c);

char *strrchr(const char *s, int c);


char *strstr(const char *haystack, const char *needle);


char *strcpy(char *dest, const char *src);

char *strncpy(char *dest, const char *src, size_t n);


int strcmp(const char *s1, const char *s2);

int strncmp(const char *s1, const char *s2, size_t n);

size_t strspn(const char *s, const char *accept);

size_t strcspn(const char *s, const char *reject);