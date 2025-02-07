
CXX = g++
CXXFLAGS = -fPIC -shared -I./lib -I./src -I/usr/include/python3.x  # Замените python3.x на вашу версию Python


SRC_DIR = src
LIB_DIR = $(SRC_DIR)/lib
BUILD_DIR = build/bin


ENGINE_SO = $(BUILD_DIR)/engine.so
OPENCV_SO = $(BUILD_DIR)/OpenCV.so


ENGINE_SRC = $(SRC_DIR)/engine.cpp
ENGINE_HDR = $(SRC_DIR)/engine.h
OPENCV_SRC = $(LIB_DIR)/OpenCV.cpp
OPENCV_HDR = $(LIB_DIR)/OpenCV.hpp


all: $(ENGINE_SO) $(OPENCV_SO)

$(ENGINE_SO): $(ENGINE_SRC) $(ENGINE_HDR) convert_audio_to_text.py
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OPENCV_SO): $(OPENCV_SRC) $(OPENCV_HDR) convert_audio_to_text.py
	$(CXX) $(CXXFLAGS) -o $@ $<


clean:
	rm -f $(BUILD_DIR)/*.so

.PHONY: all clean
