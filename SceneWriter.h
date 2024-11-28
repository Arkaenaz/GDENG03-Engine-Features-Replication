#pragma once
#include <string>

namespace GDEngine {
	class SceneWriter
	{
	public:
		SceneWriter(std::string directory);
		~SceneWriter();

		void writeToFile();

	private:
		std::string m_directory;
	};
}
