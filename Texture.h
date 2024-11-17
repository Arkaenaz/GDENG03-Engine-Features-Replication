#pragma once

#include "Resource.h"
#include <d3d11.h>

namespace GDEditor
{
	class CreditsScreen;
}

namespace GDEngine
{
	class Texture : public Resource
	{
	private:
		ID3D11Resource* m_texture = nullptr;
		ID3D11ShaderResourceView* m_shaderResourceView = nullptr;

		friend class GDEditor::CreditsScreen;
		friend class DeviceContext;
	public:
		Texture(const wchar_t* fullPath);
		~Texture() override;
	};
}
