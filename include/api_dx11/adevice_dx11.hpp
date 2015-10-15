#pragma once

#include "iadevice.hpp"

// fwd declare
struct DXGI_RATIONAL;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

namespace AGN
{
	class ADeviceDX11 : public IADevice
	{
	public:
		ADeviceDX11(class AWindowDX11* a_window);
		bool init() override;
		class IAMesh* createMesh(const uint16_t a_aId, struct AMeshData* a_meshData) override;
		class IATexture* createTexture(const uint16_t a_aId, struct ATextureData* a_textureData) override;
		class IAShader* createShader(const uint16_t a_aId, const char* a_shaderSource, EAShaderType a_type) override;
		class IAShaderPipeline* createShaderPipeline(const uint16_t a_aId, std::vector<class IAShader*> a_shaders) override;

		DXGI_RATIONAL queryRefreshRate(bool a_vsync);
		ID3D11Device* getD3D11Device() { return m_d3d11Device; }
		ID3D11DeviceContext* getD3D11DeviceContext() { return m_d3d11DeviceContext; }
		IDXGISwapChain* getD3D11SwapChain() { return m_d3d11SwapChain; }

	private:

		class AWindowDX11* m_window;

		ID3D11Device* m_d3d11Device;
		ID3D11DeviceContext* m_d3d11DeviceContext;
		IDXGISwapChain* m_d3d11SwapChain;
	};
}
