#ifndef OPTION_KEYS_H_INCLUDED
#define OPTION_KEYS_H_INCLUDED

int option_keys_init();
void option_keys_cleanup();

/**
 * Note that first option starts from 0
 * Returns the length of the corresponding option key string of i
 */
size_t get_option_key_len(size_t i);

/**
 * Note that first option starts from 0
 * Copies the corresponding option key of i into opt
 */
void cpy_option_key(size_t i, char* optbuf);

size_t get_idx_from_option_key(const char* optkey);

void print_options_list(const void* list, const size_t nitems, const size_t size, const size_t indent, const size_t (*get_name_len)(const void*, const size_t), void (*cpy_name)(const void*, const size_t, char* buffer));

#endif // OPTION_KEYS_H_INCLUDED
