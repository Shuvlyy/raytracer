##
## EPITECH PROJECT, 2025
## SOURCES_MK
## File description:
## Sources for Makefile
##

VPATH	:= src/

SRC	+= Main.cpp

SRC += Renderer.cpp

SRC += Parser/Parser.cpp

#SRC += Library/Loader.cpp

SRC += Image/Image.cpp \
	Image/Ppm/Ppm.cpp

SRC += Camera/Camera.cpp

SRC += Multithreading/Multithreading.cpp

SRC += Scene/Scene.cpp \
	Factory/ShapeFactory.cpp \
	Factory/LightFactory.cpp \
	Factory/GetColor.cpp \
	Factory/GetVec3.cpp

SRC += App.cpp

SRC += Signal/Manager.cpp \
	Signal/Handlers/Int.cpp \
	Signal/Handlers/Term.cpp

SRC += Network/Server/Server.cpp \
	Network/Server/Socket/Socket.cpp \
	Network/Server/Session/Manager.cpp \
	Network/Server/Session/Session.cpp \
	Network/Packet/Packet.cpp \
	Network/Server/Packet/Manager.cpp \
	Network/Packet/Packets/Ping.cpp \
	Network/Packet/Packets/Pong.cpp \
	Network/Packet/IO/Serializer.cpp \
	Network/Packet/IO/Deserializer.cpp \
	Network/Server/Packet/Handlers/Ping.cpp \
	Network/Server/Packet/Handlers/Pong.cpp
