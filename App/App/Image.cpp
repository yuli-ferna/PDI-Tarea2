#include "Image.h"

Image::Image(std::string path)
{
	oImg = cImg = drawImg = cv::imread(path);
	center = cv::Point2f((cImg.cols - 1.0) / 2.0, (cImg.rows - 1.0) / 2.0);
	rotation = 0.0;
	zoom = 1.0;
	addHistory(oImg);
}

Image::~Image()
{
}

void Image::Load(std::string path)
{
	//oImg = cImg = drawImg = cv::imread(path);
	oImg = cv::imread(path);
	oImg.copyTo(cImg);
	oImg.copyTo(drawImg);
}

void Image::Save(std::string name)
{
	cv::imwrite(name, drawImg);
}

void Image::Undo()
{
	if (history.size() > 1)
	{
		historyUndo.push_back(history.back());
		//cImg = history.back();
		//drawImg = oImg;
		//drawImg = history.back().clone();
		//history.back().release();
		history.pop_back();
		//drawImg.release();
		history.back().copyTo(drawImg);
		// execute event

	}
}

void Image::Redo()
{
	if (historyUndo.size())
	{
		history.push_back(historyUndo.back());
		//cImg = historyUndo.back();
		//drawImg.release();
		historyUndo.back().copyTo(drawImg);
		/*historyUndo.back().release();*/
		historyUndo.pop_back();

	}
		// execute event
}

void Image::addHistory(cv::Mat instruction)
{

	history.push_back(instruction.clone());
	//cImg = instruction;
	//drawImg = instruction;
	if (historyUndo.size()) 
	{
		historyUndo.clear();
	}
	if (history.size() > 5)
	{
		history.pop_front();
	}
}
