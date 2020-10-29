#pragma once
#include "EDClientpch.h"

#include <opencv2/core/core.hpp>

class Image
{

public:
	cv::Mat oImg, cImg, drawImg;
	cv::Mat histogramImg;
	cv::Point2f center;
	bool showRedo, showUndo;
	unsigned int texture;
	unsigned int histTexture;
	float rotation;
	float zoom;
	bool blueHist, redHist, greenHist;
	std::vector<cv::Mat> histogram;
	std::list<cv::Mat> history;
	std::list<cv::Mat> historyUndo;
	Image(std::string path = "");
	~Image();
	void createTextureHist();
	void Load(std::string path);
	void Save(std::string name);
	void Undo();
	void Redo();
	void createTexture();
	void setTexture(unsigned int& t, cv::Mat drawImg);
	void addHistory(cv::Mat);
	void calHistogram();
};
