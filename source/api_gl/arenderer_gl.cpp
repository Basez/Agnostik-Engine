#include "asharedh.hpp"
#include "arenderer_gl.hpp"
#include "adrawcommand.hpp"
#include "adrawcommander.hpp"
#include "apixelutils.hpp"
#include "aaplication.hpp"
#include "awindow_gl.hpp"
#include "iarender_api.hpp"

AGN::ARendererGL::ARendererGL()
{

}

void AGN::ARendererGL::init()
{
	
}

void AGN::ARendererGL::render(AGN::ADrawCommander& a_drawCommander)
{
	// loop through sorted draw commands & draw em
	std::vector<ADrawCommand*> list = a_drawCommander.getSortedDrawCommands();

	for (ADrawCommand* command : list)
	{
		switch (command->type)
		{
		case EADrawCommandType::ClearBuffer:
			float a, r, g, b;
			PixelUtils::getARGBFloat(command->data.clearcolorData.clearColor, a, r, g, b);
			glClearColor(r, g, b, 1.0f);
			glClear(command->data.clearcolorData.buffersToClear);
			break;

		case EADrawCommandType::DrawEntity:
			drawEntity(command);
			break;

		case EADrawCommandType::SwapBackBuffer:
			//IARenderAPI& api = g_application.getRenderAPI();
			AWindowGL& window = dynamic_cast<AWindowGL&>(g_application.getRenderAPI().getWindow());
			SDL_GL_SwapWindow(window.getSDLWindow());
			break;
		}
	}
}

void AGN::ARendererGL::drawEntity(struct ADrawCommand* a_command)
{

}
