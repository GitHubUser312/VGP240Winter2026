#include "CmdCamera.h"
#include "Camera.h"
#include "VariableCache.h"

// SetCameraPosition
bool CmdSetCameraPosition::Execute(const std::vector<std::string>& params)
{
    if (params.size() != 3)
        return false;

    VariableCache* vars = VariableCache::Get();

    float x = vars->GetFloat(params[0]);
    float y = vars->GetFloat(params[1]);
    float z = vars->GetFloat(params[2]);

    Camera::Get()->SetPosition(Vector3(x, y, z));

    return true;
}


// SetCameraDirection
bool CmdSetCameraDirection::Execute(const std::vector<std::string>& params)
{
    if (params.size() != 3)
        return false;

    VariableCache* vars = VariableCache::Get();

    float x = vars->GetFloat(params[0]);
    float y = vars->GetFloat(params[1]);
    float z = vars->GetFloat(params[2]);

    Camera::Get()->SetDirection(Vector3(x, y, z));

    return true;
}


// Near Plane
bool CmdSetCameraNearPlane::Execute(const std::vector<std::string>& params)
{
    if (params.size() != 1)
        return false;

    float nearPlane =
        VariableCache::Get()->GetFloat(params[0]);

    Camera::Get()->SetNearPlane(nearPlane);

    return true;
}


// Far Plane
bool CmdSetCameraFarPlane::Execute(const std::vector<std::string>& params)
{
    if (params.size() != 1)
        return false;

    float farPlane =
        VariableCache::Get()->GetFloat(params[0]);

    Camera::Get()->SetFarPlane(farPlane);

    return true;
}



// FOV
bool CmdSetCameraFOV::Execute(const std::vector<std::string>& params)
{
    if (params.size() != 1)
        return false;

    float fov =
        VariableCache::Get()->GetFloat(params[0]);

    Camera::Get()->SetFOV(fov);

    return true;
}
