#pragma once

#include <unordered_map>
#include <vector>

#include "GameObject.h"

namespace GDEngine
{
	class GameObjectManager
	{
	private:
		typedef std::unordered_map<std::string, AGameObject*> GameObjectTable;
		typedef std::vector<AGameObject*> GameObjectList;

	private:
		GameObjectTable m_gameObjectTable;
		GameObjectList m_gameObjectList;
		AGameObject* m_selectedObject;

	public:
		void createCube();
		void createPhysicsCube();
		void createPhysicsPlane();
		void createTexturedCube();
		void createPlane();
		void createQuad();
		void createTeapot();
		void createBunny();
		void createArmadillo();

		void update(float deltaTime);
		void draw(int width, int height);

		GameObjectList getAllObjects();
		AGameObject* findObjectByName(std::string name);
		void addObject(AGameObject* gameObject);
		void deleteObject(AGameObject* gameObject);
		void deleteObjectByName(std::string name);
		void deleteAllObjects();
		void setSelectedObject(std::string name);
		void setSelectedObject(GUID guid);
		void setSelectedObject(AGameObject* gameObject);
		AGameObject* getSelectedObject();

		void saveEditStates();
		void restoreEditStates();
		void applyAction(EditorAction* action);

		void setPhysics(bool physics);

	private:
		static GameObjectManager* P_SHARED_INSTANCE;

	private:
		GameObjectManager();
		~GameObjectManager();
		GameObjectManager(const GameObjectManager&);
		GameObjectManager& operator = (const GameObjectManager&);

	public:
		static GameObjectManager* getInstance();
		static void initialize();
		static void destroy();
	};
}