#include "Image.h"

Image::Image(std::string path)
{
	oImg = cImg = drawImg = cv::imread(path);
	center = cv::Point2f((cImg.cols - 1.0) / 2.0, (cImg.rows - 1.0) / 2.0);
	rotation = 0.0;
	zoom = 1.0;
}

Image::~Image()
{
}

void Image::Load(std::string path)
{
	oImg = cImg = drawImg = cv::imread(path);
}

void Image::Save(std::string name)
{
	cv::imwrite(name, drawImg);
}

void Image::Undo()
{
	if (history.size())
	{
		historyUndo.push_back(history.back());
		cImg = history.back();
		drawImg = history.back();
		history.pop_back();

		// execute event

	}
}

void Image::Redo()
{
	if (historyUndo.size())
	{
		history.push_back(historyUndo.back());
		cImg = historyUndo.back();
		drawImg = historyUndo.back();
		historyUndo.pop_back();

		// execute event
	}
}

void Image::addHistory(cv::Mat instruction)
{

	history.push_back(instruction);
	cImg = instruction;
	drawImg = instruction;
	if (historyUndo.size()) 
	{
		historyUndo.clear();
	}
	if (history.size() > 5)
	{
		history.pop_front();
	}
}
