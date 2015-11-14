#pragma once 

#include "i_renderer.hpp"

namespace AGN
{
	class RendererGL : public IRenderer 
	{
	public:
		RendererGL();
		~RendererGL() override;

		bool init();
		void render(class DrawCommander& a_drawCommander) override;
		void setCamera(class Camera* a_camera) override { m_currentCamera = a_camera; }

	private:
		void drawEntity(struct DrawCommand* a_command);
		void bindTexturesToShader(uint32_t a_shaderProgram, uint32_t a_textureCount, class TextureGL** a_textureArray, const char* const *a_uniformArray);
		uint32_t getGlEnumBuffers(uint32_t a_agnostikEnums);

		class Camera* m_currentCamera;
		glm::mat4 m_vp;

		class MeshGL* m_boundMesh;
		class Material* m_boundMaterial;
		class ShaderPipelineGL* m_boundShaderPipeline;

	};
}