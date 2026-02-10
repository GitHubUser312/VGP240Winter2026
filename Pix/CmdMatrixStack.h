#pragma once
#include "Command.h"
#include "Vector3.h"

class CmdPushTranslation : public Command
{
	public:
		const char* GetName() override
		{ 
			return "PushTranslation"; 
		}

		const char* GetDescription() override 
		{ 
			return "Pushes a translation matrix onto the stack. Usage: PushTranslation(x, y, z)"; 
		}

		bool Execute(const std::vector<std::string>& params) override;
};

class CmdPushRotationX : public Command
{
	public:

		const char* GetName() override
		{
			return "PushRotationX";
		}

		const char* GetDescription() override
		{
			return "Pushes a rotation matrix around the X axis onto the stack. Usage: PushRotationX(degrees)\n";
		}

		bool Execute(const std::vector<std::string>& params) override;

	private:
		float mRadians;
};

class CmdPushRotationY : public Command
{
	public:
		const char* GetName() override
		{
			return "PushRotationY";
		}
		const char* GetDescription() override
		{
			return "Pushes a rotation matrix around the Y axis onto the stack. Usage: PushRotationY(degrees)\n";
		}

		bool Execute(const std::vector<std::string>& params) override;
	private:
		float mRadians;
};

class CmdPushRotationZ : public Command
{
	public:
		const char* GetName() override
		{
			return "PushRotationZ";
		}
		const char* GetDescription() override
		{
			return "Pushes a rotation matrix around the Z axis onto the stack. Usage: PushRotationZ(degrees)\n";
		}

		bool Execute(const std::vector<std::string>& params) override;
	private:
		float mRadians;
};

class CmdPushScaling : public Command
{
	public:

		const char* GetName() override
		{
			return "PushScaling";
		}
		const char* GetDescription() override
		{
			return "Pushes a scaling matrix onto the stack. Usage: PushScaling(x, y, z)\n";
		}

		bool Execute(const std::vector<std::string>& params) override;
};

class CmdPopMatrix : public Command
{
	public:
		const char* GetName() override
		{
			return "PopMatrix";
		}
		const char* GetDescription() override
		{
			return "Pops the last matrix from the stack. Usage: PopMatrix\n";
		}
		bool Execute(const std::vector<std::string>& params) override;
};