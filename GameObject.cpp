#include "GameObject.h"

#include "BaseComponentSystem.h"
#include "Logger.h"
#include "PhysicsSystem.h"

#include "EditorAction.h"
#include "GameObjectManager.h"
#include "PhysicsComponent.h"
#include "StringUtility.h"

namespace GDEngine
{
	AGameObject::AGameObject(std::string name)
	{
		HRESULT result = CoCreateGuid(&m_guid);
		if (!Logger::log(this, result)) {
			Logger::throw_exception("Game Object GUID creation failed");
		}

		this->m_name = name;
		this->m_type = EMPTY;
		this->m_localPosition = Vector3D::zero();
		this->m_localRotation = Vector3D::zero();
		this->m_localScale = Vector3D::one();
		this->m_orientation = AQuaternion(0, 0, 0, 1);
		this->m_physics = false;

		this->updateLocalMatrix();

		this->m_active = true;
	}

	AGameObject::AGameObject(std::string name, GameObjectType type)
	{
		HRESULT result = CoCreateGuid(&m_guid);
		if (!Logger::log(this, result)) {
			Logger::throw_exception("Game Object GUID creation failed");
		}

		this->m_name = name;
		this->m_type = type;
		this->m_localPosition = Vector3D::zero();
		this->m_localRotation = Vector3D::zero();
		this->m_localScale = Vector3D::one();
		this->m_orientation = AQuaternion(0, 0, 0, 1);
		this->m_physics = false;

		this->updateLocalMatrix();

		this->m_active = true;
	}

	AGameObject::AGameObject(std::string guid, std::string name, GameObjectType type)
	{
		std::wstring temp = std::wstring(guid.begin(), guid.end());
		Logger::log(temp);
		LPCWSTR guidstr = temp.c_str();

		HRESULT result = IIDFromString(guidstr, &m_guid);
		Logger::log(StringUtility::GuidToString(m_guid));
		Logger::log(name);
		if (!Logger::log(this, result)) {
			Logger::throw_exception("Conversion of GUID failed");
		}

		this->m_name = name;
		this->m_type = type;
		this->m_localPosition = Vector3D::zero();
		this->m_localRotation = Vector3D::zero();
		this->m_localScale = Vector3D::one();
		this->m_orientation = AQuaternion(0, 0, 0, 1);
		this->m_physics = false;

		this->updateLocalMatrix();

		this->m_active = true;
	}

	AGameObject::~AGameObject()
	{
		for (AComponent* component : m_componentList)
		{
			component->detachOwner();
		}
		this->m_componentList.clear();
	}

	void AGameObject::onCreate()
	{
	}

	void AGameObject::update(float deltaTime)
	{
		//Logger::log("Updating Game Object : " + this->m_name);
		this->updateLocalMatrix();
	}

	void AGameObject::draw(int width, int height)
	{
	}

	void AGameObject::setPosition(Vector3D position)
	{
		this->m_localPosition = position;
	}

	void AGameObject::setPosition(float x, float y, float z)
	{
		this->m_localPosition = Vector3D(x, y, z);

	}

	Vector3D AGameObject::getLocalPosition()
	{
		return this->m_localPosition;
	}

	void AGameObject::setScale(float x, float y, float z)
	{
		this->m_localScale = Vector3D(x, y, z);
	}

	void AGameObject::setScale(Vector3D scale)
	{
		this->m_localScale = scale;
	}

	Vector3D AGameObject::getLocalScale()
	{
		return this->m_localScale;
	}

	void AGameObject::setRotation(float x, float y, float z)
	{
		reactphysics3d::Quaternion quat = reactphysics3d::Quaternion::fromEulerAngles(x, y, z);
		this->m_localRotation = Vector3D(x, y, z);
		this->m_orientation = Vector4D(quat.x, quat.y, quat.z, quat.w);
	}

	void AGameObject::setRotation(Vector3D rotation)
	{
		this->m_localRotation = rotation;
		reactphysics3d::Quaternion quat = reactphysics3d::Quaternion::fromEulerAngles(rotation.x, rotation.y, rotation.z);
		this->m_orientation = Vector4D(quat.x, quat.y, quat.z, quat.w);
	}

	void AGameObject::setOrientation(AQuaternion orientation)
	{
		this->m_orientation = orientation;
	}

	Vector4D AGameObject::getOrientation()
	{
		return m_orientation;
	}

	Vector3D AGameObject::getLocalRotation()
	{
		return this->m_localRotation;
	}

	void AGameObject::setName(std::string name)
	{
		this->m_name = name;
	}

	std::string AGameObject::getName()
	{
		return this->m_name;
	}

	GUID AGameObject::getGuid()
	{
		return this->m_guid;
	}

