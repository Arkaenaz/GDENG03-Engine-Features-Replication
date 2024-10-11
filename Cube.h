#pragma once

#include "GameObject.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "InputListener.h"

using namespace engine::graphics;

class Cube : public GameObject, public InputListener
{
private:
	vertex vertex_list[8];
	Vector3D color;

	VertexBuffer* m_vb;
	ConstantBuffer* m_cb;
	IndexBuffer* m_ib;
	VertexShader* m_vs;
	PixelShader* m_ps;

	float m_delta_pos;
	float m_angle;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

public:
	Cube(Vector3D position, Vector3D scale, Vector3D color);
	~Cube();

public:
	void onCreate() override;
	void update(float deltaTime) override;
	void draw() override;
	void onDestroy() override;

	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);

	void setPosition(Vector3D position) override;
	void setScale(Vector3D scale) override;
	void setColor(Vector3D color);
};
