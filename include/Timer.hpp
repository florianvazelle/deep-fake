#pragma once

#include <opencv2/opencv.hpp>

class Timer final {
    public:
        Timer() : fps(0) {}

        inline void start() {
            time_start = cv::getTickCount();
        }

        inline void display() {
            // On calcule les fps pour
            auto time_end = cv::getTickCount();
            auto time_per_frame = (time_end - time_start) / cv::getTickFrequency();

            fps = (15 * fps + (1 / time_per_frame)) / 16;

            printf("Total time: %3.5f | FPS: %3.2f\n", time_per_frame, fps);
        }

    private:
        float fps;
        int64 time_start;
};