#ifndef SCREEN_CAPTURE_H
#define SCREEN_CAPTURE_H

#include <opencv2/opencv.hpp>
#include <string>

namespace ScreenCapture {
    void captureScreen(cv::Mat& output);
    bool isSameImage(const cv::Mat& img1, const cv::Mat& img2, double threshold = 1000.0);
    void startMonitoring(const std::string& logFilePath = "activity.log",
                        double threshold = 1000.0,
                        int inactivityThreshold = 600,
                        int checkInterval = 60);
}

#endif // SCREEN_CAPTURE_H
