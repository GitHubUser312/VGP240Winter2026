#include "PrimitivesManager.h"
#include "Rasterizer.h"
#include "Clipper.h"
#include "Camera.h"
#include "MatrixStack.h"
#include "LightManager.h"

extern float gResolutionX;
extern float gResolutionY;

namespace
{
	Matrix4 GetScreenTransform()
	{
		const float hw = gResolutionX * 0.5f;
		const float hh = gResolutionY * 0.5f;
		return
		{
			hw, 0.0f, 0.0f, 0.0f,
			0.0f, -hh, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			hw, hh, 0.0f, 1.0f
		};
	}

	Vector3 CreateFaceNormal(const std::vector<Vertex>& triangles)
	{
		// to creat a face normal
		// we take the clockwise direction and do a cross product
		// so 0-1(a-b), 0-2(a-c) for the directions
		// cross product
		// return normal
		Vector3 abDir = triangles[1].pos - triangles[0].pos;
		Vector3 acDir = triangles[2].pos - triangles[0].pos;
		Vector3 faceNormal = MathHelper::Normalize(MathHelper::Cross(abDir, acDir));
		return faceNormal;
	}

	bool CullTriangle(CullMode mode, const std::vector<Vertex>& triangleInNDC)
	{
		if (mode == CullMode::None)
		{
			return false;
		}
		Vector3 faceNormal = CreateFaceNormal(triangleInNDC);
		if (mode == CullMode::Back)
		{
			return faceNormal.z > 0.0f;
		}
		if (mode == CullMode::Front)
		{
			return faceNormal.z < 0.0f;
		}

		return false;
	}
}

PrimitivesManager* PrimitivesManager::Get()
{
	static PrimitivesManager instance;
	return &instance;
}

PrimitivesManager::PrimitivesManager()
{
	// chagne
}

void PrimitivesManager::OnNewFrame()
{
	mCullMode = CullMode::Back;
	mCorrectUV = false;
}
void PrimitivesManager::SetCullMode(CullMode mode)
{
	mCullMode = mode;
}
void PrimitivesManager::SetCorrectUV(bool mode)
{
	mCorrectUV = mode;
}

bool PrimitivesManager::BeginDraw(Topology topology, bool applyTransform)
{
	Matrix4 testMatrix = Matrix4::RotationY(0.3f);
	Matrix4 invTest = MathHelper::Inverse(testMatrix);
	Matrix4 result = testMatrix * invTest;
	// change
	mVertexBuffer.clear();
	mTopology = topology;
	mApplyTransform = applyTransform;
	mDrawBegin = true;
	return true;
}

