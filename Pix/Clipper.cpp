#include "Clipper.h"
#include "Viewport.h"
#include <cmath>
#include <vector>

const short BIT_INSIDE = 0;      // 0000
const short BIT_LEFT = 1 << 0; // 0001
const short BIT_RIGHT = 1 << 1; // 0010
const short BIT_BOTTOM = 1 << 2; // 0100
const short BIT_TOP = 1 << 3; // 1000


short GetOutputCode(float x, float y)
{
    Viewport* vp = Viewport::Get();

    short code = BIT_INSIDE;

    if (x < vp->GetMinX()) code |= BIT_LEFT;
    if (x > vp->GetMaxX()) code |= BIT_RIGHT;
    if (y < vp->GetMinY()) code |= BIT_BOTTOM;
    if (y > vp->GetMaxY()) code |= BIT_TOP;

    return code;
}

enum ClipEdge
{
    CE_LEFT,
    CE_TOP,
    CE_RIGHT,
    CE_BOTTOM,
    CE_COUNT
};


bool isInFront(ClipEdge edge, const Vector3& pos)
{
    Viewport* vp = Viewport::Get();

    switch (edge)
    {
    case CE_LEFT:
        return pos.x >= vp->GetMinX();

    case CE_TOP:
        return pos.y <= vp->GetMaxY();

    case CE_RIGHT:
        return pos.x <= vp->GetMaxX();

    case CE_BOTTOM:
        return pos.y >= vp->GetMinY();

    default:
        return false;
    }
}


Vertex ComputeIntersection(ClipEdge edge,
    const Vertex& v,
    const Vertex& vPO)
{
    Viewport* vp = Viewport::Get();

    float t = 0.0f;

    switch (edge)
    {
    case CE_LEFT:
        t = (vp->GetMinX() - v.pos.x) / (vPO.pos.x - v.pos.x);
        break;

    case CE_TOP:
        t = (vp->GetMaxY() - v.pos.y) / (vPO.pos.y - v.pos.y);
        break;

    case CE_RIGHT:
        t = (vp->GetMaxX() - v.pos.x) / (vPO.pos.x - v.pos.x);
        break;

    case CE_BOTTOM:
        t = (vp->GetMinY() - v.pos.y) / (vPO.pos.y - v.pos.y);
        break;
    }

    return LerpVertex(v, vPO, t);
}

Clipper::Clipper()
{
}


Clipper* Clipper::Get()
{
    static Clipper sInstance;
    return &sInstance;
}


void Clipper::OnNewFrame()
{

}


bool Clipper::ClipPoint(const Vertex& v)
{
    if (!mIsClipping)
        return false;

    Viewport* vp = Viewport::Get();

    return
        v.pos.x < vp->GetMinX() ||
        v.pos.x > vp->GetMaxX() ||
        v.pos.y < vp->GetMinY() ||
        v.pos.y > vp->GetMaxY();
}

bool Clipper::ClipLine(Vertex& a, Vertex& b)
{
    if (!mIsClipping)
        return false;

    Viewport* vp = Viewport::Get();

    float minX = vp->GetMinX();
    float minY = vp->GetMinY();
    float maxX = vp->GetMaxX();
    float maxY = vp->GetMaxY();

    short codeA = GetOutputCode(a.pos.x, a.pos.y);
    short codeB = GetOutputCode(b.pos.x, b.pos.y);

    while (true)
    {
        if (!(codeA | codeB))
        {
            return false;
        }

        if (codeA & codeB)
        {
            return true;
        }

        float t = 0.0f;

        short outCode = codeA ? codeA : codeB;

        if (outCode & BIT_TOP)
        {
            float dy = b.pos.y - a.pos.y;
            if (fabs(dy) < 1e-6f) return true;

            t = (maxY - a.pos.y) / dy;
        }
        else if (outCode & BIT_BOTTOM)
        {
            float dy = b.pos.y - a.pos.y;
            if (fabs(dy) < 1e-6f) return true;

            t = (minY - a.pos.y) / dy;
        }
        else if (outCode & BIT_LEFT)
        {
            float dx = b.pos.x - a.pos.x;
            if (fabs(dx) < 1e-6f) return true;

            t = (minX - a.pos.x) / dx;
        }
        else if (outCode & BIT_RIGHT)
        {
            float dx = b.pos.x - a.pos.x;
            if (fabs(dx) < 1e-6f) return true;

            t = (maxX - a.pos.x) / dx;
        }

        if (outCode == codeA)
        {
            a = LerpVertex(a, b, t);
            codeA = GetOutputCode(a.pos.x, a.pos.y);
        }
        else
        {
            b = LerpVertex(a, b, t);
            codeB = GetOutputCode(b.pos.x, b.pos.y);
        }
    }
}

bool Clipper::ClipPolygon(std::vector<Vertex>& vertices)
{
    if (!mIsClipping)
        return false;

    std::vector<Vertex> newVertices;

    for (int i = 0; i < CE_COUNT; i++)
    {
        if (vertices.empty())
            break;

        newVertices.clear();

        ClipEdge edge = (ClipEdge)i;

        for (size_t n = 0; n < vertices.size(); n++)
        {
            size_t nPO = (n + 1) % vertices.size();

            const Vertex& v = vertices[n];
            const Vertex& vPO = vertices[nPO];

            bool in1 = isInFront(edge, v.pos);
            bool in2 = isInFront(edge, vPO.pos);

            if (in1 && in2)
            {
                newVertices.push_back(vPO);
            }
            else if (in1 && !in2)
            {
                newVertices.push_back(
                    ComputeIntersection(edge, v, vPO)
                );
            }
            else if (!in1 && in2)
            {
                newVertices.push_back(
                    ComputeIntersection(edge, v, vPO)
                );

                newVertices.push_back(vPO);
            }
        }

        vertices = newVertices;
    }

    return vertices.empty();
}


bool Clipper::IsClipping() const
{
    return mIsClipping;
}


void Clipper::SetClipping(bool enable)
{
    mIsClipping = enable;
}