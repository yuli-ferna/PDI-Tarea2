#include "Event.h"

Event::Event()
{
	row = 3;
	col = 3;
	kernel = std::vector(64, 1);
	structElem = 0;
	structElemSize = 1;
}

Event::~Event()
{
}

void Event::setImage(Image &i)
{
	image = i;
}

cv::Mat Event::getKernel() {
	return cv::Mat(row, col, CV_32FC1, kernel.data());
}

cv::Mat Event::getMorphMat() {
	cv::Mat element;
	int erosion_type = 0;
	if (structElem == 0) { erosion_type = cv::MorphShapes::MORPH_RECT; }
	else if (structElem == 1) { erosion_type = cv::MorphShapes::MORPH_CROSS; }
	else if (structElem == 2) { erosion_type = cv::MorphShapes::MORPH_ELLIPSE; }

	if (structElem == 3)
	{
		element = getKernel();
	}
	else {
		element = cv::getStructuringElement(erosion_type,
			cv::Size(2 * structElemSize + 1, 2 * structElemSize + 1),
			cv::Point(structElemSize, structElemSize));
	}
	return element;
}

void Event::erode()
{
	cv::erode(image.drawImg, image.drawImg, getMorphMat());
	//image.addHistory(image.drawImg);
}

void Event::dilate()
{
	cv::dilate(image.drawImg, image.drawImg, getMorphMat());
	//image.addHistory(image.drawImg);
}

void Event::morphOpen()
{
	morphologyEx(image.drawImg, image.drawImg, cv::MORPH_OPEN, getMorphMat());
	//image.addHistory(image.drawImg);
}

void Event::morphClose() 
{
	morphologyEx(image.drawImg, image.drawImg, cv::MORPH_CLOSE, getMorphMat());
	//image.addHistory(image.drawImg);
}