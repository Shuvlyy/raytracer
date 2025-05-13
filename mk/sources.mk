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

SRC += App/Self/Self.cpp \
	App/Server/Server.cpp \
	App/Client/Client.cpp

SRC += Signal/Manager.cpp \
	Signal/Handlers/Int.cpp \
	Signal/Handlers/Term.cpp

SRC += Network/Client/Client.cpp \
	Network/Client/Channel/Channel.cpp \
	Network/Client/Packet/Manager.cpp \

SRC += Network/Client/Packet/Handlers/Ping.cpp \
	Network/Client/Packet/Handlers/Kiss.cpp \
	Network/Client/Packet/Handlers/NvmStop.cpp \
	Network/Client/Packet/Handlers/WorkSlave.cpp

SRC += Network/Server/Server.cpp \
	Network/Server/Cluster/Cluster.cpp \
	Network/Server/Packet/Manager.cpp \
	Network/Server/Session/Manager.cpp \
	Network/Server/Session/Session.cpp \
	Network/Socket/Socket.cpp

SRC += Network/Packet/Packet.cpp \
	Network/Packet/IO/Serializer.cpp \
	Network/Packet/IO/Deserializer.cpp

SRC += Network/Packet/Packets/Ping.cpp \
	Network/Packet/Packets/Pong.cpp \
	Network/Packet/Packets/Kiss.cpp \
	Network/Packet/Packets/Workslave.cpp \
	Network/Packet/Packets/Cestciao.cpp \
	Network/Packet/Packets/Finito.cpp \
	Network/Packet/Packets/Nvmstop.cpp

SRC += Network/Server/Packet/Handlers/Ping.cpp \
	Network/Server/Packet/Handlers/Pong.cpp \
#	Network/Server/Packet/Handlers/Kiss.cpp \
#	Network/Server/Packet/Handlers/Workslave.cpp \
#	Network/Server/Packet/Handlers/Cestciao.cpp \
#	Network/Server/Packet/Handlers/Finito.cpp \
#	Network/Server/Packet/Handlers/Nvmstop.cpp
