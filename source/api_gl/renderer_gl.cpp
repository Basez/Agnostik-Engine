#include "shared.hpp"
#include "renderer_gl.hpp"
#include "draw_command.hpp"
#include "draw_commander.hpp"
#include "pixel_utils.hpp"
#include "application.hpp"
#include "window_gl.hpp"
#include "render_api_gl.hpp"
#include "mesh_gl.hpp"
#include "texture_gl.hpp"
#include "material.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "shaderpipeline_gl.hpp"
#include "i_shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <SDL/SDL.h>
#include <GL/glew.h>

using namespace glm;

static const vec4 g_white(1);
static const vec4 g_black(0);

AGN::RendererGL::RendererGL()
	: m_currentCamera(nullptr)
{

}

bool AGN::RendererGL::init()
{
	return true;
}

void AGN::RendererGL::render(AGN::DrawCommander& a_drawCommander)
{
	if (m_currentCamera == nullptr)
	{
		g_log.error("Camera was not set, cannot render!");
		return;
	}

	m_boundMesh = nullptr;
	m_boundMaterial = nullptr;
	m_boundShaderPipeline = nullptr;

	// make sure the camera has the correct ViewSettings
	m_currentCamera->setProjectionRH(60.0f, 0.1f, 10000.0f);

	// pre-calculate matrices that we are going to re-use through the frame
	m_vp = m_currentCamera->getProjectionMatrix() * m_currentCamera->getViewMatrix();

	// loop through sorted draw commands & draw em
	std::vector<DrawCommand*> list = a_drawCommander.getSortedDrawCommands();

	for (unsigned int i = 0; i < list.size(); i++)
	{
		DrawCommand* command = list[i];

		switch (command->type)
		{
		case EDrawCommandType::ClearBuffer:
			
			float a, r, g, b;
			PixelUtils::getRGBAFloat(command->data.clearBufferData.clearColor, r, g, b, a);
			glClearColor(r, g, b, 1.0f);
			glClear(getGlEnumBuffers(command->data.clearBufferData.buffersToClear));
			break;

		case EDrawCommandType::DrawEntity:
			drawEntity(command);
			break;

		case EDrawCommandType::SwapBackBuffer:
			WindowGL& window = dynamic_cast<WindowGL&>(g_application.getRenderAPI().getWindow());
			SDL_GL_SwapWindow(window.getSDLWindow());
			break;
		}
	}

	// TODO: totally whipe state?
}

void AGN::RendererGL::drawEntity(DrawCommand* a_command)
{
	DrawEntityData& data = a_command->data.entityData;
	MeshGL* mesh = dynamic_cast<MeshGL*>(data.mesh);
	Material* material = dynamic_cast<Material*>(data.material);
	ShaderPipelineGL* shaderPipeline = dynamic_cast<ShaderPipelineGL*>(data.shaderPipeline);
	
	// different shader? 
	if (m_boundShaderPipeline == nullptr || m_boundShaderPipeline->getAId() != shaderPipeline->getAId())
	{
		shaderPipeline->bind();
		m_boundShaderPipeline = shaderPipeline;
	}

	// different material? 
	if (m_boundMaterial == nullptr || m_boundMaterial->getAId() != material->getAId())
	{
		TextureGL* diffuse = dynamic_cast<TextureGL*>(material->diffuseTexture);
		TextureGL* normal = dynamic_cast<TextureGL*>(material->normalTexture);		// TODO:
		TextureGL* specular = dynamic_cast<TextureGL*>(material->specularTexture);	// TODO:

		// TODO: refactor
		//uint32_t texturesToBind[3] = { m_textureDiffuse, m_textureNormal, m_textureSpecular };
		if (diffuse != nullptr)
		{
			TextureGL* texturesToBind[1] = { diffuse };
			bindTexturesToShader(shaderPipeline->getGlProgramId(), 1, texturesToBind);
		}
	
		if (shaderPipeline->hasConstantBuffer(EShaderType::PixelShader,"MaterialProperties"))
		{
			// TODO: get buffer offset, this is hardcoded
			unsigned char buffer[40];
			memcpy(buffer + 0, glm::value_ptr(material->diffuseColor), sizeof(material->diffuseColor)); // material diffuse
			memcpy(buffer + 12, glm::value_ptr(material->specularColor), sizeof(material->specularColor)); // material specular
			memcpy(buffer + 24, glm::value_ptr(material->ambientColor), sizeof(material->ambientColor)); // material ambient
			memcpy(buffer + 36, &material->transparency, sizeof(material->transparency)); // material transparency // TODO: check if this is correct

			shaderPipeline->setConstantBufferData(EShaderType::PixelShader, "MaterialProperties", &buffer, 32);
		}

		m_boundMaterial = material;
	}

	// different mesh? bind VAO
	if (m_boundMesh == nullptr || m_boundMesh->getAId() != mesh->getAId())
	{
		glBindVertexArray(mesh->getVao());
		m_boundMesh = mesh;
	}

	// Set object individual uniforms
	// This will be executed for every entity that is rendered
	{
		// retrieve model matrix from array in struct
		glm::mat4 modelMatrix = glm::make_mat4(data.modelMatrixArray);
		mat4 mvp = m_vp * modelMatrix;

		// set entity specific data
		glUniformMatrix4fv(shaderPipeline->getUniformIdByName("uModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));

		if (shaderPipeline->hasUniform("uModelMatrix"))
		{
			glUniformMatrix4fv(shaderPipeline->getUniformIdByName("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		}
	}

	// render the entity
	glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
	
#ifdef AGN_DEBUG
	AGN::RenderAPIGL::getOpenGLError();
#endif

}

void AGN::RendererGL::bindTexturesToShader(uint32_t a_shaderProgram, uint32_t a_textureCount, TextureGL** a_textureArray)
{
	if (a_textureCount > 32)
	{
		g_log.error("Max textures to bind to a shader is 32!");
		return;
	}

	// bind them one by one
	for (unsigned int i = 0; i < a_textureCount; i++)
	{
		if (a_textureArray[i]->getGlId() == uint32_t(-1))
		{
			g_log.error("one of the textures is invalid (-1)");
			return;
		}

		std::string samplerName = std::string("textureSampler").append(std::to_string(i));
		uint32_t uniformSampler = glGetUniformLocation(a_shaderProgram, samplerName.c_str());

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

uint32_t AGN::RendererGL::getGlEnumBuffers(uint32_t a_agnostikEnums)
{
	uint32_t accumilatedBuffer = 0;

	if (a_agnostikEnums & (uint32_t)EDrawBufferType::COLOR) accumilatedBuffer |= GL_COLOR_BUFFER_BIT;
	if (a_agnostikEnums & (uint32_t)EDrawBufferType::DEPTH) accumilatedBuffer |= GL_DEPTH_BUFFER_BIT;

	return accumilatedBuffer;
}
