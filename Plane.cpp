#include "Plane.h"

#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "ShaderLibrary.h"

namespace GDEngine {
	Plane::Plane(std::string name) : AGameObject(name, PLANE)
	{

		Vector3D color1 = Vector3D(255.0f / 255.0f, 227.0f / 255.0f, 222.0f / 255.0f);
		Vector3D color2 = Vector3D(241.0f / 255.0f, 204.0f / 255.0f, 202.0f / 255.0f);
		Vector3D color3 = Vector3D(226.0f / 255.0f, 180.0f / 255.0f, 181.0f / 255.0f);
		Vector3D color4 = Vector3D(212.0f / 255.0f, 157.0f / 255.0f, 161.0f / 255.0f);

		vertex list[] =
		{
			{ Vector3D(-8.0f,0.0f,-8.0f),   color1, color1 },
			{ Vector3D(-8.0f,0.0f,8.0f),   color2, color2 },
			{ Vector3D(8.0f,0.0f,-8.0f), color3,  color3 },
			{ Vector3D(8.0f,0.0f,8.0f),    color4, color4 },

			{ Vector3D(-8.0f,0.0f,-8.0f),   color1, color1 },
			{ Vector3D(-8.0f,0.0f,8.0f),   color2, color2 },
			{ Vector3D(8.0f,0.0f,-8.0f), color3,  color3 },
			{ Vector3D(8.0f,0.0f,8.0f),    color4, color4 }
		};

		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

		CBObjectData cbData;
		cbData.time = 0.0f;

		m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));

		UINT sizeList = ARRAYSIZE(list);

		ShaderNames shaderNames;

		void* shaderByteCode = NULL;
		size_t sizeShader = 0;

		ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.BASE_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);

		m_vertexBuffer = renderSystem->createVertexBuffer(list, sizeof(vertex), sizeList, shaderByteCode, sizeShader);
	}

	Plane::Plane(std::string guid, std::string name, GameObjectType type) : AGameObject(guid, name, type)
	{
		Vector3D color1 = Vector3D(255.0f / 255.0f, 227.0f / 255.0f, 222.0f / 255.0f);
		Vector3D color2 = Vector3D(241.0f / 255.0f, 204.0f / 255.0f, 202.0f / 255.0f);
		Vector3D color3 = Vector3D(226.0f / 255.0f, 180.0f / 255.0f, 181.0f / 255.0f);
		Vector3D color4 = Vector3D(212.0f / 255.0f, 157.0f / 255.0f, 161.0f / 255.0f);

		vertex list[] =
		{
			{ Vector3D(-8.0f,0.0f,-8.0f),   color1, color1 },
			{ Vector3D(-8.0f,0.0f,8.0f),   color2, color2 },
			{ Vector3D(8.0f,0.0f,-8.0f), color3,  color3 },
			{ Vector3D(8.0f,0.0f,8.0f),    color4, color4 },

			{ Vector3D(-8.0f,0.0f,-8.0f),   color1, color1 },
			{ Vector3D(-8.0f,0.0f,8.0f),   color2, color2 },
			{ Vector3D(8.0f,0.0f,-8.0f), color3,  color3 },
			{ Vector3D(8.0f,0.0f,8.0f),    color4, color4 }
		};

		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

		CBObjectData cbData;
		cbData.time = 0.0f;

		m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));

		UINT sizeList = ARRAYSIZE(list);

		ShaderNames shaderNames;

		void* shaderByteCode = NULL;
		size_t sizeShader = 0;

		ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.BASE_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);

		m_vertexBuffer = renderSystem->createVertexBuffer(list, sizeof(vertex), sizeList, shaderByteCode, sizeShader);
	}

	Plane::~Plane()
	{
		delete m_constantBuffer;
		delete m_vertexBuffer;
		AGameObject::~AGameObject();
	}

	void Plane::onCreate()
	{
		AGameObject::onCreate();
	}

	void Plane::update(float deltaTime)
	{
		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

		CBObjectData cbData;
		cbData.time = 0.0f;

		this->updateLocalMatrix();

		cbData.worldMatrix.setMatrix(this->m_localMatrix);

		m_constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cbData);
	}

	// Sets shaders and draws afterwards
	void Plane::draw(int width, int height)
	{
		ShaderNames shaderNames;
		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

		VertexShader* vertexShader = ShaderLibrary::getInstance()->getVertexShader(shaderNames.BASE_VERTEX_SHADER_NAME);
		PixelShader* pixelShader = ShaderLibrary::getInstance()->getPixelShader(shaderNames.BASE_PIXEL_SHADER_NAME);

		renderSystem->getImmediateDeviceContext()->setConstantBuffer(m_constantBuffer, 0);

		renderSystem->getImmediateDeviceContext()->setVertexShader(vertexShader);
		renderSystem->getImmediateDeviceContext()->setPixelShader(pixelShader);

		renderSystem->getImmediateDeviceContext()->setVertexBuffer(m_vertexBuffer);

		renderSystem->getImmediateDeviceContext()->drawTriangleStrip(m_vertexBuffer->getSizeVertexList(), 0);
	}

	void Plane::onDestroy()
	{
	}
}