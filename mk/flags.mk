##
## EPITECH PROJECT, 2025
## FLAGS_MK
## File description:
## Flags for Makefile
##

CXXFLAGS	+= -Wall -Wextra -Werror
CXXFLAGS	+= -std=c++20
CXXFLAGS	+= -iquote src/
CXXFLAGS	+= -g # Only for debug.

LDFLAGS		+= -lsfml-window -lsfml-graphics -lsfml-system
