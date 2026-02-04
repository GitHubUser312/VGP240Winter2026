#pragma once
#include "Command.h"

class CmdEndDraw : public Command
{
	public:
		const char* GetName() override
		{
			return "EndDraw";
		}
		const char* GetDescription() override
		{
			return  "EndDraw()\n"
				"\n"
				"- ends the drawing of primitives and sends them to the rasterizer";
		}
		

		bool Execute(const std::vector<std::string>& params) override;
};