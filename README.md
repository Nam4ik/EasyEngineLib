<h1>Usage</h1>

__At the moment, this is my pet project and it is still in development. He will participate in other projects when he stops being a bit raw. But I don't think anyone is interested in this, so I'll go to the documentation.__ 

Opportunities
--------------
As it says in the description, it can render .gif, .jpg, and .png. The drawing takes place relative to the coordinates set in the function. It supports asynchronous operation and rendering of several images or GIFs at once. 
<h3>Functions</h3> 

- `initEngine` - **Initializes the engine, creates a window and a renderer.** Please add it to your code before using it.
```C
void initEngine(const char* title, int width, int height);
```
   - `title`: **Window title** (not used for a window without a frame). <br>
   - `width`: **The width of the window.** <br>
   - `height`: **The height of the window.** <br>
  <br>
  <br>

- `cleanupEngine' - Just **cleans up the memory and shuts down**, at the end of the code there should be
```C
void cleanupEngine();
```  
 <br>
 <br>

- `renderImage` - Renders an image on the screen
```C
void renderImage(const char* imagePath, int x, int y);
```
  - `ImagePath': **The path to the image.**
  - `x`: **The X coordinate of the upper-left corner.**
  - `y`: **The Y coordinate of the upper-left corner.**
<br>
<br>

- `renderRectangle` - **Will draw a rectangle on the screen**, but why not
```C
void renderRectangle(int x, int y, int width, int height, unsigned int color);
```
   - `x`: **The X coordinate of the upper-left corner.**
   - `y`: **The Y coordinate of the upper-left corner.**
   - `width`: **The width of the rectangle.**
   - `height`: **The height of the rectangle is**.
   - `color`: **Color in ARGB** format (for example, 0xFF0000 for red).
 <br>
 <br>

 - `renderGIF` - **Displays a GIF on the screen, the same as with the image**
```C
void renderGIF(const char* gifPath, int x, int y);
```
  - `gifPath': **The path to the GIF animation.**
  - `x`: **The X coordinate of the upper-left corner.**
  - `y`: **The Y coordinate of the upper-left corner.**
<br>
<br>

 - `startRenderingThread` - **Starts a stream for asynchronous rendering** of images and GIF animations.
```C
void startRenderingThread();
```
<br>
<br>

 - `stopRenderingThread` - Stops the stream for asynchronous rendering of images and GIF animations.
```C
void stopRenderingThread();
``` 
<br>
<br>

- `startRecordingThread` - Starts the program to record audio and convert it to text.
```C
void startRecordingThread();
```
<br>
<br>

 - `stopRecordingThread` -Stops the stream to record audio and convert it to text.
```C
void stopRecordingThread();
```
<br>
<br>

Usage examples 
------------

<h3>Synchronous Operation</h3> 
In this example, we will initialize the engine, render the image and GIF animation, and then shut down the engine.<br>
<br>


```C++
#include <SDL2/SDL.h>
#include "engine.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    initEngine("2D Character Engine", 800, 600);

    if (!window) {
        printf("Failed to initialize the game engine!\n");
        return -1;
    }

    // Clearing the screen in black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw an image in the center of the screen
    renderImage("character.jpg", 350, 250);

    // Drawing a cloud and GIF animation
renderImage("cloud.png", 300, 200);
    renderGIF("speaking.gif", 350, 250);

    // Updating the display
    SDL_RenderPresent(renderer);

    // Waiting for 5 seconds
    SDL_Delay(5000);

    cleanupEngine();
return 0;
}
```
**Compilation**
**For Lixux**
```shell
gcc sync_example.c -o sync_example -L. -lengine -lSDL2 -lSDL2_image -lpthread -lportaudio -lcjson -Wl,-rpath,.
```
**For windows**
```shell
gcc sync_example.c -o sync_example.exe -L. -lengine -lSDL2 -lSDL2_image -lpthread -lportaudio -lcjson
```
**Variation with Python**
```Python
import engine
import time

# Initializing the engine
engine.initEngine("2D Character Engine", 800, 600)

# Clearing the screen in black
engine.renderRectangle(0, 0, 800, 600, 0x000000)

# Draw an image in the center of the screen
engine.renderImage("character.jpg", 350, 250)

