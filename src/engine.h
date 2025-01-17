#ifndef ENGINE_H
#define ENGINE_H

#ifdef _WIN32
    #ifdef ENGINE_EXPORTS
        #define ENGINE_API __declspec(dllexport)
    #else
        #define ENGINE_API __declspec(dllimport)
    #endif
#else
    #define ENGINE_API
#endif

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    int x;
    int y;
} Point;

ENGINE_API void initEngine(const char* title, int width, int height);
ENGINE_API void cleanupEngine();
ENGINE_API void renderRectangle(int x, int y, int width, int height, unsigned int color);
ENGINE_API void renderImage(const char* imagePath, int x, int y);
ENGINE_API void renderGIF(const char* gifPath, int x, int y);
ENGINE_API void startRenderingThread();
ENGINE_API void stopRenderingThread();
ENGINE_API void startRecordingThread();
ENGINE_API void stopRecordingThread();
ENGINE_API void setPrompt(const char* prompt);

#endif // ENGINE_H
