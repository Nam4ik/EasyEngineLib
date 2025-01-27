//#include <checking.h>
#include <windows.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <fstream>
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <pthread>
#include <cstlib>

//FIXFILE FOR OpenCV.cpp 
//Not released and unuseful now.


#ifdef __WIN32
void captureScreen(Mat& screen) {
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
#else 
void captureScreen(Mat& screen) {
   const char* wm = getenv("XDG_SESSION_TYPE");
   if (wm == "wayland")
   {
      std::cerr << "Not supported window server";
      return -1;
   }
   else
   {
      
   }

/*
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
    XCloseDisplay(display);/ */
}
#endif

int main(int argc, cahr argv[])
{
  if (argc != 0)
  {
    if (argv[1] == "--help" || argv[1] == "-h"){
        std::cout << "Использование EzCV" << std::endl;
        std::cout << "
    }
  }
}
