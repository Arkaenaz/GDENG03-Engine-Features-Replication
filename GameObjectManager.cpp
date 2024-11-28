#include "GameObjectManager.h"

#include "BaseComponentSystem.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

#include "Armadillo.h"
#include "Bunny.h"
#include "Cube.h"
#include "Plane.h"
#include "Quad.h"
#include "Teapot.h"
#include "TexturedCube.h"

#include "EditorAction.h"
#include "EmptyGameObject.h"

#include "Logger.h"

using namespace GDEngine;

GameObjectManager* GameObjectManager::P_SHARED_INSTANCE = nullptr;

void GameObjectManager::createCube()
{
	Cube* cube = new Cube("Cube");
	this->addObject(cube);
}

void GameObjectManager::createPhysicsCube()
{
	Cube* cube = new Cube("Physics Cube");
	cube->setPosition(0.0f, 5.0f, 0.0f);
	cube->setPhysics(true);
	this->addObject(cube);
	cube->attachComponent(new PhysicsComponent("PhysicsComponent " + cube->getName(), cube));
}

void GameObjectManager::createPhysicsPlane()
{
	Cube* plane = new Cube("Physics Plane");
	plane->setScale(64, 0.5f, 64);
	plane->setPhysics(true);
	this->addObject(plane);
	plane->attachComponent(new PhysicsComponent("PhysicsComponent " + plane->getName(), plane));
	PhysicsComponent* component = (PhysicsComponent*)plane->findComponentOfType(AComponent::ComponentType::Physics, "PhysicsComponent " + plane->getName());
	component->getRigidBody()->setType(BodyType::KINEMATIC);

	/*Plane* plane = new Plane("Physics Plane");
	plane->attachComponent(new PhysicsComponent("PhysicsComponent", plane));
	PhysicsComponent* component = (PhysicsComponent*)plane->findComponentOfType(AComponent::ComponentType::Physics, "PhysicsComponent");
	component->getRigidBody()->setType(BodyType::KINEMATIC);
	this->addObject(plane);*/
}

void GameObjectManager::createTexturedCube()
{
	TexturedCube* cube = new TexturedCube("Textured Cube");
	this->addObject(cube);
}

void GameObjectManager::createPlane()
{
	Plane* plane = new Plane("Plane");
	this->addObject(plane);
}

void GameObjectManager::createQuad()
{
	Quad* quad = new Quad("Quad");
	this->addObject(quad);
}

void GameObjectManager::createTeapot()
{
	Teapot* teapot = new Teapot("Teapot");
	this->addObject(teapot);
}

void GameObjectManager::createBunny()
{
	Bunny* bunny = new Bunny("Bunny");
	this->addObject(bunny);
}

void GameObjectManager::createArmadillo()
{
	Armadillo* armadillo = new Armadillo("Armadillo");
	this->addObject(armadillo);
}

void GameObjectManager::update(float deltaTime)
{
	for (AGameObject* gameObject : this->m_gameObjectList)
	{
		if (gameObject->isActive())
		{
			gameObject->update(deltaTime);
		}
	}
}

void GameObjectManager::draw(int width, int height)
{
	for (AGameObject* gameObject : this->m_gameObjectList)
	{
		if (gameObject->isActive())
			gameObject->draw(width, height);
	}
}

GameObjectManager::GameObjectList GameObjectManager::getAllObjects()
{
	return this->m_gameObjectList;
}

AGameObject* GameObjectManager::findObjectByName(std::string name)
{
	if (this->m_gameObjectTable[name] != NULL)
	{
		return this->m_gameObjectTable[name];
	}
	else
	{
		Logger::error(this, "Object " + name + " is not found");
		return NULL;
	}
}

void GameObjectManager::addObject(AGameObject* gameObject)
{
	if (this->m_gameObjectTable[gameObject->getName()] != NULL)
	{
		int count = 1;
		std::string newName = gameObject->getName() + " " + std::to_string(count);

		while (this->m_gameObjectTable[newName] != NULL)
		{
			count++;
			newName = gameObject->getName() + " " + std::to_string(count);
		}
		gameObject->m_name = newName;
		this->m_gameObjectTable[newName] = gameObject;
	}
	else
	{
		this->m_gameObjectTable[gameObject->getName()] = gameObject;
	}

	this->m_gameObjectList.push_back(gameObject);
	gameObject->onCreate();
	Logger::log(gameObject->getName() + " added to hierarchy");
}

