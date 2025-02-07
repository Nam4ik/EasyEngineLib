#include "OpenCV.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
#endif

extern "C"{
    void captureScreen(cv::Mat& output);
    bool isSameImage(const cv::Mat& img1, const cv::Mat& img2, double threshold = 1000.0);
    void startMonitoring(const std::string& logFilePath = "activity.log",
                        double threshold = 1000.0,
                        int inactivityThreshold = 600,
                        int checkInterval = 60);
}


//gcc OpenCV.cpp -o -shared OpenCV.so -fPIC -lthread 

//TODO: optimize that fith multythreading 
//TODO: Gui.....

using namespace cv;
using namespace std;

namespace ScreenCapture {

void captureScreen(Mat& output) {
#ifdef _WIN32
    HWND hDesktop = GetDesktopWindow();
    HDC hdc = GetDC(hDesktop);
    int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    HDC hCaptureDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hCaptureDC, hBitmap);
    BitBlt(hCaptureDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

    BITMAPINFOHEADER bi = {0};
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;

    output.create(height, width, CV_8UC4);
    GetDIBits(hdc, hBitmap, 0, height, output.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    ReleaseDC(hDesktop, hdc);
    DeleteDC(hCaptureDC);
    DeleteObject(hBitmap);
#else
    Display* display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(display);
    XWindowAttributes attributes;
    XGetWindowAttributes(display, root, &attributes);

    XImage* img = XGetImage(display, root, 0, 0, 
                          attributes.width, 
                          attributes.height, 
                          AllPlanes, ZPixmap);

    if (img) {
        output.create(attributes.height, attributes.width, CV_8UC4);
        memcpy(output.data, img->data, output.total() * output.elemSize());
        XDestroyImage(img);
        cvtColor(output, output, COLOR_BGRA2BGR);
    }
    XCloseDisplay(display);
#endif
}

bool isSameImage(const Mat& img1, const Mat& img2, double threshold) {
    if (img1.size() != img2.size() || img1.type() != img2.type())
        return false;

    Mat diff;
    absdiff(img1, img2, diff);
    cvtColor(diff, diff, COLOR_BGR2GRAY);
    threshold(diff, diff, 32, 255, THRESH_BINARY);
    return (countNonZero(diff) < threshold);
}

void startMonitoring(const std::string& logFilePath,
                    double threshold,
                    int inactivityThreshold,
                    int checkInterval) {
    ofstream logFile(logFilePath, ios::app);
    if (!logFile.is_open()) {
        cerr << "Error opening log file!" << endl;
        return;
    }

    Mat prevFrame, currentFrame;
    int inactiveSeconds = 0;

    while (true) {
        captureScreen(currentFrame);

        if (!prevFrame.empty()) {
            if (isSameImage(prevFrame, currentFrame, threshold)) {
                inactiveSeconds += checkInterval;
                logFile << "Inactivity detected: " << inactiveSeconds 
                       << "/" << inactivityThreshold << " seconds\n";
                
                if (inactiveSeconds >= inactivityThreshold) {
                    logFile << "ALERT: Maximum inactivity reached!\n";
                    // Здесь можно добавить отправку email
                }
            } else {
                inactiveSeconds = 0;
            }
        }

        prevFrame = currentFrame.clone();
        this_thread::sleep_for(chrono::seconds(checkInterval));
    }
}

} // namespace ScreenCapture
