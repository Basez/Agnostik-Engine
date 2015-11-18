#pragma once
#include "i_imgui.hpp"

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
struct ImDrawData;

namespace AGN
{
	class ImGuiDX11 : public IImGui
	{
	public:
		ImGuiDX11();
		~ImGuiDX11() override;

		void init(class RenderAPIDX11* a_api);
		void shutdown() override;
		void update(float a_deltaTime) override;
		void render(ImDrawData* draw_data);
		bool isEnabled() override { return m_isEnabled; }
		void setEnabled(bool a_value) { m_isEnabled = a_value; }

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
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		ID3D10Blob* m_vertexShaderBlob;
		ID3D11VertexShader* m_vertexShader;
		ID3D11InputLayout* m_inputLayout;
		ID3D11Buffer* m_vertexConstantBuffer;
		ID3D10Blob* m_pixelShaderBlob;
		ID3D11PixelShader* m_pixelShader;
		ID3D11SamplerState* m_fontSampler;
		ID3D11ShaderResourceView* m_fontTextureView;
		ID3D11RasterizerState* m_rasterizerState;
		ID3D11BlendState* m_blendState;
		int32_t m_vertexBufferSize;
		int32_t m_indexBufferSize;
	};
}