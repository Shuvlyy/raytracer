##
## EPITECH PROJECT, 2025
## SOURCES_MK
## File description:
## Sources for Makefile
##

VPATH	:= src/

SRC	+= src/Main.cpp

SRC += src/Renderer.cpp

SRC += src/Parser/Parser.cpp

#SRC += src/Library/Loader.cpp

SRC += src/Image/Image.cpp \
	src/Image/Ppm/Ppm.cpp

SRC += src/Camera/Camera.cpp

SRC += src/Scene/Scene.cpp \
	src/Factory/ShapeFactory.cpp \
	src/Factory/LightFactory.cpp \
	src/Factory/GetColor.cpp \
	src/Factory/GetVec3.cpp
