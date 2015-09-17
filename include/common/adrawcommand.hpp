#pragma once

// TODO: optimize the data of these objects? (important for later)
\
namespace AGN
{
	enum class EADrawCommandType
	{
		DrawEntity = 1,
		ClearBuffer = 2,
		SwapBackBuffer = 4
	};

	struct ADrawEntityData
	{
		class IAMesh* mesh;
		class IAMaterial* shaderProgram;
	};

	struct AClearBufferData
	{
		uint32_t clearColor;
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
			AClearBufferData clearcolorData;
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