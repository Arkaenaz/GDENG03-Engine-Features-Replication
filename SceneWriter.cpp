#include "SceneWriter.h"

#include <fstream>

#include "GameObjectManager.h"

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
}
