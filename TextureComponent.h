#pragma once
#include "AComponent.h"
#include "Vector2D.h"
#include "ShaderLibrary.h"
#include "Texture.h"
#include <d3d11.h>

namespace GDEngine
{
	class Texture;
	class TextureComponent : public AComponent
	{
		public:
			TextureComponent(std::string name, AGameObject* owner);
			~TextureComponent() override;

			void setTexture(std::string path);
			void removeTexture();
			void updateTexture();

			Texture* getTexture();
			std::string filePath;
		private:	
			
			Texture* texture;
	};
}

