#include "PrimitivesManager.h"
#include "Rasterizer.h"
#include "Clipper.h"
#include "Camera.h"
#include "MatrixStack.h"

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
	Matrix4 matFinal = matWorld * matView * matProj * matScreen;

	Rasterizer* rasterizer = Rasterizer::Get();

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
					for (uint32_t v = 0; v < triangle.size(); v++)
					{
						triangle[v].pos = MathHelper::TransformCoord(triangle[v].pos, matFinal);
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