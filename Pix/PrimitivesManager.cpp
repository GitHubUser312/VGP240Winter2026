#include "PrimitivesManager.h"
#include "Rasterizer.h"
#include "Clipper.h"

PrimitivesManager* PrimitivesManager::Get()
{
	static PrimitivesManager instance;
	return &instance;
}

PrimitivesManager::PrimitivesManager()
{

}

bool PrimitivesManager::BeginDraw(Topology topology)
{
	Matrix4 testMatrix = Matrix4::RotationY(0.3f);
	Matrix4 invTest = MathHelper::Inverse(testMatrix);
	Matrix4 result = testMatrix * invTest;
	// change
	mVertexBuffer.clear();
	mTopology = topology;
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