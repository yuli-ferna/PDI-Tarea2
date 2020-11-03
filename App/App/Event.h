#pragma once
#include "EDClientpch.h"
#include "Image.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 

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
	int nBits;
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
	void bitReduction(Image* image);

	void fillImage(Image* image, cv::Vec2i seed, float fillColor[], bool fillType, bool rageType, int loDiff, int upDiff);
	void kMeans(Image* image, int k);
	void dithering(Image* image);
};