void GameObjectManager::deleteObject(AGameObject* gameObject)
{
	int index = -1;

	for (int i = 0; i < this->m_gameObjectList.size() && index == -1; i++)
	{
		if (this->m_gameObjectList[i] == gameObject)
			index = i;
	}

	if (index != -1)
	{
		this->m_gameObjectTable.erase(this->m_gameObjectList[index]->getName());
		this->m_gameObjectList.erase(this->m_gameObjectList.begin() + index);
		gameObject->onDestroy();
		delete gameObject;
	}
}

void GameObjectManager::deleteObjectByName(std::string name)
{
	AGameObject* object = this->findObjectByName(name);

	if (object != NULL)
	{
		this->deleteObject(object);
	}
}

void GameObjectManager::deleteAllObjects()
{
	if (!this->m_gameObjectList.empty())
	{
		for (AGameObject* gameObject : this->m_gameObjectList)
			gameObject->onDestroy();
		this->m_gameObjectList.clear();
		this->m_gameObjectTable.clear();
	}
}

void GameObjectManager::setSelectedObject(GUID guid)
{
	for (AGameObject* object : m_gameObjectList)
	{
		GUID objectGuid = object->getGuid();
		if (IsEqualGUID(objectGuid, guid))
		{
			this->setSelectedObject(object);
			return;
		}
	}
}

void GameObjectManager::setSelectedObject(std::string name)
{
	if (this->m_gameObjectTable[name] != NULL)
	{
		this->setSelectedObject(this->m_gameObjectTable[name]);
	}
}

void GameObjectManager::setSelectedObject(AGameObject* gameObject)
{
	this->m_selectedObject = gameObject;
}

AGameObject* GameObjectManager::getSelectedObject()
{
	return this->m_selectedObject;
}

void GameObjectManager::saveEditStates()
{
	for (AGameObject* gameObject : m_gameObjectList) {
		gameObject->saveEditState();
	}
}

void GameObjectManager::restoreEditStates()
{
	for (AGameObject* gameObject : m_gameObjectList) {
		gameObject->restoreEditState();
	}
}

void GameObjectManager::applyAction(EditorAction* action)
{
	Logger::log(action->getOwnerName());
	AGameObject* gameObject = this->findObjectByName(action->getOwnerName());
	if (gameObject != nullptr)
	{
		//gameObject->setLocalMatrix(action->getStoredMatrix().getMatrix());
		gameObject->setPosition(action->getStoredPosition());
		gameObject->setRotation(action->getStoredOrientation().x, action->getStoredOrientation().y, action->getStoredOrientation().z);
		gameObject->setScale(action->getStoredScale());
	}
}

void GameObjectManager::setPhysics(bool physics)
{
	for (AGameObject* gameObject : m_gameObjectList)
	{
		AGameObject::ComponentList physicsList = gameObject->getComponentsOfType(AComponent::Physics);

		if (physicsList.size() != 0)
			gameObject->setPhysics(physics);
	}
}

void GameObjectManager::createObjectFromFile(std::string objectGuid, std::string objectName, Vector3D position, Vector3D rotation,
	Vector3D scale)
{
	EmptyGameObject* gameObject = new EmptyGameObject(objectGuid, objectName);
	gameObject->setPosition(position);
	gameObject->setRotation(rotation);
	gameObject->setScale(scale);
	this->addObject(gameObject);
}

GameObjectManager::GameObjectManager()
{
	Logger::log(P_SHARED_INSTANCE, "Initialized");

}

GameObjectManager::~GameObjectManager()
{
	this->deleteAllObjects();
	P_SHARED_INSTANCE = nullptr;
	Logger::log(P_SHARED_INSTANCE, "Destroyed");
}

GameObjectManager::GameObjectManager(const GameObjectManager&) {}

GameObjectManager* GameObjectManager::getInstance() {
	return P_SHARED_INSTANCE;
}

void GameObjectManager::initialize()
{
	if (P_SHARED_INSTANCE)
	{
		Logger::throw_exception("Game Object Manager already created");
	}
	P_SHARED_INSTANCE = new GameObjectManager();
	
}

void GameObjectManager::destroy()
{
	delete P_SHARED_INSTANCE;
}
