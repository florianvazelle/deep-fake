#pragma once

#include <opencv2/opencv.hpp>

/**
 *  @brief Singleton
 */
class DeepFake final {
    public:
        static DeepFake* GetInstance();

        static void analyse(cv::Mat& img);
        void run(const std::string& videoname = "");

    private:
        DeepFake();

        // images extraite du flux vidéos
        cv::Mat prevInput;
        cv::Mat nextInput;

        // les points détectés
        std::vector<cv::Point2f> prevPoints;
        std::vector<cv::Point2f> nextPoints;

        void draw(cv::Mat& img) const;
};