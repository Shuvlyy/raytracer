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

SRC += src/Multithreading/Multithreading.cpp

SRC += src/Scene/Scene.cpp \
	src/Factory/ShapeFactory.cpp \
	src/Factory/LightFactory.cpp \
	src/Factory/GetColor.cpp \
	src/Factory/GetVec3.cpp

SRC += src/App.cpp

SRC += src/Signal/Manager.cpp \
	src/Signal/Handlers/Int.cpp \
	src/Signal/Handlers/Term.cpp

SRC += src/Network/Server/Server.cpp \
	src/Network/Server/Socket/Socket.cpp \
	src/Network/Server/Session/Manager.cpp \
	src/Network/Server/Session/Session.cpp \
	src/Network/Packet/Packet.cpp \
	src/Network/Server/Packet/Manager.cpp \
	src/Network/Packet/Packets/Ping.cpp \
	src/Network/Packet/Packets/Pong.cpp \
	src/Network/Packet/IO/Serializer.cpp \
	src/Network/Packet/IO/Deserializer.cpp \
	src/Network/Server/Packet/Handlers/Ping.cpp \
	src/Network/Server/Packet/Handlers/Pong.cpp
