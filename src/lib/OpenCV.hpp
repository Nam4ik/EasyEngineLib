#ifndef SCREEN_CAPTURE_H
#define SCREEN_CAPTURE_H

#include <opencv4/opencv2/opencv.hpp>
#include <string>
#include <chrono>
#include <thread>

namespace screen_capture {

    void captureScreenWindows(cv::Mat& screen);
    void captureScreenLinux(cv::Mat& screen);
    bool isSameImage(const cv::Mat& img1, const cv::Mat& img2, double threshold);
    void startMonitoring(const std::string& logFilePath, double threshold, int inactivityThreshold, int sleepDuration);

}

#endif // SCREEN_CAPTURE_H
