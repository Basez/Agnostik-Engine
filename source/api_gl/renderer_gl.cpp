#include "shared.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

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

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <SDL/SDL.h>
#include <GL/glew.h>

using namespace glm;

static const vec4 g_white(1);
static const vec4 g_black(0);

AGN::RendererGL::RendererGL()
	: m_currentCamera(nullptr)
	, m_doNormalMapping(true)
	, m_doSpecularMapping(true)
{

}

AGN::RendererGL::~RendererGL()
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

		case EDrawCommandType::DrawGUI:
			ImGui::Render();
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
		TextureGL* normal = dynamic_cast<TextureGL*>(material->normalTexture);
		TextureGL* specular = dynamic_cast<TextureGL*>(material->specularTexture);

		const bool hasDiffuse = diffuse != nullptr;
		const bool hasNormal = (normal != nullptr && m_doNormalMapping);
		const bool hasSpecular = (specular != nullptr && m_doSpecularMapping);

		// TODO: refactor
		//uint32_t texturesToBind[3] = { m_textureDiffuse, m_textureNormal, m_textureSpecular };
		{
			TextureGL* texturesToBind[3] = { nullptr, nullptr, nullptr };
			static const char* const names[3] = { "diffuseSampler", "normalSampler", "specularSampler" };
			
			if (hasDiffuse) texturesToBind[0] = diffuse; 
			if (hasNormal) texturesToBind[1] = normal;
			if (hasSpecular) texturesToBind[2] = specular;
			
			bindTexturesToShader(shaderPipeline->getGlProgramId(), 3, texturesToBind, names);
		}
	
		if (shaderPipeline->hasConstantBuffer(EShaderType::PixelShader,"MaterialProperties"))
		{
			ConstantBufferGL* uniformConstantBuffer = shaderPipeline->getUniformConstantBufferByName("MaterialProperties");

			const size_t bufferSize = uniformConstantBuffer->size;
			uint8_t* buffer = new uint8_t[bufferSize]; // TODO: optimize with memory pool
			memset(buffer, 0, bufferSize);
			
			// TODO: super similar to DX11, abstract this!
			ConstantBufferPropertyGL* uTransparency = uniformConstantBuffer->getPropertyByName("uMaterialTransparency");
			ConstantBufferPropertyGL* uDiffuse = uniformConstantBuffer->getPropertyByName("uMaterialDiffuseColor");
			ConstantBufferPropertyGL* uAmbient = uniformConstantBuffer->getPropertyByName("uMaterialAmbientColor");
			ConstantBufferPropertyGL* uSpecularPower = uniformConstantBuffer->getPropertyByName("uMaterialSpecularPower");
			ConstantBufferPropertyGL* hasDiffuseProp = uniformConstantBuffer->getPropertyByName("uMaterialHasDiffuse");
			ConstantBufferPropertyGL* hasNormalProp = uniformConstantBuffer->getPropertyByName("uMaterialHasNormalMap");
			ConstantBufferPropertyGL* hasSpecularProp = uniformConstantBuffer->getPropertyByName("uMaterialHasSpecularMap");

			memcpy(buffer + uTransparency->offset, &material->transparency, sizeof(material->transparency));
			memcpy(buffer + uDiffuse->offset, glm::value_ptr(material->diffuseColor), sizeof(material->diffuseColor));
			memcpy(buffer + uAmbient->offset, glm::value_ptr(material->ambientColor), sizeof(material->ambientColor));
			memcpy(buffer + uSpecularPower->offset, &material->specularPower, sizeof(material->specularPower));
			memcpy(buffer + hasDiffuseProp->offset, &hasDiffuse, sizeof(hasDiffuse));
			memcpy(buffer + hasNormalProp->offset, &hasNormal, sizeof(hasNormal));
			memcpy(buffer + hasSpecularProp->offset, &hasSpecular, sizeof(hasSpecular));

			shaderPipeline->setConstantBufferData(EShaderType::PixelShader, "MaterialProperties", buffer, uniformConstantBuffer->size);

			delete[] buffer; // TODO: optimize with memory pool
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
	AGN::RenderAPIGL::getOpenGLErrors();
#endif

}

void AGN::RendererGL::bindTexturesToShader(uint32_t a_shaderProgram, uint32_t a_textureCount, TextureGL** a_textureArray, const char* const *a_uniformArray)
{
	if (a_textureCount > 32)
	{
		g_log.error("Max textures to bind to a shader is 32!");
		assert(false);
		return;
	}

	// bind them one by one
	for (unsigned int i = 0; i < a_textureCount; i++)
	{
		if (a_textureArray[i] != nullptr)
		{
			if (a_textureArray[i]->getGlId() == uint32_t(-1))
			{
				g_log.error("one of the textures is invalid (-1)");
				assert(false);
				return;
			}

			uint32_t uniformSampler = glGetUniformLocation(a_shaderProgram, a_uniformArray[i]);

			if (uniformSampler == (unsigned int)(-1))
			{
				g_log.error("could not find uniform texture sampler with the name: %s", a_uniformArray[i]);
				assert(false);
				return;
			}

			GLenum glType = a_textureArray[i]->getGlType();

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(glType, a_textureArray[i]->getGlId());
			glUniform1i(uniformSampler, i);

			glTexParameteri(glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else
		{
			// TODO: unbind?
			//glActiveTexture(GL_TEXTURE0 + i);
			//glBindTexture(GL_TEXTURE_2D, 0);
			//g_log.warning("Test texture unbinding");
		}
		
	}
}

uint32_t AGN::RendererGL::getGlEnumBuffers(uint32_t a_agnostikEnums)
{
	uint32_t accumilatedBuffer = 0;

	if (a_agnostikEnums & (uint32_t)EDrawBufferType::COLOR) accumilatedBuffer |= GL_COLOR_BUFFER_BIT;
	if (a_agnostikEnums & (uint32_t)EDrawBufferType::DEPTH) accumilatedBuffer |= GL_DEPTH_BUFFER_BIT;

	return accumilatedBuffer;
}
