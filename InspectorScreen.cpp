#include "InspectorScreen.h"

#include "ActionHistory.h"
#include "EngineBackend.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "UIManager.h"
#include "GameObjectManager.h"
#include "PhysicsComponent.h"

using namespace GDEditor;

InspectorScreen::InspectorScreen() : UIScreen("InspectorScreen")
{
	GDEngine::Logger::log(this, "Initialized");
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
		ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
		ImGui::OpenPopup("SelectComponent");
	}
	ImVec2 buttonSize = ImGui::GetItemRectSize();
	buttonSize.x -= 17;
	buttonSize.y = 0;
	if (ImGui::BeginPopup("SelectComponent"))
	{
		if (ImGui::Selectable("Rigidbody", false, 0, buttonSize))
		{
			// ADD RIGIDBODY TO OBJECT
			m_selectedObject->setPhysics(true);
			m_selectedObject->attachComponent(new PhysicsComponent("PhysicsComponent " + m_selectedObject->getName(), m_selectedObject));
		}
		ImGui::EndPopup();
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

	std::string labels[] = { "Position", "Rotation", "Scale" };

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
				if (!m_hasChanged)
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
			PhysicsComponent* physicsComponent = dynamic_cast<PhysicsComponent*>(component);

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
			if (ImGui::CollapsingHeader("Rigidbody", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.800000190734863f);

				float mass = physicsComponent->getMass();
				float linearDrag = physicsComponent->getLinearDrag();
				float angularDrag = physicsComponent->getAngularDrag();
				bool gravity = physicsComponent->getUseGravity();

				ImGui::DragFloat("Mass", &mass);
				ImGui::DragFloat("Angular Drag", &angularDrag);
				ImGui::Checkbox("Gravity", &gravity);

				BodyType bodyType = physicsComponent->getType();
				const char* items[] = { "Static", "Kinematic", "Dynamic" };
				int item_selected_idx = static_cast<int>(bodyType); // Here we store our selection data as an index.
				// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
				const char* combo_preview_value = items[item_selected_idx];

				if (ImGui::BeginCombo("Body Type", combo_preview_value))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						const bool is_selected = (item_selected_idx == n);
						if (ImGui::Selectable(items[n], is_selected))
							item_selected_idx = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (ImGui::TreeNode("Constraints"))
				{
					bool freezePosX = physicsComponent->getConstraint(PhysicsComponent::EConstraints::FreezePositionX);
					bool freezePosY = physicsComponent->getConstraint(PhysicsComponent::EConstraints::FreezePositionY);
					bool freezePosZ = physicsComponent->getConstraint(PhysicsComponent::EConstraints::FreezePositionZ);
					bool freezeRotX = physicsComponent->getConstraint(PhysicsComponent::EConstraints::FreezeRotationX);
					bool freezeRotY = physicsComponent->getConstraint(PhysicsComponent::EConstraints::FreezeRotationY);
					bool freezeRotZ = physicsComponent->getConstraint(PhysicsComponent::EConstraints::FreezeRotationZ);

					if (ImGui::BeginTable("ConstraintsTable", 4, ImGuiTableFlags_SizingFixedFit))
					{
						ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);
						ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn("Z", ImGuiTableColumnFlags_WidthFixed);

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Freeze Position");
						ImGui::TableNextColumn();
						ImGui::Checkbox("X##Pos", &freezePosX);
						ImGui::TableNextColumn();
						ImGui::Checkbox("Y##Pos", &freezePosY);
						ImGui::TableNextColumn();
						ImGui::Checkbox("Z##Pos", &freezePosZ);

						ImGui::TableNextColumn();
						ImGui::Text("Freeze Rotation");
						ImGui::TableNextColumn();
						ImGui::Checkbox("X##Rot", &freezeRotX);
						ImGui::TableNextColumn();
						ImGui::Checkbox("Y##Rot", &freezeRotY);
						ImGui::TableNextColumn();
						ImGui::Checkbox("Z##Rot", &freezeRotZ);

						ImGui::EndTable();
					}
					ImGui::TreePop();

					physicsComponent->setConstraints(
						static_cast<PhysicsComponent::EConstraints>(
							freezePosX << 0
							| freezePosY << 1
							| freezePosZ << 2
							| freezeRotX << 3
							| freezeRotY << 4
							| freezeRotZ << 5));
				}

				physicsComponent->setMass(mass);
				physicsComponent->setLinearDrag(linearDrag);
				physicsComponent->setAngularDrag(angularDrag);
				physicsComponent->setUseGravity(gravity);
				physicsComponent->setType(static_cast<BodyType>(item_selected_idx));

				std::string buttonName = "Delete##" + component->getName();
				if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowSize().x - 15, 20)))
				{
					gameObject->detachComponent(component);
				}
				ImGui::PopStyleVar();
			}
			ImGui::PopStyleVar();
			ImGui::Separator();
		}

	}
}
