#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <reactphysics3d/reactphysics3d.h>

namespace GDEngine
{
	using namespace reactphysics3d;
	class PhysicsComponent;
	class PhysicsSystem
	{
	private:
		typedef std::unordered_map<std::string, PhysicsComponent*> PhysicsComponentMap;
		typedef std::vector<PhysicsComponent*> PhysicsComponentList;

	private:
		PhysicsComponentMap m_componentMap;
		PhysicsComponentList m_componentList;

		PhysicsCommon* m_physicsCommon;
		PhysicsWorld* m_physicsWorld;

	public:
		PhysicsSystem();
		~PhysicsSystem();

	public:
		void registerComponent(PhysicsComponent* component);
		void unregisterComponent(PhysicsComponent* component);
		void unregisterComponentByName(std::string name);
		PhysicsComponent* findComponentByName(std::string name);
		PhysicsComponentList getAllComponents();

		void updateAllComponents();
		PhysicsWorld* getPhysicsWorld();
		PhysicsCommon* getPhysicsCommon();
	};
}