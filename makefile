CC=gcc
CFLAGS=-Wall -O2

# This makefile is auto-generated

# General Parameters
BUILD_WIN:= __build_tmp__
BUILD_LIN:= ./__build_tmp__

# Windows compilation code starts here

ICON_WIN=win_app_info\icon.res
INFO_WIN=win_app_info\info.res
FINAL_WIN=waidle.exe

WIN_FLAGS=-static-libgcc

WIN_SRCS := src\error\print_error.c src\main.c src\manual_input\manual_type.c src\pages\homepage.c src\pages\tester.c src\pages\wlist_interface.c src\picking_algorithm\algorithms.c src\picking_algorithm\highest_information_entropy.c src\picking_algorithm\hybrid.c src\picking_algorithm\information_theory\string_to_size_t.c src\picking_algorithm\information_theory\stts_item.c src\picking_algorithm\most-frequent-in-column.c src\picking_algorithm\random_pick.c src\settings\colour_blind.c src\sorting\heap_idx.c src\sorting\heap_sort.c src\terminal_helper\ccolours.c src\terminal_helper\helper_fxs.c src\terminal_helper\progress_bar.c src\test_solve\test_algo_1.c src\test_solve\tester.c src\utilities\file_util.c src\utilities\get_tz.c src\utilities\hashmap.c src\utilities\input-helper.c src\utilities\int_util.c src\utilities\obfuscator.c src\utilities\str_util.c src\utilities\time_util.c src\word_list\hardcoded_dictionary.c src\word_list\wbitem.c src\word_list\word_bank.c src\wordle\guess_bucket.c src\wordle\wlgame.c src\wordle\word_list.c src\wordle\wordle_logic.c src\wordle_filter\list_words_by_clue.c src\wordle_simulator\simulate_game.c src\wordle_solver\solver.c src\wordle_stats.c

WIN_OBJS := $(WIN_SRCS:%=$(BUILD_WIN)\\%.o)

WIN_L_OBJS := $(BUILD_LIN)\src\error\print_error.c.o $(BUILD_LIN)\src\main.c.o $(BUILD_LIN)\src\manual_input\manual_type.c.o $(BUILD_LIN)\src\pages\homepage.c.o $(BUILD_LIN)\src\pages\tester.c.o $(BUILD_LIN)\src\pages\wlist_interface.c.o $(BUILD_LIN)\src\picking_algorithm\algorithms.c.o $(BUILD_LIN)\src\picking_algorithm\highest_information_entropy.c.o $(BUILD_LIN)\src\picking_algorithm\hybrid.c.o $(BUILD_LIN)\src\picking_algorithm\information_theory\string_to_size_t.c.o $(BUILD_LIN)\src\picking_algorithm\information_theory\stts_item.c.o $(BUILD_LIN)\src\picking_algorithm\most-frequent-in-column.c.o $(BUILD_LIN)\src\picking_algorithm\random_pick.c.o $(BUILD_LIN)\src\settings\colour_blind.c.o $(BUILD_LIN)\src\sorting\heap_idx.c.o $(BUILD_LIN)\src\sorting\heap_sort.c.o $(BUILD_LIN)\src\terminal_helper\ccolours.c.o $(BUILD_LIN)\src\terminal_helper\helper_fxs.c.o $(BUILD_LIN)\src\terminal_helper\progress_bar.c.o $(BUILD_LIN)\src\test_solve\test_algo_1.c.o $(BUILD_LIN)\src\test_solve\tester.c.o $(BUILD_LIN)\src\utilities\file_util.c.o $(BUILD_LIN)\src\utilities\get_tz.c.o $(BUILD_LIN)\src\utilities\hashmap.c.o $(BUILD_LIN)\src\utilities\input-helper.c.o $(BUILD_LIN)\src\utilities\int_util.c.o $(BUILD_LIN)\src\utilities\obfuscator.c.o $(BUILD_LIN)\src\utilities\str_util.c.o $(BUILD_LIN)\src\utilities\time_util.c.o $(BUILD_LIN)\src\word_list\hardcoded_dictionary.c.o $(BUILD_LIN)\src\word_list\wbitem.c.o $(BUILD_LIN)\src\word_list\word_bank.c.o $(BUILD_LIN)\src\wordle\guess_bucket.c.o $(BUILD_LIN)\src\wordle\wlgame.c.o $(BUILD_LIN)\src\wordle\word_list.c.o $(BUILD_LIN)\src\wordle\wordle_logic.c.o $(BUILD_LIN)\src\wordle_filter\list_words_by_clue.c.o $(BUILD_LIN)\src\wordle_simulator\simulate_game.c.o $(BUILD_LIN)\src\wordle_solver\solver.c.o $(BUILD_LIN)\src\wordle_stats.c.o

.PHONY: for_windows
for_windows: $(FINAL_WIN)
	echo Done
	echo Type $(FINAL_WIN) to start the solver!
	echo You can now move $(FINAL_WIN) to any folder you like!

$(FINAL_WIN): $(WIN_OBJS)
	$(CC) $(WIN_L_OBJS) -o $@ $(WIN_FLAGS) $(ICON_WIN) $(INFO_WIN)

