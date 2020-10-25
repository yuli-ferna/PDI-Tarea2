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
	std::vector<int> kernel;
	Event();
	~Event();
	void setImage(Image);
	void erode();
};
