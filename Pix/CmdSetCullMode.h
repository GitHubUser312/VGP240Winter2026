#pragma once
#include "Command.h"

class CmdSetCullMode : public Command
{
public:
	const char* GetName() override
	{
		return "SetCUllMode";
	}
	const char* GetDescription() override
	{
		return
			"SetCullMode(mode)"
			"\n"
			"- sets the cullmode, none, back, front";
	}
	bool Execute(const std::vector<std::string>& params) override;
};