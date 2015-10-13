#pragma once

#include "iamesh.hpp"
#include "amaterial.hpp" // TODO: remove

namespace AGN
{
	class AMeshDX11 : public IAMesh
	{
	public:
		AMeshDX11() { };
		
		std::string getRelativePath() override { return ""; }		// TODO: Restore
		uint16_t getAId() override { return 0; }		// TODO: Restore
		class AMaterial* getMaterial() override { return nullptr; } // TODO: Restore
		glm::vec3 getCenterPoint() override { return glm::vec3(); } // TODO: Restore

		void setMaterial(class AMaterial* a_material) override {  } // TODO: Restore?

	
	private:
		// TODO:

		AMeshData* m_meshData;
	};
}