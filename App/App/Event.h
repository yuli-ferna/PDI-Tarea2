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
	int radius;
	std::vector<cv::Mat> complexImages;
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
	void fourierTransform(Image* image);
	void showLowPass();
	cv::Mat dft(cv::Mat mat);
	void lowPass(cv::Mat& imgDft, int d);
	cv::Mat createImgFilter(cv::Mat filter, cv::Mat complexI);
	void fftShift(cv::Mat& magI);
	cv::Mat EspectrumMag(cv::Mat complexI);
};
