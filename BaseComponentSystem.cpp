#include "BaseComponentSystem.h"

#include "PhysicsSystem.h"
#include "Logger.h"

using namespace GDEngine;

BaseComponentSystem* BaseComponentSystem::P_SHARED_INSTANCE = NULL;

PhysicsSystem* BaseComponentSystem::getPhysicsSystem()
{
	return this->m_physicsSystem;
}

BaseComponentSystem::BaseComponentSystem()
{
	try
	{
		this->m_physicsSystem = new PhysicsSystem();
	}
	catch (...)
	{
		Logger::throw_exception("PhysicsSystem not created successfully");
	}
	
	Logger::log(P_SHARED_INSTANCE, "Initialized");
}

BaseComponentSystem::~BaseComponentSystem()
{
	delete this->m_physicsSystem;
	P_SHARED_INSTANCE = nullptr;
	Logger::log(P_SHARED_INSTANCE, "Released");
}

BaseComponentSystem::BaseComponentSystem(const BaseComponentSystem&) {}

BaseComponentSystem* BaseComponentSystem::getInstance()
{
	return P_SHARED_INSTANCE;
}

void BaseComponentSystem::initialize()
{
	if (P_SHARED_INSTANCE)
		Logger::throw_exception("BaseComponentSystem already created");
	P_SHARED_INSTANCE = new BaseComponentSystem();

}

void BaseComponentSystem::destroy()
{
	if (P_SHARED_INSTANCE != NULL)
	{
		delete P_SHARED_INSTANCE;
	}
}
