#include "Cube.h"

#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "CameraManager.h"
#include "EngineTime.h"
#include "ShaderLibrary.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "GameObjectManager.h"
#include "IndexBuffer.h"

using namespace GDEngine;

Cube::Cube(std::string name) : AGameObject(name, CUBE)
{

	Vector3D color1 = Vector3D(230.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f);
	Vector3D color2 = Vector3D(214.0f / 255.0f, 209.0f / 255.0f, 236.0f / 255.0f);
	Vector3D color3 = Vector3D(182.0f / 255.0f, 166.0f / 255.0f, 209.0f / 255.0f);
	Vector3D color4 = Vector3D(150.0f / 255.0f, 123.0f / 255.0f, 182.0f / 255.0f);

	Vector3D color5 = Vector3D(230.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f);
	Vector3D color6 = Vector3D(214.0f / 255.0f, 209.0f / 255.0f, 236.0f / 255.0f);
	Vector3D color7 = Vector3D(182.0f / 255.0f, 166.0f / 255.0f, 209.0f / 255.0f);
	Vector3D color8 = Vector3D(150.0f / 255.0f, 123.0f / 255.0f, 182.0f / 255.0f);

	vertex vertexList[] =
	{
		{Vector3D(-1.0f,-1.0f,-1.0f),color3, color7 },
		{Vector3D(-1.0f,1.0f, -1.0f),color1, color5 },
		{Vector3D(1.0f, 1.0f, -1.0f),color1, color5 },
		{Vector3D(1.0f, -1.0f,-1.0f),color3, color7 },
																									
		{Vector3D(1.0f, -1.0f,1.0f), color4, color8 },
		{Vector3D(1.0f, 1.0f, 1.0f), color2, color6 },
		{Vector3D(-1.0f,1.0f, 1.0f), color2, color6 },
		{Vector3D(-1.0f,-1.0f,1.0f), color4, color8 }
	};

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	ShaderNames shaderNames;

	void* shaderByteCode = NULL;
	size_t sizeShader = 0;

	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.BASE_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);

	m_vertexBuffer = renderSystem->createVertexBuffer(vertexList, sizeof(vertex), ARRAYSIZE(vertexList), shaderByteCode, sizeShader);

	unsigned int indexList[] = {
		0,1,2,
		2,3,0,

		4,5,6,
		6,7,4,

		1,6,5,
		5,2,1,

		7,0,3,
		3,4,7,

		3,2,5,
		5,4,3,

		7,6,1,
		1,0,7
	};

	m_indexBuffer = renderSystem->createIndexBuffer(indexList, ARRAYSIZE(indexList));

	CBObjectData cbData;
	angle = 0.0f;
	cbData.time = angle;

	m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));

	speed = 1.0f;
	deltaRotation = 0.0f;
}

Cube::Cube(std::string guid, std::string name, GameObjectType type) : AGameObject(guid, name, type)
{
	Vector3D color1 = Vector3D(230.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f);
	Vector3D color2 = Vector3D(214.0f / 255.0f, 209.0f / 255.0f, 236.0f / 255.0f);
	Vector3D color3 = Vector3D(182.0f / 255.0f, 166.0f / 255.0f, 209.0f / 255.0f);
	Vector3D color4 = Vector3D(150.0f / 255.0f, 123.0f / 255.0f, 182.0f / 255.0f);

	Vector3D color5 = Vector3D(230.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f);
	Vector3D color6 = Vector3D(214.0f / 255.0f, 209.0f / 255.0f, 236.0f / 255.0f);
	Vector3D color7 = Vector3D(182.0f / 255.0f, 166.0f / 255.0f, 209.0f / 255.0f);
	Vector3D color8 = Vector3D(150.0f / 255.0f, 123.0f / 255.0f, 182.0f / 255.0f);

	vertex vertexList[] =
	{
		{Vector3D(-1.0f,-1.0f,-1.0f),color3, color7 },
		{Vector3D(-1.0f,1.0f, -1.0f),color1, color5 },
		{Vector3D(1.0f, 1.0f, -1.0f),color1, color5 },
		{Vector3D(1.0f, -1.0f,-1.0f),color3, color7 },

		{Vector3D(1.0f, -1.0f,1.0f), color4, color8 },
		{Vector3D(1.0f, 1.0f, 1.0f), color2, color6 },
		{Vector3D(-1.0f,1.0f, 1.0f), color2, color6 },
		{Vector3D(-1.0f,-1.0f,1.0f), color4, color8 }
	};

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	ShaderNames shaderNames;

	void* shaderByteCode = NULL;
	size_t sizeShader = 0;

	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.BASE_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);

	m_vertexBuffer = renderSystem->createVertexBuffer(vertexList, sizeof(vertex), ARRAYSIZE(vertexList), shaderByteCode, sizeShader);

	unsigned int indexList[] = {
		0,1,2,
		2,3,0,

		4,5,6,
		6,7,4,

		1,6,5,
		5,2,1,

		7,0,3,
		3,4,7,

		3,2,5,
		5,4,3,

		7,6,1,
		1,0,7
	};

	m_indexBuffer = renderSystem->createIndexBuffer(indexList, ARRAYSIZE(indexList));

	CBObjectData cbData;
	angle = 0.0f;
	cbData.time = angle;

	m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));

	speed = 1.0f;
	deltaRotation = 0.0f;
}

Cube::~Cube()
{
	delete m_vertexBuffer;
	delete m_indexBuffer;
	delete m_constantBuffer;
	AGameObject::~AGameObject();
}

void Cube::onCreate()
{
	AGameObject::onCreate();
}

void Cube::update(float deltaTime)
{
	AGameObject::update(deltaTime);
	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
	CBObjectData cbObjectData;

	cbObjectData.time = 0.0f;

	elapsedTime += EngineTime::getDeltaTime() / 2.0f;

	this->updateLocalMatrix();

	cbObjectData.worldMatrix.setMatrix(this->m_localMatrix);

	m_constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cbObjectData);
}

// Sets shaders and draws afterwards
void Cube::draw(int height, int width)
{
	ShaderNames shaderNames;
	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	VertexShader* vertexShader = ShaderLibrary::getInstance()->getVertexShader(shaderNames.BASE_VERTEX_SHADER_NAME);
	PixelShader* pixelShader = ShaderLibrary::getInstance()->getPixelShader(shaderNames.BASE_PIXEL_SHADER_NAME);

	renderSystem->getImmediateDeviceContext()->setConstantBuffer(m_constantBuffer, 0);

	renderSystem->getImmediateDeviceContext()->setVertexShader(vertexShader);
	renderSystem->getImmediateDeviceContext()->setPixelShader(pixelShader);

	renderSystem->getImmediateDeviceContext()->setVertexBuffer(m_vertexBuffer);
	renderSystem->getImmediateDeviceContext()->setIndexBuffer(m_indexBuffer);

	renderSystem->getImmediateDeviceContext()->drawIndexedTriangleList(m_indexBuffer->getSizeIndexList(),0,0);
}

void Cube::onDestroy()
{
	
}

void Cube::setSpeed(float speed)
{
	this->speed = speed;
}
