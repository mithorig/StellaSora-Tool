#pragma once
#include <src/Feature.h>

namespace cheat::feature
{
    class GameSpeed : public Feature
    {
    public:
        // Режимы скорости: 0=1x, 1=2x, 2=5x, 3=Custom
        int m_SpeedMode = 0; 
        float m_CustomSpeed = 1.0f;

        static GameSpeed& GetInstance();

        const FeatureGUIInfo& GetGUIInfo() const override;
        void DrawMain() override;

        // Функция, которая будет вызываться каждый кадр
        void OnUpdate();

    private:
        GameSpeed();
    };
}
