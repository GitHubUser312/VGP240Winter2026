#pragma once
#include "Command.h"

class CmdSetCorrectUV : public Command
{
	public:
		const char* GetName() override
		{
			return "SetCorrectUV";
		}
		const char* GetDescription() override
		{
			return 
				"SetCorrectUV(correctUV)"
				"\n"
				"- enables/disables correct uv for texture objects";
		}
		bool Execute(const std::vector<std::string>& params) override;
};