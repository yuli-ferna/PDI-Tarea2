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
		history.pop_back();

		// execute event
	}
}

void Image::Redo()
{
	if (historyUndo.size())
	{
		history.push_back(historyUndo.back());
		historyUndo.pop_back();

		// execute event
	}
}


void Image::addHistory(std::string instruction)
{

	history.push_back(instruction);
	if (historyUndo.size()) {
		historyUndo.clear();
	}
	if (history.size() > 5)
	{
		history.pop_front();
	}
}

void Image::printHistory() {
	std::cout << "history" << std::endl;
	for (std::list<std::string>::iterator it1 = history.begin(); it1 != history.end(); ++it1)
		std::cout << ' ' << *it1;
	std::cout << '\n';
	std::cout << "historyUndo" << std::endl;
	for (std::list<std::string>::iterator it1 = historyUndo.begin(); it1 != historyUndo.end(); ++it1)
		std::cout << ' ' << *it1;
	std::cout << '\n';

}