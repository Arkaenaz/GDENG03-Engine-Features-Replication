#include "Bunny.h"

#include "ConstantBuffer.h"
#include "ShaderLibrary.h"
#include "DeviceContext.h"
#include "Mesh.h"

using namespace graphics;

Bunny::Bunny(std::string name) : GameObject(name)
{
	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
	MeshManager* meshManager = GraphicsEngine::getInstance()->getMeshManager();

	this->mesh = meshManager->createMeshFromFile(L"assets/meshes/bunny.obj");

	CBObjectData cbData;
	cbData.time = 0.0f;

	constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));
}

Bunny::~Bunny()
{
	delete constantBuffer;
}

void Bunny::onCreate()
{
	GameObject::onCreate();
}

void Bunny::update(float deltaTime)
{
	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
	CBObjectData cbObjectData;

	cbObjectData.time = 0.0f;

	Matrix4x4 transform, rotation;
	Matrix4x4 temp;

	// Scale
	transform.setIdentity();
	transform.setScale(this->localScale);

	// Scale * Rotation
	rotation.setIdentity();
	rotation.setRotationZ(this->localRotation.z);

	temp.setIdentity();
	temp.setRotationY(this->localRotation.y);
	rotation *= temp;

	temp.setIdentity();
	temp.setRotationX(this->localRotation.x);
	rotation *= temp;

	transform *= rotation;
	// Scale * Rotation * Translation
	temp.setIdentity();
	temp.setTranslation(this->localPosition);
	transform *= temp;

	cbObjectData.worldMatrix.setMatrix(transform);

	constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cbObjectData);
}

void Bunny::draw(int width, int height)
{
	ShaderNames shaderNames;
	DeviceContext* deviceContext = GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext();

	Texture* brickTex = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/brick.png");

	VertexShader* vertexShader = ShaderLibrary::getInstance()->getVertexShader(shaderNames.TEXTURED_VERTEX_SHADER_NAME);
	PixelShader* pixelShader = ShaderLibrary::getInstance()->getPixelShader(shaderNames.TEXTURED_PIXEL_SHADER_NAME);

	deviceContext->setTexture(brickTex);
	deviceContext->setConstantBuffer(constantBuffer, 0);

	deviceContext->setVertexShader(vertexShader);
	deviceContext->setPixelShader(pixelShader);

	deviceContext->setVertexBuffer(this->mesh->getVertexBuffer());
	deviceContext->setIndexBuffer(this->mesh->getIndexBuffer());

	deviceContext->drawIndexedTriangleList(this->mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
}

void Bunny::onDestroy()
{
	GameObject::onDestroy();
}
