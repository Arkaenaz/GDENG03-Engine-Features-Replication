#pragma once
#include "GameObject.h"
#include "UIScreen.h"

namespace GDEditor
{
	using namespace GDEngine;
	class InspectorScreen : public UIScreen
	{
	private:
		friend class UIManager;

	private:
		InspectorScreen();
		~InspectorScreen();

	private:
		virtual void draw() override;
		void drawInspector();
		void drawTransformTable(AGameObject* gameObject);
		void drawComponentList(AGameObject* gameObject);

	private:
		AGameObject* m_selectedObject = nullptr;
		float m_position[3];
		float m_scale[3];
		float m_rotation[3];
		bool m_hasChanged = false;
		bool m_isLeftDown = false;

	};
}