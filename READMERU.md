<h1>Использование</h1>

__В данный момент это мой пет проект и он еще в разработке. Он будет учавствовать в других проектах когда перестанет быть сыроватым. Но я думаю никому это не интересно по этому перейду к документации.__ 

Возможности
--------------
Как и написано в описании может рендерить .gif, .jpg и .png. Отрисовка происходит относительно заданных в функции координат. Поддерживает асинхронную работу и рендер сразу нескольких картинок или гифок. 
<h3>Функции</h3> 

- `initEngine` - **Инициализирует движок, создает окно и рендерер.** Кароче перед использованием его добавьте в ваш код.
```C
void initEngine(const char* title, int width, int height);
```
 - `title`: **Заголовок окна** (не используется для окна без рамки). <br>
 - `width`: **Ширина окна.** <br>
 - `height`: **Высота окна.** <br>
  <br>
  <br>

- `cleanupEngine` - Просто **очищает память и завершает работу**, в конце кода должна быть
```C
void cleanupEngine();
```  
 <br>
 <br>

- `renderImage` - Отрисовываает картинку на экране
```C
void renderImage(const char* imagePath, int x, int y);
```
 - `imagePath`: **Путь к изображению.**
 - `x`: **Координата X верхнего левого угла.**
 - `y`: **Координата Y верхнего левого угла.**
<br>
<br>

- `renderRectangle` - **Нарисует прямоугольник на экране**, а че бы и нет
```C
void renderRectangle(int x, int y, int width, int height, unsigned int color);
```
 - `x`: **Координата X верхнего левого угла.**
 - `y`: **Координата Y верхнего левого угла.**
 - `width`: **Ширина прямоугольника.**
 - `height`: **Высота прямоугольника**.
 - `color`: **Цвет в формате ARGB** (например, 0xFF0000 для красного цвета).
 <br>
 <br>

 - `renderGIF` - **Отображает GIF на экране, тоже самое что и с изображением**
```C
void renderGIF(const char* gifPath, int x, int y);
```
  - `gifPath`: **Путь к GIF-анимации.**
  - `x`: **Координата X верхнего левого угла.**
  - `y`: **Координата Y верхнего левого угла.**
<br>
<br>

 - `startRenderingThread` - **Запускает поток для асинхронной отрисовки** изображений и GIF-анимаций.
```C
void startRenderingThread();
```
<br>
<br>

 - `stopRenderingThread` - Останавливает поток для асинхронной отрисовки изображений и GIF-анимаций.
```C
void stopRenderingThread();
``` 
<br>
<br>

- `startRecordingThread` - Запускает потом для записи аудио и преобразования его в текст.
```C
void startRecordingThread();
```
<br>
<br>

 - `stopRecordingThread` -Останавливает поток для записи аудио и преобразования его в текст. 
```C
void stopRecordingThread();
```
<br>
<br>

Примеры использования 
------------

<h3>Синхронная Работа</h3> 
В этом примере мы инициализируем движок, отрисуем изображение и GIF-анимацию, а затем завершим работу движка.<br>
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

    // Очищаем экран черным цветом
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Рисуем изображение в центре экрана
    renderImage("character.jpg", 350, 250);

    // Рисуем облако и GIF-анимацию
    renderImage("cloud.png", 300, 200);
    renderGIF("speaking.gif", 350, 250);

    // Обновляем отображение
    SDL_RenderPresent(renderer);

    // Ждем 5 секунд
    SDL_Delay(5000);

    cleanupEngine();
    return 0;
}
```
**Компиляция**
**Для Lixux**
```shell
gcc sync_example.c -o sync_example -L. -lengine -lSDL2 -lSDL2_image -lpthread -lportaudio -lcjson -Wl,-rpath,.
```
**Для windows**
```shell
gcc sync_example.c -o sync_example.exe -L. -lengine -lSDL2 -lSDL2_image -lpthread -lportaudio -lcjson
```
**Вариация с Python**
```Python
import engine
import time

# Инициализируем движок
engine.initEngine("2D Character Engine", 800, 600)

# Очищаем экран черным цветом
engine.renderRectangle(0, 0, 800, 600, 0x000000)

# Рисуем изображение в центре экрана
engine.renderImage("character.jpg", 350, 250)

# Рисуем облако и GIF-анимацию
engine.renderImage("cloud.png", 300, 200)
engine.renderGIF("speaking.gif", 350, 250)

