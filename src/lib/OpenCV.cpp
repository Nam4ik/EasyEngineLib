#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <vector>
#include <signal.h>
#include <checking.h>
#include <windows.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <fstream>
#include <chrono>
#include <thread>
#include "get.h"

using namespace std;
using namespace cv;

// Функция для захвата экрана на Windows
void captureScreenWindows(Mat& screen) {
    HWND hwndDesktop = GetDesktopWindow();
    HDC hdcDesktop = GetDC(hwndDesktop);
    HDC hdcCapture = CreateCompatibleDC(hdcDesktop);
    HBITMAP hbmCapture = CreateCompatibleBitmap(hdcDesktop, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    SelectObject(hdcCapture, hbmCapture);
    BitBlt(hdcCapture, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), hdcDesktop, 0, 0, SRCCOPY);

    BITMAP bmp;
    GetObject(hbmCapture, sizeof(BITMAP), &bmp);
    screen.create(bmp.bmHeight, bmp.bmWidth, CV_8UC4);
    GetBitmapBits(hbmCapture, bmp.bmWidthBytes * bmp.bmHeight, screen.data);

    DeleteObject(hbmCapture);
    DeleteDC(hdcCapture);
    ReleaseDC(hwndDesktop, hdcDesktop);
}

// Функция для захвата экрана на Linux
void captureScreenLinux(Mat& screen) {
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        cerr << "Cannot open display" << endl;
        return;
    }
    Window root = DefaultRootWindow(display);
    XWindowAttributes window_attributes;
    XGetWindowAttributes(display, root, &window_attributes);
    int width = window_attributes.width;
    int height = window_attributes.height;
    screen.create(height, width, CV_8UC4);
    XImage* x_image = XGetImage(display, root, 0, 0, width, height, AllPlanes, ZPixmap);
    if (x_image) {
        memcpy(screen.data, x_image->data, width * height * 4);
        cvtColor(screen, screen, COLOR_BGRA2BGR);
        XDestroyImage(x_image);
    }
    XCloseDisplay(display);
}

bool isSameImage(const Mat& img1, const Mat& img2, double threshold) {
    Mat gray1, gray2;
    cvtColor(img1, gray1, COLOR_BGRA2GRAY);
    cvtColor(img2, gray2, COLOR_BGRA2GRAY);

    Mat diff;
    absdiff(gray1, gray2, diff);
    double nonZeroCount = countNonZero(diff);
    return (nonZeroCount < threshold);
}

int main() {
    ofstream logFile("fgui_stdout.txt");
    Mat prevScreen, currScreen;
    const double threshold = 1000;
    const int inactivityThreshold = 10; //TODO Нужно сделать это чтобы можно было менять из графических настроек
    int inactivityTime = 0;

    while (true) {
        #ifdef _WIN32
            captureScreenWindows(currScreen);
        #else
            captureScreenLinux(currScreen);
        #endif

        if (prevScreen.empty()) {
            prevScreen = currScreen.clone();
            this_thread::sleep_for(chrono::seconds(1));
            continue;
        }

        if (isSameImage(prevScreen, currScreen, threshold)) {
            inactivityTime++;
            if (inactivityTime >= inactivityThreshold) {
                logFile << "No activity detected for " << inactivityTime << " seconds.\n";
                // Отправка лог-файла на email
                get::send("fgui_stdout.txt");
            }
        } else {
            inactivityTime = 0;
        }

        prevScreen = currScreen.clone();
        this_thread::sleep_for(chrono::seconds(120));  //TODO Чтоб мозги не ебала тоже настраиваться должно
    }

    logFile.close();
    return 0;
}
