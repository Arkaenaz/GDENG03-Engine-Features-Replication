#pragma once

#include "Resource.h"

#include "Prerequisites.h"

namespace GDEngine
{
	class Mesh : public Resource
	{
	private:
		VertexBuffer* m_vertexBuffer;
		IndexBuffer* m_indexBuffer;

	private:
		friend class DeviceContext;

	public:
		Mesh(const wchar_t* fullPath);
		~Mesh() override;

	public:
		VertexBuffer* getVertexBuffer();
		IndexBuffer* getIndexBuffer();
	};

}