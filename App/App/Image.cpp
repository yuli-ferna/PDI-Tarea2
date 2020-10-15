#include "Image.h"

Image::Image(std::string path)
{
	mat = cv::imread(path);
}

Image::~Image()
{
}

void Image::Load(std::string path)
{
}

void Image::Save(std::string name)
{
	cv::imwrite(name, mat);
}
