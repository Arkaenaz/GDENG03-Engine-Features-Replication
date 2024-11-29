#include "SceneReader.h"

#include <fstream>
#include <vector>

#include "BaseComponentSystem.h"
#include "GameObjectManager.h"
#include "PhysicsSystem.h"
#include "StringUtility.h"
#include "Vector3D.h"
#include "json/json.h"

namespace GDEngine
{
	SceneReader::SceneReader(std::string directory)
	{
		m_directory = directory;
	}

	SceneReader::~SceneReader()
	{
	}

	void SceneReader::readFromFile()
	{
		std::string fileDirectory = m_directory + ".iet";
		if (m_directory.find(".iet") != std::string::npos)
		{
			fileDirectory = m_directory;
		}

		std::fstream sceneFile;
		sceneFile.open(fileDirectory, std::ios::in);

		int index = 0;
		std::string readLine;

		std::string objectGuid;
		std::string objectName;

		Vector3D position;
		Vector3D rotation;
		Vector3D scale;
		while (std::getline(sceneFile, readLine))
		{
			if (index == 0)
			{
				objectGuid = readLine;
				index++;
			}
			else if (index == 1)
			{
				std::vector<std::string> stringSplit = StringUtility::split(readLine, ' ');
				objectName = stringSplit[1];
				index++;
			}
			else if (index == 2) {
				std::vector<std::string> stringSplit = StringUtility::split(readLine, ' ');
				position = Vector3D(std::stof(stringSplit[1]), std::stof(stringSplit[2]), std::stof(stringSplit[3]));
				index++;
			}
			else if (index == 3) {
				std::vector<std::string> stringSplit = StringUtility::split(readLine, ' ');
				rotation = Vector3D(std::stof(stringSplit[1]), std::stof(stringSplit[2]), std::stof(stringSplit[3]));
				index++;
			}
			else if (index == 4) {
				std::vector<std::string> stringSplit = StringUtility::split(readLine, ' ');
				scale = Vector3D(std::stof(stringSplit[1]), std::stof(stringSplit[2]), std::stof(stringSplit[3]));
				index = 0;

				//GameObjectManager::getInstance()->createObjectFromFile(objectGuid, objectName, position, rotation, scale);
			}
		}
	}

	void SceneReader::readFromJson()
	{
		std::string fileDirectory = m_directory + ".level";
		if (m_directory.find(".level") != std::string::npos)
		{
			fileDirectory = m_directory;
		}

		std::ifstream sceneFile(fileDirectory, std::ifstream::binary);
		Json::Value scene;

		sceneFile >> scene;

		std::vector<std::string> guidList;

		for (std::string id : scene.getMemberNames()) {
			guidList.push_back(id);
		}

		for (std::string guid : guidList)
		{
			std::string name = scene[guid]["name"].asString();
			std::string type = scene[guid]["type"].asString();

			Vector3D position;
			position.x = scene[guid]["position"]["x"].asFloat();
			position.y = scene[guid]["position"]["y"].asFloat();
			position.z = scene[guid]["position"]["z"].asFloat();

			Vector3D rotation;
			rotation.x = scene[guid]["rotation"]["x"].asFloat();
			rotation.y = scene[guid]["rotation"]["y"].asFloat();
			rotation.z = scene[guid]["rotation"]["z"].asFloat();

			Vector3D scale;
			scale.x = scene[guid]["scale"]["x"].asFloat();
			scale.y = scene[guid]["scale"]["y"].asFloat();
			scale.z = scene[guid]["scale"]["z"].asFloat();

			AGameObject* gameObject = GameObjectManager::getInstance()->createObjectFromFile(guid, name, type, position, rotation, scale);

			std::vector<std::string> componentGuidList;
			for (std::string componentId : scene[guid]["components"].getMemberNames())
			{
				componentGuidList.push_back(componentId);
			}

			for (std::string componentGuid : componentGuidList)
			{
				std::string componentName = scene[guid]["components"][componentGuid]["name"].asString();
				std::string componentClassType = scene[guid]["components"][componentGuid]["class"].asString();
				AComponent::ComponentType componentType = static_cast<AComponent::ComponentType>(scene[guid]["components"][componentGuid]["type"].asInt());
				float mass = scene[guid]["components"][componentGuid]["mass"].asFloat();
				bool gravity = scene[guid]["components"][componentGuid]["gravity"].asBool();
				BodyType componentBodyType = static_cast<BodyType>(scene[guid]["components"][componentGuid]["body_type"].asInt());
				float linearDrag = scene[guid]["components"][componentGuid]["linear_drag"].asFloat();
				float angularDrag = scene[guid]["components"][componentGuid]["angular_drag"].asFloat();

				BaseComponentSystem::getInstance()->getPhysicsSystem()->createComponentFromFile(componentGuid, componentName, gameObject, componentType,
					mass, gravity, componentBodyType, linearDrag, angularDrag);
			}

		}
		
	}
}