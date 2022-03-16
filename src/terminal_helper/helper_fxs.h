#ifndef HELPER_FXS_H_INCLUDED
#define HELPER_FXS_H_INCLUDED

#include <stdarg.h>
#include <stdlib.h>

#define PGINDENT 4

void clear_console();
void pause_console();

void vfprint_wraped_f(FILE* f, const char* line, int indentation, int indent_size, va_list args);
void vfprint_wraped_linef(FILE* f, const char* line, int indentation, int indent_size, va_list args);
void print_wraped_linef(const char* line, int indentation, int indent_size, ...);

#endif // HELPER_FXS_H_INCLUDED
