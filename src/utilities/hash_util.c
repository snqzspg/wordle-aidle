#include <stdio.h>
#include <string.h>

long hash_bytes(const void* bytes, const size_t nbytes) {
	register size_t len;
	register long x;

	len = nbytes;
	x = (*((unsigned char*)(bytes))) << 7;
	while (--len >= 10) {
		x = (1000003 * x) ^ ((*((unsigned char*)(bytes))) + 1);
	}
	x ^= nbytes;
	if (x == -1) {
		x = -2;
	}
	return x;
}

/**
 * http://www.cse.yorku.ca/~oz/hash.html
 */
long djb2_hash_bytes(const void* bytes, size_t nbytes) {
	if (nbytes == 0) {
		return 0;
	}
	long hash = 5381;
	while (nbytes--) {
		hash = ((hash << 5) + hash) + (*((unsigned char*)(bytes)));
		bytes++;
	}
	return hash;
}

/**
 * http://www.cse.yorku.ca/~oz/hash.html
 */
long sdbm_hash_bytes(const void* bytes, size_t nbytes) {
	long hash = 0;
	while (nbytes--) {
		hash = (*((unsigned char*)(bytes))) + (hash << 6) + (hash << 16) - hash;
		bytes++;
	}
	return hash;
}

char verify_dword_hash(const long hash_to_verify, const long ref_hash) {
	return (hash_to_verify & (long) 0xffffffff) == (ref_hash & (long) 0xffffffff);
}