void PrimitivesManager::AddVertex(const Vertex& vertex)
{
	if (mDrawBegin)
	{
		mVertexBuffer.push_back(vertex);
	}
}
bool PrimitivesManager::EndDraw()
{
	if (!mDrawBegin)
	{
		return false;
	}

	// this matrix transforms the local vertices to world vertices 
	Matrix4 matWorld = MatrixStack::Get()->GetTransform();
	// transforms world vertices to camera vertices
	Matrix4 matView = Camera::Get()->GetViewMatrix();
	// transforms camera vertices to projection vertices
	Matrix4 matProj = Camera::Get()->GetProjectionMatrix();
	// matrix transforms the NDC space vertices to screen space vertices
	Matrix4 matScreen = GetScreenTransform();
	// get the calculation to NDC space
	Matrix4 matNDC = matView * matProj;

	Rasterizer* rasterizer = Rasterizer::Get();
	LightManager* lm = LightManager::Get();

	ShadeMode shadeMode = rasterizer->GetShadeMode();
	switch (mTopology)
	{
		case Topology::Point:
		{
			for (uint32_t i = 0; i < mVertexBuffer.size(); ++i)
			{
				if (!Clipper::Get()->ClipPoint(mVertexBuffer[i]))
				{
					rasterizer->DrawPoint(mVertexBuffer[i]);
				}
			}
		}
			break;
		case Topology::Line:
		{
			for (uint32_t i = 0; i + 1 < mVertexBuffer.size(); i += 2)
			{
				if (!Clipper::Get()->ClipLine(mVertexBuffer[i], mVertexBuffer[i + 1]))
				{
					rasterizer->DrawLine(mVertexBuffer[i],
					mVertexBuffer[i + 1]);
				}
			}
		}
		break;
		case Topology::Triangles:
		{
			for (uint32_t i = 2; i < mVertexBuffer.size(); i += 3)
			{
				std::vector<Vertex> triangle = {mVertexBuffer[i - 2], mVertexBuffer[i - 1], mVertexBuffer[i]};
				if (mApplyTransform)
				{
					// calculate the normal in local space
					if (MathHelper::CheckEqual(MathHelper::MagnitudeSquared(triangle[0].norms), 0.0f))
					{
						Vector3 faceNormal = CreateFaceNormal(triangle);
						for (uint32_t v = 0; v < triangle.size(); v++)
						{
							triangle[v].norms = faceNormal;
						}
					}

					// convert triangle position to world space
					for (uint32_t v = 0; v < triangle.size(); v++)
					{
						// Local → World
						triangle[v].pos = MathHelper::TransformCoord(triangle[v].pos, matWorld);
						triangle[v].posWorld = triangle[v].pos;

						// Transform normal to world
						triangle[v].norms = MathHelper::Normalize(MathHelper::TransformNormal(triangle[v].norms, matWorld));
					}

					// if color.z >= 0, then it is a colored shape, otherwise its a texture
					if (triangle[0].color.z >= 0.0f)
					{
						// Flat shading is vertex based
						if (shadeMode == ShadeMode::Flat)
						{
							triangle[0].color *= lm->ComputeLightColor(triangle[0].pos, triangle[0].norms);
							triangle[1].color = triangle[0].color;
							triangle[2].color = triangle[0].color;
						}

						else if (shadeMode == ShadeMode::Gouraud)
						{
					
							// apply lighting in world space (Gouraud Shading)
							for (uint32_t v = 0; v < triangle.size(); v++)
							{
								triangle[v].color *= lm->ComputeLightColor(triangle[v].pos, triangle[v].norms);
							}

						}
					}
					else if (mCorrectUV)
					{
						// apply the corrective uv in view space
						// at this point, we are in world space, so next step is
						// multiply by matView
						for (uint32_t v = 0; v < triangle.size(); ++v)
						{
							Vector3 viewSpacePos = MathHelper::TransformCoord(triangle[v].posWorld, matView);
							triangle[v].color.x /= viewSpacePos.z;
							triangle[v].color.y /= viewSpacePos.z;
							triangle[v].color.w = 1.0f / viewSpacePos.z;
						}
					}

					// otherwise, convert the localspace normal to world space


					// convert triangle positions to NDC space
					for (uint32_t v = 0; v  < triangle.size(); v ++)
					{
						triangle[v].pos = MathHelper::TransformCoord(triangle[v].pos, matNDC);
					}

					// while in NDC space, we can see if the face is facing the camera or away
					if (CullTriangle(mCullMode, triangle))
					{
						continue;
					}
					
					// convert ndc space triangles to Screen space
					for (uint32_t v = 0; v < triangle.size(); v++)
					{
						triangle[v].pos = MathHelper::TransformCoord(triangle[v].pos, matScreen);
						// Flatten only on screen space and only pixel x, y values
						MathHelper::FlattenVectorScreenCoord(triangle[v].pos);
					}
				}
				if (!Clipper::Get()->ClipPolygon(triangle))
				{
					for (uint32_t v = 2; v < triangle.size(); ++v)
					{
						rasterizer->DrawTriangle(triangle[0], triangle[v - 1], triangle[v]);
					}
				}
			}
		}
		break;
	default:
		return false;
	}

	mDrawBegin = false;
	return true;
}