#pragma once
#include "EDClientpch.h"
#include "Image.h"

#include <opencv2/core/core.hpp>

class Event
{
	Image image;
	
public:
	int row;
	int col;
	int structElem;
	int structElemSize;
	std::vector<int> kernel;
	Event();
	~Event();
	void setImage(Image);
	cv::Mat getKernel();
	cv::Mat getMorphMat();
	void erode();
	void dilate();
	void morphOpen();
	void morphClose();
};
