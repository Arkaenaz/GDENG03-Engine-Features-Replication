#pragma comment(lib, "rpcrt4.lib")

#include "HierarchyScreen.h"

#include "imgui.h"
#include "UIManager.h"
#include "GameObjectManager.h"

namespace GDEngine
{
HierarchyScreen::HierarchyScreen() : UIScreen("HierarchyScreen")
{
	Logger::log(this, "Initialized");
}

HierarchyScreen::~HierarchyScreen()
{
}

void HierarchyScreen::draw()
{
	ImGui::SetNextWindowSize(ImVec2(UIManager::WINDOW_WIDTH / 6, UIManager::WINDOW_HEIGHT), ImGuiCond_Once);
	ImGui::Begin("Hierarchy", &isActive);
	
	std::vector<AGameObject*> list = GameObjectManager::getInstance()->getAllObjects();

	for (AGameObject* gameObject : list)
	{
		std::string guidString = gameObject->getGuidString();
		
		std::string label = gameObject->getName() + "###" + guidString;

		if (ImGui::Button(label.c_str(), ImVec2(ImGui::GetWindowSize().x - 15, 20)))
		{
			Logger::log("Selected : " + label);
			GameObjectManager::getInstance()->setSelectedObject(gameObject);
		}
	}

	ImGui::End();
}
}