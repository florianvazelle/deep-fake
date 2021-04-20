#include <DeepFake.hpp>

#define FACE_RECOGNITION_MODEL "assets/models/dlib_face_recognition_resnet_model_v1.dat"

DeepFake* DeepFake::GetInstance() {
    // Not thread safe
    static DeepFake* instance = new DeepFake();
    return instance;
}

DeepFake::DeepFake() {
    dlib::deserialize(FACE_RECOGNITION_MODEL) >> face_landmark;
}