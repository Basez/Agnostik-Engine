#pragma once 

#include "iarenderer.hpp"

namespace AGN
{
	class ARendererGL : public IARenderer 
	{
	public:
		ARendererGL();
		void init() override;
		void render(class ADrawCommander& a_drawCommander) override;
		void setCamera(class ACamera* a_camera) override { m_currentCamera = a_camera; }

	private:
		void drawEntity(struct ADrawCommand* a_command, struct ADrawCommand* a_prevCommand = nullptr);
		void bindTexturesToShader(GLuint a_shaderProgram, GLuint a_textureCount, class ATextureGL** a_textureArray);

		class ACamera* m_currentCamera;
		glm::mat4 m_vp;

	};
}