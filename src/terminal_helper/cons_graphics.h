#ifndef CCOLOURS_H_INCLUDED
#define CCOLOURS_H_INCLUDED

void pgcg_init_console_graphics();
void pgcg_activate_utf8();
void pgcg_deactivate_utf8();
void pgcg_set_tile_colour(char col);
void pgcg_set_note_colour();
void pgcg_set_warning_colour();
void pgcg_set_error_colour();
void pgcg_set_note_colour_stderr();
void pgcg_set_warning_colour_stderr();
void pgcg_set_error_colour_stderr();
void pgcg_set_progress_bar_colour();
void pgcg_set_distro_bar_colour();
void pgcg_reset_colour();
void pgcg_reset_colour_stderr();

int pgcg_get_console_rows();
int pgcg_get_console_cols();
void pgcg_cpy_console_rows_cols(int* rowsptr, int* colsptr);

#endif // CCOLOURS_H_INCLUDED
