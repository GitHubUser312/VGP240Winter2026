#include "Rasterizer.h"

void DrawLineHorizontal(const Vertex& left, const Vertex& right)
{
	float dx = right.pos.x - left.pos.x;
	int startX = static_cast<int>(left.pos.x);
	int endX = static_cast<int>(right.pos.x);
	for (int x = startX; x < endX; ++x)
	{
		float t = static_cast<float>(x - startX) / dx;
		Vertex v = LerpVertex(left, right, t);
		Rasterizer::Get()->DrawPoint(v);
	}
}

void DrawLineVertical(const Vertex& bottom, const Vertex& top)
{
	float dy = top.pos.y - bottom.pos.y;
	int startY = static_cast<int>(bottom.pos.y);
	int endY = static_cast<int>(top.pos.x);
	for (int y = startY; y < endY; y++)
	{
		float t = static_cast<float>(y - startY) / dy;
		Vertex v = LerpVertex(bottom, top, t);
		Rasterizer::Get()->DrawPoint(v);
	}
}

Rasterizer* Rasterizer::Get()
{
	static Rasterizer sInstance;
	return &sInstance;
}

void Rasterizer::SetColor(X::Color color)
{
	mColor = color;
}

void Rasterizer::SetFillMode(FillMode mode)
{
	mFillMode = mode;
}

void Rasterizer::DrawPoint(int x, int y)
{
	X::DrawPixel(x, y, mColor);
}

void Rasterizer::DrawPoint(const Vertex& vertex)
{
	int x = static_cast<int>(vertex.pos.x);
	int y = static_cast<int>(vertex.pos.y);
	SetColor(vertex.color);
	X::DrawPixel(x, y, vertex.color);
}

void Rasterizer::DrawLine(const Vertex& a, const Vertex& b)
{
	float dx = b.pos.x - a.pos.x;
	float dy = b.pos.y - a.pos.y;
	// if true, line is going up/down or more vertical than horizontal
	if (MathHelper::CheckEqual(dx, 0.0f) || abs(dy/dx) >= 1.0f)
	{
		if (a.pos.y < b.pos.y)
		{
			DrawLineVertical(a, b);
		}
		else 
		{
			DrawLineVertical(b, a);
		}
	}
	// line is going more horizontal than vertical
	else
	{
		if (a.pos.x < b.pos.x)
		{
			DrawLineHorizontal(a, b);
		}
		else
		{
			DrawLineHorizontal(b, a);
		}
	}
}

void Rasterizer::DrawTriangle(const Vertex& a, const Vertex& b, const Vertex& c)
{
	switch (mFillMode)
	{
		case FillMode::Wireframe:
		{
			DrawLine(a, b);
			DrawLine(b, c);
			DrawLine(c, a);
			break;
		}
		case FillMode::Solid:
		{
			std::vector<Vertex> sortedVertices = { a, b, c };
			std::sort(sortedVertices.begin(), sortedVertices.end(),
				[](const Vertex& lhs, const Vertex& rhs)
				{
					return lhs.pos.y < rhs.pos.y;
				});
			DrawFilledTriangle(sortedVertices[0], sortedVertices[1], sortedVertices[2]);
			break;
		}
		default:
			break;
	}

}

void Rasterizer::DrawFilledTriangle(const Vertex& a, const Vertex& b, const Vertex& c)
{
	// highest delta y
	float dy = c.pos.y - a.pos.y;
	// if a and b are the same y, flat bottom
	if (MathHelper::CheckEqual(a.pos.y, b.pos.y))
	{
		int startY = static_cast<int>(a.pos.y);
		int endY = static_cast<int>(c.pos.y);
		for (int y = startY; y <= endY; ++y)
		{
			float t = static_cast<float>(y - startY) / dy;
			Vertex aSide = LerpVertex(a, c, t);
			Vertex bSide = LerpVertex(b, c, t);
			DrawLine(aSide, bSide);
		}
	}
	// if b and c are the same y, flat top
	else if (MathHelper::CheckEqual(b.pos.y, c.pos.y))
	{
		int startY = static_cast<int>(a.pos.y);
		int endY = static_cast<int>(c.pos.y);
		for (int y = startY; y <= endY; ++y)
		{
			float t = static_cast<float>(y - startY) / dy;
			Vertex aSide = LerpVertex(a, b, t);
			Vertex bSide = LerpVertex(a, c, t);
			DrawLine(aSide, bSide);
		}
	}
	// if none are equal, need to split triangle
	else
	{
		float t = (b.pos.y - a.pos.y) / dy;
		Vertex splitVertex = LerpVertex(a, c, t);
		// bottom fill
		DrawFilledTriangle(a, b, splitVertex);
		// top fill
		DrawFilledTriangle(b, splitVertex, c);
	}
}