# Ждем 5 секунд
time.sleep(5)

# Очищаем экран и завершаем работу движка
engine.cleanupEngine()

```
В общем все тоже самое.

<br>
<h3>Асинхронная Работа</h3>

В этом примере мы инициализируем движок, запустим потоки для асинхронной отрисовки и записи аудио, а затем завершим работу движка. 

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

        // Очищаем экран черным цветом
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Проверяем наличие ответа
        pthread_mutex_lock(&mutex);
        if (prompt) {
            printf("Prompt: %s\n", prompt);
            // Рисуем облако и GIF-анимацию
            renderImage("cloud.png", 300, 200);
            renderGIF("speaking.gif", 350, 250);
        }
        pthread_mutex_unlock(&mutex);

        // Рисуем изображение в центре экрана
        renderImage("character.jpg", 350, 250);

        // Обновляем отображение
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Приблизительно 60 кадров в секунду
    }

    cleanupEngine();
    return 0;
}
```
**Компиляция** <br>
Все тоже самое что и с синхронным примером.

**Вариация с Python**
```Python
import engine
import time

# Инициализируем движок
engine.initEngine("2D Character Engine", 800, 600)

# Запускаем потоки для отрисовки и записи аудио
engine.startRenderingThread()
engine.startRecordingThread()

try:
    while True:
        # Очищаем экран черным цветом
        engine.renderRectangle(0, 0, 800, 600, 0x000000)

        # Рисуем изображение в центре экрана
        engine.renderImage("character.jpg", 350, 250)

        # Проверяем наличие ответа
        if engine.prompt:
            print(f"Prompt: {engine.prompt}")
            # Рисуем облако и GIF-анимацию
            engine.renderImage("cloud.png", 300, 200)
            engine.renderGIF("speaking.gif", 350, 250)

        # Обновляем отображение
        time.sleep(0.016)  # Приблизительно 60 кадров в секунду

except KeyboardInterrupt:
    pass

finally:
    # Останавливаем потоки и завершаем работу движка
    engine.stopRenderingThread()
    engine.stopRecordingThread()
    engine.cleanupEngine()
```



Зависимости
--------------
Зависимости включают в себя стандартные хедеры ,так и некоторые дополнительные.
```lst
engine.h #Включен в стандартную поставку
SDL2 - SDL.h, SDL_image.h, SDL2/SDL_gfxPrimitivies
Portaudio
cjson
stdlib
stdbool
string
pthread
```
<h3>Установка для Linux</h3>

**Debian-based distros**
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libportaudio2 portaudio19-dev libjson-c-dev
```
**Arch-based distros**
```bash
sudo pacman -S sdl2 sdl2_image portaudio json-c base-devel
```
<h3>Установка для windows</h3> 
<br>

1. SDL2: Скачайте библиотеку с [SDL2](https://github.com/libsdl-org/SDL/releases). <br>
2. SDL2_image: Скачайте библиотеку с [SDL_image](https://github.com/libsdl-org/SDL/releases). <br>
3. PortAudio: Скачайте библиотеку с [PortAudio](https://www.portaudio.com/download.html). <br>
4. cJSON: Скачайте библиотеку с [cJSON](https://github.com/DaveGamble/cJSON/releases/tag/v1.7.18). <br>

<h3>Кросплатформа через vcpkg</h3>

<br>

```bash
vcpkg install sdl2 cjson portaudio
```

Сборка в динамическую библиотеку
------------

<br>

**Для Linux**
```shell
gcc -shared -o libengine.so engine.o -lSDL2 -lSDL2_image -lpthread -lportaudio -lcjson
```
<br>

**Для Windows  (используя MinGW):** 
```shell
gcc -c engine.c -o engine.o -DENGINE_EXPORTS
gcc -shared -o engine.dll engine.o -lSDL2 -lSDL2_image -lpthread -lportaudio -lcjson
```


Вариации предоставления
-----------

<h3>Python библиотека</h3>

**Есть на PyPi** - `pip install EasyEngineLib`


<h3>C/C++ заголовок</h3>

**Скопируйте src** и добавьте компилятору флаг к директории в которой распологается engine.h. Например - 
`gcc -I~/include/engine example.c -o example.o `

Самостоятельная сборка библиотеки Python
----------------
Если у вас шалит в жопе и вам не нравиться PyPi соберите библиотеку сами.
```
python setup.py sdist bdist_wheel
```
