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

		// Make sure all the bits add up to a total of 64.
		static const unsigned int DEPTH_BITS = 16;
		static const unsigned int MATERIAL_BITS = 12;
		static const unsigned int MESH_BITS = 16;
		static const unsigned int SHADERPIPELINE_BITS = 10;
		static const unsigned int CMD_BITS = 2;
		static const unsigned int TRANSLUCENCYTYPE_BITS = 2;
		static const unsigned int LAYER_BITS = 3;
		static const unsigned int RENDERPHASE_BITS = 3;

		// TODO: Implement differently per platform / build target
		union AUSortKey
		{
			// Bitfield struct defines number of bits each field takes
			// in the sort key and also the order of significance.
			struct 
			{
				uint64_t depth : DEPTH_BITS;
				uint64_t materialID : MATERIAL_BITS;
				uint64_t meshID : MESH_BITS;
				uint64_t shaderPipelineID : SHADERPIPELINE_BITS;
				uint64_t cmd : CMD_BITS;
				uint64_t translucencyType : TRANSLUCENCYTYPE_BITS;
				uint64_t layer : LAYER_BITS;
				uint64_t renderPhase : RENDERPHASE_BITS;
			} bitfield;
			uint64_t value;
		};
		

	private:
		std::vector<struct ADrawCommand*> m_drawCommands;
	};
}