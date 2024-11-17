#pragma once
#include "UIScreen.h"

namespace GDEditor
{
	class HierarchyScreen : public UIScreen
	{
	private:
		HierarchyScreen();
		~HierarchyScreen();

	private:
		friend class UIManager;

	private:
		virtual void draw() override;
	};
}