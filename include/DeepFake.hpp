#pragma once

#include <Image.hpp>

/**
 *  @brief Singleton
 */
class DeepFake final {
    public:
        static DeepFake* GetInstance();

        void run(const std::string& filename) const;

    private:
        DeepFake();
};