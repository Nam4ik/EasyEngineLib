//gcc -fPIC -lportaudio -lcjson -lSDL2 -lSDL2_image -lpthread engine.cpp -o engine.o
//gcc -shared -o libengine.so engine.o -fPIC -lportaudio -lcjson -lSDL2 -lSDL2_image -lpthread 

//TODO Python turtle UI and audio processsing with whisper.cpp 
//TODO Fix .wav buffer and optimize project.
//TODO Multythread optimisation
//TODO Add OpenCV screen capture func (FOR DEBUG AND NEW FUNCS)
//Need to remove useless funcs for optimise
//Core dumped...
//Maybe shared object will work?
//Idk i compiled this but dont used and debugged :D 
//... 
//Я не определился C или C++ по этому похуй
//Developing for AthenaUI

/*
  __________________                      ______
  ___    |_  /___  /___________________ _ ___  /______  __
  __  /| |  __/_  __ \  _ \_  __ \  __ `/ __  __ \_  / / /
  _  ___ / /_ _  / / /  __/  / / / /_/ /  _  /_/ /  /_/ /
  /_/  |_\__/ /_/ /_/\___//_/ /_/\__,_/   /_.___/_\__, /
                                               /____/
  _______                                ________              __________
  ___    |__________________ _______________  __ \_______   __ ___  ___  \
  __  /| |_  ___/  ___/  __ `/_  __ \  _ \_  / / /  _ \_ | / / __  / _ \  |
  _  ___ |  /   / /__ / /_/ /_  / / /  __/  /_/ //  __/_ |/ /  _  / , _/ /
  /_/  |_/_/    \___/ \__,_/ /_/ /_/\___//_____/ \___/_____/   | /_/|_| /
                                                                \______/

MIT License

Copyright (c) 2025 ArcaneDev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

All rights reserved Arcane ® 2025.
*/


//#include "lib/OpenCV.hpp"
//#include "convert.h"
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

//TODO Add debug parameters 
//TODO Change console - like kitty, ps, sh, blackbox, gnometerm 
//TODO Turtle UI and add to UI installation builder 


extern "C++" {
    void initEngine(const char* title, int width, int height);
    void cleanupEngine();
    void renderRectangle(int x, int y, int width, int height, unsigned int color);
    void renderImage(const char* imagePath, int x, int y);
    void renderGIF(const char* gifPath, int x, int y);
}



static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static pthread_t renderingThread;
static pthread_t recordingThread;
static bool rendering = false;
static bool recording = false;
static bool debugMode = false;
static const char* console =  "kitty";
static char* currentImagePath = NULL;
static char* currentGIFPath = NULL;
static int currentX = 0;
static int currentY = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static char* prompt = NULL;

//Warning! Unstable!


typedef struct {
    char* PocketSphinx_model_path;
    char* VoiceRecognizer_Type;
    char* NaturalLanguage_Type;
} VoiceRecognizerConfig;

typedef struct {
    int EngineDebug_Mode;
    int MaxOptimisation_Mode;
    char* DebugShell_Type;
    char* ConsoleEmulator_Type;
    int MultithreadWorkEngine_Mode;
    char* BuildVer_Type;
} EngineWorkModeConfig;

typedef struct {
    char* XDG_SESSION_Type;
    char* IgnoringWindows_List;
    int IgnoreMyScreen_Mode;
    double TresholdFrameProcessing_Time;
    int InactivityTimeTreshold;
    int CheckIntervalTime;
} ScreenProcessingConfig;

typedef struct {
    char* LogLevel_Mode;
    int PrivateLogs_Mode;
    int LogToFile_Mode;
    int LogToConsole_Mode;
} LoggingConfig;

typedef struct {
    VoiceRecognizerConfig VoiceRecognizer;
    EngineWorkModeConfig EngineWorkMode;
    ScreenProcessingConfig ScreenProcessing;
    LoggingConfig Logging;
} Config;

