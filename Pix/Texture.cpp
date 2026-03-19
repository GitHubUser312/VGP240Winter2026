#include "Texture.h"

namespace
{
    #pragma pack(push, 1)
        struct BitmapFileHeader
        {
            uint16_t type;          // magic identifier
            uint32_t size;          // file size in bytes
            uint16_t reserved1;
            uint16_t reserved2;
            uint32_t offset;        // byte offset to image data
        };

        struct BitmapInfoHeader
        {
            uint32_t size;                  // header size in bytes
            int width, height;              // width and height of the image
            uint16_t planes;                // number of color planes
            uint16_t bits;                  // bits per pixel
            uint32_t compression;           // compression type
            uint32_t imageSize;             // image size in bytes
            int xResolution, yResolution;   // pixels per meter
            uint32_t numColors;             // number of colors
            uint32_t importantColors;       // important colors
        };

        uint32_t MakeStringAligned(uint32_t rowStride, uint32_t alignStride)
        {
            uint32_t newStride = rowStride;
            while (newStride % alignStride != 0)
            {
                newStride++;
            }

            return newStride;
        }
    #pragma pack(pop)
        X::Color GetBilinearFilterPixelColor(const Texture& tex, float u, float v)
        {
            // step 1, convert u, v coordinates to texel coordinates
            float uTex = u * static_cast<float>(tex.GetWidth());
            float vTex = v * static_cast<float>(tex.GetHeight());

            // step 2, convert the floats to ints to get the pixel indices
            int uTextInt = static_cast<int>(uTex);
            int vTextInt = static_cast<int>(vTex);

            // example: if uTex = 128.7654, uTextInt = 128, this step gives 0.7654
            // step 3, get the float remainder as a ratio
            float uRatio = uTex - static_cast<float>(uTextInt);
            float vRatio = vTex - static_cast<float>(vTextInt);

            // if with above example, 0.7654 would be 0.2346
            // step 4, get the inverse ratio
            float uInverse = 1.0f - uRatio;
            float vInverse = 1.0f - vRatio;

            // step 5, get all neighboring pixel colors
            //[a][b]
            //[c][d]
            X::Color a = tex.GetPixel(uTextInt, vTextInt) * vInverse;
            X::Color b = tex.GetPixel(uTextInt + 1, vTextInt) * uRatio;
            X::Color c = tex.GetPixel(uTextInt, vTextInt + 1) * uInverse;
            X::Color d = tex.GetPixel(uTextInt + 1, vTextInt + 1) * uRatio;
            
            // step 6, blend all together
            return (a + b) * vInverse
                + (c + d) * vRatio;
        }
}

void Texture::Load(const std::string& fileName)
{
    mFileName = fileName;
    FILE* file = nullptr;
    fopen_s(&file, fileName.c_str(), "rb");
    if (file == nullptr)
    {
        char buffer[128];
        sprintf_s(buffer, "Cam't open file %s", fileName.c_str());
        MessageBoxA(nullptr, buffer, "Texture Error", MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;
    fread(&fileHeader, sizeof(fileHeader), 1, file);
    fread(&infoHeader, sizeof(infoHeader), 1, file);

    if (infoHeader.bits != 24)
    {
        fclose(file);
        MessageBoxA(nullptr, "File Not 24 bit aligned!", "Texture Error!", MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    mWidth = infoHeader.width;
    mHeight = infoHeader.height;
    mPixels = std::make_unique<X::Color[]>(mWidth * mHeight);

    fseek(file, fileHeader.offset, SEEK_SET);

    uint32_t rowStride = mWidth * infoHeader.bits / 8;
    uint32_t paddedStride = MakeStringAligned(rowStride, 4);
    std::vector<uint8_t> paddedBytes(paddedStride - rowStride);
    for (int h = 0; h < mHeight; ++h)
    {
        for (int w = 0; w < mWidth; ++w)
        {
            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t b = 0;
            fread(&b, sizeof(uint8_t), 1, file);
            fread(&g, sizeof(uint8_t), 1, file);
            fread(&r, sizeof(uint8_t), 1, file);
            uint32_t index = w + ((mHeight - h - 1) * mWidth);
            mPixels[index] = { r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
        }
        fread((char*)paddedBytes.data(), paddedBytes.size(), 1, file);
    }
    fclose(file);
}

const std::string& Texture::GetFileName() const
{
	return mFileName;
}

// pases in a value from 0-1 for u and v coordinates
X::Color Texture::GetPixel(float u, float v, AddressMode addressMode, bool filter) const
{
    switch (addressMode)
    {
        case AddressMode::Border:
            if (u < 0.0f || u > 1.0f || v < 0.0f || v > 1.0f)
            {
                return X::Colors::HotPink;
            }
            break;
        case AddressMode::Clamp:

            // use last color outside of 0-1
            u = std::clamp(u, 0.0f, 1.0f);
            v = std::clamp(v, 0.0f, 1.0f);
            break;
        case AddressMode::Wrap:

            // reduce value if more than 1, increase if less than 0 to keep between 0-1
            while (u > 1.0f)
            {
                u -= 1.0f;
            }
            while (u < 0.0f)
            {
                u += 1.0f;
            }
            while (v < 0.0f)
            {
                v += 1.0f;
            }
            while (v < 0.0f)
            {
                v += 1.0f;
            }
            break;
        case AddressMode::Mirror:

            // reduce/increase if touside of 0-2, then if over 1, flip by 2 - value
            while (u > 2.0f)
            {
                u -= 2.0f;
            }
            while (u < 0.0f)
            {
                u += 2.0f;
            }
            u = (u > 1.0f) ? 2.0f - u : u;
            while (v > 2.0f)
            {
                v -= 2.0f;
            }
            while (v < 0.0f)
            {
                v += 2.0f;
            }
            v = (v > 1.0f) ? 2.0f - v : v;
        break;
        default:
            break;
    }

    if (filter)
    {
        return GetBilinearFilterPixelColor(*this, u, v);
    }
    int uIndex = static_cast<int>(u * (mWidth - 1));
    int vIndex = static_cast<int>(v * (mHeight - 1));
    return GetPixel(uIndex, vIndex);
}

// passes in a value from 0- Width-1 and 0-Height-1 (clamp for sanity)
X::Color Texture::GetPixel(int u, int v) const
{
	u = std::clamp(u, 0, mWidth - 1);
	v = std::clamp(v, 0, mHeight - 1);
	return mPixels[u + (v * mWidth)];
}

int Texture::GetWidth() const
{
    return mWidth;
}
int Texture::GetHeight() const
{
    return mHeight;
}