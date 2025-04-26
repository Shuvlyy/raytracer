##
## EPITECH PROJECT, 2025
## FLAGS_MK
## File description:
## Flags for Makefile
##

CXXFLAGS	+= -Wall -Wextra -Werror
CXXFLAGS	+= -std=c++20
CXXFLAGS	+= -iquote src/ # include/
CXXFLAGS	+= -iquote lib/Logger/include/
CXXFLAGS	+= -iquote lib/YmlParser/src/
CXXFLAGS	+= -g # Only for debug.

LDFLAGS		+= -L lib/.output/
LDFLAGS		+= -l logger
LDFLAGS		+= -l yml
LDFLAGS		+= -lsfml-window -lsfml-graphics -lsfml-system
