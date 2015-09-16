#include "asharedh.hpp"
#include "arenderer_gl.hpp"
#include "adrawcommand.hpp"
#include "adrawcommander.hpp"

AGN::ARendererGL::ARendererGL()
{

}

void AGN::ARendererGL::init()
{
	
}

void AGN::ARendererGL::render(AGN::ADrawCommander& a_drawCommander)
{
	// TODO: loop through sorted draw commands & draw em
	std::vector<ADrawCommandMesh*> list = a_drawCommander.getSortedMeshDrawCommands();

	for (ADrawCommandMesh* command : list)
	{
		// TODO: Draw Mesh
	}
}