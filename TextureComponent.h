#pragma once
#include "AComponent.h"
#include "Vector2D.h"
#include "ShaderLibrary.h"
#include "Texture.h"
#include "TextureLibrary.h"
#include <d3d11.h>

namespace GDEngine
{
	class Texture;
	class TextureComponent : public AComponent
	{
		public:
			TextureComponent(std::string name, AGameObject* owner);
			~TextureComponent() override;

			void setTexture(TextureName texName);
			void removeTexture();
			void updateTexture();

			Texture* getTexture();
			std::string filePath;
			int textureIndex = 0;
		private:	
			
			Texture* texture;
	};
}

