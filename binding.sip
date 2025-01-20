%Module engine

%{
#include "engine.h"
%}

void initEngine(const char* title, int width, int height);
void cleanupEngine();
void renderRectangle(int x, int y, int width, int height, unsigned int color);
void renderImage(const char* imagePath, int x, int y);
void renderGIF(const char* gifPath, int x, int y);

