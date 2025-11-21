#include <pch.h>
#include "GameSpeed.h"
#include <src/main.h> 
#include <appdata/helpers.h>

namespace cheat::feature
{
    // Статическая функция-обертка для обновления
    // Она нужна, потому что std::list ожидает обычный указатель на функцию
    static void GameSpeed_OnUpdate_Wrapper() {
        GameSpeed::GetInstance().OnUpdate();
    }

    GameSpeed& GameSpeed::GetInstance() {
        static GameSpeed instance;
        return instance;
    }

    GameSpeed::GameSpeed() : Feature() {
        // Теперь передаем адрес статической функции, а не лямбду
        global::AdventureModuleController_Update.push_back(&GameSpeed_OnUpdate_Wrapper);
    }

    const FeatureGUIInfo& GameSpeed::GetGUIInfo() const {
        static const FeatureGUIInfo info{ "Game Speed", "World", false };
        return info;
    }

    void GameSpeed::DrawMain() {
        ImGui::Text("Select Speed:");
        
        if (ImGui::RadioButton("1.0x (Normal)", &m_SpeedMode, 0)) {}
        ImGui::SameLine();
        if (ImGui::RadioButton("2.0x", &m_SpeedMode, 1)) {}
        ImGui::SameLine();
        if (ImGui::RadioButton("5.0x", &m_SpeedMode, 2)) {}
        
        if (ImGui::RadioButton("Custom", &m_SpeedMode, 3)) {}

        if (m_SpeedMode == 3) {
            ImGui::SliderFloat("##speed_slider", &m_CustomSpeed, 0.1f, 20.0f, "Speed: %.1f");
        }
    }

    void GameSpeed::OnUpdate() {
        float targetSpeed = 1.0f;

        switch (m_SpeedMode) {
            case 0: targetSpeed = 1.0f; break;
            case 1: targetSpeed = 2.0f; break;
            case 2: targetSpeed = 5.0f; break;
            case 3: targetSpeed = m_CustomSpeed; break;
        }

        // Используем нашу новую функцию конвертации (Float -> FP)
        auto speedFP = app::FP_op_Implicit_Float(targetSpeed, nullptr);

        // Устанавливаем скорость через LockStepManager
        app::LockStepManager_set_globalTimeScale(speedFP, nullptr);
    }
}
