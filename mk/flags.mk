##
## EPITECH PROJECT, 2025
## FLAGS_MK
## File description:
## Flags for Makefile
##

CXXFLAGS	+= -Wall -Wextra -Werror
CXXFLAGS	+= -std=c++20
CXXFLAGS	+= -iquote src/
CXXFLAGS	+= -iquote lib/Logger/include/
CXXFLAGS	+= -g # Only for debug.

LDFLAGS		+= -lsfml-window -lsfml-graphics -lsfml-system
LDFLAGS		+= -L lib/.output -l logger
