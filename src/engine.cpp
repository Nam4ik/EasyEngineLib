//gcc -c -fPIC engine.c -o engine.o
//gcc -shared -o libengine.so engine.o -lSDL2 -lSDL2_image -lpthread

#include "engine.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <portaudio.h>
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static pthread_t renderingThread;
static pthread_t recordingThread;
static bool rendering = false;
static bool recording = false;
static char* currentImagePath = NULL;
static char* currentGIFPath = NULL;
static int currentX = 0;
static int currentY = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static char* prompt = NULL;

void* recordingThreadFunc(void* arg) {
    PaStreamParameters inputParameters;
    PaStream* stream;
    PaError err;
    int bufferSize = 256;
    short buffer[bufferSize];
    FILE* fp = fopen("recording.wav", "wb");
    if (!fp) {
        printf("Could not open file for recording\n");
        return NULL;
    }

    err = Pa_Initialize();
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return NULL;
    }

    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {
        printf("No default input device found\n");
        return NULL;
    }

    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = paInt16;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
        &stream,
        &inputParameters,
        NULL,
        44100,
        bufferSize,
        paClipOff,
        NULL,
        NULL
    );
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return NULL;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return NULL;
    }

    fprintf(fp, "RIFF....WAVEfmt \x10\x00\x00\x00\x01\x00\x01\x00\x80\xbb\x00\x00\x00\xee\x02\x00\x04\x00\x10\x00data....");
    long dataChunkPos = ftell(fp);

    while (recording) {
        err = Pa_ReadStream(stream, buffer, bufferSize);
        if (err != paNoError) {
            printf("PortAudio error: %s\n", Pa_GetErrorText(err));
            break;
        }
        fwrite(buffer, 1, bufferSize * sizeof(short), fp);
    }

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    long dataSize = ftell(fp) - dataChunkPos - 8;
    fseek(fp, dataChunkPos + 4, SEEK_SET);
    fwrite(&dataSize, 1, 4, fp);
    fseek(fp, 4, SEEK_SET);
    long fileSize = ftell(fp) - 8;
    fwrite(&fileSize, 1, 4, fp);

    fclose(fp);

    // Convert WAV to text using Python script
    system("python3 convert_audio_to_text.py recording.wav");

    // Read the prompt from answer.json
    FILE* jsonFile = fopen("answer.json", "r");
    if (!jsonFile) {
        printf("Could not open answer.json\n");
        return NULL;
    }

    fseek(jsonFile, 0, SEEK_END);
    long length = ftell(jsonFile);
    fseek(jsonFile, 0, SEEK_SET);

    char* jsonString = (char*)malloc(length + 1);
    fread(jsonString, 1, length, jsonFile);
    jsonString[length] = '\0';
    fclose(jsonFile);

    cJSON* json = cJSON_Parse(jsonString);
    if (!json) {
        printf("Error parsing JSON\n");
        free(jsonString);
        return NULL;
    }

    cJSON* promptJson = cJSON_GetObjectItemCaseSensitive(json, "prompt");
    if (cJSON_IsString(promptJson) && (promptJson->valuestring != NULL)) {
        pthread_mutex_lock(&mutex);
        if (prompt) {
            free(prompt);
        }
        prompt = strdup(promptJson->valuestring);
        pthread_mutex_unlock(&mutex);
    }

    cJSON_Delete(json);
    free(jsonString);

    return NULL;
}

