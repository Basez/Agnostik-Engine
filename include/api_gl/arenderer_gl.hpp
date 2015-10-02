#pragma once 

#include "iarenderer.hpp"

namespace AGN
{
	class ARendererGL : public IARenderer 
	{
	public:
		ARendererGL();
		void init() override;
		void render(class AGN::ADrawCommander& a_drawCommander) override;
		void setCamera(class AGN::ACamera* a_camera) override { m_currentCamera = a_camera; }

	private:
		void drawEntity(struct ADrawCommand* a_command);
		void bindTexturesToShader(GLuint a_shaderProgram, GLuint a_textureCount, class ATextureGL** a_textureArray);

		class ACamera* m_currentCamera;
		glm::mat4 m_vp;

		class AMeshGL* m_boundMesh;
		class AMaterial* m_boundMaterial;
		class AShaderPipelineGL* m_boundShaderPipeline;

	};
}