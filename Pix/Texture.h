#pragma once
#include <XEngine.h>

enum class AddressMode
{
	Border,  // set color outside of 0-1
	Clamp,  // use last colot from 0-1
	Wrap,   // repeat after going beyong 0-1
	Mirror  // mirror after crossing each 0-1
};

class Texture
{
	public:

		void Load(const std::string& fileName);
		const std::string& GetFileName() const;

		X::Color GetPixel(float u, float v) const;
		X::Color GetPixel(int u, int v) const;

		int GetWidth() const;
		int GetHeight() const;

	private:

		std::string mFileName;
		std::unique_ptr<X::Color[]> mPixels;
		int mWidth = 0;
		int mHeight = 0;
};
