#include "Armadillo.h"

#include "GraphicsEngine.h"
#include "ShaderLibrary.h"

namespace GDEngine
{
	Armadillo::Armadillo(std::string name) : MeshObject(name, L"assets/meshes/armadillo.obj")
	{
	}

	Armadillo::~Armadillo()
	{
		MeshObject::~MeshObject();
	}

	void Armadillo::onCreate()
	{
		AGameObject::onCreate();
	}

	void Armadillo::update(float deltaTime)
	{
		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
		CBObjectData cbObjectData;

		cbObjectData.time = 0.0f;

		this->updateLocalMatrix();

		cbObjectData.worldMatrix.setMatrix(this->m_localMatrix);

		m_constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cbObjectData);
	}

	void Armadillo::draw(int width, int height)
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

	void Armadillo::onDestroy()
	{
		AGameObject::onDestroy();
	}
}