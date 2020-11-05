#include "Image.h"

Image::Image(std::string p)
{
	path = p;
	oImg = cv::imread(p);
	preview = oImg.clone();
	drawImg = oImg.clone();
	center = cv::Point2f((preview.cols - 1.0) / 2.0, (preview.rows - 1.0) / 2.0);
	rotation = 0.0;
	zoom = 1.0;
	showRedo = showUndo = false;
	texture = -1;
	histTexture = -1;
	blueHist = redHist = greenHist = true;
	addHistory(oImg);
}

Image::~Image()
{
	if (texture != -1)
	{
		glDeleteTextures(1, &texture);
	}
	
	if (histTexture != -1)
	{
		glDeleteTextures(1, &histTexture);
	}
}

void Image::createTexture()
{
	setTexture(texture, drawImg);
}

void Image::createTexturePrev()
{
	setTexture(texture, preview);
}

void Image::setTexture(unsigned int& t, cv::Mat drawImg)
{
	if (t != -1)
	{
		glDeleteTextures(1, &t);
	}

	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, drawImg.cols, drawImg.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, drawImg.data);

}

void Image::Load(std::string p)
{
	//oImg = preview = drawImg = cv::imread(p);
	oImg = cv::imread(p);
	oImg.copyTo(preview);
	oImg.copyTo(drawImg);
}

void Image::Save(std::string name)
{
	cv::imwrite(name, drawImg);
	path = name;
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
		calHistogram();
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
		calHistogram();
	}
}

void Image::addHistory(cv::Mat instruction)
{

	history.push_back(instruction.clone());
	createTexture();
	calHistogram();
	if (history.size() > 1)
	{
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
	std::vector<cv::Mat> bgr_mat;
	cv::split(drawImg, bgr_mat);
	bool uniform = true, accumulate = false;

	cv::Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_mat[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, true, false);
	calcHist(&bgr_mat[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, true, false);
	calcHist(&bgr_mat[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, true, false);

	//int hist_w = 512, hist_h = 400;
	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	histogramImg = cv::Mat(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

	normalize(b_hist, b_hist, 0, histogramImg.rows, cv::NORM_MINMAX, -1, cv::Mat());
	normalize(g_hist, g_hist, 0, histogramImg.rows, cv::NORM_MINMAX, -1, cv::Mat());
	normalize(r_hist, r_hist, 0, histogramImg.rows, cv::NORM_MINMAX, -1, cv::Mat());
	for (int i = 0; i < histSize; i++)
	{
		if (blueHist)
		{

			line(histogramImg, cv::Point(bin_w * i, hist_h),
				cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
				cv::Scalar(255, 0, 0), 2, cv::FILLED, 0);

		}
		if (greenHist)
		{
			line(histogramImg, cv::Point(bin_w * i , hist_h),
				cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
				cv::Scalar(0, 255, 0), 2, cv::FILLED, 0);
		}
		if (redHist)
		{
			line(histogramImg, cv::Point(bin_w * i, hist_h),
				cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
				cv::Scalar(0, 0, 255), 2, cv::FILLED, 0);

		}
	}

	//imshow("calcHist Demo", histogramImg);
	setTexture(histTexture, histogramImg);
}

