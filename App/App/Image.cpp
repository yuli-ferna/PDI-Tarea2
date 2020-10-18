#include "Image.h"

Image::Image(std::string path)
{
	oImg = cImg = drawImg = cv::imread(path);
	center= cv::Point2f((cImg.cols - 1) / 2.0, (cImg.rows - 1) / 2.0);

}

Image::~Image()
{
}

void Image::Load(std::string path)
{
	oImg = cImg = drawImg = cv::imread(path);
}

void Image::Save(std::string name)
{
	cv::imwrite(name, drawImg);
}
