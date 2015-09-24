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
#include "ashaderpipeline_gl.hpp"

using namespace glm;

static const vec4 g_white(1);
static const vec4 g_black(0);
static const vec4 g_ambient(0.2f, 0.2f, 0.2f, 1.0f);

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

	// make sure the camera has the correct ViewSettings
	m_currentCamera->setProjectionRH(60.0f, 0.1f, 10000.0f);

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
	AShaderPipelineGL* shaderPipeline = dynamic_cast<AShaderPipelineGL*>(entity->getShaderPipeline());
	
	// get previous data 
	AEntity* prevEntity = nullptr;
	AMeshGL* prevMesh = nullptr;
	AMaterial* prevMaterial = nullptr;
	AShaderPipelineGL* prevShaderPipeline = nullptr;

	if (a_prevCommand != nullptr && a_prevCommand->type == EADrawCommandType::DrawEntity)
	{
		prevEntity = a_prevCommand->data.entityData.entity;
		prevMesh = dynamic_cast<AMeshGL*>(prevEntity->getMesh());
		prevMaterial = dynamic_cast<AMaterial*>(prevEntity->getMaterial());
		prevShaderPipeline = dynamic_cast<AShaderPipelineGL*>(prevEntity->getShaderPipeline());
	}

	// different mesh? bind VAO
	if (prevMesh == nullptr || mesh->getAId() != prevMesh->getAId())
	{
		glBindVertexArray(mesh->getVao());
	}

	// different shader? 
	if (prevShaderPipeline == nullptr || shaderPipeline->getAId() != prevShaderPipeline->getAId())
	{
		shaderPipeline->bind();
	}

	// different material? 
	if (prevMaterial == nullptr || material->getAId() != prevMaterial->getAId())
	{
		// TODO: make dynamic?
		ATextureGL* diffuse = dynamic_cast<ATextureGL*>(material->getDiffuseTexture());
		ATextureGL* normal = dynamic_cast<ATextureGL*>(material->getNormalTexture());		// TODO:
		ATextureGL* specular = dynamic_cast<ATextureGL*>(material->getSpecularTexture());	// TODO:

		//GLuint texturesToBind[3] = { m_textureDiffuse, m_textureNormal, m_textureSpecular };
		ATextureGL* texturesToBind[1] = { diffuse };
		bindTexturesToShader(shaderPipeline->getGlProgramId(), 1, texturesToBind);

		// Material properties
		glUniform4fv(shaderPipeline->getUniformIdByName("uMaterialEmissive"), 1, glm::value_ptr(g_black));
		glUniform4fv(shaderPipeline->getUniformIdByName("uMaterialDiffuse"), 1, glm::value_ptr(g_white));
	}

	// Set object individual uniforms
	// This will be executed for every entity that is rendered
	{
		// calculate model matrix
		mat4 translation = glm::translate(entity->getPosition());
		mat4 rotation = toMat4(entity->getRotation());
		mat4 scaling = scale(entity->getScale());
		mat4 modelMatrix = translation * rotation * scaling;

		mat4 mvp = m_vp * modelMatrix;

		// set entity specific data
		glUniformMatrix4fv(shaderPipeline->getUniformIdByName("uModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix4fv(shaderPipeline->getUniformIdByName("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	}

	// render the entity
	glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	
	
#ifdef AGN_DEBUG
	AGN::getOpenGLError();
#endif

	/*
	// unbind ??
	unbindShaders();
	glBindVertexArray(0);
	glDisable(GL_CLIP_DISTANCE0);
	*/
}

void AGN::ARendererGL::bindTexturesToShader(GLuint a_shaderProgram, GLuint a_textureCount, ATextureGL** a_textureArray)
{
	if (a_textureCount > 32)
	{
		g_log.error("Max textures to bind to a shader is 32!");
		return;
	}

	// bind them one by one
	for (unsigned int i = 0; i < a_textureCount; i++)
	{
		if (a_textureArray[i]->getGlId() == GLuint(-1))
		{
			g_log.error("one of the textures is invalid (-1)");
			return;
		}

		std::string samplerName = std::string("textureSampler").append(std::to_string(i));
		GLuint uniformSampler = glGetUniformLocation(a_shaderProgram, samplerName.c_str());

		if (uniformSampler == (unsigned int)(-1))
		{
			g_log.error("could not find uniform texture sampler with the name: %s", samplerName.c_str());
			return;
		}
		
		GLenum glType = a_textureArray[i]->getGlType();

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(glType, a_textureArray[i]->getGlId());
		glUniform1i(uniformSampler, i);
	}
}