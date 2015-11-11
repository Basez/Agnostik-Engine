#pragma once 

#include "i_renderer.hpp"

// fwd declarations
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct D3D11_VIEWPORT;
struct ID3D11BlendState;

namespace AGN
{
	class RendererDX11 : public IRenderer 
	{
	public:
		RendererDX11(class RenderAPIDX11& a_renderAPIReference, class DeviceDX11& a_deviceReference, class WindowDX11& a_windowReference);
		~RendererDX11() override;

		bool init();
		void render(class DrawCommander& a_drawCommander) override;
		void setCamera(class Camera* a_camera) override { m_currentCamera = a_camera; }
		void onWindowUpdated(glm::ivec2 a_dimentions) override;

	private:
		void drawEntity(struct DrawCommand& a_command);
		void clearBuffer(struct DrawCommand& a_command);
		void setStaticStages();
		void renderGUI();
		class DeviceDX11& m_deviceReference;
		class WindowDX11& m_windowReference;
		class RenderAPIDX11& m_renderAPIReference;
		
		class Camera* m_currentCamera;
		glm::mat4 m_vp;

		// TODO: experiment with this, as I am not sure if keeping track of bound mesh/material/shaders will improve performence for either DX11 or OpenGL!
		class MeshDX11* m_boundMesh;
		class Material* m_boundMaterial;
		class ShaderPipelineDX11* m_boundShaderPipeline;

		ID3D11RenderTargetView* m_d3dRenderTargetView;		// Render target view for the back buffer of the swap chain.
		ID3D11DepthStencilView* m_d3dDepthStencilView;		// Depth/stencil view for use as a depth buffer.
		ID3D11Texture2D* m_d3dDepthStencilBuffer;			// A texture to associate to the depth stencil view.
		ID3D11DepthStencilState* m_d3dDepthStencilState;	// Define the functionality of the depth/stencil stages.
		ID3D11RasterizerState* m_d3dRasterizerState;		// Define the functionality of the rasterizer stage.
		ID3D11BlendState* m_d3dDefaultblendState;
		D3D11_VIEWPORT* m_viewport;

	};
}