#include "LightTypes.h"
#include "MaterialManager.h"
#include "Camera.h"

X::Color DirectionalLight::ComputeLightColor(const Vector3& position, const Vector3& normal)
{
	Camera* camera = Camera::Get();
	MaterialManager* mm = MaterialManager::Get();

	// calculate the ambient color
	X::Color ambient = mAmbient * mm->GetMaterialAmbient();

	// calculate the diffuse color
	// directional light iL is 1.0f so we can ignore
	Vector3 L = -mDirection;
	float dot = X::Math::Max(MathHelper::Dot(L, normal), 0.0f);
	X::Color diffuse = mDiffuse * mm->GetMaterialDiffuse() * dot;

	// calculate the specular
	// directional light iL is 1.0f so we can ignore
	Vector3 v = MathHelper::Normalize(camera->GetPosition() - position);
	Vector3 r = MathHelper::Normalize(L + v);
	float fallOff = X::Math::Max((float)pow(MathHelper::Dot(r, normal), mm->GetMaterialShininess()), 0.0f);
	X::Color specular = mSpecular * mm->GetMaterialSpecular() * fallOff;

	return ambient + diffuse + specular;
}

void DirectionalLight::SetDirection(const Vector3& direction)
{
	mDirection = MathHelper::Normalize(direction);
}

X::Color PointLight::ComputeLightColor(const Vector3& position, const Vector3& normal)
{
	Camera* camera = Camera::Get();
	MaterialManager* mm = MaterialManager::Get();

	// Direction to the light
	Vector3 L = mPosition - position;
	float distance = MathHelper::Magnitude(L);
	L /= distance;

	// calculate the iL
	float attenuation = 1.0f / (mAttenConst + (mAttenLinear * distance) + (mAttenQuad * distance * distance));
	float iL = X::Math::Clamp(attenuation, 0.0f, 1.0f);

	// calculate the ambient color
	X::Color ambient = mAmbient * mm->GetMaterialAmbient();

	// calculate the diffuse color
	// directional light iL is 1.0f so we can ignore
	float dot = X::Math::Max(MathHelper::Dot(L, normal), 0.3f);
	X::Color diffuse = mDiffuse * mm->GetMaterialDiffuse() * dot * iL;

	// calculate the specular
	// directional light iL is 1.0f so we can ignore
	Vector3 v = MathHelper::Normalize(camera->GetPosition() - position);
	Vector3 r = MathHelper::Normalize(L + v);
	float fallOff = X::Math::Max((float)pow(MathHelper::Dot(r, normal), mm->GetMaterialShininess()), 0.0f);
	X::Color specular = mSpecular * mm->GetMaterialSpecular() * fallOff * iL;

	return ambient + diffuse + specular;
}

X::Color SpotLight::ComputeLightColor(const Vector3& position, const Vector3& normal)
{
    Camera* camera = Camera::Get();
    MaterialManager* mm = MaterialManager::Get();

    Vector3 L = mPosition - position;
    float distance = MathHelper::Magnitude(L);
    L /= distance;


    float attenuation =   1.0f / (mAttenConst + (mAttenLinear * distance) + (mAttenQuad * distance * distance));

    float iL = X::Math::Clamp(attenuation, 0.0f, 1.0f);

    Vector3 lightDir = MathHelper::Normalize(-mPosition);
    float spotDot = MathHelper::Dot(L, lightDir);

    if (spotDot > mCosAngle)
    {
        iL *= pow(spotDot, mDecay);
    }
    else
    {
        iL = 0.0f;
    }

    X::Color ambient = mAmbient * mm->GetMaterialAmbient();

    float dot = X::Math::Max(MathHelper::Dot(L, normal), 0.0f);

    X::Color diffuse = mDiffuse *
        mm->GetMaterialDiffuse() *
        dot *
        iL;


    // Specular
    Vector3 v =
        MathHelper::Normalize(camera->GetPosition() - position);

    Vector3 r =
        MathHelper::Normalize(L + v);

    float fallOff =
        X::Math::Max(
            (float)pow(
                MathHelper::Dot(r, normal),
                mm->GetMaterialShininess()),
            0.0f);

    X::Color specular =
        mSpecular *
        mm->GetMaterialSpecular() *
        fallOff *
        iL;


    return ambient + diffuse + specular;

}

void PointLight::SetPosition(const Vector3& position)
{
	mPosition = position;
}

void PointLight::SetAttenuation(float kConstant, float kLinear, float kQuadratic)
{
	mAttenConst = kConstant;
	mAttenLinear = kLinear;
	mAttenQuad = kQuadratic;
}

void PointLight::SetAngle(float angle)
{
	mCosAngle = cos(angle);
}

void PointLight::SetDecay(float decay)
{
	mDecay = decay;
}