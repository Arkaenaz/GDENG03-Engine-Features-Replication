#include "GizmoLayer.h"

#ifndef IMGUIZMO_IMGUI_FOLDER 
#define IMGUIZMO_IMGUI_FOLDER
#endif

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "GameObjectManager.h"
#include "CameraManager.h"
#include "imgui.h"
#include "imGuIZMOquat.h"
#include "vgMath.h"
#include "ViewportScreen.h"
#include "libs/imguizmo/ImGuizmo.h"

namespace GDEngine
{
	GizmoLayer* GizmoLayer::P_SHARED_INSTANCE = nullptr;

	GizmoLayer* GizmoLayer::getInstance()
	{
		return P_SHARED_INSTANCE;
	}

	void GizmoLayer::initialize()
	{
		P_SHARED_INSTANCE = new GizmoLayer();
	}

	void GizmoLayer::destroy()
	{
		delete P_SHARED_INSTANCE;
	}

	void GizmoLayer::draw(ViewportScreen* viewport)
	{
		std::string label = "Gizmos " + viewport->getName();
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize /*| ImGuiWindowFlags_NoMove*/ | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::Begin(label.c_str(), nullptr, windowFlags);

		Vector3D cameraRotation = viewport->getCamera()->getLocalRotation();
		quat rotation(1.0f, cameraRotation.x, cameraRotation.y, cameraRotation.z);

		Vector3D cameraLocation = viewport->getCamera()->getLocalPosition();
		vec3 translation(cameraLocation.x, cameraLocation.y, cameraLocation.z);

		if (ImGui::gizmo3D("Camera Rotate", rotation, 100, imguiGizmo::mode3Axes | imguiGizmo::cubeAtOrigin))
		{
			viewport->getCamera()->setRotation(rotation.x, rotation.y, rotation.z);
		}
		ImGui::SameLine();
		if (ImGui::gizmo3D("Camera Translate", translation, 100, imguiGizmo::modePanDolly | imguiGizmo::cubeAtOrigin))
		{
			viewport->getCamera()->setPosition(translation.x, translation.y, translation.z);
		}

		ImGui::Checkbox("Show Grid", &m_gridShow);

		viewport->getCamera()->setRotation(rotation.x, rotation.y, rotation.z);
		viewport->getCamera()->setPosition(translation.x, translation.y, translation.z);

		if (ImGui::RadioButton("Translate", m_translate))
		{
			m_translate = true;
			m_rotate = false;
			m_scale = false;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", m_rotate))
		{
			m_translate = false;
			m_rotate = true;
			m_scale = false;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", m_scale))
		{
			m_translate = false;
			m_rotate = false;
			m_scale = true;
		}

		ImGui::End();
		ImGuizmo::BeginFrame();
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

		Matrix4x4 viewMatrix = CameraManager::getInstance()->getSceneCameraViewMatrix();
		Matrix4x4 projectionMatrix = CameraManager::getInstance()->getSceneCameraProjMatrix();
		Matrix4x4 locationMatrix = CameraManager::getInstance()->getSceneCamera()->getLocMatrix();

		float* viewMatrix16 = viewMatrix.getMatrix();
		float* projectionMatrix16 = projectionMatrix.getMatrix();
		float* locationMatrix16 = locationMatrix.getMatrix();

		if (m_gridShow)
		{
			Matrix4x4 identity;
			identity.setIdentity();
			float* identityMatrix16 = identity.getMatrix();
			ImGuizmo::DrawGrid(viewMatrix16, projectionMatrix16, identityMatrix16, 100);
		}

		m_selectedObject = GameObjectManager::getInstance()->getSelectedObject();
		if (m_selectedObject != nullptr)
		{
			float* locationMatrix = m_selectedObject->getLocalMatrix();

			if (m_translate)
				Manipulate(viewMatrix16, projectionMatrix16, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, locationMatrix);
			if (m_rotate)
				Manipulate(viewMatrix16, projectionMatrix16, ImGuizmo::ROTATE, ImGuizmo::LOCAL, locationMatrix);
			if (m_scale)
				Manipulate(viewMatrix16, projectionMatrix16, ImGuizmo::SCALE, ImGuizmo::LOCAL, locationMatrix);

			if (m_translate || m_rotate || m_scale)
			{
				float translate[3] = { 0.0f, 0.0f, 0.0f };
				float rotate[3] = { 0.0f, 0.0f, 0.0f };
				float scale[3] = { 0.0f, 0.0f, 0.0f };
				ImGuizmo::DecomposeMatrixToComponents(locationMatrix, translate, rotate, scale);

				m_selectedObject->setPosition(translate[0], translate[1], translate[2]);
				m_selectedObject->setRotation(rotate[0], rotate[1], rotate[2]);
				m_selectedObject->setScale(scale[0], scale[1], scale[2]);
			}
		}
	}

	void GizmoLayer::enableGizmo(AGameObject* selectedObject)
	{
		m_selectedObject = selectedObject;
	}

	GizmoLayer::GizmoLayer()
	{
	}

	GizmoLayer::~GizmoLayer()
	{
	}
}