// Функция для загрузки и парсинга JSON файла
void parse_config(const char* json_str, Config* config) {
    cJSON* root = cJSON_Parse(json_str);
    if (!root) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return;
    }

    // Проверка наличия объекта Config
    cJSON* config_obj = cJSON_GetObjectItemCaseSensitive(root, "Config");
    if (!config_obj || !cJSON_IsObject(config_obj)) {
        fprintf(stderr, "ParseError - unknown variable: Config\n");
        cJSON_Delete(root);
        return;
    }

    // Парсим VoiceRecognizer
    cJSON* voice_recognizer = cJSON_GetObjectItemCaseSensitive(config_obj, "VoiceRecognizer");
    if (voice_recognizer && cJSON_IsObject(voice_recognizer)) {
        cJSON* model_path = cJSON_GetObjectItemCaseSensitive(voice_recognizer, "PocketSphinx_model_path");
        if (!model_path || !cJSON_IsString(model_path)) {
            fprintf(stderr, "ParseError - unknown type: PocketSphinx_model_path\n");
            cJSON_Delete(root);
            return;
        }
        config->VoiceRecognizer.PocketSphinx_model_path = strdup(model_path->valuestring);

        cJSON* recognizer_type = cJSON_GetObjectItemCaseSensitive(voice_recognizer, "VoiceRecognizer_Type");
        if (!recognizer_type || !cJSON_IsString(recognizer_type)) {
            fprintf(stderr, "ParseError - unknown type: VoiceRecognizer_Type\n");
            cJSON_Delete(root);
            return;
        }
        config->VoiceRecognizer.VoiceRecognizer_Type = strdup(recognizer_type->valuestring);

        cJSON* language_type = cJSON_GetObjectItemCaseSensitive(voice_recognizer, "NaturalLanguage_Type");
        if (!language_type || !cJSON_IsString(language_type)) {
            fprintf(stderr, "ParseError - unknown type: NaturalLanguage_Type\n");
            cJSON_Delete(root);
            return;
        }
        config->VoiceRecognizer.NaturalLanguage_Type = strdup(language_type->valuestring);
    } else {
        fprintf(stderr, "ParseError - unknown variable: VoiceRecognizer\n");
        cJSON_Delete(root);
        return;
    }

    // Парсим EngineWorkMode
    cJSON* engine_work_mode = cJSON_GetObjectItemCaseSensitive(config_obj, "EngineWorkMode");
    if (engine_work_mode && cJSON_IsObject(engine_work_mode)) {
        cJSON* debug_mode = cJSON_GetObjectItemCaseSensitive(engine_work_mode, "EngineDebug_Mode");
        if (!debug_mode || !cJSON_IsBool(debug_mode)) {
            fprintf(stderr, "ParseError - unknown type: EngineDebug_Mode\n");
            cJSON_Delete(root);
            return;
        }
        config->EngineWorkMode.EngineDebug_Mode = debug_mode->valueint;

        cJSON* optimisation_mode = cJSON_GetObjectItemCaseSensitive(engine_work_mode, "MaxOptimisation_Mode");
        if (!optimisation_mode || !cJSON_IsBool(optimisation_mode)) {
            fprintf(stderr, "ParseError - unknown type: MaxOptimisation_Mode\n");
            cJSON_Delete(root);
            return;
        }
        config->EngineWorkMode.MaxOptimisation_Mode = optimisation_mode->valueint;

        cJSON* shell_type = cJSON_GetObjectItemCaseSensitive(engine_work_mode, "DebugShell_Type");
        if (!shell_type || !cJSON_IsString(shell_type)) {
            fprintf(stderr, "ParseError - unknown type: DebugShell_Type\n");
            cJSON_Delete(root);
            return;
        }
        config->EngineWorkMode.DebugShell_Type = strdup(shell_type->valuestring);

        cJSON* console_type = cJSON_GetObjectItemCaseSensitive(engine_work_mode, "ConsoleEmulator_Type");
        if (!console_type || !cJSON_IsString(console_type)) {
            fprintf(stderr, "ParseError - unknown type: ConsoleEmulator_Type\n");
            cJSON_Delete(root);
            return;
        }
        config->EngineWorkMode.ConsoleEmulator_Type = strdup(console_type->valuestring);

        cJSON* multithread_mode = cJSON_GetObjectItemCaseSensitive(engine_work_mode, "MultithreadWorkEngine_Mode");
        if (!multithread_mode || !cJSON_IsBool(multithread_mode)) {
            fprintf(stderr, "ParseError - unknown type: MultithreadWorkEngine_Mode\n");
            cJSON_Delete(root);
            return;
        }
        config->EngineWorkMode.MultithreadWorkEngine_Mode = multithread_mode->valueint;

        cJSON* build_type = cJSON_GetObjectItemCaseSensitive(engine_work_mode, "BuildVer_Type");
        if (!build_type || !cJSON_IsString(build_type)) {
            fprintf(stderr, "ParseError - unknown type: BuildVer_Type\n");
            cJSON_Delete(root);
            return;
        }
        config->EngineWorkMode.BuildVer_Type = strdup(build_type->valuestring);
    } else {
        fprintf(stderr, "ParseError - unknown variable: EngineWorkMode\n");
        cJSON_Delete(root);
        return;
    }

    // Парсим ScreenProcessing
    cJSON* screen_processing = cJSON_GetObjectItemCaseSensitive(config_obj, "ScreenProcessing");
    if (screen_processing && cJSON_IsObject(screen_processing)) {
        cJSON* session_type = cJSON_GetObjectItemCaseSensitive(screen_processing, "XDG_SESSION_Type");
        if (!session_type || !cJSON_IsString(session_type)) {
            fprintf(stderr, "ParseError - unknown type: XDG_SESSION_Type\n");
            cJSON_Delete(root);
            return;
        }
        config->ScreenProcessing.XDG_SESSION_Type = strdup(session_type->valuestring);

        cJSON* ignoring_list = cJSON_GetObjectItemCaseSensitive(screen_processing, "IgnoringWindows_List");
        if (!ignoring_list || !cJSON_IsString(ignoring_list)) {
            fprintf(stderr, "ParseError - unknown type: IgnoringWindows_List\n");
            cJSON_Delete(root);
            return;
        }
        config->ScreenProcessing.IgnoringWindows_List = strdup(ignoring_list->valuestring);

        cJSON* ignore_screen_mode = cJSON_GetObjectItemCaseSensitive(screen_processing, "IgnoreMyScreen_Mode");
        if (!ignore_screen_mode || !cJSON_IsBool(ignore_screen_mode)) {
            fprintf(stderr, "ParseError - unknown type: IgnoreMyScreen_Mode\n");
            cJSON_Delete(root);
            return;
        }
        config->ScreenProcessing.IgnoreMyScreen_Mode = ignore_screen_mode->valueint;

        cJSON* threshold_time = cJSON_GetObjectItemCaseSensitive(screen_processing, "TresholdFrameProcessing_Time");
        if (!threshold_time || !cJSON_IsNumber(threshold_time)) {
            fprintf(stderr, "ParseError - unknown type: TresholdFrameProcessing_Time\n");
            cJSON_Delete(root);
            return;
        }
        config->ScreenProcessing.TresholdFrameProcessing_Time = threshold_time->valuedouble;

        cJSON* inactivity_time = cJSON_GetObjectItemCaseSensitive(screen_processing, "InactivityTimeTreshold");
        if (!inactivity_time || !cJSON_IsNumber(inactivity_time)) {
            fprintf(stderr, "ParseError - unknown type: InactivityTimeTreshold\n");
            cJSON_Delete(root);
            return;
        }
        config->ScreenProcessing.InactivityTimeTreshold = inactivity_time->valueint;

        cJSON* check_interval = cJSON_GetObjectItemCaseSensitive(screen_processing, "CheckIntervalTime");
        if (!check_interval || !cJSON_IsNumber(check_interval)) {
            fprintf(stderr, "ParseError - unknown type: CheckIntervalTime\n");
            cJSON_Delete(root);
            return;
        }
        config->ScreenProcessing.CheckIntervalTime = check_interval->valueint;
    } else {
        fprintf(stderr, "ParseError - unknown variable: ScreenProcessing\n");
        cJSON_Delete(root);
        return;
    }

    // Парсим Logging
    cJSON* logging = cJSON_GetObjectItemCaseSensitive(config_obj, "Logging");
    if (logging && cJSON_IsObject(logging)) {
        cJSON* log_level = cJSON_GetObjectItemCaseSensitive(logging, "LogLevel_Mode");
        if (!log_level || !cJSON_IsString(log_level)) {
            fprintf(stderr, "ParseError - unknown type: LogLevel_Mode\n");
            cJSON_Delete(root);
            return;
        }
        config->Logging.LogLevel_Mode = strdup(log_level->valuestring);

        cJSON* private_logs = cJSON_GetObjectItemCaseSensitive(logging, "PrivateLogs_Mode");
        if (!private_logs || !cJSON_IsBool(private_logs)) {
            fprintf(stderr, "ParseError - unknown type: PrivateLogs_Mode\n");
            cJSON_Delete(root);
            return;
        }
        config->Logging.PrivateLogs_Mode = private_logs->valueint;

        cJSON* log_to_file = cJSON_GetObjectItemCaseSensitive(logging, "LogToFile_Mode");
        if (!log_to_file || !cJSON_IsBool(log_to_file)) {
            fprintf(stderr, "ParseError - unknown type: LogToFile_Mode\n");
            cJSON_Delete(root);
            return;
        }
        config->Logging.LogToFile_Mode = log_to_file->valueint;

        cJSON* log_to_console = cJSON_GetObjectItemCaseSensitive(logging, "LogToConsole_Mode");
        if (!log_to_console || !cJSON_IsBool(log_to_console)) {
            fprintf(stderr, "ParseError - unknown type: LogToConsole_Mode\n");
            cJSON_Delete(root);
            return;
        }
        config->Logging.LogToConsole_Mode = log_to_console->valueint;
    } else {
        fprintf(stderr, "ParseError - unknown variable: Logging\n");
        cJSON_Delete(root);
        return;
    }

      FILE* file = fopen("config.json", "r");
    if (!file) {
        fprintf(stderr, "Failed to open config.json\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);

    Config config;
    parse_config(buffer, &config);
  
    cJSON_Delete(root);
    delete config;
    delete[] buffer;
}
/*
int main() {
    FILE* file = fopen("config.json", "r");
    if (!file) {
        fprintf(stderr, "Failed to open config.json\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);

    Config config;
    parse_config(buffer, &config);
*/

/*
    // Выводим значения для проверки
    printf("PocketSphinx_model_path: %s\n", config.VoiceRecognizer.PocketSphinx_model_path);
    printf("VoiceRecognizer_Type: %s\n", config.VoiceRecognizer.VoiceRecognizer_Type);
    printf("NaturalLanguage_Type: %s\n", config.VoiceRecognizer.NaturalLanguage_Type);

    printf("EngineDebug_Mode: %d\n", config.EngineWorkMode.EngineDebug_Mode);
    printf("MaxOptimisation_Mode: %d\n", config.EngineWorkMode.MaxOptimisation_Mode);
    printf("DebugShell_Type: %s\n", config.EngineWorkMode.DebugShell_Type);
    printf("ConsoleEmulator_Type: %s\n", config.EngineWorkMode.ConsoleEmulator_Type);
    printf("MultithreadWorkEngine_Mode: %d\n", config.EngineWorkMode.MultithreadWorkEngine_Mode);
    printf("BuildVer_Type: %s\n", config.EngineWorkMode.BuildVer_Type);

    printf("XDG_SESSION_Type: %s\n", config.ScreenProcessing.XDG_SESSION_Type);
    printf("IgnoringWindows_List: %s\n", config.ScreenProcessing.IgnoringWindows_List);
    printf("IgnoreMyScreen_Mode: %d\n", config.ScreenProcessing.IgnoreMyScreen_Mode);
    printf("TresholdFrameProcessing_Time: %f\n", config.ScreenProcessing.TresholdFrameProcessing_Time);
    printf("InactivityTimeTreshold: %d\n", config.ScreenProcessing.InactivityTimeTreshold);
    printf("CheckIntervalTime: %d\n", config.ScreenProcessing.CheckIntervalTime);

    printf("LogLevel_Mode: %s\n", config.Logging.LogLevel_Mode);
    printf("PrivateLogs_Mode: %d\n", config.Logging.PrivateLogs_Mode);
    printf("LogToFile_Mode: %d\n", config.Logging.LogToFile_Mode);
    printf("LogToConsole_Mode: %d\n", config.Logging.LogToConsole_Mode);
*/
  

void launchConsole(const char* title) {

    #ifdef _WIN32
        //Win
        char command[256];
        snprintf(command, sizeof(command), "start cmd /k title %s", title);
        system(command);
    #else
        //Unix
        char command[256];
        snprintf(command, sizeof(command), "gnome-terminal --title='%s'", title);
        system(command);
    #endif

}


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
    pthread_mutex_unlock(&mutex);}
/* Main not need for shared object
int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "--debug") == 0) {
        debugMode = true;
    }
    if (debugMode) {
        launchConsole("Log Console");
        launchConsole("Command Console");
    }
*/






