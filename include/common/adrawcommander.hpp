#pragma once

namespace AGN
{
	enum class EADrawCommandType;

	class ADrawCommander
	{
	public:
		ADrawCommander();
		
		void sortBuckets();
		void clearCommands();
		struct ADrawCommand& addDrawCommand(EADrawCommandType a_type); // TODO: key as argument?
		std::vector<struct ADrawCommand*> getSortedDrawCommands();

	private:
		std::vector<struct ADrawCommand*> m_drawCommands;
	};
}