void* renderingThreadFunc(void* arg){
   SDL_Texture* texture = NULL;
    SDL_Rect dstRect;
    int frameDelay = 100; // Delay between frames in milliseconds

    while (rendering) {
        pthread_mutex_lock(&mutex);
        while ((!currentImagePath && !currentGIFPath) && rendering) {
            pthread_cond_wait(&cond, &mutex);
        }
        if (!rendering) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        char* imagePath = currentImagePath ? strdup(currentImagePath) : NULL;
        char* gifPath = currentGIFPath ? strdup(currentGIFPath) : NULL;
        int x = currentX;
        int y = currentY;
        currentImagePath = NULL;
        currentGIFPath = NULL;
        pthread_mutex_unlock(&mutex);

        if (!renderer) {
            continue;
        }

        if (imagePath) {
            SDL_Surface* loadedSurface = IMG_Load(imagePath);
            if (!loadedSurface) {
                printf("Unable to load image %s! SDL_image Error: %s\n", imagePath, IMG_GetError());
                free(imagePath);
                continue;
            }

            texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            if (!texture) {
                printf("Unable to create texture from %s! SDL Error: %s\n", imagePath, SDL_GetError());
                SDL_FreeSurface(loadedSurface);
                free(imagePath);
                continue;
            }

            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = loadedSurface->w;
            dstRect.h = loadedSurface->h;
            SDL_FreeSurface(loadedSurface);
            free(imagePath);
        } else if (gifPath) {
            SDL_Surface* loadedSurface = IMG_Load(gifPath);
            if (!loadedSurface) {
                printf("Unable to load GIF %s! SDL_image Error: %s\n", gifPath, IMG_GetError());
                free(gifPath);
                continue;
            }

            texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            if (!texture) {
                printf("Unable to create texture from %s! SDL Error: %s\n", gifPath, IMG_GetError());
                SDL_FreeSurface(loadedSurface);
                free(gifPath);
                continue;
            }

            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = loadedSurface->w;
            dstRect.h = loadedSurface->h;
            SDL_FreeSurface(loadedSurface);
            free(gifPath);
        }

        if (texture) {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
            SDL_RenderPresent(renderer);
            SDL_DestroyTexture(texture);
            SDL_Delay(frameDelay);
        }
    }
    return NULL;
}


void initEngine(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) & (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF))) {
        printf("SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        return;
    }

    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP
    );

    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    rendering = true;
    pthread_create(&renderingThread, NULL, renderingThreadFunc, NULL);

    recording = true;
    pthread_create(&recordingThread, NULL, recordingThreadFunc, NULL);
}

void cleanupEngine() {
    rendering = false;
    pthread_cond_signal(&cond);
    pthread_join(renderingThread, NULL);

    recording = false;
    pthread_cond_signal(&cond);
    pthread_join(recordingThread, NULL);

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    IMG_Quit();
    SDL_Quit();
    
    // Освобождение памяти
    free(currentImagePath);
    free(currentGIFPath);
    free(prompt);
}

void renderRectangle(int x, int y, int width, int height, unsigned int color) {
    if (!renderer) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
    SDL_Rect rect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}

void renderImage(const char* imagePath, int x, int y) {
    pthread_mutex_lock(&mutex);
    if (currentImagePath) {
        free(currentImagePath);
    }
    currentImagePath = strdup(imagePath);
    currentX = x;
    currentY = y;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void renderGIF(const char* gifPath, int x, int y) {
    pthread_mutex_lock(&mutex);
    if (currentGIFPath) {
        free(currentGIFPath);
    }
    currentGIFPath = strdup(gifPath);
    currentX = x;
    currentY = y;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}






void startRenderingThread() {
    rendering = true;
    pthread_create(&renderingThread, NULL, renderingThreadFunc, NULL);
}

void stopRenderingThread() {
    rendering = false;
    pthread_cond_signal(&cond);
    pthread_join(renderingThread, NULL);
}

void startRecordingThread() {
    recording = true;
    pthread_create(&recordingThread, NULL, recordingThreadFunc, NULL);
}

void stopRecordingThread() {
    recording = false;
    pthread_cond_signal(&cond);
    pthread_join(recordingThread, NULL);
}

void setPrompt(const char* prompt) {
    pthread_mutex_lock(&mutex);
    if (prompt) {
        if (prompt) {
            free((void*)prompt);
        }
        prompt = strdup(prompt);
    }
    pthread_mutex_unlock(&mutex);
}

int main(){
    initEngine("EasyEngine", 800, 600);
}