# Drawing a cloud and GIF animation
engine.renderImage("cloud.png", 300, 200)
engine.renderGIF("speaking.gif", 350, 250)

# Waiting for 5 seconds
of time.sleep(5)

# Clear the screen and shut down the engine
engine.cleanupEngine()

```
In general, everything is the same.

<br>
<h3>Asynchronous Operation</h3>

In this example, we will initialize the engine, run streams for asynchronous rendering and audio recording, and then shut down the engine. 

```C++
#include <SDL2/SDL.h>
#include "engine.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    initEngine("2D Character Engine", 800, 600);

    if (!window) {
        printf("Failed to initialize the game engine!\n");
        return -1;
    }

    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Clearing the screen in black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Checking for a response
        pthread_mutex_lock(&mutex);
        if (prompt) {
            printf("Prompt: %s\n", prompt);
            // Drawing a cloud and GIF animation
            renderImage("cloud.png", 300, 200);
            renderGIF("speaking.gif", 350, 250);
        }
        pthread_mutex_unlock(&mutex);

        // Draw an image in the center of the screen
        renderImage("character.jpg", 350, 250);

        // Updating the display
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Approximately 60 frames per second
    }

    cleanupEngine();
return 0;
}
```
**Compilation** <br>
It's the same as with the synchronous example.

**Variation with Python**
```Python
import engine
import time

# Initializing the engine
engine.initEngine("2D Character Engine", 800, 600)

# Running streams for rendering and recording audio
engine.startRenderingThread()
engine.startRecordingThread()

try:
    while True:
        # Clear the screen in black
        engine.renderRectangle(0, 0, 800, 600, 0x000000)

        # Draw an image in the center of the screen
        engine.renderImage("character.jpg", 350, 250)

        # Checking for a response
        if engine.prompt:
print(f"Prompt: {engine.prompt}")
# Draw a cloud and a GIF animation
            engine.renderImage("cloud.png", 300, 200)
            engine.renderGIF("speaking.gif", 350, 250)

        # Updating the display
        time.sleep(0.016) # Approximately 60 frames per second

except KeyboardInterrupt:
    pass

finally:
# Stop the threads and shut down the engine
    engine.stopRenderingThread()
    engine.stopRecordingThread()
    engine.cleanupEngine()
```



Dependencies
--------------
Dependencies include both standard headers and some additional ones.
```lst
engine.h # is included in the standard delivery
SDL2 - SDL.h, SDL_image.h, SDL2/SDL_gfxPrimitivies
Portaudio
cjson
stdlib
stdbool
string
pthread
```
<h3>Installation for Linux</h3>

**Debian-based distros**
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libportaudio2 portaudio19-dev libjson-c-dev
```
**Arch-based distros**
```bash
sudo pacman -S sdl2 sdl2_image portaudio json-c base-devel
```
<h3>Installation for windows</h3> 
<br>

1. SDL2: Download the library from [SDL2](https://github.com/libsdl-org/SDL/releases ). <br>
2. SDL2_image: Download the library from [SDL_image](https://github.com/libsdl-org/SDL/releases ). <br>
3. PortAudio: Download the library from [PortAudio](https://www.portaudio.com/download.html ). <br>
4. cJSON: Download the library from [cJSON](https://github.com/DaveGamble/cJSON/releases/tag/v1.7.18 ). <br>

<h3>Cross-platform via vcpkg</h3>

<br>

```bash
vcpkg install sdl2 cjson portaudio
```

Building into a dynamic library
------------

<br>

**For Linux**
```shell
gcc -shared -o libengine.so engine.o -lSDL2 -lSDL2_image -lpthread -lportaudio -lcjson
```
<br>

**For Windows (using MinGW):** 
```shell
gcc -c engine.c -o engine.o -DENGINE_EXPORTS
gcc -shared -o engine.dll engine.o -lSDL2 -lSDL2_image -lpthread -lportaudio -lcjson
```


Variations of the provision
-----------

<h3>Python library</h3>

**Available on PyPI** - `pip install EasyEngineLib`


<h3>C/C++ header</h3>

**Copy the src** and add the compiler flag to the directory where engine.h is located. For example - 
`gcc -I~/include/engine example.c -o example.o `

Self-assembly of the Python library
----------------
If you're messing up and you don't like PyPI, build the library yourself.
```
python setup.py sdist bdist_wheel
```
