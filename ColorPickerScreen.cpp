#include "ColorPickerScreen.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui.h"
#include "Logger.h"

namespace GDEngine {
    ColorPickerScreen::ColorPickerScreen() : UIScreen("ColorPickerScreen")
    {
        isActive = false;
        Logger::log(this, "Initialized");
    }

    ColorPickerScreen::~ColorPickerScreen()
    {
    }

    void ColorPickerScreen::draw()
    {
        ImGui::Begin("Color Picker", &isActive);

        if (ImGui::ColorPicker4("Color", color, ImGuiColorEditFlags_PickerHueWheel))
        {

        };

        ImGui::End();
    }
}