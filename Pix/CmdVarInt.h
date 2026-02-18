#pragma once
#include "Command.h"

class CmdVarInt : public Command
{
public:
	const char* GetName() override
	{
		return "int";
	}
	const char* GetDescription() override
	{
		return "Declares an int variable. can optionally specify a drag speed, min, and max.\n";
		"\n"
			"syntax: int &<name>, <speed>, <min>, <max>";
		"\n"
			"e.g.\n"
			"- int $numVertices = 10\n"
			"_ int $step = 1, 1, 0, 100";

	}

	bool Execute(const std::vector<std::string>& params) override;

};