#ifndef STR_UTIL_H_INCLUDED
#define STR_UTIL_H_INCLUDED

char str_islen(const char *s, size_t len);
void lowercase(char* s);
void uppercase(char* s);
void insert_int_bef_ext(char* fname, const int n, const size_t buflen);

long hash_str(const char* s);

/**
 * http://www.cse.yorku.ca/~oz/hash.html
 */
long djb2_hash_str(const char* s);

/**
 * http://www.cse.yorku.ca/~oz/hash.html
 */
long sdbm_hash_str(const char* s);

#endif // STR_UTIL_H_INCLUDED