	std::string AGameObject::getGuidString()
	{
		/*RPC_CSTR rpcString = NULL;

		UuidToStringA(&m_guid, &rpcString);
		std::string guidString = (char*)rpcString;
		::RpcStringFreeA(&rpcString);*/
		return StringUtility::GuidToString(m_guid);
	}

	GameObjectType AGameObject::getType()
	{
		return m_type;
	}

	void AGameObject::setType(GameObjectType type)
	{
		m_type = type;
	}

	void AGameObject::updateLocalMatrix()
	{
		if (!m_physics)
		{
			Matrix4x4 transform, rotation, translate;
			Matrix4x4 temp;

			// Scale
			transform.setIdentity();
			transform.setScale(this->m_localScale);

			// Scale * Rotation
			/*rotation.setIdentity();
			rotation.setRotationZ(this->m_localRotation.z);

			temp.setIdentity();
			temp.setRotationY(this->m_localRotation.y);
			rotation *= temp;

			temp.setIdentity();
			temp.setRotationX(this->m_localRotation.x);
			rotation *= temp;*/

			rotation.setIdentity();
			rotation.setRotation(this->m_orientation);

			// Scale * Rotation * Translation
			translate.setIdentity();
			translate.setTranslation(this->m_localPosition);


			translate *= rotation;
			transform *= translate;

			this->m_localMatrix = transform;
		}
	}

	void AGameObject::setLocalMatrix(float matrix[16])
	{
		Matrix4x4 transform, scale, translate, temp;

		/*transform.setIdentity();
		transform.setScale(this->m_localScale);

		temp.setIdentity();
		temp.setMatrix(matrix);

		transform *= temp;
		this->m_localMatrix = transform;*/

		//temp.setIdentity();
		//temp.setTranslation(this->m_localPosition);
		//transform *= temp;

		scale.setIdentity();
		scale.setScale(this->m_localScale);

		translate.setIdentity();
		translate.setTranslation(this->m_localPosition);

		temp.setIdentity();
		temp.setMatrix(matrix);
		//temp.getMatrix();
		//debug::Logger::log(temp.getTranslation().toString());

		translate *= temp;
		//translate.getMatrix();
		scale *= translate;
		transform = scale;

		this->m_localMatrix = transform;

		/*this->m_localPosition = Vector3D(matrix[12], matrix[13], matrix[14]);

		debug::Logger::log("[" + std::to_string(matrix[0]) + "," + std::to_string(matrix[1]) + "," + std::to_string(matrix[2]) + "," + std::to_string(matrix[3]) + "]");
		debug::Logger::log("[" + std::to_string(matrix[4]) + "," + std::to_string(matrix[5]) + "," + std::to_string(matrix[6]) + "," + std::to_string(matrix[7]) + "]");
		debug::Logger::log("[" + std::to_string(matrix[8]) + "," + std::to_string(matrix[9]) + "," + std::to_string(matrix[10]) + "," + std::to_string(matrix[11]) + "]");
		debug::Logger::log("[" + std::to_string(matrix[12]) + "," + std::to_string(matrix[13]) + "," + std::to_string(matrix[14]) + "," + std::to_string(matrix[15]) + "]");

		//matrix[12] = 0;
		//matrix[13] = 0;
		//matrix[14] = 0;

		scale.setIdentity();
		scale.setScale(this->m_localScale);

		/*temp.setIdentity();
		temp.setMatrix(matrix);

		temp.getMatrix();

		Matrix4x4 rotate;
		rotate.setIdentity();
		rotate.setRotation(m_orientation);
		rotate.transpose();
		rotate.getMatrix();

		translate.setIdentity();
		translate.setTranslation(this->m_localPosition);

		/*debug::Logger::log("[" + std::to_string(m_mat[0]) + "," + std::to_string(m_mat[1]) + "," + std::to_string(m_mat[2]) + "," + std::to_string(m_mat[3]) + "]");
		debug::Logger::log("[" + std::to_string(m_mat[4]) + "," + std::to_string(m_mat[5]) + "," + std::to_string(m_mat[6]) + "," + std::to_string(m_mat[7]) + "]");
		debug::Logger::log("[" + std::to_string(m_mat[8]) + "," + std::to_string(m_mat[9]) + "," + std::to_string(m_mat[10]) + "," + std::to_string(m_mat[11]) + "]");
		debug::Logger::log("[" + std::to_string(m_mat[12]) + "," + std::to_string(m_mat[13]) + "," + std::to_string(m_mat[14]) + "," + std::to_string(m_mat[15]) + "]");

		translate *= rotate;
		translate.getMatrix();
		scale *= translate;
		transform = scale;

		this->m_localMatrix = transform;*/
		//this->m_localPosition = this->m_localMatrix.getTranslation();
	}

