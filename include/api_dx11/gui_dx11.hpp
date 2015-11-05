#pragma once
#include "i_gui.hpp"
#include <imgui/imgui.h>	// TODO: refactor out later

// fwd declare
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D10Blob;
struct ID3D11VertexShader;
struct ID3D11InputLayout;
struct ID3D11PixelShader;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;
struct ID3D11RasterizerState;
struct ID3D11BlendState;

namespace AGN
{
	class GUIDX11 : public IGUI
	{
	public:
		GUIDX11();
		void init(class RenderAPIDX11* a_api);
		//void processEvent(SDL_Event* a_event);
		void shutdown() override;
		void update(float a_deltaTime) override;
		void render(ImDrawData* draw_data);
		bool isEnabled() override { return m_isEnabled; }

	private:
		void invalidateDeviceObjects();
		void createDeviceObjects();
		void createImGUIFont();

#if defined(__GNUC__) || defined(__GNUG__)
		// GCC doesnt support static friend
		friend void renderDrawLists(ImDrawData* draw_data);
#else
		static friend void renderDrawLists(ImDrawData* draw_data);
#endif

		bool m_isEnabled;
		class RenderAPIDX11* m_renderAPI;
		ID3D11Buffer* m_vertexBuffer = NULL;
		ID3D11Buffer* m_indexBuffer = NULL;
		ID3D10Blob* m_vertexShaderBlob = NULL;
		ID3D11VertexShader* m_vertexShader = NULL;
		ID3D11InputLayout* m_inputLayout = NULL;
		ID3D11Buffer* m_vertexConstantBuffer = NULL;
		ID3D10Blob* m_pixelShaderBlob = NULL;
		ID3D11PixelShader* m_pixelShader = NULL;
		ID3D11SamplerState* m_fontSampler = NULL;
		ID3D11ShaderResourceView* m_fontTextureView = NULL;
		ID3D11RasterizerState* m_rasterizerState = NULL;
		ID3D11BlendState* m_blendState = NULL;
		int32_t m_vertexBufferSize = 5000;
		int32_t m_indexBufferSize = 10000;
	};
}