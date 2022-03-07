#ifndef TESTER_PAGE_H_INCLUDED
#define TESTER_PAGE_H_INCLUDED

#include "../test_solve/tester.h"

void print_test_line(test_sess* session);
int results_page_loop(test_sess* session, size_t* current_page, const size_t total_pages, char* invalid);
void tester_thread();

#endif // TESTER_PAGE_H_INCLUDED