	void AGameObject::setLocalMatrix(Vector3D position, AQuaternion orientation, float matrix[16])
	{
		Matrix4x4 transform, scale, translate, temp;

		this->m_localPosition = position;
		this->m_orientation = orientation;

		scale.setIdentity();
		scale.setScale(this->m_localScale);

		temp.setIdentity();
		temp.setMatrix(matrix);

		//translate *= temp;
		scale *= temp;
		transform = scale;

		this->m_localMatrix = transform;
	}

	float* AGameObject::getLocalMatrix()
	{
		return this->m_localMatrix.getMatrix();
	}


	float* AGameObject::getPhysicsLocalMatrix()
	{
		Matrix4x4 transform, rotation;
		Matrix4x4 temp;

		// Scale
		transform.setIdentity();
		transform.setScale(Vector3D::one());

		// Scale * Rotation
		rotation.setIdentity();
		/*rotation.setRotationZ(this->m_orientation.z);

		temp.setIdentity();
		temp.setRotationY(this->m_orientation.y);
		rotation *= temp;

		temp.setIdentity();
		temp.setRotationX(this->m_orientation.x);
		rotation *= temp;*/

		rotation.setRotation(this->m_orientation);

		transform *= rotation;
		// Scale * Rotation * Translation
		temp.setIdentity();
		temp.setTranslation(this->m_localPosition);
		transform *= temp;

		return transform.getMatrix();
	}

	bool AGameObject::isActive()
	{
		return this->m_active;

	}

	void AGameObject::setActive(bool active)
	{
		this->m_active = active;
	}

	bool AGameObject::isPhysics()
	{
		return this->m_physics;
	}

	void AGameObject::setPhysics(bool physics)
	{
		this->m_physics = physics;
	}

	void AGameObject::attachComponent(AComponent* component)
	{
		this->m_componentList.push_back(component);
		component->attachOwner(this);
	}

	void AGameObject::detachComponent(AComponent* component)
	{
		int index = -1;

		for (int i = 0; i < this->m_componentList.size() && index == -1; i++)
		{
			if (this->m_componentList[i] == component)
				index = i;
		}

		if (index != -1)
			this->m_componentList.erase(this->m_componentList.begin() + index);
	}

	AComponent* AGameObject::findComponentByName(std::string name)
	{
		for (int i = 0; i < this->m_componentList.size(); i++)
		{
			if (this->m_componentList[i]->getName() == name)
			{
				return this->m_componentList[i];
			}
		}

		return nullptr;
	}

	AComponent* AGameObject::findComponentOfType(AComponent::ComponentType type, std::string name)
	{
		for (int i = 0; i < this->m_componentList.size(); i++)
		{
			if (this->m_componentList[i]->getName() == name && this->m_componentList[i]->getType() == type)
			{
				return this->m_componentList[i];
			}
		}

		return NULL;
	}

	AGameObject::ComponentList AGameObject::getComponentsOfType(AComponent::ComponentType type)
	{
		std::vector<AComponent*> components;
		for (int i = 0; i < this->m_componentList.size(); i++)
		{
			if (this->m_componentList[i]->getType() == type)
			{
				components.push_back(this->m_componentList[i]);
			}
		}

		return components;
	}

	AGameObject::ComponentList AGameObject::getComponentsOfTypeRecursive(AComponent::ComponentType type)
	{
		std::vector<AComponent*> components;
		for (int i = 0; i < this->m_componentList.size(); i++)
		{
			if (this->m_componentList[i]->getType() == type)
			{
				components.push_back(this->m_componentList[i]);
			}
		}

		return components;
	}

	void AGameObject::saveEditState()
	{
		m_lastEditState = new EditorAction(this);
	}

	void AGameObject::restoreEditState()
	{
		if (m_lastEditState)
		{
			m_localPosition = m_lastEditState->getStoredPosition();
			m_localScale = m_lastEditState->getStoredScale();
			m_orientation = m_lastEditState->getStoredOrientation();
			m_localMatrix = m_lastEditState->getStoredMatrix();

			ComponentList physicsList = getComponentsOfType(AComponent::Physics);
			for (AComponent* component : physicsList)
			{
				PhysicsComponent* physicsComponent = dynamic_cast<PhysicsComponent*>(component);
				physicsComponent->setTransformFromOpenGL(m_localMatrix.getMatrix());
				physicsComponent->getRigidBody()->setAngularVelocity(Vector3(0, 0, 0));
				physicsComponent->getRigidBody()->setLinearVelocity(Vector3(0, 0, 0));
			}

			m_lastEditState = nullptr;
		}
	}

	void AGameObject::onDestroy()
	{
	}

}
