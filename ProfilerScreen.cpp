#include "ProfilerScreen.h"

#include "imgui.h"
#include "UIManager.h"

ProfilerScreen::ProfilerScreen() : UIScreen("ProfilerScreen")
{
    debug::Logger::log(this, "Initialized");
}

ProfilerScreen::~ProfilerScreen()
{
}

void ProfilerScreen::draw()
{
    ImGui::Begin("Profiler", &isActive);

    ImGui::Text("%.3f ms/frame, %.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //ImGui::PlotLines()

    ImGui::End();
}