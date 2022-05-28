#ifndef FILE_UTIL_H_INCLUDED
#define FILE_UTIL_H_INCLUDED


/**
 * Delete using free
 */
char* create_log_filename(const char* prefix);

char get_file_seperator();
char* find_file_sep(const char* path);
char* get_path_leaf(const char* path);

#endif // FILE_UTIL_H_INCLUDED
