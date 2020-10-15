#pragma once
#include "EDClientpch.h"

#include <opencv2/core/core.hpp>

class Image
{
public:
	cv::Mat mat;

	Image(std::string path = "");
	~Image();
	void Load(std::string path);
	void Save(std::string name);
	
};
