

#ifndef __ED_IMAGE__
#define __ED_IMAGE__

#include "EDpch.h"

float clamp(const float u, const float d, const float v);
#include <stb_image.h>
#include <stb_image_write.h>

//#include <stb_image_write.h>

class EDImage
{
public:
	static bool TryLoad(const std::string& path, EDImage*& img);
	static bool TrySave(EDImage* img, const std::string& fileName);
	static EDImage* CreateCanvas(const EDImage& other);
	
	unsigned char* Get(int x, int y) const;
	void Set(int x, int y, unsigned char* color, int nChannels);
	~EDImage();
	int GetWidth() const;
	int GetHeight() const;
	int GetNChannels() const;
	int GetWidthChannels() const;
private:
	size_t widthChannels;
	int width, height, nChannels;
	unsigned char* data;
	EDImage();
};

#endif // !__ED_IMAGE__

