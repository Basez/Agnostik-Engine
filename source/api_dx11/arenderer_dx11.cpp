#include "asharedh.hpp"
#include "arenderer_dx11.hpp"
#include "adrawcommand.hpp"
#include "adrawcommander.hpp"
#include "apixelutils.hpp"
#include "aaplication.hpp"
#include "awindow_dx11.hpp"
#include "arender_api_dx11.hpp"
#include "amesh_dx11.hpp"
#include "atexture_dx11.hpp"
#include "amaterial.hpp"
#include "acamera.hpp"
#include "aentity.hpp"
#include "ashaderpipeline_dx11.hpp"

#include <glm/gtc/type_ptr.hpp>

using namespace glm;

static const vec4 g_white(1);
static const vec4 g_black(0);

AGN::ARendererDX11::ARendererDX11()
	: m_currentCamera(nullptr)
{

}

void AGN::ARendererDX11::init()
{
	
}

void AGN::ARendererDX11::render(AGN::ADrawCommander& a_drawCommander)
{
	/*
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
	std::vector<ADrawCommand*> list = a_drawCommander.getSortedDrawCommands();

	for (unsigned int i = 0; i < list.size(); i++)
	{
		ADrawCommand* command = list[i];

		switch (command->type)
		{
		case EADrawCommandType::ClearBuffer:
			// TODO: 
			break;

		case EADrawCommandType::DrawEntity:
			// TODO: 
			break;

		case EADrawCommandType::SwapBackBuffer:
			// TODO: 
			break;
		}
	}
	*/
	
	// TODO: unbind everything?
}

void AGN::ARendererDX11::drawEntity(ADrawCommand* a_command)
{
	// TODO:

}

