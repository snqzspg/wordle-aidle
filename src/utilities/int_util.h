#ifndef INT_UTIL_H_INCLUDED
#define INT_UTIL_H_INCLUDED

size_t num_char_int(int n);
size_t num_digit(size_t n);
size_t num_digit_unsigned_int(unsigned int n);
void cpynumstr(char* buf, size_t n, size_t ndigits);
void cpynumstr_usi(char* buf, unsigned int n, size_t ndigits);
void cpynumstr_int(char* buf, int n, size_t ndigits);
size_t idx_from_str(const char* s);
int int_from_str(const char* s);

#endif // INT_UTIL_H_INCLUDED
