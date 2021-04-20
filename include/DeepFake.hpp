#pragma once

/**
 *  @brief Singleton
 */
class DeepFake final {
    public:
        static DeepFake* GetInstance();

    private:
        DeepFake();
};