#include "RasterizerState.h"

#include "RenderSystem.h"
#include "Logger.h"

using namespace graphics;

RasterizerState::RasterizerState(RenderSystem* system) : system(system)
{
}

RasterizerState::~RasterizerState()
{
}

void RasterizerState::updateState()
{
	//GraphicsEngine::getInstance()->getImmediateDeviceContext()->setRasterizerState(this);
}

bool RasterizerState::init(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode)
{
	D3D11_RASTERIZER_DESC desc = {};
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = fillMode;
	desc.CullMode = cullMode;

	if (!debug::Logger::log(this, this->system->m_d3d_device->CreateRasterizerState(&desc, &m_rasterizer_state)))
		return false;

	return true;
}

bool RasterizerState::release()
{
	m_rasterizer_state->Release();
	delete this;
	return true;
}

void RasterizerState::setFillMode(D3D11_FILL_MODE fillMode)
{
	/*D3D11_RASTERIZER_DESC desc = {};
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	m_rasterizer_state->GetDesc(&desc);
	desc.FillMode = fillMode;
	GraphicsEngine::getInstance()->m_d3d_device->CreateRasterizerState(&desc, &m_rasterizer_state);
	updateState();*/
}
