#include "PlaybackScreen.h"

#include "EngineBackend.h"
#include "imgui.h"
#include "Logger.h"

namespace GDEngine
{
	PlaybackScreen::PlaybackScreen() : UIScreen("PlaybackScreen")
	{
		Logger::log(this, "Initialized");
	}

	PlaybackScreen::~PlaybackScreen()
	{
	}

	void PlaybackScreen::draw()
	{
		ImGui::Begin("Playback", &isActive);

		EngineBackend* backend = EngineBackend::getInstance();
		if (ImGui::Button("Play"))
		{
			if (backend->getMode() == EngineBackend::EDITOR)
			{
				backend->setMode(EngineBackend::PLAY);
			}
			else
			{
				backend->setMode(EngineBackend::EDITOR);
			}

		}
		if (ImGui::Button("Pause"))
		{
			if (backend->getMode() == EngineBackend::PLAY)
			{
				backend->setMode(EngineBackend::PAUSED);
			}
			else if (backend->getMode() == EngineBackend::PAUSED)
			{
				backend->setMode(EngineBackend::PLAY);
			}
		}
		if (ImGui::Button("Step"))
		{
			if (backend->getMode() == EngineBackend::PAUSED)
			{
				backend->startFrameStep();
			}
		}

		ImGui::End();
	}
}
