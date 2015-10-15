#pragma once 

#include "iarenderer.hpp"

struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct D3D11_VIEWPORT;

namespace AGN
{
	class ARendererDX11 : public IARenderer 
	{
	public:
		ARendererDX11(class ADeviceDX11* a_device, class AWindowDX11* a_window);
		bool init() override;
		void render(class ADrawCommander& a_drawCommander) override;
		void setCamera(class ACamera* a_camera) override { m_currentCamera = a_camera; }

	private:
		void drawEntity(struct ADrawCommand* a_command);

		class ACamera* m_currentCamera;
		glm::mat4 m_vp;

		class AMeshDX11* m_boundMesh;
		class AMaterial* m_boundMaterial;
		class AShaderPipelineDX11* m_boundShaderPipeline;
		class ADeviceDX11* m_device;
		class AWindowDX11* m_window;

		ID3D11RenderTargetView* m_d3dRenderTargetView;		// Render target view for the back buffer of the swap chain.
		ID3D11DepthStencilView* m_d3dDepthStencilView;		// Depth/stencil view for use as a depth buffer.
		ID3D11Texture2D* m_d3dDepthStencilBuffer;			// A texture to associate to the depth stencil view.
		ID3D11DepthStencilState* m_d3dDepthStencilState;	// Define the functionality of the depth/stencil stages.
		ID3D11RasterizerState* m_d3dRasterizerState;		// Define the functionality of the rasterizer stage.
		D3D11_VIEWPORT* m_viewport;

	};
}