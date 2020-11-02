#pragma once
#include "EDClientpch.h"
#include "Image.h"

#include <opencv2/core/core.hpp>

class Event
{
	//Image image;
	
public:
	bool showPrev;
	int row;
	int col;
	int structElem;
	int structElemSize;
	int morphMethod;
	int typeThresh;
	int cw;
	int Ncolors;
	double thresh;
	double maxValue;
	float angle;
	std::vector<int> kernel;
	Event();
	~Event();
	void resetKernel();
	cv::Mat getKernel();
	cv::Mat getMorphMat();
	void erode(Image *);
	void dilate(Image *);
	void morphOpen(Image *);
	void morphClose(Image *);
	void threshold(Image *);
	void ecHistogram(Image * image);
	void rotation(Image* image, int type);
	void traslate(Image* image, int translateX, int translateY, bool preview = false);
	void uniformQuantization(Image* image);

};
