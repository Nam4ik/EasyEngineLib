CXX = g++
CXXFLAGS = -O3 -Wall -shared -fPIC -I/usr/include/ 
LDFLAGS = -lSDL2 -lSDL2_image -lpthread -lportaudio -lcjson

all: engine.so

engine.so: binding.o engine.o
    $(CXX) -o $@ $^ $(LDFLAGS)

binding.o: binding.cpp
    $(CXX) $(CXXFLAGS) -c binding.cpp

engine.o: engine.cpp
    $(CXX) $(CXXFLAGS) -c engine.cpp

clean:
    rm -f *.o engine.so
