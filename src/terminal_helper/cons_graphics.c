#include <stdio.h>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <Windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif // _WIN32

#include "cons_graphics.h"

#ifdef _WIN32
CONSOLE_SCREEN_BUFFER_INFO console_info;
HANDLE console_handle;
WORD saved_attributes;
CONSOLE_SCREEN_BUFFER_INFO stderr_console_info;
HANDLE stderr_console_handle;
WORD stderr_saved_attributes;
#endif // _WIN32

unsigned char utf8active = 0;

void pgcg_init_console_graphics() {
	#ifdef _WIN32
	// No need to close this -> https://docs.microsoft.com/en-us/windows/console/getstdhandle
	console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console_handle, &console_info);
	saved_attributes = console_info.wAttributes;
	stderr_console_handle = GetStdHandle(STD_ERROR_HANDLE);
	GetConsoleScreenBufferInfo(stderr_console_handle, &stderr_console_info);
	stderr_saved_attributes = stderr_console_info.wAttributes;
	#endif // _WIN32
}

/**
 *  This enables you to print special characters on the terminal, like emojis.
 *
 *  Note that after this you cannot use printf, only wprintf.
 *  To restore the functionality of printf, use CG_deactivateUTF8()
 */
void pgcg_activate_utf8() {
	#ifdef _WIN32
	_setmode(_fileno(stdout), _O_U8TEXT);
	#endif // _WIN32
	utf8active = 1;
}

/**
 *  Use this to restore the functionality of printf.
 */
void pgcg_deactivate_utf8() {
	#ifdef _WIN32
	_setmode(_fileno(stdout), _O_TEXT);
	#endif // _WIN32
	utf8active = 0;
}

char pgcg_is_utf8_active() {
	return utf8active;
}

void pgcg_set_tile_colour(char col) {
	#ifdef _WIN32
	switch (col) {
	case 'b':
		SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
		break;
	case 'g':
		SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case 'y':
		SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	#else
	// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
	switch (col) {
	case 'b':
		printf("\x1b[40m\x1b[97m");
		break;
	case 'g':
		printf("\x1b[42m\x1b[97m");
		break;
	case 'y':
		printf("\x1b[43m\x1b[97m");
	}
	#endif // _WIN32
}

void pgcg_set_note_colour() {
	#ifdef _WIN32
	SetConsoleTextAttribute(console_handle, FOREGROUND_BLUE | FOREGROUND_GREEN);
	#else
	printf("\x1b[36m");
	#endif // _WIN32
}

void pgcg_set_warning_colour() {
	#ifdef _WIN32
	SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN);
	#else
	printf("\x1b[33m");
	#endif // _WIN32
}

void pgcg_set_error_colour() {
	#ifdef _WIN32
	SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
	#else
	printf("\x1b[91m");
	#endif // _WIN32
}

void pgcg_set_note_colour_stderr() {
	#ifdef _WIN32
	SetConsoleTextAttribute(stderr_console_handle, FOREGROUND_BLUE | FOREGROUND_GREEN);
	#else
	fprintf(stderr, "\x1b[36m");
	#endif // _WIN32
}

void pgcg_set_warning_colour_stderr() {
	#ifdef _WIN32
	SetConsoleTextAttribute(stderr_console_handle, FOREGROUND_RED | FOREGROUND_GREEN);
	#else
	fprintf(stderr, "\x1b[33m");
	#endif // _WIN32
}

void pgcg_set_error_colour_stderr() {
	#ifdef _WIN32
	SetConsoleTextAttribute(stderr_console_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
	#else
	fprintf(stderr, "\x1b[91m");
	#endif // _WIN32
}

void pgcg_set_progress_bar_colour() {
	#ifdef _WIN32
	SetConsoleTextAttribute(console_handle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	#else
	printf("\x1b[96m\x1b[106m");
	#endif // _WIN32
}

void pgcg_set_distro_bar_colour() {
	#ifdef _WIN32
	SetConsoleTextAttribute(console_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	#else
	printf("\x1b[92m\x1b[102m");
	#endif // _WIN32
}

void pgcg_set_yellow_block_colour() {
	#ifdef _WIN32
	SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN);
	#else
	printf("\x1b[33m");
	#endif // _WIN32
}

void pgcg_reset_colour() {
	#ifdef _WIN32
	SetConsoleTextAttribute(console_handle, saved_attributes);
	#else
	printf("\x1b[0m");
	#endif // _WIN32
}

void pgcg_reset_colour_stderr() {
	#ifdef _WIN32
	SetConsoleTextAttribute(stderr_console_handle, saved_attributes);
	#else
	fprintf(stderr, "\x1b[0m");
	#endif // _WIN32
}

/**
 * Thanks! https://stackoverflow.com/questions/6812224/getting-terminal-size-in-c-for-windows/12642749#12642749
 */
int pgcg_get_console_rows() {
	#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(console_handle, &csbi);
	return (int) csbi.srWindow.Bottom - (int) csbi.srWindow.Top + 1;
	#else
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	return ws.ws_row;
	#endif // _WIN32
}

int pgcg_get_console_cols() {
	#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(console_handle, &csbi);
	return (int) csbi.dwSize.X;
	#else
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	return ws.ws_col;
	#endif // _WIN32
}

void pgcg_cpy_console_rows_cols(int* rowsptr, int* colsptr) {
	#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(console_handle, &csbi);
	*rowsptr = (int) csbi.dwSize.X;
	*colsptr = (int) csbi.srWindow.Bottom - (int) csbi.srWindow.Top + 1;
	#else
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	*rowsptr = ws.ws_row;
	*colsptr = ws.ws_col;
	#endif // _WIN32
}
