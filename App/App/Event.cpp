#include "Event.h"

Event::Event()
{
	row = 4;
	col = 1;
	kernel = std::vector(64, 1);
	structElem = 0;
}

Event::~Event()
{
}

void Event::setImage(Image i)
{
	image = i;
}

void Event::erode()
{
	//cv::imshow("mat", mat);
	cv::Mat element;
	int erosion_type = 0;
	int erosion_size = 4;
	if (structElem == 0) { erosion_type = cv::MorphShapes::MORPH_RECT; }
	else if (structElem == 1) { erosion_type = cv::MorphShapes::MORPH_CROSS; }
	else if (structElem == 2) { erosion_type = cv::MorphShapes::MORPH_ELLIPSE; }
	
	if (structElem == 3)
	{
		element = cv::Mat(row, col, CV_32FC1, kernel.data());
	}
	else {
		element = cv::getStructuringElement(erosion_type,
			cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
			cv::Point(erosion_size, erosion_size));
	}
	cv::erode(image.cImg, image.drawImg, element);

}
