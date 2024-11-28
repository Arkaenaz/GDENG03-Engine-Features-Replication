#pragma once
#include <string>

namespace GDEngine {
	class SceneReader
	{
	public:
		SceneReader(std::string directory);
		~SceneReader();

		void readFromFile();

	private:
		std::string m_directory;
	};
}

