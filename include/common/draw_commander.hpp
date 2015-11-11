#pragma once

namespace AGN
{
	enum class EDrawCommandType;

	class DrawCommander
	{
	public:
		DrawCommander();
		~DrawCommander();

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
		struct DrawCommand& addDrawCommand(EDrawCommandType a_type, uint64_t a_sortKey);
		std::vector<struct DrawCommand*> getSortedDrawCommands();

		// num of bits each sort key property is occupying
		enum class ESortKeyBitAmount
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
		enum class ESortKeyShift
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
		std::vector<struct DrawCommand*> m_drawCommands;
	};
}