#pragma once 

#include "iarenderer.hpp"

namespace AGN
{
	class ARendererDX11 : public IARenderer 
	{
	public:
		ARendererDX11();
		void init() override;
		void render(class ADrawCommander& a_drawCommander) override;
		void setCamera(class ACamera* a_camera) override { m_currentCamera = a_camera; }

	private:
		void drawEntity(struct ADrawCommand* a_command);

		class ACamera* m_currentCamera;
		glm::mat4 m_vp;

		class AMeshDX11* m_boundMesh;
		class AMaterial* m_boundMaterial;
		class AShaderPipelineDX11* m_boundShaderPipeline;
	};
}