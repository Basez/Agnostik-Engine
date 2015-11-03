#include "asharedh.hpp"
#include "adrawcommander.hpp"
#include "adrawcommand.hpp"
#include <algorithm>

using namespace glm;

AGN::ADrawCommander::ADrawCommander()
{
	// TODO: Check if this works on gcc
	static_assert((uint64_t)SortKeyBitAmount::RenderPhase +
		(uint64_t)SortKeyBitAmount::Layer +
		(uint64_t)SortKeyBitAmount::TranslucencyType +
		(uint64_t)SortKeyBitAmount::CMD +
		(uint64_t)SortKeyBitAmount::ShaderPipelineID +
		(uint64_t)SortKeyBitAmount::MeshID +
		(uint64_t)SortKeyBitAmount::MaterialID +
		(uint64_t)SortKeyBitAmount::Depth == 64, "Sortkey bits do not add up to 64!");
}

AGN::ADrawCommand& AGN::ADrawCommander::addDrawCommand(AGN::EADrawCommandType a_type, uint64_t a_sortKey) // TODO: key as argument?
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
	// check if key values are under limit
	if (a_renderPhase >= static_cast<uint8_t>(1)		<< static_cast<uint8_t>(SortKeyBitAmount::RenderPhase) ||
		a_layer >= static_cast<uint8_t>(1)				<< static_cast<uint8_t>(SortKeyBitAmount::Layer) ||
		a_translucencyType >= static_cast<uint8_t>(1)	<< static_cast<uint8_t>(SortKeyBitAmount::TranslucencyType) ||
		a_cmd >= static_cast<uint8_t>(1)				<< static_cast<uint8_t>(SortKeyBitAmount::CMD) ||
		a_shaderPipelineId >= static_cast<uint16_t>(1)	<< static_cast<uint16_t>(SortKeyBitAmount::ShaderPipelineID) ||
		a_meshId >= static_cast<uint32_t>(1)			<< static_cast<uint32_t>(SortKeyBitAmount::MeshID) ||
		a_materialId >= static_cast<uint16_t>(1)		<< static_cast<uint16_t>(SortKeyBitAmount::MaterialID) ||
		a_depth >= static_cast<uint32_t>(1)				<< static_cast<uint32_t>(SortKeyBitAmount::Depth))
	{
		g_log.error("Value gone overflow when generating Sortkey!");
		assert(false);
		return 0;
	}
#endif // AGN_DEBUG
	
	uint64_t sortKey = 0;
	sortKey += static_cast<uint64_t>(a_renderPhase)			<< static_cast<int>(SortKeyShift::RenderPhase);
	sortKey += static_cast<uint64_t>(a_layer)				<< static_cast<int>(SortKeyShift::Layer);
	sortKey += static_cast<uint64_t>(a_translucencyType)	<< static_cast<int>(SortKeyShift::TranslucencyType);
	sortKey += static_cast<uint64_t>(a_cmd)					<< static_cast<int>(SortKeyShift::CMD);
	sortKey += static_cast<uint64_t>(a_shaderPipelineId)	<< static_cast<int>(SortKeyShift::ShaderPipelineID);
	sortKey += static_cast<uint64_t>(a_meshId)				<< static_cast<int>(SortKeyShift::MeshID);
	sortKey += static_cast<uint64_t>(a_materialId)			<< static_cast<int>(SortKeyShift::MaterialID);
	sortKey += static_cast<uint64_t>(a_depth)				<< static_cast<int>(SortKeyShift::Depth);

	return sortKey;
}
