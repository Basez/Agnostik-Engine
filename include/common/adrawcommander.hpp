#pragma once

namespace AGN
{
	enum class EADrawCommandType;

	class ADrawCommander
	{
	public:
		ADrawCommander();
		
		static uint64_t getSortKey(
			uint8_t& a_renderPhase,
			uint8_t& a_layer,
			uint8_t& a_translucencyType,
			uint8_t& a_cmd,
			uint16_t& a_shaderPipelineId,
			uint32_t& a_meshId,
			uint16_t& a_materialId,
			uint32_t& a_depth);

		void sortCommandList();
		void clearCommands();
		struct ADrawCommand& addDrawCommand(EADrawCommandType a_type, uint64_t a_sortKey);
		std::vector<struct ADrawCommand*> getSortedDrawCommands();

		// num of bits each sort key property is occupying
		enum class SortKeyBitAmount
		{
			Depth = 16,
			MaterialID = 12,
			MeshID = 16,
			ShaderPipelineID = 10,
			CMD = 2,
			TranslucencyType = 2,
			Layer = 3,
			RenderPhase = 3
		};

		// amount the bits are shifted for these values
		enum class SortKeyShift
		{
			Depth = 0,
			MaterialID = 16,
			MeshID = 28,
			ShaderPipelineID = 44,
			CMD = 54,
			TranslucencyType = 56,
			Layer = 58,
			RenderPhase = 61
		};
	private:

		std::vector<struct ADrawCommand*> m_drawCommands;
	};
}