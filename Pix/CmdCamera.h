#pragma once
#include "Command.h"

class CmdSetCameraPosition : public Command
{
	public:
		const char* GetName() override
		{
			return "SetCameraPosition";
		}
		const char* GetDescription() override
		{
			return "Sets the camera position. Usage: SetCameraPosition(x, y, z)\n";
		}
		bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraDirection : public Command
{
	public:
		const char* GetName() override
		{
			return "SetCameraDirection";
		}
		const char* GetDescription() override
		{
			return "Sets the camera direction. Usage: SetCameraDirection(x, y, z)\n";
		}
		bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraNearPlane : public Command
{
	public:
		const char* GetName() override
		{
			return "SetCameraNearPlane";
		}
		const char* GetDescription() override
		{
			return "Sets the camera near plane. Usage: SetCameraNearPlane(nearPlane)\n";
		}
		bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraFarPlane : public Command
{
	public:
		const char* GetName() override
		{
			return "SetCameraFarPlane";
		}
		const char* GetDescription() override
		{
			return "Sets the camera far plane. Usage: SetCameraFarPlane(farPlane)\n";
		}
		bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraFOV : public Command
{
	public:
		const char* GetName() override
		{
			return "SetCameraFOV";
		}
		const char* GetDescription() override
		{
			return "Sets the camera field of view. Usage: SetCameraFOV(fov)\n";
		}
		bool Execute(const std::vector<std::string>& params) override;
};
