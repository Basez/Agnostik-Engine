#include "asharedh.hpp"
#include "adrawcommander.hpp"
#include "adrawcommand.hpp"

using namespace glm;

AGN::ADrawCommander::ADrawCommander()
{
}

AGN::ADrawCommandMesh& AGN::ADrawCommander::addMeshDrawCommand()
{
	// TODO: use pooling?
	// TODO: generate key? how will it work
	ADrawCommandMesh* drawCommandMesh = new ADrawCommandMesh();
	drawCommandMesh->mesh = nullptr;
	drawCommandMesh->shaderProgram = nullptr;
	m_meshDrawCommands.push_back(drawCommandMesh);
	return *drawCommandMesh;
}

void AGN::ADrawCommander::sortBuckets()
{
	// TODO: sort list / buckets
	// TODO: sort on key?
}


std::vector<AGN::ADrawCommandMesh*> AGN::ADrawCommander::getSortedMeshDrawCommands()
{
	// TODO: Sort
	// TODO: Multi-threaded
	// TODO: Refactor

	return m_meshDrawCommands;
}

void AGN::ADrawCommander::clearBuckets()
{
	// TODO: delete commands & clear vectors
	while (m_meshDrawCommands.size() > 0)
	{
		delete m_meshDrawCommands[0];
		m_meshDrawCommands.erase(m_meshDrawCommands.begin());
	}
}
