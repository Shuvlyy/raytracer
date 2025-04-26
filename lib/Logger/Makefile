##
## EPITECH PROJECT, 2025
## MAKEFILE
## File description:
## Makefile for Logger library
##

NAME	:= liblogger

vpath %.cpp $(VPATH)

BUILD_DIR	:= .build/
MK_DIR		:= mk/
LOGS_DIR	:= logs/

include ../../$(MK_DIR)/colors.mk

SRC	:= src/Logger.cpp
OBJ	:= $(SRC:%.cpp=$(BUILD_DIR)/%.o)

CXX	:= g++

CXXFLAGS	:= -Wall -Wextra -Werror -pedantic -std=c++20
CXXFLAGS	+= -iquote include/

all: $(NAME)

$(NAME): $(OBJ)
	@ ar rcs $(NAME).a $(OBJ) && \
	echo -e $(BOLD) $(BG_GRN) "Logger - BUILD OK" $(RESET) || \
	echo -e $(BOLD) $(BG_RED) "Logger - BUILD KO" $(RESET)

$(BUILD_DIR)/%.o: %.cpp
	@ mkdir -p $(dir $@)
	@ $(CXX) -o $@ -c $< $(CXXFLAGS) && \
	echo -e $(BOLD) $(GRN) "(OK)" $(RESET) $< || \
	echo -e $(BOLD) $(RED) "(KO)" $(RESET) $<

clean:
	@ $(RM) $(OBJ) && \
	echo -e $(BOLD) $(BG_GRN) "Logger - CLEAN OK" $(RESET) || \
	echo -e $(BOLD) $(BG_RED) "Logger - CLEAN KO" $(RESET)

fclean: clean
	@ $(RM) $(NAME).a
	@ $(RM) -r $(BUILD_DIR)

re: fclean
	@ $(MAKE) all

.PHONY: all clean fclean re
