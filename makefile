CXX = g++
CXXFLAGS = -O3 -Wall -fPIC -I/usr/include/ 
LDFLAGS = -lSDL2 -lSDL2_image -lpthread -lportaudio -lcjson 

all: engine.so, OpenCV.so 

binding.o: binding.cpp
    $(CXX) $(CXXFLAGS) -c binding.cpp -o binding.o

engine.so: binding.o engine.o
    $(CXX) -o $@ $^ $(LDFLAGS)

OpevCV.so: OpenCV.cpp 
    $(CXX) -shared -o $@ $^ -lthread -fPIC

engine.o: engine.cpp
    $(CXX) $(CXXFLAGS) -c engine.cpp -o engine.o

clean:
    rm -f *.o engine.so
