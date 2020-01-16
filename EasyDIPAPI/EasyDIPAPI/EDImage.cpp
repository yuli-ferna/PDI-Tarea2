#include "EDImage.h"


bool EDImage::TryLoad(const std::string& path, EDImage*& img) {
	img = new EDImage();
	std::cout << "loading " << path << "\n";

	img->data = stbi_load(path.data(), &img->width, &img->height, &img->nChannels, 0);

	
	//stbi_info(path.data());


	img->widthChannels = img->width * img->nChannels;
	return img->data;
}

bool EDImage::TrySave(const EDImage& img, const std::string& fileName)
{
	return stbi_write_png(fileName.data(), img.width, img.height, img.nChannels, img.data, img.nChannels * img.width);
}

EDImage* EDImage::CreateCanvas(const EDImage& other, EDImage* destiny)
{
	EDImage* img;

	if (destiny)
	{
		img = destiny;
		delete destiny->data;
	}
	else
	{
		img = new EDImage();
	}

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
	if (x < 0 || x >= widthChannels)
	{
		return defColor;
	}

	if (y < 0 || y >= height) {
		return defColor;
	}

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
	delete defColor;
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

bool EDImage::Compatibles(const EDImage & imA, const EDImage & imB)
{
	if (imA.widthChannels != imB.widthChannels)
	{
		return false;
	}
	if (imA.height != imB.height)
	{
		return false;
	}
	return true;
}

EDImage * EDImage::Operate(const EDImage & imA, const EDImage & imB, std::function<void(unsigned char*, unsigned char*, unsigned char*, int)> operation, EDImage * destiny)
{
	if (!destiny)
	{
		destiny = EDImage::CreateCanvas(imA);
	}

	size_t width = imA.widthChannels * imA.height;
	int nChannels = imA.nChannels;

	for (size_t ii = 0; ii < width; ii += nChannels)
	{
		operation(&destiny->data[ii], &imA.data[ii], &imB.data[ii], nChannels);
	}

	return destiny;
}

EDImage * EDImage::Add(const EDImage & imA, const EDImage & imB, EDImage * destiny)
{
	return Operate(imA, imB, [](unsigned char* dest, unsigned char* a, unsigned char* b, int nChannels) {
		for (int ii = 0; ii < nChannels; ii++)
		{
			dest[ii] = clamp(255, 0, a[ii] + b[ii]);
		}
	}, destiny);
}

EDImage * EDImage::Gradient(const EDImage & imA, const EDImage & imB, EDImage * destiny)
{
	return Operate(imA, imB, [](unsigned char* dest, unsigned char* a, unsigned char* b, int nChannels) {
		for (int ii = 0; ii < nChannels; ii++)
		{
			dest[ii] = clamp(255, 0, sqrt(a[ii] * a[ii] + b[ii] * b[ii]));
		}

	}, destiny);
}

EDImage * EDImage::Multiply(const EDImage & imA, const EDImage & imB, EDImage * destiny)
{
	return Operate(imA, imB, [](unsigned char* dest, unsigned char* a, unsigned char* b, int nChannels) {
		for (int ii = 0; ii < nChannels; ii++)
		{
			dest[ii] = clamp(255, 0, a[ii] * b[ii]);
		}
	}, destiny);
}

EDImage * EDImage::Divide(const EDImage & imA, const EDImage & imB, EDImage * destiny)
{
	return Operate(imA, imB, [](unsigned char* dest, unsigned char* a, unsigned char* b, int nChannels) {
		for (int ii = 0; ii < nChannels; ii++)
		{
			dest[ii] = a[ii] / b[ii];
		}
	}, destiny);
}

EDImage * EDImage::Substract(const EDImage & imA, const EDImage & imB, EDImage * destiny)
{
	return Operate(imA, imB, [](unsigned char* dest, unsigned char* a, unsigned char* b, int nChannels) {
		for (int ii = 0; ii < nChannels; ii++)
		{
			dest[ii] = clamp(255, 0, a[ii] - b[ii]);
		}
	}, destiny);
}

float clamp(const float u, const float d, const float v)
{
	return std::min(std::max(d,v), u);
}


