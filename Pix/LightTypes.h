#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
	public:
		X::Color ComputeLightColor(const Vector3& position, const Vector3& normal) override;
		void SetDirection(const Vector3& direction);

	private:
		Vector3 mDirection;
};

class PointLight : public Light
{
	public:
		X::Color ComputeLightColor(const Vector3& position, const Vector3& normal) override;
		void SetPosition(const Vector3& position);
		void SetAttenuation(float kConstant, float kLinear, float kQuadratic);
		void SetAngle(float angle);
		void SetDecay(float decay);

	private:
		Vector3 mPosition;
		float mAttenConst = 1.0f;
		float mAttenLinear = 0.0f;
		float mAttenQuad = 0.0f;
		float mCosAngle = cos(X::Math::kPiByTwo);
		float mDecay = 10.0f;
};

class SpotLight : public Light
{
	public:
		X::Color ComputeLightColor(const Vector3& position, const Vector3& normal) override;
		void SetPosition(const Vector3& position);
		void SetDirection(const Vector3& direction);
		void SetAttenuation(float kConstant, float kLinear, float kQuadratic);
		void SetAngle(float angle);
		void SetDecay(float decay);

	private:
		Vector3 mPosition;
		float mAttenConst = 1.0f;
		float mAttenLinear = 0.0f;
		float mAttenQuad = 0.0f;
		float mCosAngle = cos(X::Math::kPiByTwo);
		float mDecay = 10.0f;
};