#include "Image.h"

Image::Image(std::string path)
{
	oImg = cv::imread(path);
	cImg = oImg.clone();
	drawImg = oImg.clone();
	center = cv::Point2f((cImg.cols - 1.0) / 2.0, (cImg.rows - 1.0) / 2.0);
	rotation = 0.0;
	zoom = 1.0;
	showRedo = showUndo = false;
	texture = -1;
	addHistory(oImg);
	//createTexture();
}

Image::~Image()
{
	if (texture != -1)
	{
		glDeleteTextures(1, &texture);
	}
}

void Image::createTexture()
{
	if (texture != -1)
	{
		glDeleteTextures(1, &texture);
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, drawImg.cols, drawImg.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, drawImg.data);

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
		showRedo = true;

		history.pop_back();
		history.back().copyTo(drawImg);
		
		if (history.size() == 1)
		{
			showUndo = false;
		}
		createTexture();
	}

}

void Image::Redo()
{
	if (historyUndo.size())
	{
		history.push_back(historyUndo.back());
		showUndo = true;
		historyUndo.back().copyTo(drawImg);
		historyUndo.pop_back();

		if (historyUndo.size() == 0) {
			showRedo = false;
		}
		createTexture();

	}
}

void Image::addHistory(cv::Mat instruction)
{

	history.push_back(instruction.clone());

	if (history.size() > 1)
	{
		createTexture();
		showUndo = true;
	}
	
	if (history.size() > 5)
	{
		history.pop_front();
	}
	if (historyUndo.size()) 
	{
		historyUndo.clear();
		showRedo = false;
	}
}

void Image::calHistogram() 
{
	int histSize = 256;
	float range[] = { 0, 256 }; //the upper boundary is exclusive
	const float* histRange = { range };
	//cv::split(drawImg);
}
