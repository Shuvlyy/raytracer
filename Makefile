##
## EPITECH PROJECT, 2025
## MAKEFILE
## File description:
## Makefile for Raytracer project
##

NAME	= raytracer

vpath %.cpp $(VPATH)

BUILD_DIR	:= .build/
MK_DIR		:= mk/
LOGS_DIR	:= logs/
LIB_DIR		:= lib/

include $(MK_DIR)/sources.mk
include $(MK_DIR)/flags.mk
include $(MK_DIR)/colors.mk

OBJ	:= $(SRC:%.cpp=$(BUILD_DIR)/%.o)

CC	:= g++

all: libs $(NAME)

$(NAME): $(OBJ)
	@ $(CC) -o $@ $^ $(LDFLAGS) && \
	echo -e $(BOLD) $(BG_GRN) "Raytracer - BUILD OK" $(RESET) || \
	echo -e $(BOLD) $(BG_RED) "Raytracer - BUILD KO" $(RESET)

$(BUILD_DIR)/%.o: %.cpp
	@ mkdir -p $(dir $@)
	@ $(CC) -o $@ -c $< $(CXXFLAGS) && \
	echo -e $(BOLD) $(GRN) "(OK)" $(RESET) $< || \
	echo -e $(BOLD) $(RED) "(KO)" $(RESET) $<

libs:
	@ make -s -C $(LIB_DIR) && \
 	echo -e $(BOLD) $(BG_GRN) "Raytracer - LIB BUILD OK" $(RESET) || \
 	echo -e $(BOLD) $(BG_RED) "Raytracer - LIB BUILD KO" $(RESET)

clean:
	@ $(RM) $(OBJ) && \
	make -s -C $(LIB_DIR) clean && \
	echo -e $(BOLD) $(BG_GRN) "Raytracer - CLEAN OK" $(RESET) || \
	echo -e $(BOLD) $(BG_RED) "Raytracer - CLEAN KO" $(RESET)

fclean: clean
	@ $(RM) $(NAME)
	@ $(RM) -r $(BUILD_DIR)
	@ make -s -C $(LIB_DIR) fclean

re: fclean
	@ $(MAKE) all

cleanlogs:
	@ rm -rf $(LOGS_DIR) && \
	echo -e $(BOLD) $(BG_GRN) "Raytracer - LOG CLEAN OK" $(RESET) || \
	echo -e $(BOLD) $(BG_RED) "Raytracer - LOG CLEAN KO" $(RESET)

.PHONY: all libs clean fclean re cleanlogs
