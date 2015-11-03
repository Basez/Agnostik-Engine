#pragma once 

namespace AGN
{
	class IRenderer
	{
	public:
		virtual bool init() = 0;
		virtual void render(class DrawCommander& a_drawCommander) = 0;
		virtual void setCamera(class Camera* a_camera) = 0;
		virtual void onWindowUpdated(glm::ivec2 a_dimentions) {}

		virtual ~IRenderer() {};
	};
}