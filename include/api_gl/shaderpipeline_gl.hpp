#pragma once

#include "i_shader_pipeline.hpp"

namespace AGN
{

	class ShaderPipelineGL : public IShaderPipeline
	{
	public:
		ShaderPipelineGL(const uint32_t a_glprogramId, ShaderPipelineData& a_data);
		~ShaderPipelineGL() override;

		class IShader* getShader(const EShaderType a_type) override;
		uint16_t getAId() { return m_aId; }
		void setConstantBufferData(const EShaderType a_shader, const char* a_name, void* a_data, size_t a_dataSize) override;
		virtual bool hasConstantBuffer(const EShaderType a_shader, const char* a_name) override;

		uint32_t getGlProgramId() { return m_glProgramId; }
		int32_t getUniformIdByName(const char* a_name);
		bool hasUniform(const char* a_name);
		struct ConstantBufferGL* getConstantBufferByName(const char* a_name);
		
		static const int MAX_UNIFORM_NAME = 128;

	private:
		const uint16_t m_aId;				
		const uint32_t m_glProgramId;
		class IShader* m_shaders[6];
		std::vector<struct ConstantBufferGL*> m_constantBuffers;
		int32_t m_uniformPropertyCount;
	};	

	struct ConstantBufferPropertyGL
	{
		char name[ShaderPipelineGL::MAX_UNIFORM_NAME];
		int32_t id;
		int32_t offset;
	};

	struct ConstantBufferGL
	{
		ConstantBufferPropertyGL* getPropertyByName(const char* a_name);

		int32_t index;
		char name[ShaderPipelineGL::MAX_UNIFORM_NAME];
		int32_t size;
		uint32_t glID;
		int32_t uniformProperty;
		uint8_t* buffer;
		std::vector<ConstantBufferPropertyGL*> propertyList;
	};

}