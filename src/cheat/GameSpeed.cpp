#include <pch.h>
#include "GameSpeed.h"
#include <src/main.h> // Нужно для доступа к global::AdventureModuleController_Update
#include <appdata/helpers.h> // Для конвертации float -> FP

namespace cheat::feature
{
    GameSpeed& GameSpeed::GetInstance() {
        static GameSpeed instance;
        return instance;
    }

    GameSpeed::GameSpeed() : Feature() {
        // Сеньорский мув: подписываемся на существующий апдейт-луп, 
        // чтобы применять скорость каждый кадр без лишних хуков.
        global::AdventureModuleController_Update.push_back([this]() {
            this->OnUpdate();
        });
    }

    const FeatureGUIInfo& GameSpeed::GetGUIInfo() const {
        static const FeatureGUIInfo info{ "Game Speed", "World", false };
        return info;
    }

    void GameSpeed::DrawMain() {
        ImGui::Text("Select Speed:");
        
        // Радио-кнопки для быстрого переключения
        if (ImGui::RadioButton("1.0x (Normal)", &m_SpeedMode, 0)) {
            // Можно добавить логику при переключении, если нужно
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("2.0x", &m_SpeedMode, 1)) {}
        ImGui::SameLine();
        if (ImGui::RadioButton("5.0x", &m_SpeedMode, 2)) {}
        
        // Опция своей скорости
        if (ImGui::RadioButton("Custom", &m_SpeedMode, 3)) {}

        // Если выбран кастомный режим, показываем слайдер
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

        // Если скорость 1.0x (норма), можно ничего не делать, чтобы не ломать катсцены,
        // но лучше принудительно ставить 1.0, чтобы сбросить ускорение.
        
        // Конвертируем float в FixedPoint (FP), который использует игра
        // app::FP_op_Implicit_1 обычно конвертирует float -> FP. Проверь helpers.h если имя отличается.
        auto speedFP = app::FP_op_Implicit_1(targetSpeed, nullptr);

        // Применяем скорость
        app::LockStepManager_set_globalTimeScale(speedFP, nullptr);
    }
}
