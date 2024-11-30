#include "SceneCamera.h"

#include "AppWindow.h"
#include "EngineTime.h"
#include "InputSystem.h"

using namespace GDEngine;

SceneCamera::SceneCamera(std::string name) : Camera(name)
{
	m_localMatrix.setIdentity();
}

SceneCamera::~SceneCamera()
{
	Camera::~Camera();
}

void SceneCamera::update(float deltaTime)
{
	Camera::update(deltaTime);
	if (isControllable)
	{
		Vector3D newPosition = this->getLocalPosition();

		Matrix4x4 viewMatrix = this->viewMatrix;
		viewMatrix.inverse();

		if (InputSystem::getInstance()->getKey('W'))
		{
			newPosition += viewMatrix.getZDirection() * (speed * deltaTime); 
		}
		else if (InputSystem::getInstance()->getKey('S'))
		{
			newPosition -= viewMatrix.getZDirection() * (speed * deltaTime);
		}
		if (InputSystem::getInstance()->getKey('A'))
		{
			newPosition -= viewMatrix.getXDirection() * (speed * deltaTime);
		}
		else if (InputSystem::getInstance()->getKey('D'))
		{
			newPosition += viewMatrix.getXDirection() * (speed * deltaTime);
		}
		if (InputSystem::getInstance()->getKey('E'))
		{
			newPosition += viewMatrix.getYDirection() * (speed * deltaTime);
		}
		else if (InputSystem::getInstance()->getKey('Q'))
		{
			newPosition -= viewMatrix.getYDirection() * (speed * deltaTime);
		}

		// If type < 2
		// Scroll to zoom in or out: +- in Z direction

		setPosition(newPosition);
	}
}


void SceneCamera::setSpeed(float speed)
{
	this->speed = speed;
}

void SceneCamera::setControllable(bool controllable)
{
	this->isControllable = controllable;
}

void SceneCamera::onKeyDown(int key)
{
	if (key == VK_SHIFT) 
	{
		this->speed = 20.0f;
	}
}

void SceneCamera::onKeyUp(int key)
{
	if (key == VK_SHIFT)
	{
		this->speed = 1.0f;
	}
}

void SceneCamera::onMouseMove(const Vector2D& deltaMousePosition)
{
	if (isControllable)
	{
		/*RECT windowRect = AppWindow::getInstance()->getClientWindowRect();

		float m_width = windowRect.right - windowRect.left;
		float height = windowRect.bottom - windowRect.top;

		Vector2D position = Vector2D(windowRect.left + m_width / 2.0f, windowRect.top + height / 2.0f);

		InputSystem::getInstance()->setCursorPosition(position);*/

		m_localRotation.x += ((deltaMousePosition.y) - (InputSystem::getInstance()->getOldMousePosition().y)) * 0.2f * EngineTime::getDeltaTime();
		m_localRotation.y += ((deltaMousePosition.x) - (InputSystem::getInstance()->getOldMousePosition().x)) * 0.2f * EngineTime::getDeltaTime();
	}
}

void SceneCamera::onLeftMouseDown(const Vector2D& mousePosition)
{
}

void SceneCamera::onLeftMouseUp(const Vector2D& mousePosition)
{
}

void SceneCamera::onRightMouseDown(const Vector2D& mousePosition)
{
	isControllable = true;
}

void SceneCamera::onRightMouseUp(const Vector2D& mousePosition)
{
	isControllable = false;
}

