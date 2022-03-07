CC=gcc
CFLAGS=-Wall -O2

# General Parameters
BUILD_WIN:= __build_tmp__
BUILD_LIN:= ./__build_tmp__

# Windows compilation code starts here

ICON_WIN=win_app_info\icon.res
INFO_WIN=win_app_info\info.res
FINAL_WIN=waidle.exe

WIN_FLAGS=-static-libgcc

WIN_SRCS := src\error\print_error.c src\main.c src\manual_input\manual_type.c src\pages\homepage.c src\pages\tester.c src\pages\wlist_interface.c src\picking_algorithm\algorithms.c src\picking_algorithm\highest_information_entropy.c src\picking_algorithm\information_theory\string_to_size_t.c src\picking_algorithm\information_theory\stts_item.c src\picking_algorithm\most-frequent-in-column.c src\picking_algorithm\random_pick.c src\settings\colour_blind.c src\sorting\heap_idx.c src\sorting\heap_sort.c src\terminal_helper\ccolours.c src\terminal_helper\helper_fxs.c src\terminal_helper\progress_bar.c src\test_solve\tester.c src\test_solve\test_algo_1.c src\utilities\file_util.c src\utilities\get_tz.c src\utilities\hashmap.c src\utilities\input-helper.c src\utilities\int_util.c src\utilities\obfuscator.c src\utilities\str_util.c src\utilities\time_util.c src\wordle\guess_bucket.c src\wordle\wlgame.c src\wordle\wordle_logic.c src\wordle\word_list.c src\wordle_filter\list_words_by_clue.c src\wordle_simulator\simulate_game.c src\wordle_solver\solver.c src\wordle_stats.c src\word_list\hardcoded_dictionary.c src\word_list\wbitem.c src\word_list\word_bank.c

WIN_OBJS := $(WIN_SRCS:%=$(BUILD_WIN)\\%.o)

.PHONY: for_windows
for_windows: $(FINAL_WIN)
	echo Done
	echo You can now move $(FINAL_WIN) to any folder you like!

$(FINAL_WIN): $(WIN_OBJS)
	$(CC) $(WIN_OBJS) -o $@ $(WIN_FLAGS) $(ICON_WIN) $(INFO_WIN)

$(BUILD_WIN)\\%.c.o: %.c
	if not exist $(dir $@) (mkdir $(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean_windows
clean_windows:
	rmdir /s $(BUILD_WIN)

# Windows compilation code ends here

# LINUX

FINAL_LIN=waidle

LIN_SRCS := src/error/print_error.c src/main.c src/manual_input/manual_type.c src/pages/homepage.c src/pages/tester.c src/pages/wlist_interface.c src/picking_algorithm/algorithms.c src/picking_algorithm/highest_information_entropy.c src/picking_algorithm/information_theory/string_to_size_t.c src/picking_algorithm/information_theory/stts_item.c src/picking_algorithm/most-frequent-in-column.c src/picking_algorithm/random_pick.c src/settings/colour_blind.c src/sorting/heap_idx.c src/sorting/heap_sort.c src/terminal_helper/ccolours.c src/terminal_helper/helper_fxs.c src/terminal_helper/progress_bar.c src/test_solve/tester.c src/test_solve/test_algo_1.c src/utilities/file_util.c src/utilities/get_tz.c src/utilities/hashmap.c src/utilities/input-helper.c src/utilities/int_util.c src/utilities/obfuscator.c src/utilities/str_util.c src/utilities/time_util.c src/wordle/guess_bucket.c src/wordle/wlgame.c src/wordle/wordle_logic.c src/wordle/word_list.c src/wordle_filter/list_words_by_clue.c src/wordle_simulator/simulate_game.c src/wordle_solver/solver.c src/wordle_stats.c src/word_list/hardcoded_dictionary.c src/word_list/wbitem.c src/word_list/word_bank.c

LIN_OBJS := $(LIN_SRCS:%=$(BUILD_LIN)/%.o)

.PHONY: for_linux
for_linux: $(FINAL_LIN)
	echo Done
	echo You can now move the "$(FINAL_LIN)" file to any folder you like!

$(FINAL_LIN): $(LIN_OBJS)
	mkdir -p $(dir $@)
	$(CC) $(LIN_OBJS) -o $@ -lm

$(BUILD_LIN)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean_linux
clean_linux:
	rm -r $(BUILD_LIN)