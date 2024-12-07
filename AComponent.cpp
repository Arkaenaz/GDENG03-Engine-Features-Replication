#include "AComponent.h"

using namespace GDEngine;

AComponent::AComponent(std::string name, ComponentType type, AGameObject* owner) : m_name(name), m_type(type), m_owner(owner)
{
}

AComponent::~AComponent()
{
	this->m_owner = nullptr;
	this->m_type = NotSet;
}

void AComponent::attachOwner(AGameObject* owner)
{
	this->m_owner = owner;
}

void AComponent::detachOwner()
{
	this->m_owner = nullptr;
	delete this;
}

AGameObject* AComponent::getOwner()
{
	return this->m_owner;
}

AComponent::ComponentType AComponent::getType()
{
	return this->m_type;
}

std::string AComponent::getName()
{
	return this->m_name;
}

void AComponent::perform(float deltaTime)
{
}
