#pragma once
#include "Command.h"

class CmdVector : public Command
{
	public:
		const char* GetName() override
		{
			return "vector";
		}
		const char* GetDescription() override
		{
			return "Creates a vector with the given components. Usage: vector x y z";
		}
}