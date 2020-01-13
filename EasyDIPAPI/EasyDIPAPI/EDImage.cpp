#include "EDImage.h"


bool EDImage::TryLoad(const std::string& path, EDImage*& img) {
	img = new EDImage();
	std::cout << "loading " << path << "\n";

	img->data = stbi_load(path.data(), &img->width, &img->height, &img->nChannels, 0);
	img->widthChannels = img->width * img->nChannels;
	return img->data;
}

bool EDImage::TrySave(EDImage* img, const std::string& fileName)
{
	return stbi_write_png(fileName.data(), img->width, img->height, img->nChannels, img->data, img->nChannels * img->width);
}

EDImage* EDImage::CreateCanvas(const EDImage& other)
{
	EDImage* img = new EDImage();

	img->width = other.width;
	img->nChannels = other.nChannels;
	img->height = other.height;
	img->data = new unsigned char[img->width * img->height * img->nChannels + 1]{};
	img->widthChannels = img->width * img->nChannels;

	return img;
}

EDImage::EDImage() {

	data = nullptr;
}

unsigned char* EDImage::Get(int x, int y) const
{
	return &data[y * widthChannels + x];
}

void EDImage::Set(int x, int y, unsigned char* color, int nChannels)
{
	unsigned char* col = Get(x, y);

	for (size_t ii = 0; ii < nChannels; ii++)
	{
		col[ii] = color[ii];
	}
}

EDImage::~EDImage() {
	stbi_image_free(data);
}

int EDImage::GetWidth() const
{
	return width;
}

int EDImage::GetHeight() const
{
	return height;
}

int EDImage::GetNChannels() const
{
	return nChannels;
}

int EDImage::GetWidthChannels() const
{
	return widthChannels;
}

float clamp(const float u, const float d, const float v)
{
	return std::min(std::max(d,v), u);
}
