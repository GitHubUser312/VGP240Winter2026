#pragma once
#include "Command.h"

class CmdSetAddressMode : public Command
{
public:
	const char* GetName() override
	{
		return "SetAddressMode";
	}
	const char* GetDescription() override
	{
		return
			"SetAddressMode(mode)"
			"\n"
			"- sets the address mode for texture objects\n"
			"- <border>, <clamp>, <wrap>, <mirror>";
	}
	bool Execute(const std::vector<std::string>& params) override;
};