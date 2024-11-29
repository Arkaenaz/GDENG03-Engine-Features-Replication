#include "EmptyGameObject.h"

namespace GDEngine
{
	EmptyGameObject::EmptyGameObject(std::string guid, std::string name, GameObjectType type) : AGameObject(guid, name, type)
	{
	}

	EmptyGameObject::~EmptyGameObject()
	{
	}
}