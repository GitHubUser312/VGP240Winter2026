#pragma once
#include "Command.h"

class CmdVarBool : public Command
{
	public:
		const char* GetName() override
		{
			return "bool";
		}

		const char* GetDescription() override
		{
			return
				"Delcares a bool variable.\n"
				"\n"
				"syntax: bool $<name> = <value>\n"
				"\n"
				"e.g. \n"
				"- bool enableDepth = true";
		}

		bool Execute(const std::vector<std::string>& params) override;
};
