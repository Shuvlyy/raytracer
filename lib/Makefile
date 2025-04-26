##
## EPITECH PROJECT, 2025
## MAKEFILE
## File description:
## Makefile for Raytracer project: Libraries
##

OUTPUT_DIR	:= .output/

LOGGER_DIR	:= Logger/
YML_PARSER_DIR	:= YmlParser/

all: logger yml_parser

logger:
	@ mkdir -p $(OUTPUT_DIR)
	@ $(MAKE) -s -C $(LOGGER_DIR) && \
	mv $(LOGGER_DIR)/*.a .output

yml_parser:
	@ mkdir -p $(OUTPUT_DIR)
	@ $(MAKE) -s -C $(YML_PARSER_DIR) && \
	mv $(YML_PARSER_DIR)/*.a .output

clean:
	@ $(MAKE) -s -C $(LOGGER_DIR) clean
	@ $(MAKE) -s -C $(YML_PARSER_DIR) clean

fclean:
	@ $(RM) -rf $(OUTPUT_DIR)
	@ $(MAKE) -s -C $(LOGGER_DIR) fclean
	@ $(MAKE) -s -C $(YML_PARSER_DIR) fclean

re: fclean
	@ $(MAKE) -s all

.PHONY: all logger yml_parser clean fclean re
