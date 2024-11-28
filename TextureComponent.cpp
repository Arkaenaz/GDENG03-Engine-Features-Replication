#include "TextureComponent.h"

#include "GameObject.h"
#include "GraphicsEngine.h"

using namespace GDEngine;

TextureComponent::TextureComponent(std::string name, AGameObject* owner) : AComponent(name, Physics, owner) 
{
	this->setTexture("assets/textures/wood.jpg");
	this->updateTexture();
}

GDEngine::TextureComponent::~TextureComponent()
{
	this->texture = nullptr;
}

void GDEngine::TextureComponent::setTexture(std::string path)
{
	this->filePath = path;
	//convert string to wchar_t
	std::wstring widestr = std::wstring(filePath.begin(), filePath.end());
	const wchar_t* usablePath = widestr.c_str();

	this->texture = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(usablePath);

	if (this->texture == nullptr)
	{
		this->getOwner()->setHasTexture(false);
	}
	else
	{
		this->updateTexture();
	}
		
}

void GDEngine::TextureComponent::removeTexture()
{
	this->filePath = "";
	this->texture = nullptr;
	this->getOwner()->setHasTexture(false);
}

void GDEngine::TextureComponent::updateTexture()
{
	this->getOwner()->setTexture(this->texture);
	this->getOwner()->setHasTexture(true);
}

Texture* GDEngine::TextureComponent::getTexture()
{
	return this->texture;
}
