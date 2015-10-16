#pragma once

// TODO: optimize the data of these objects? (important for later)

namespace AGN
{
	// used to distinguish command types (isn't used in sorting)
	enum class EADrawCommandType
	{
		ClearBuffer,
		DrawEntity,
		SwapBackBuffer
	};

	// used in sort key (higher is first)
	enum class RenderPhase
	{
		PreDraw = 4,
		FullscreenViewport = 2,
		BackbufferViewport = 1,
		PostDraw = 0
	};

	// used in sort key (higher is first)
	enum class RenderLayer
	{
		Skybox = 3,
		PreEntities = 2,
		Entities = 1,
		Effects = 0,
	};

	// TODO: this is not really memory friendly, if the data is stored here directly, its faster (cache coherent), now it is simply accessing the data irregularly
	struct ADrawEntityData
	{
		class IAShaderPipeline* shaderPipeline;
		class IAMesh* mesh;
		class AMaterial* material;
		float modelMatrixArray[16];
	};

	enum class ADrawBufferType
	{
		COLOR = 1, 
		DEPTH = 2
	};

	struct AClearBufferData
	{
		uint32_t clearColor;
		uint32_t buffersToClear;
	};
	
	struct ADrawCommand
	{
		ADrawCommand(const uint64_t a_sortKey, EADrawCommandType a_type)
			: sortKey(a_sortKey)
			, type(a_type)
		{
			// ensure data is null when command is instantiated
			memset(&data, 0, sizeof(DrawData));
		}
		const uint64_t sortKey;
		const EADrawCommandType type;

		union DrawData
		{
			ADrawEntityData entityData;
			AClearBufferData clearBufferData;
		} data;
	};

	struct ASortDrawCommand
	{
		inline bool operator() (const AGN::ADrawCommand* struct1, const AGN::ADrawCommand* struct2)
		{
			return (struct1->sortKey > struct2->sortKey);
		}
	};


}

// TODO: Reference: http://blog.molecular-matters.com/2015/02/13/stateless-layered-multi-threaded-rendering-part-4-memory-management-synchronization/

/*
// Example of commands
namespace commands
{
  struct Draw
  {
	uint32_t vertexCount;
	uint32_t startVertex;
 
	VertexLayoutHandle vertexLayoutHandle;
	VertexBufferHandle vertexBuffer;
	IndexBufferHandle indexBuffer;
  };
 
  struct DrawIndexed
  {
	uint32_t indexCount;
	uint32_t startIndex;
	uint32_t baseVertex;
 
	VertexLayoutHandle vertexLayoutHandle;
	VertexBufferHandle vertexBuffer;
	IndexBufferHandle indexBuffer;
  };
 
  struct CopyConstantBufferData
  {
	ConstantBufferHandle constantBuffer;
	void* data;
	uint32_t size;
  };
*/