#include "asharedh.hpp"
#include "adrawcommander.hpp"
#include "adrawcommand.hpp"
#include <algorithm>

using namespace glm;

AGN::ADrawCommander::ADrawCommander()
{
#ifdef AGN_DEBUG
	AUSortKey keyasd = { 0 };
	
	if (sizeof(keyasd.bitfield) != sizeof(keyasd.value))
	{
		g_log.error("Bitfield is not using the same amount as its sortkey counterpart");
		assert(false);
	}
#endif // AGN_DEBUG

}

AGN::ADrawCommand& AGN::ADrawCommander::addDrawCommand(AGN::EADrawCommandType a_type, uint64_t a_sortKey)
{
	// TODO: use pooling?
	ADrawCommand* drawCommandMesh = new ADrawCommand(a_sortKey, a_type);
	m_drawCommands.push_back(drawCommandMesh);
	return *drawCommandMesh;
}

void AGN::ADrawCommander::sortCommandList()
{
	// TODO: Multithread this
	// TODO: Find a method of sorting it even quicker! (not re-arraging pointers for example)
	std::sort(m_drawCommands.begin(), m_drawCommands.end(), AGN::ASortDrawCommand());
}

std::vector<AGN::ADrawCommand*> AGN::ADrawCommander::getSortedDrawCommands()
{
	// TODO: Check if sorted?
	return m_drawCommands;
}

void AGN::ADrawCommander::clearCommands()
{
	while (m_drawCommands.size() > 0)
	{
		delete m_drawCommands[0];
		m_drawCommands.erase(m_drawCommands.begin());
	}
}

uint64_t AGN::ADrawCommander::getSortKey(uint8_t& a_renderPhase,
	uint8_t& a_layer,
	uint8_t& a_translucencyType,
	uint8_t& a_cmd,
	uint16_t& a_shaderPipelineId,
	uint32_t& a_meshId,
	uint16_t& a_materialId,
	uint32_t& a_depth)
{
	
#ifdef AGN_DEBUG
	// check if values are overflown
	if (a_renderPhase >= static_cast<uint8_t>(1)		<< static_cast<uint8_t>(RENDERPHASE_BITS) ||
		a_layer >= static_cast<uint8_t>(1)				<< static_cast<uint8_t>(LAYER_BITS) ||
		a_translucencyType >= static_cast<uint8_t>(1)	<< static_cast<uint8_t>(TRANSLUCENCYTYPE_BITS) ||
		a_cmd >= static_cast<uint8_t>(1)				<< static_cast<uint8_t>(CMD_BITS) ||
		a_shaderPipelineId >= static_cast<uint16_t>(1)	<< static_cast<uint16_t>(SHADERPIPELINE_BITS) ||
		a_meshId >= static_cast<uint32_t>(1)			<< static_cast<uint32_t>(MESH_BITS) ||
		a_materialId >= static_cast<uint16_t>(1)		<< static_cast<uint16_t>(MATERIAL_BITS) ||
		a_depth >= static_cast<uint32_t>(1)				<< static_cast<uint32_t>(DEPTH_BITS))
	{
		g_log.error("Value gone overflow when generating Sortkey!");
		assert(false);
		return 0;
	}
#endif // AGN_DEBUG

	// generate sortkey using bitfields
	AUSortKey AUsortkey;
	AUsortkey.bitfield.renderPhase = a_renderPhase;
	AUsortkey.bitfield.layer = a_layer;
	AUsortkey.bitfield.translucencyType = a_translucencyType;
	AUsortkey.bitfield.cmd = a_cmd;
	AUsortkey.bitfield.shaderPipelineID = a_shaderPipelineId;
	AUsortkey.bitfield.meshID = a_meshId;
	AUsortkey.bitfield.materialID = a_materialId;
	AUsortkey.bitfield.depth = a_depth;

	//size_t sizeofbitfield = sizeof(AUsortkey.bitfield);
	//size_t sizeofsortkeyValue = sizeof(AUsortkey.value);
	//size_t sizeofUnion = sizeof(AUsortkey);

	return AUsortkey.value;
}
