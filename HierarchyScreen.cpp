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


    for (AGameObject* rootObject : list) 
    {
        if (rootObject->getParent() == nullptr)
        {
            displayNode(rootObject); 
        }
    }

	ImGui::End();
}

void HierarchyScreen::displayNode(AGameObject* gameObject)
{
    std::string guidString = gameObject->getGuidString();
    std::string label = gameObject->getName() + "###" + guidString;

    bool hasChildren = !gameObject->getChildren().empty();
    bool isExpanded = m_expandedState[guidString];

    static bool hasValidDropTarget = false;

    AGameObject* selectedObject = GameObjectManager::getInstance()->getSelectedObject();

    if (hasChildren)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (selectedObject == gameObject)
            flags |= ImGuiTreeNodeFlags_Selected;
        if (isExpanded)
            flags |= ImGuiTreeNodeFlags_DefaultOpen;

        if (ImGui::TreeNodeEx(label.c_str(), flags))
        {
            if (ImGui::IsItemClicked())
            {
                GameObjectManager::getInstance()->setSelectedObject(gameObject);
            }

            // Start Drag-and-Drop Source
            if (ImGui::BeginDragDropSource())
            {
                GameObjectManager::getInstance()->setSelectedObject(gameObject);
                ImGui::SetDragDropPayload("PARENTING", &gameObject, sizeof(AGameObject*));

                hasValidDropTarget = false;

                ImGui::EndDragDropSource();
            }

            // Handle Drop Target
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PARENTING"))
                {
                    AGameObject* draggedObject = *(AGameObject**)payload->Data;

                    if (draggedObject && draggedObject != gameObject && !gameObject->isDescendant(draggedObject))
                    {
                        draggedObject->setParent(gameObject);
                        m_expandedState[guidString] = true;
                        hasValidDropTarget = true;
                    }
                }

                ImGui::EndDragDropTarget();
            }

            // Recursively display child nodes
            for (AGameObject* child : gameObject->getChildren())
            {
                displayNode(child);
            }

            ImGui::TreePop();
        }

        if (ImGui::IsItemHovered())
        {
            m_expandedState[guidString] = true;
        }
    }
    else
    {
        if (ImGui::Selectable(label.c_str(), GameObjectManager::getInstance()->getSelectedObject() == gameObject))
        {
            GameObjectManager::getInstance()->setSelectedObject(gameObject);
        }

        // Start Drag-and-Drop Source
        if (ImGui::BeginDragDropSource())
        {
            GameObjectManager::getInstance()->setSelectedObject(gameObject);
            ImGui::SetDragDropPayload("PARENTING", &gameObject, sizeof(AGameObject*));
            hasValidDropTarget = false;
            ImGui::EndDragDropSource();
        }

        // Handle Drop Target for leaf nodes
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PARENTING"))
            {
                AGameObject* draggedObject = *(AGameObject**)payload->Data;

                if (draggedObject && draggedObject != gameObject && !gameObject->isDescendant(draggedObject))
                {
                    draggedObject->setParent(gameObject);
                    m_expandedState[guidString] = true;
                    hasValidDropTarget = true;
                }
            }

            ImGui::EndDragDropTarget();
        }
    }

    // Dropped to hierarchy screen
    if (ImGui::IsWindowHovered() && ImGui::IsMouseReleased(0) && !ImGui::IsAnyItemHovered() && !hasValidDropTarget)
    {
        AGameObject* selectedObject = GameObjectManager::getInstance()->getSelectedObject();

        if (selectedObject)
        {
            Logger::log("Detached: " + selectedObject->getName() + " from its parent.");

            selectedObject->setParent(nullptr);

            selectedObject->setPosition(selectedObject->getWorldPosition());

        }
    }
}


}
