#Ricardo Guido Marelli
#Makefile Simple Proto

CLASSDIR=
PROGDIR=
BINDIR=


CXXFLAGS = -Wall -ansi -pedantic-errors -fno-inline 

client_sources = $(PROGDIR)client.cpp $(CLASSDIR)common.cpp $(CLASSDIR)cSocket.cpp $(CLASSDIR)cClientSocket.cpp $(CLASSDIR)cDispatcher.cpp $(CLASSDIR)cClientProto.cpp
server_sources = $(PROGDIR)server.cpp $(CLASSDIR)common.cpp $(CLASSDIR)cServerSocket.cpp
serverproto_sources = $(PROGDIR)serverproto.cpp $(CLASSDIR)common.cpp $(CLASSDIR)cSocket.cpp $(CLASSDIR)cDispatcher.cpp $(CLASSDIR)cServerProto.cpp

o_files_proto = $(patsubst %.cpp,%.o,$(serverproto_sources))
o_files_server = $(patsubst %.cpp,%.o,$(server_sources))
o_files_client = $(patsubst %.cpp,%.o,$(client_sources))

.PHONY: all clean

all: serverproto client server

serverproto: $(o_files_proto)

	$(CXX) $(o_files_proto) -o $(BINDIR)serverproto

server: $(o_files_server)

	$(CXX) $(o_files_server) -o $(BINDIR)server

client: $(o_files_client)

	$(CXX) $(o_files_client) -o $(BINDIR)client

clean:
	@$(RM) -fv $(o_files_server) $(o_files_proto) $(o_files_client) $(BINDIR)client $(BINDIR)server $(BINDIR)serverproto
