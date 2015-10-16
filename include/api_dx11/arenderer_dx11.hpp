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
		ARendererDX11(class ARenderAPIDX11& a_renderAPIReference, class ADeviceDX11& a_deviceReference, class AWindowDX11& a_windowReference);
		bool init() override;
		void render(class ADrawCommander& a_drawCommander) override;
		void setCamera(class ACamera* a_camera) override { m_currentCamera = a_camera; }

	private:
		void drawEntity(struct ADrawCommand& a_command);
		void clearBuffer(struct ADrawCommand& a_command);

		class ADeviceDX11& m_deviceReference;
		class AWindowDX11& m_windowReference;
		class ARenderAPIDX11& m_renderAPIReference;
		
		class ACamera* m_currentCamera;
		glm::mat4 m_vp;

		// TODO: experiment with this, as I am not sure if keeping track of bound mesh/material/shaders will improve performence for either DX11 or OpenGL!
		//class AMeshDX11* m_boundMesh;
		//class AMaterial* m_boundMaterial;
		//class AShaderPipelineDX11* m_boundShaderPipeline;

		ID3D11RenderTargetView* m_d3dRenderTargetView;		// Render target view for the back buffer of the swap chain.
		ID3D11DepthStencilView* m_d3dDepthStencilView;		// Depth/stencil view for use as a depth buffer.
		ID3D11Texture2D* m_d3dDepthStencilBuffer;			// A texture to associate to the depth stencil view.
		ID3D11DepthStencilState* m_d3dDepthStencilState;	// Define the functionality of the depth/stencil stages.
		ID3D11RasterizerState* m_d3dRasterizerState;		// Define the functionality of the rasterizer stage.
		D3D11_VIEWPORT* m_viewport;

	};
}