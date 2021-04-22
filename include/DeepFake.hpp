#pragma once

#include <Image.hpp>

/**
 *  @brief DeepFake est l'objet représentant notre application
 */
class DeepFake final {
    public:
        static DeepFake* GetInstance();

        void run(const std::string& filename) const;

    private:
        DeepFake();
};