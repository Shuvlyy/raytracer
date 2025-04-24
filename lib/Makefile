##
## EPITECH PROJECT, 2025
## MAKEFILE
## File description:
## Makefile for Raytracer project: Libraries
##

OUTPUT_DIR	:= .output

LOGGER_DIR	:= Logger/

all: logger

logger:
	@ mkdir -p $(OUTPUT_DIR)
	@ $(MAKE) -s -C $(LOGGER_DIR) && \
	mv Logger/*.a .output

clean:
	@ $(MAKE) -s -C $(LOGGER_DIR) clean

fclean:
	@ $(RM) -rf $(OUTPUT_DIR)
	@ $(MAKE) -s -C $(LOGGER_DIR) fclean

re: fclean
	@ $(MAKE) -s all

.PHONY: all logger clean fclean re
