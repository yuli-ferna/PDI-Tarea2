#include "Event.h"

Event::Event()
{
	row = 3;
	col = 3;
	typeThresh = 0;
	kernel = std::vector<int>(64, 1);
	structElem = 0;
	structElemSize = 1;
	thresh = 170;
	maxValue = 255;
}

Event::~Event()
{
}

void Event::resetKernel() {
	kernel = std::vector<int>(64, 1);
}

cv::Mat Event::getKernel() {

	std::vector<int> subKernel;
	int MatSize = row * col;
	subKernel.insert(subKernel.begin(),  kernel.begin(), kernel.begin() + MatSize);
	/*cv::Mat asd = cv::Mat(row, col, CV_32SC1, subKernel.data());
	std::cout << asd << std::endl;*/
	return cv::Mat(row, col, CV_32SC1, subKernel.data());
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

void Event::erode(Image& image)
{
	cv::erode(image.drawImg, image.drawImg, getMorphMat());
	//image.createTexture();
	image.addHistory(image.drawImg);
}

void Event::dilate(Image& image)
{
	cv::dilate(image.drawImg, image.drawImg, getMorphMat());
	//image.createTexture();
	image.addHistory(image.drawImg);
}

void Event::morphOpen(Image& image)
{
	morphologyEx(image.drawImg, image.drawImg, cv::MORPH_OPEN, getMorphMat());
	//image.createTexture();
	image.addHistory(image.drawImg);
}

void Event::morphClose(Image& image)
{
	morphologyEx(image.drawImg, image.drawImg, cv::MORPH_CLOSE, getMorphMat());
	//image.createTexture();
	image.addHistory(image.drawImg);
}

void Event::threshold(Image& image)
{
	int thresholdType;
	switch (typeThresh)
	{
		case 0:
			thresholdType = cv::THRESH_OTSU;
			break;
		case 1:
			thresholdType = cv::THRESH_TRIANGLE;
			break;
		default:
			thresholdType = cv::THRESH_TRIANGLE;
			break;
	}
	cv::Mat grayScale;
	cv::cvtColor(image.drawImg, grayScale, cv::COLOR_BGR2GRAY);
	cv::threshold(grayScale, image.drawImg, thresh, maxValue, thresholdType);
	cv::cvtColor(image.drawImg, image.drawImg, cv::COLOR_GRAY2BGR);
	//image.createTexture();
	image.addHistory(image.drawImg);
}