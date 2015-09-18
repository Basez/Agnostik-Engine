#include "asharedh.hpp"
#include "arenderer_gl.hpp"
#include "adrawcommand.hpp"
#include "adrawcommander.hpp"
#include "apixelutils.hpp"
#include "aaplication.hpp"
#include "awindow_gl.hpp"
#include "arender_api_gl.hpp"
#include "amesh_gl.hpp"
#include "atexture_gl.hpp"
#include "amaterial.hpp"
#include "acamera.hpp"
#include "aentity.hpp"

using namespace glm;

AGN::ARendererGL::ARendererGL()
	: m_currentCamera(nullptr)
{

}

void AGN::ARendererGL::init()
{
	
}

void AGN::ARendererGL::render(AGN::ADrawCommander& a_drawCommander)
{
	if (m_currentCamera == nullptr)
	{
		g_log.error("Camera was not set, cannot render!");
		return;
	}

	// precalculate matrices that we are going to re-use through the frame
	m_vp = m_currentCamera->getProjectionMatrix() * m_currentCamera->getViewMatrix();

	// loop through sorted draw commands & draw em
	std::vector<ADrawCommand*> list = a_drawCommander.getSortedDrawCommands();

	for (unsigned int i = 0; i < list.size(); i++)
	{
		ADrawCommand* command = list[i];
		ADrawCommand* prevCommand = (i != 0) ? list[i - 1] : nullptr;

		switch (command->type)
		{
		case EADrawCommandType::ClearBuffer:
			float a, r, g, b;
			PixelUtils::getARGBFloat(command->data.clearcolorData.clearColor, a, r, g, b);
			glClearColor(r, g, b, 1.0f);
			glClear(command->data.clearcolorData.buffersToClear);
			break;

		case EADrawCommandType::DrawEntity:
			drawEntity(command, prevCommand);
			break;

		case EADrawCommandType::SwapBackBuffer:
			AWindowGL& window = dynamic_cast<AWindowGL&>(g_application.getRenderAPI().getWindow());
			SDL_GL_SwapWindow(window.getSDLWindow());
			break;
		}
	}
}

void AGN::ARendererGL::drawEntity(ADrawCommand* a_command, ADrawCommand* a_prevCommand)
{
	AEntity* entity = a_command->data.entityData.entity;
	AMeshGL* mesh = dynamic_cast<AMeshGL*>(entity->getMesh());
	AMaterial* material = dynamic_cast<AMaterial*>(entity->getMaterial());
	
	// get previous data 
	AEntity* prevEntity = nullptr;
	AMeshGL* prevMesh = nullptr;
	AMaterial* prevMaterial = nullptr;
	if (a_prevCommand != nullptr && a_prevCommand->type == EADrawCommandType::DrawEntity)
	{
		prevEntity = a_prevCommand->data.entityData.entity;
		prevMesh = dynamic_cast<AMeshGL*>(prevEntity->getMesh());
		prevMaterial = dynamic_cast<AMaterial*>(prevEntity->getMaterial());
	}

	// different mesh? bind VAO
	if (prevMesh == nullptr || mesh->getAId() != prevMesh->getAId())
	{
		glBindVertexArray(mesh->getVao());
	}

	// different shader? 
	if (prevMaterial == nullptr || material->getAId() != prevMaterial->getAId())
	{
		// TODO: set uniforms!
		/*
		//GLuint texturesToBind[3] = { m_textureDiffuse, m_textureNormal, m_textureSpecular };
		BTexture* texturesToBind[1] = { m_textureDiffuse };
		ShaderManager::bindTexturesToShader(m_shaderProgram, 1, texturesToBind);

		static const glm::vec4 white(1);
		static const glm::vec4 black(0);
		static const glm::vec4 ambient(0.2f, 0.2f, 0.2f, 1.0f);

		// Material properties.
		glUniform4fv(m_uniformMaterialEmissive, 1, glm::value_ptr(black));
		glUniform4fv(m_uniformMaterialDiffuse, 1, glm::value_ptr(white));
		glUniform4fv(m_uniformAmbient, 1, glm::value_ptr(ambient));
		glUniform4fv(m_uniformLightColor, 1, glm::value_ptr(white));
		*/
	}

	// TODO: set object individual uniforms
	// This will be executed for every entity that is rendered
	{
		// calculate model matrix
		mat4 translation = glm::translate(entity->getPosition());
		mat4 rotation = toMat4(entity->getRotation());
		mat4 scaling = scale(entity->getScale());
		mat4 modelMatrix = translation * rotation * scaling;

		// set data for shader
		//glUniformMatrix4fv(m_uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));
		//glUniformMatrix4fv(m_uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(a_modelMatrix));
		//glUniform4fv(m_uniformLightDirection, 1, glm::value_ptr(a_lightDirection));
		//glUniform4fv(m_uniformLightColor, 1, glm::value_ptr(a_lightColor));
	}

	// render the thing
	// glDrawElements(GL_TRIANGLES, m_indicies.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	
	
	/*
	// unbind ??
	unbindShaders();
	glBindVertexArray(0);
	glDisable(GL_CLIP_DISTANCE0);
	*/
}
