#pragma once

#include "i_device.hpp"

// fwd declare
struct DXGI_RATIONAL;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11Debug;
struct ID3D11DeviceChild;
struct ID3DUserDefinedAnnotation;

namespace AGN
{
	class DeviceDX11 : public IDevice
	{
	public:
		DeviceDX11();
		~DeviceDX11() override;

		bool init(class WindowDX11* a_window);
		class IMesh* createMesh(const uint16_t a_aId, struct MeshData* a_meshData) override;
		class ITexture* createTexture(const uint16_t a_aId, struct TextureData* a_textureData) override;
		class IShader* createShader(const uint16_t a_aId, const char* a_shaderSource, EShaderType a_type) override;
		class IShaderPipeline* createShaderPipeline(const uint16_t a_aId, std::vector<class IShader*> a_shaders) override;

		void logLiveObjects();
		void onWindowUpdated(glm::ivec2 a_dimentions);
		DXGI_RATIONAL queryRefreshRate(bool a_vsync);
		ID3D11Device* getD3D11Device() { return m_d3d11Device; }
		ID3D11DeviceContext* getD3D11DeviceContext() { return m_d3d11DeviceContext; }
		IDXGISwapChain* getD3D11SwapChain() { return m_d3d11SwapChain; }
		
		// debug functionality
		static void setDebugName(ID3D11DeviceChild* a_child, const std::string& a_name, const uint32_t a_lineNum = 0, const char* a_fileName = "");
		void beginDebugEvent(const std::string& a_eventName);
		void setDebugMarker(const std::string& a_markerName);
		void endDebugEvent();

	private:
		class WindowDX11* m_window;
		ID3D11Device* m_d3d11Device;
		ID3D11DeviceContext* m_d3d11DeviceContext;
		IDXGISwapChain* m_d3d11SwapChain;
		ID3D11Debug* m_d3dDebug;
		ID3DUserDefinedAnnotation* m_debugUDA;
	};
}

#define D3D11_SET_DEBUG_NAME(child, name) AGN::DeviceDX11::setDebugName(child, name, __LINE__, __FILE__)
