#pragma once

#include <dlib/image_processing.h>

/**
 *  @brief Singleton
 */
class DeepFake final {
    public:
        static DeepFake* GetInstance();

    private:
        DeepFake();

        // Modèle de repérage de visage
        dlib::shape_predictor face_landmark;
};