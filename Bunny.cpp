#include "Bunny.h"

#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "ShaderLibrary.h"

namespace GDEngine {
	Bunny::Bunny(std::string name) : MeshObject(name, BUNNY, L"assets/meshes/bunny.obj")
	{
	}

	Bunny::Bunny(std::string guid, std::string name, GameObjectType type) : MeshObject(guid, name, type, L"assets/meshes/bunny.obj")
	{
	}

	Bunny::~Bunny()
	{
		MeshObject::~MeshObject();
	}

	void Bunny::onCreate()
	{
		AGameObject::onCreate();
	}

	void Bunny::update(float deltaTime)
	{
		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
		CBObjectData cbObjectData;

		cbObjectData.time = 0.0f;

		this->updateLocalMatrix();

		cbObjectData.worldMatrix.setMatrix(this->m_localMatrix);

		m_constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cbObjectData);
	}

	void Bunny::draw(int width, int height)
	{
		ShaderNames shaderNames;
		DeviceContext* deviceContext = GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext();

		Texture* brickTex = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/brick.png");

		VertexShader* vertexShader = ShaderLibrary::getInstance()->getVertexShader(shaderNames.TEXTURED_VERTEX_SHADER_NAME);
		PixelShader* pixelShader = ShaderLibrary::getInstance()->getPixelShader(shaderNames.TEXTURED_PIXEL_SHADER_NAME);

		deviceContext->setTexture(brickTex);
		deviceContext->setConstantBuffer(m_constantBuffer, 0);

		deviceContext->setVertexShader(vertexShader);
		deviceContext->setPixelShader(pixelShader);

		MeshObject::draw(width, height);
	}

	void Bunny::onDestroy()
	{
		AGameObject::onDestroy();
	}
}