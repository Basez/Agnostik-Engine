#include "shared.hpp"
#include <algorithm>

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "draw_commander.hpp"
#include "draw_command.hpp"

using namespace glm;

AGN::DrawCommander::DrawCommander()
{
	// TODO: Check if this works on gcc
	static_assert((uint64_t)ESortKeyBitAmount::RenderPhase +
		(uint64_t)ESortKeyBitAmount::Layer +
		(uint64_t)ESortKeyBitAmount::TranslucencyType +
		(uint64_t)ESortKeyBitAmount::CMD +
		(uint64_t)ESortKeyBitAmount::ShaderPipelineID +
		(uint64_t)ESortKeyBitAmount::MeshID +
		(uint64_t)ESortKeyBitAmount::MaterialID +
		(uint64_t)ESortKeyBitAmount::Depth == 64, "Sortkey bits do not add up to 64!");
}

AGN::DrawCommand& AGN::DrawCommander::addDrawCommand(AGN::EDrawCommandType a_type, uint64_t a_sortKey) // TODO: key as argument?
{
	// TODO: use pooling?
	DrawCommand* drawCommandMesh = new DrawCommand(a_sortKey, a_type);
	m_drawCommands.push_back(drawCommandMesh);
	return *drawCommandMesh;
}

void AGN::DrawCommander::sortCommandList()
{
	// TODO: Multithread this
	// TODO: Find a method of sorting it even quicker! (not re-arraging pointers for example)
	std::sort(m_drawCommands.begin(), m_drawCommands.end(), AGN::SortDrawCommand());
}

std::vector<AGN::DrawCommand*> AGN::DrawCommander::getSortedDrawCommands()
{
	// TODO: Check if sorted?
	return m_drawCommands;
}

void AGN::DrawCommander::clearCommands()
{
	while (m_drawCommands.size() > 0)
	{
		delete m_drawCommands[0];
		m_drawCommands.erase(m_drawCommands.begin());
	}
}

uint64_t AGN::DrawCommander::getSortKey(uint8_t& a_renderPhase,
	uint8_t& a_layer,
	uint8_t& a_translucencyType,
	uint8_t& a_cmd,
	uint16_t& a_shaderPipelineId,
	uint32_t& a_meshId,
	uint16_t& a_materialId,
	uint32_t& a_depth)
{
#ifdef AGN_DEBUG
	// check if key values are under limit
	if (a_renderPhase >= static_cast<uint8_t>(1)		<< static_cast<uint8_t>(ESortKeyBitAmount::RenderPhase) ||
		a_layer >= static_cast<uint8_t>(1)				<< static_cast<uint8_t>(ESortKeyBitAmount::Layer) ||
		a_translucencyType >= static_cast<uint8_t>(1)	<< static_cast<uint8_t>(ESortKeyBitAmount::TranslucencyType) ||
		a_cmd >= static_cast<uint8_t>(1)				<< static_cast<uint8_t>(ESortKeyBitAmount::CMD) ||
		a_shaderPipelineId >= static_cast<uint16_t>(1)	<< static_cast<uint16_t>(ESortKeyBitAmount::ShaderPipelineID) ||
		a_meshId >= static_cast<uint32_t>(1)			<< static_cast<uint32_t>(ESortKeyBitAmount::MeshID) ||
		a_materialId >= static_cast<uint16_t>(1)		<< static_cast<uint16_t>(ESortKeyBitAmount::MaterialID) ||
		a_depth >= static_cast<uint32_t>(1)				<< static_cast<uint32_t>(ESortKeyBitAmount::Depth))
	{
		g_log.error("Value gone overflow when generating Sortkey!");
		assert(false);
		return 0;
	}
#endif // AGN_DEBUG
	
	uint64_t sortKey = 0;
	sortKey += static_cast<uint64_t>(a_renderPhase)			<< static_cast<int>(ESortKeyShift::RenderPhase);
	sortKey += static_cast<uint64_t>(a_layer)				<< static_cast<int>(ESortKeyShift::Layer);
	sortKey += static_cast<uint64_t>(a_translucencyType)	<< static_cast<int>(ESortKeyShift::TranslucencyType);
	sortKey += static_cast<uint64_t>(a_cmd)					<< static_cast<int>(ESortKeyShift::CMD);
	sortKey += static_cast<uint64_t>(a_shaderPipelineId)	<< static_cast<int>(ESortKeyShift::ShaderPipelineID);
	sortKey += static_cast<uint64_t>(a_meshId)				<< static_cast<int>(ESortKeyShift::MeshID);
	sortKey += static_cast<uint64_t>(a_materialId)			<< static_cast<int>(ESortKeyShift::MaterialID);
	sortKey += static_cast<uint64_t>(a_depth)				<< static_cast<int>(ESortKeyShift::Depth);

	return sortKey;
}
