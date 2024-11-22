#include "InspectorScreen.h"

#include "ActionHistory.h"
#include "EngineBackend.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "UIManager.h"
#include "GameObjectManager.h"

using namespace GDEditor;

InspectorScreen::InspectorScreen() : UIScreen("InspectorScreen")
{
	Logger::log(this, "Initialized");
}

InspectorScreen::~InspectorScreen()
{
}

void InspectorScreen::draw()
{
	ImGui::SetNextWindowSize(ImVec2(UIManager::WINDOW_WIDTH / 6, UIManager::WINDOW_HEIGHT), ImGuiCond_Once);
	ImGui::Begin("Inspector", &isActive);

	m_selectedObject = GameObjectManager::getInstance()->getSelectedObject();
	if (m_selectedObject != nullptr)
	{
		this->drawInspector();
	}
	
	ImGui::End();
}

void InspectorScreen::drawInspector()
{
	std::string name = m_selectedObject->getName();
	bool isActive = m_selectedObject->isActive();

	if (ImGui::Checkbox("##Active", &isActive))
	{
		m_selectedObject->setActive(isActive);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1);
	if (ImGui::InputText("##Name", &name))
	{
		if (ImGui::IsItemDeactivatedAfterEdit())
			m_selectedObject->setName(name);
	}

	this->drawTransformTable(m_selectedObject);
	this->drawComponentList(m_selectedObject);

	if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowSize().x - 15, 20)))
	{
		
	}

	if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowSize().x - 15, 20)))
	{
		GameObjectManager::getInstance()->setSelectedObject(nullptr);
		GameObjectManager::getInstance()->deleteObject(m_selectedObject);
	}
}

void InspectorScreen::drawTransformTable(AGameObject* gameObject)
{
	int rows = 3;

	std::string labels[] = { "Position", "Rotation", "Scale"};

	Vector3D vectorValues[3];
	vectorValues[0] = gameObject->getLocalPosition();
	vectorValues[1] = gameObject->getLocalRotation();
	vectorValues[2] = gameObject->getLocalScale();

	if (ImGui::BeginTable("Transform", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Values", ImGuiTableColumnFlags_WidthStretch);

		for (int i = 0; i < rows; i++)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text(labels[i].c_str());
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-1);
			float values[3] = { vectorValues[i].x, vectorValues[i].y, vectorValues[i].z };
			std::string dragLabel = "##" + labels[i];

			if (ImGui::DragFloat3(dragLabel.c_str(), values, 0.01f))
			{
				if(!m_hasChanged)
				{
					ActionHistory::getInstance()->recordAction(gameObject);
				}

				m_hasChanged = true;

				switch (i)
				{
				case 0:
					gameObject->setPosition(Vector3D(values[0], values[1], values[2]));
					break;
				case 1:
					gameObject->setRotation(Vector3D(values[0], values[1], values[2]));
					break;
				case 2:
					gameObject->setScale(Vector3D(values[0], values[1], values[2]));
					break;
				}
			}

			if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				m_isLeftDown = true;
			}
			else
			{
				m_isLeftDown = false;
			}

			if (m_hasChanged && !m_isLeftDown)
			{
				m_hasChanged = false;
			}
		}

		ImGui::EndTable();
	}
}

void InspectorScreen::drawComponentList(AGameObject* gameObject)
{
	AGameObject::ComponentList componentList = gameObject->getComponentsOfType(AComponent::Physics);
	for (AComponent* component : componentList)
	{
		if (component->getName().find("PhysicsComponent") != std::string::npos)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
			if (ImGui::CollapsingHeader("Rigidbody", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.800000190734863f);
				float mass; // TODO: place mass here
				bool gravity; // TODO: place gravity here
				ImGui::DragFloat("Mass", &mass);
				ImGui::Checkbox("Gravity", &gravity);

				// TODO: set mass and gravity

				ImGui::PopStyleVar();

				std::string buttonName = "Delete##" + component->getName();
				if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowSize().x - 15, 20)))
				{
					component->detachOwner();
				}
			}
			ImGui::PopStyleVar();
		}
	}
}
