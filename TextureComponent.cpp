#include "TextureComponent.h"

#include "GameObject.h"
#include "GraphicsEngine.h"

using namespace GDEngine;

TextureComponent::TextureComponent(std::string name, AGameObject* owner) : AComponent(name, Physics, owner) 
{
	this->setTexture(TextureName::DEFAULT);
	this->updateTexture();
}

GDEngine::TextureComponent::~TextureComponent()
{
	this->texture = nullptr;
}

void GDEngine::TextureComponent::setTexture(TextureName texName)
{

	this->texture = TextureLibrary::getInstance()->getTexture(texName);

	if (this->texture == nullptr)
	{
		this->getOwner()->setHasTexture(false);
		//this->getOwner()->toggleTexture();
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
	//this->getOwner()->toggleTexture();
}

Texture* GDEngine::TextureComponent::getTexture()
{
	return this->texture;
}
