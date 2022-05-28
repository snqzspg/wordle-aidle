#ifndef HASH_UTIL_H_INCLUDED
#define HASH_UTIL_H_INCLUDED

long hash_bytes(const void* bytes, const size_t nbytes);

/**
 * http://www.cse.yorku.ca/~oz/hash.html
 */
long djb2_hash_bytes(const void* bytes, size_t nbytes);

/**
 * http://www.cse.yorku.ca/~oz/hash.html
 */
long sdbm_hash_bytes(const void* bytes, size_t nbytes);

char verify_dword_hash(const long hash_to_verify, const long ref_hash);

#endif // HASH_UTIL_H_INCLUDED
