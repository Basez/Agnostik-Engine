#pragma once

namespace AGN
{
	class ADrawCommander
	{
	public:
		ADrawCommander();
		
		void sortBuckets();
		void clearBuckets();
		struct ADrawCommandMesh& addMeshDrawCommand(); // TODO: key as argument?
		std::vector<struct ADrawCommandMesh*> getSortedMeshDrawCommands();

	private:
		std::vector<struct ADrawCommandMesh*> m_meshDrawCommands;
	};
}
