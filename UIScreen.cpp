#include "UIScreen.h"

UIScreen::UIScreen(String name)
{
	this->name = name;
}

UIScreen::~UIScreen()
{
	
}

String UIScreen::getName()
{
	return this->name;
}

void UIScreen::setActive(bool isActive)
{
	this->isActive = true;
}