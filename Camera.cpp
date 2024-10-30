#include "Camera.h"
#include "Camera.h"

#include "EngineTime.h"
#include "InputSystem.h"
#include "AppWindow.h"

Camera::Camera(std::string name) : GameObject(name)
{
	CBCameraData cbData;
	constantBuffer = GraphicsEngine::getInstance()->getRenderSystem()->createConstantBuffer(&cbData, sizeof(CBCameraData));
}

Camera::~Camera()
{
	delete constantBuffer;
	GameObject::~GameObject();
}

void Camera::onCreate()
{
	GameObject::onCreate();
}

void Camera::update(float deltaTime)
{
	GameObject::update(deltaTime);

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	if (this->type == 2) {
		this->localPosition.y = 20.0f;
		this->localRotation = Vector3D(1.57f, 0, 0);
	}

	this->updateViewMatrix();
	this->updateProjectionMatrix();

	CBCameraData cbCameraData = {};
	cbCameraData.viewMatrix = this->viewMatrix;
	cbCameraData.projMatrix = this->projMatrix;

	renderSystem->getImmediateDeviceContext()->setConstantBuffer(constantBuffer, 1);
	constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cbCameraData);
}

void Camera::updateViewMatrix()
{
	Matrix4x4 worldCam;
	worldCam.setIdentity();

	Matrix4x4 temp;

	temp.setIdentity();
	temp.setRotationX(this->localRotation.x);
	worldCam *= temp;

	temp.setIdentity();
	temp.setRotationY(this->localRotation.y);
	worldCam *= temp;

	temp.setIdentity();
	temp.setTranslation(this->localPosition);
	worldCam *= temp;

	worldCam.inverse();
	this->viewMatrix = worldCam;
}

void Camera::updateProjectionMatrix()
{
	RECT viewport = AppWindow::getInstance()->getClientWindowRect();
	int width = (viewport.right - viewport.left);
	int height = (viewport.bottom - viewport.top);

	Matrix4x4 proj;
	switch (type) {
		case 0:
			proj.setOrthoLH(
				width / 100.0f,
				height / 100.0f,
				-100.0f, 100.0f
			);
			break;
		case 1:
			proj.setPerspectiveFovLH(
				1.57f, // fov
				(float)width / (float)height, // aspect
				0.1f, // near
				100.0f // far
			);
			break;
		case 2:
			this->projMatrix.setOrthoLH(
				width / 100.0f,
				height / 100.0f,
				-100.0f, 100.0f
			);
		default:
			proj.setPerspectiveFovLH(
				1.57f, // fov
				(float)width / (float)height, // aspect
				0.1f, // near
				100.0f // far
			);
			break;
	}
	this->projMatrix = proj;
}

void Camera::onDestroy()
{
	GameObject::onDestroy();
}

Matrix4x4 Camera::getViewMatrix()
{
	return this->viewMatrix;
}

Matrix4x4 Camera::getProjMatrix()
{
	return this->projMatrix;
}

void Camera::setProjectionType(int type)
{
	this->type = type;
}