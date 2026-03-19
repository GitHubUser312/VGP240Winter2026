#pragma once
#include "Command.h"

class CmdSetUseFilter : public Command
{
public:
	const char* GetName() override
	{
		return "SetUseFilter";
	}
	
	const char* GetDescription() override
	{
		return
			"SetUseFilter(use)"
			"\n"
			"- enables/disables bilinear filter on textures";
	}
	bool Execute(const std::vector <std::string>& params) override;
};