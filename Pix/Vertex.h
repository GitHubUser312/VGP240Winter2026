#pragma once
#include "MathHelper.h"
#include <XColors.h>

struct Vertex
{
	Vector3 pos;	  // position along the graphics pipeline, local to screen space
	Vector3 posWorld; // position in world space
	Vector3 norms;	  // normal in world space
	X::Color color;   // color of the vertex / OR / is the UV coordinates of a texture
};

inline Vector3 LerpPosition(const Vector3& a, const Vector3& b, float t)
{
	return {
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t,
		a.z + (b.z - a.z) * t
	};
}

inline Vector3 LerpNormal(const Vector3& a, const Vector3& b, float t)
{
	return MathHelper::Normalize ({
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t,
		a.z + (b.z - a.z) * t
	});
}

inline X::Color LerpColor(const X::Color& a, const X::Color& b, float t)
{
	return {
		a.x + (b.r - a.r) * t,
		a.y + (b.g - a.g) * t,
		a.z + (b.b - a.b) * t,
		a.w + (b.a - a.a) * t
	};
}

inline Vertex LerpVertex(const Vertex& a, const Vertex& b, float t)
{
	Vertex v;
	v.pos = LerpPosition(a.pos, b.pos, t);
	v.color = LerpColor(a.color, b.color, t);
	v.posWorld = LerpPosition(a.posWorld, b.posWorld, t);
	v.norms = LerpNormal(a.norms, b.norms, t);

	v.pos.x = floor(v.pos.x + 0.5f);
	v.pos.y = floor(v.pos.y + 0.5f); 
	return v;
}