##
## EPITECH PROJECT, 2025
## MAKEFILE
## File description:
## Makefile for Raytracer project: Libraries
##

OUTPUT_DIR	:= .output

all: logger

logger:
	@ make -s -C Logger/ &&
	(mkdir -p $(OUTPUT_DIR)
	@ mv Logger/*.a .output)

clean:
	@ make -s -C Logger/ clean

fclean:
	@ $(RM) -f $(OUTPUT_DIR)
	@ make -s -C Logger fclean

re: fclean all

.PHONY: all logger clean fclean re
