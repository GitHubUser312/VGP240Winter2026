#include "CmdLights.h"
#include "LightManager.h"
#include "VariableCache.h"

bool CmdSetLightDiffuse::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	const float r = vc->GetFloat(params[0]);
	const float g = vc->GetFloat(params[1]);
	const float b = vc->GetFloat(params[2]);
	LightManager::Get()->SetLightAmbient({ r, g, b, 1.0f });
	return true;
}


bool CmdSetLightAmbient::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	const float r = vc->GetFloat(params[0]);
	const float g = vc->GetFloat(params[1]);
	const float b = vc->GetFloat(params[2]);
	LightManager::Get()->SetLightAmbient({r, g, b, 1.0f});
	return true;
}

bool CmdSetLightSpecular::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	const float r = vc->GetFloat(params[0]);
	const float g = vc->GetFloat(params[1]);
	const float b = vc->GetFloat(params[2]);
	LightManager::Get()->SetLightAmbient({ r, g, b, 1.0f });
	return true;
}

bool CmdAddDirectionalLight::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	const float x = vc->GetFloat(params[0]);
	const float y = vc->GetFloat(params[1]);
	const float z = vc->GetFloat(params[2]);
	LightManager::Get()->AddDirectionalLight({x, y, z});
	return true;
}

bool CmdAddPointLight::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		return false;
	}
	
	VariableCache* vc = VariableCache::Get();
	const float x = vc->GetFloat(params[0]);
	const float y = vc->GetFloat(params[1]);
	const float z = vc->GetFloat(params[2]);
	float kConstant = 1.0f;
	float kLinear = 0.0f;
	float kQuad = 0.0f;
	float angle = 0.0f;
	float decay = 0.0f;
	if (params.size() > 3)
	{
		kConstant = vc->GetFloat(params[3]);
	}
	if (params.size() > 4)
	{
		kLinear = vc->GetFloat(params[4]);
	}
	if (params.size() > 5)
	{
		kQuad = vc->GetFloat(params[5]);
	}

	LightManager::Get()->AddPointLight({ x, y, z }, kConstant, kLinear, kQuad);
	return true;
}

bool CmdAddSpotLight::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 6)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	const float pX = vc->GetFloat(params[0]);
	const float pY = vc->GetFloat(params[1]);
	const float pZ = vc->GetFloat(params[2]);

	const float dX = vc->GetFloat(params[3]);
	const float dY = vc->GetFloat(params[4]);
	const float dZ = vc->GetFloat(params[5]);

	float kConstant = 1.0f;
	float kLinear = 0.0f;
	float kQuad = 0.0f;
	float angle = 0.0f;
	float decay = 0.0f;
	if (params.size() > 6)
	{
		kConstant = vc->GetFloat(params[6]);
	}
	if (params.size() > 7)
	{
		kLinear = vc->GetFloat(params[7]);
	}
	if (params.size() > 9)
	{
		angle = vc->GetFloat(params[9]) * X::Math::kDegToRad;
	}
	if (params.size() > 10)
	{
		decay = vc->GetFloat(params[10]);
	}
	LightManager::Get()->AddSpotLight({ pX, pY, pZ }, { dX, dY, dZ }, kConstant, kLinear, kQuad, angle, decay);
	return true;
}