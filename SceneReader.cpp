#include "SceneReader.h"

#include <fstream>
#include <vector>

#include "GameObjectManager.h"
#include "StringUtility.h"
#include "Vector3D.h"

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

				GameObjectManager::getInstance()->createObjectFromFile(objectGuid,objectName, position, rotation, scale);
			}
		}
	}
}