$(BUILD_WIN)\\%.c.o: %.c
	if not exist $(dir $@) (mkdir $(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean_windows
clean_windows:
	rmdir /s $(BUILD_WIN)

# Windows compilation code ends here

# LINUX

FINAL_LIN=waidle

LIN_SRCS := src/error/print_error.c src/main.c src/manual_input/manual_type.c src/pages/homepage.c src/pages/tester.c src/pages/wlist_interface.c src/picking_algorithm/algorithms.c src/picking_algorithm/highest_information_entropy.c src/picking_algorithm/hybrid.c src/picking_algorithm/information_theory/string_to_size_t.c src/picking_algorithm/information_theory/stts_item.c src/picking_algorithm/most-frequent-in-column.c src/picking_algorithm/random_pick.c src/settings/colour_blind.c src/sorting/heap_idx.c src/sorting/heap_sort.c src/terminal_helper/ccolours.c src/terminal_helper/helper_fxs.c src/terminal_helper/progress_bar.c src/test_solve/test_algo_1.c src/test_solve/tester.c src/utilities/file_util.c src/utilities/get_tz.c src/utilities/hashmap.c src/utilities/input-helper.c src/utilities/int_util.c src/utilities/obfuscator.c src/utilities/str_util.c src/utilities/time_util.c src/word_list/hardcoded_dictionary.c src/word_list/wbitem.c src/word_list/word_bank.c src/wordle/guess_bucket.c src/wordle/wlgame.c src/wordle/word_list.c src/wordle/wordle_logic.c src/wordle_filter/list_words_by_clue.c src/wordle_simulator/simulate_game.c src/wordle_solver/solver.c src/wordle_stats.c

LIN_OBJS := $(LIN_SRCS:%=$(BUILD_LIN)/%.o)

LIN_L_OBJS := $(BUILD_LIN)/src/error/print_error.c.o $(BUILD_LIN)/src/main.c.o $(BUILD_LIN)/src/manual_input/manual_type.c.o $(BUILD_LIN)/src/pages/homepage.c.o $(BUILD_LIN)/src/pages/tester.c.o $(BUILD_LIN)/src/pages/wlist_interface.c.o $(BUILD_LIN)/src/picking_algorithm/algorithms.c.o $(BUILD_LIN)/src/picking_algorithm/highest_information_entropy.c.o $(BUILD_LIN)/src/picking_algorithm/hybrid.c.o $(BUILD_LIN)/src/picking_algorithm/information_theory/string_to_size_t.c.o $(BUILD_LIN)/src/picking_algorithm/information_theory/stts_item.c.o $(BUILD_LIN)/src/picking_algorithm/most-frequent-in-column.c.o $(BUILD_LIN)/src/picking_algorithm/random_pick.c.o $(BUILD_LIN)/src/settings/colour_blind.c.o $(BUILD_LIN)/src/sorting/heap_idx.c.o $(BUILD_LIN)/src/sorting/heap_sort.c.o $(BUILD_LIN)/src/terminal_helper/ccolours.c.o $(BUILD_LIN)/src/terminal_helper/helper_fxs.c.o $(BUILD_LIN)/src/terminal_helper/progress_bar.c.o $(BUILD_LIN)/src/test_solve/test_algo_1.c.o $(BUILD_LIN)/src/test_solve/tester.c.o $(BUILD_LIN)/src/utilities/file_util.c.o $(BUILD_LIN)/src/utilities/get_tz.c.o $(BUILD_LIN)/src/utilities/hashmap.c.o $(BUILD_LIN)/src/utilities/input-helper.c.o $(BUILD_LIN)/src/utilities/int_util.c.o $(BUILD_LIN)/src/utilities/obfuscator.c.o $(BUILD_LIN)/src/utilities/str_util.c.o $(BUILD_LIN)/src/utilities/time_util.c.o $(BUILD_LIN)/src/word_list/hardcoded_dictionary.c.o $(BUILD_LIN)/src/word_list/wbitem.c.o $(BUILD_LIN)/src/word_list/word_bank.c.o $(BUILD_LIN)/src/wordle/guess_bucket.c.o $(BUILD_LIN)/src/wordle/wlgame.c.o $(BUILD_LIN)/src/wordle/word_list.c.o $(BUILD_LIN)/src/wordle/wordle_logic.c.o $(BUILD_LIN)/src/wordle_filter/list_words_by_clue.c.o $(BUILD_LIN)/src/wordle_simulator/simulate_game.c.o $(BUILD_LIN)/src/wordle_solver/solver.c.o $(BUILD_LIN)/src/wordle_stats.c.o

.PHONY: for_mac
for_mac: $(FINAL_LIN)
	echo Done
	echo Type "./$(FINAL_LIN)" to start the solver!
	echo You can now move the "$(FINAL_LIN)" file to any folder you like!

.PHONY: for_linux
for_linux: $(FINAL_LIN)
	echo Done
	echo Type "./$(FINAL_LIN)" to start the solver!
	echo You can now move the "$(FINAL_LIN)" file to any folder you like!

$(FINAL_LIN): $(LIN_OBJS)
	mkdir -p $(dir $@)
	$(CC) $(LIN_L_OBJS) -o $@ -lm

$(BUILD_LIN)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean_mac
clean_mac:
	rm -r $(BUILD_LIN)

.PHONY: clean_linux
clean_linux:
	rm -r $(BUILD_LIN)
