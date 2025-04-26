##
## EPITECH PROJECT, 2023
## MAKEFILE
## File description:
## Makefile for the YML parser
##

NAME	:= libyml

vpath %.cpp $(VPATH)

BUILD_DIR	:= .build/
MK_DIR		:= mk/

include $(MK_DIR)/sources.mk
include $(MK_DIR)/flags.mk
include ../../$(MK_DIR)/colors.mk

OBJ	:= $(SRC:%.cpp=$(BUILD_DIR)/%.o)

CXX	= g++

all: $(NAME)

$(NAME): $(OBJ)
	@ ar rcs $(NAME).a $(OBJ) && \
	echo -e $(BOLD) $(BG_GRN) "YML Parser - BUILD OK" $(RESET) || \
	echo -e $(BOLD) $(BG_RED) "YML Parser - BUILD KO" $(RESET)

$(BUILD_DIR)/%.o: %.cpp
	@ mkdir -p $(dir $@)
	@ $(CXX) -o $@ -c $< $(CXXFLAGS) && \
	echo -e $(BOLD) $(GRN) "(OK)" $(RESET) $< || \
	echo -e $(BOLD) $(RED) "(KO)" $(RESET) $<

clean:
	@ $(RM) $(OBJ) && \
	echo -e $(BOLD) $(BG_GRN) "YML Parser - CLEAN OK" $(RESET) || \
	echo -e $(BOLD) $(BG_RED) "YML Parser - CLEAN KO" $(RESET)

fclean: clean
	@ $(RM) $(NAME).a
	@ $(RM) -r $(BUILD_DIR)
	@ $(RM) tester

re: fclean
	@ $(MAKE) all

tests_run: re
	@ $(CXX) -o tester tests/Tester.cpp -g -iquote include/yml/ -L . -l yml

.PHONY: all clean fclean re tests_run
