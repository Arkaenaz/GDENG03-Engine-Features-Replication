#pragma once

#ifndef IMGUIZMO_IMGUI_FOLDER 
#define IMGUIZMO_IMGUI_FOLDER
#endif

#include "GameObject.h"

namespace GDEngine
{
	class ViewportScreen;
	class GizmoLayer
	{
	public:
		static GizmoLayer* getInstance();
		static void initialize();
		static void destroy();

		void draw(ViewportScreen* viewport);
		void enableGizmo(AGameObject* selectedObject);

	private:
		GizmoLayer();
		~GizmoLayer();
		GizmoLayer(GizmoLayer const&) {}
		GizmoLayer& operator=(GizmoLayer const&) {}

		static GizmoLayer* P_SHARED_INSTANCE;

		bool m_gridShow = true;
		bool m_translate = true;
		bool m_rotate = false;
		bool m_scale = false;
		AGameObject* m_selectedObject = nullptr;
	};
}

