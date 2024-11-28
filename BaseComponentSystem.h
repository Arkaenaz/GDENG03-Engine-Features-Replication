#pragma once

namespace GDEngine
{
	class PhysicsSystem;
	class BaseComponentSystem
	{
	private:
		PhysicsSystem* m_physicsSystem;

	public:
		PhysicsSystem* getPhysicsSystem();

	private:
		static BaseComponentSystem* P_SHARED_INSTANCE;

	private:
		BaseComponentSystem();
		~BaseComponentSystem();
		BaseComponentSystem(BaseComponentSystem const&);
		BaseComponentSystem& operator = (BaseComponentSystem const&);

	public:
		static BaseComponentSystem* getInstance();
		static void initialize();
		static void destroy();
	};
}