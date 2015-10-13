#include "asharedh.hpp"
#include "ashaderpipeline_dx11.hpp"
#include "ashader_dx11.hpp"
#include "aosutils.hpp"

using namespace glm;

AGN::AShaderPipelineDX11::AShaderPipelineDX11()
{
	
}

void AGN::AShaderPipelineDX11::bind()
{
	// TODO:
}


void AGN::AShaderPipelineDX11::setUniformBufferData(const char* a_name, void* a_data, size_t a_dataSize)
{
	// TODO:
}

bool AGN::AShaderPipelineDX11::hasUniformBuffer(const char* a_name)
{
	return false; // TODO:
}