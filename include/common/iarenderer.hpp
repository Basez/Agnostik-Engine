#pragma once 

namespace AGN
{
	class IARenderer
	{
	public:
		virtual bool init() = 0;
		virtual void render(class ADrawCommander& a_drawCommander) = 0;
		virtual void setCamera(class ACamera* a_camera) = 0;
		virtual void onWindowUpdated(glm::ivec2 a_dimentions) {}

		virtual ~IARenderer() {};
	};
}