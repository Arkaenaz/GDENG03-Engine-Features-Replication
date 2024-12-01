#include "SceneWriter.h"

#include <json/json.h>
#include <fstream>

#include "BaseComponentSystem.h"
#include "GameObjectManager.h"
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include "TextureComponent.h"

namespace GDEngine
{
	SceneWriter::SceneWriter(std::string directory)
	{
		m_directory = directory;
	}

	SceneWriter::~SceneWriter()
	{
	}

	void SceneWriter::writeToFile()
	{
		std::string fileDirectory = m_directory + ".iet";

		if (m_directory.find(".iet") != std::string::npos)
		{
			
		}

		std::fstream sceneFile;
		sceneFile.open(fileDirectory, std::ios::out);

		Logger::log("Selected File Name : " + fileDirectory);

		GameObjectManager::GameObjectList objectList = GameObjectManager::getInstance()->getAllObjects();

		for (AGameObject* gameObject : objectList)
		{
			sceneFile <<  gameObject->getGuidString() << std::endl;
			sceneFile << "Name: " << gameObject->getName() << std::endl;

			Vector3D position = gameObject->getLocalPosition();
			Vector3D rotation = gameObject->getLocalRotation();
			Vector3D scale = gameObject->getLocalScale();

			sceneFile << "Position: " << position.x << " " << position.y << " " << position.z << std::endl;
			sceneFile << "Rotation: " << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;
			sceneFile << "Scale: " << scale.x << " " << scale.y << " " << scale.z << std::endl;
		}
		sceneFile.close();
	}

	void SceneWriter::writeToJson()
	{
		std::string fileDirectory = m_directory + ".level";

		if (m_directory.find(".level") != std::string::npos)
		{

		}

		std::fstream sceneFile;
		sceneFile.open(fileDirectory, std::ios::out);

		Logger::log("Selected File Name : " + fileDirectory);

		Json::Value root;

		GameObjectManager::GameObjectList objectList = GameObjectManager::getInstance()->getAllObjects();

		for (AGameObject* gameObject : objectList)
		{
			std::string guid = gameObject->getGuidString();

			root[guid];
			root[guid]["name"] = gameObject->getName();
			root[guid]["type"] = gameObject->getType();

			Vector3D position = gameObject->getLocalPosition();
			Vector3D rotation = gameObject->getLocalRotation();
			Vector3D scale = gameObject->getLocalScale();

			root[guid]["position"]["x"] = position.x;
			root[guid]["position"]["y"] = position.y;
			root[guid]["position"]["z"] = position.z;

			root[guid]["rotation"]["x"] = rotation.x;
			root[guid]["rotation"]["y"] = rotation.y;
			root[guid]["rotation"]["z"] = rotation.z;

			root[guid]["scale"]["x"] = scale.x;
			root[guid]["scale"]["y"] = scale.y;
			root[guid]["scale"]["z"] = scale.z;

			AGameObject::ComponentList physicsList = gameObject->getComponentsOfType(AComponent::ComponentType::Physics);
			for (AComponent* component : physicsList)
			{
				PhysicsComponent* physicsComponent = dynamic_cast<PhysicsComponent*>(component);
				std::string componentGuid = component->getGuidString();

				root[guid]["components"][componentGuid];
				root[guid]["components"][componentGuid]["name"] = physicsComponent->getName();
				root[guid]["components"][componentGuid]["class"] = physicsComponent->getClassType();
				root[guid]["components"][componentGuid]["type"] = physicsComponent->getType();

				root[guid]["components"][componentGuid]["mass"] = physicsComponent->getMass();
				root[guid]["components"][componentGuid]["gravity"] = physicsComponent->getUseGravity();
				root[guid]["components"][componentGuid]["body_type"] = static_cast<int>(physicsComponent->getBodyType());
				root[guid]["components"][componentGuid]["linear_drag"] = physicsComponent->getLinearDrag();
				root[guid]["components"][componentGuid]["angular_drag"] = physicsComponent->getAngularDrag();
				root[guid]["components"][componentGuid]["constraints"] = physicsComponent->getConstraints();
			}
			AGameObject::ComponentList renderList = gameObject->getComponentsOfType(AComponent::ComponentType::Renderer);
			for (AComponent* component : renderList)
			{
				std::string componentGuid = component->getGuidString();
				root[guid]["components"][componentGuid];
				root[guid]["components"][componentGuid]["name"] = component->getName();
				root[guid]["components"][componentGuid]["class"] = component->getClassType();
				root[guid]["components"][componentGuid]["type"] = component->getType();

				/*if (component->getClassType() == typeid(TextureComponent).raw_name())
				{
					TextureComponent* textureComponent = dynamic_cast<TextureComponent*>(component);
				}*/
			}
		}

		std::cout << root << "\n";

		Json::StyledWriter styledWriter;
		sceneFile << styledWriter.write(root);
		sceneFile.close();
	}
}
