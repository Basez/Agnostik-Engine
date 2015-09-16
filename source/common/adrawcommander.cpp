#include "asharedh.hpp"
#include "adrawcommander.hpp"
#include "adrawcommand.hpp"

using namespace glm;

AGN::ADrawCommander::ADrawCommander()
{
}

AGN::ADrawCommand& AGN::ADrawCommander::addDrawCommand(AGN::EADrawCommandType a_type) // TODO: key as argument?
{
	// TODO: use pooling?
	// TODO: generate key? how will it work
	ADrawCommand* drawCommandMesh = new ADrawCommand(a_type);
	m_drawCommands.push_back(drawCommandMesh);
	return *drawCommandMesh;
}

void AGN::ADrawCommander::sortBuckets()
{
	// TODO: sort list / buckets
	// TODO: sort on key?
}


std::vector<AGN::ADrawCommand*> AGN::ADrawCommander::getSortedDrawCommands()
{
	// TODO: Sort
	// TODO: Multi-threaded
	// TODO: Refactor

	return m_drawCommands;
}

void AGN::ADrawCommander::clearCommands()
{
	// TODO: delete commands & clear vectors
	while (m_drawCommands.size() > 0)
	{
		delete m_drawCommands[0];
		m_drawCommands.erase(m_drawCommands.begin());
	}
}
