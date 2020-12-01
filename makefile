CXX = g++
INCLUDE = -IC:/SFML/include/
LIBS = -LC:/SFML/lib/
LFLAGS = -lsfml-graphics-d  -lsfml-window-d  -lsfml-system-d  -lsfml-network-d  -lsfml-audio-d 
SRCS = Pong.cpp Network.cpp
OBJS = Pong.exe


all:
	$(CXX) $(SRCS) $(INCLUDE) $(LIBS) $(LFLAGS) -o $(OBJS)  -static-libgcc -static-libstdc++

clean:
	del $(OBJS)
