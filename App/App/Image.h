#pragma once
#include "EDClientpch.h"

#include <opencv2/core/core.hpp>

class Image
{

public:
	cv::Mat oImg,cImg,drawImg;
	cv::Point2f center;
	float rotation;
	float zoom;
	std::list<cv::Mat> history;
	std::list<cv::Mat> historyUndo;
	Image(std::string path = "");
	~Image();
	void Load(std::string path);
	void Save(std::string name);
	void Undo();
	void Redo();
	void addHistory(cv::Mat);

	void printHistory();


};
