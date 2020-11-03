#include "Event.h"

Event::Event()
{
	row = 3;
	col = 3;
	typeThresh = 0;
	kernel = std::vector<int>(64, 1);
	structElem = 0;
	morphMethod = 0;
	angle = 0.0;
	structElemSize = 1;
	thresh = 170;
	maxValue = 255;
	cw = 1;
	showPrev = false;
	Ncolors = 16;
	nBits = 4;
}

Event::~Event()
{
}

void Event::resetKernel() {
	kernel = std::vector<int>(64, 1);
}

cv::Mat Event::getKernel() {

	std::vector<int> subKernel;
	int MatSize = row * col;
	subKernel.insert(subKernel.begin(),  kernel.begin(), kernel.begin() + MatSize);
	/*cv::Mat asd = cv::Mat(row, col, CV_32SC1, subKernel.data());
	std::cout << asd << std::endl;*/
	return cv::Mat(row, col, CV_32SC1, subKernel.data());
}

cv::Mat Event::getMorphMat() {
	cv::Mat element;
	int erosion_type = 0;
	if (structElem == 0) { erosion_type = cv::MorphShapes::MORPH_RECT; }
	else if (structElem == 1) { erosion_type = cv::MorphShapes::MORPH_CROSS; }
	else if (structElem == 2) { erosion_type = cv::MorphShapes::MORPH_ELLIPSE; }

	if (structElem == 3)
	{
		element = getKernel();
	}
	else {
		element = cv::getStructuringElement(erosion_type,
			cv::Size(2 * structElemSize + 1, 2 * structElemSize + 1),
			cv::Point(structElemSize, structElemSize));
	}
	return element;
}

void Event::erode(Image* image)
{
	cv::erode(image->drawImg, image->drawImg, getMorphMat());
	image->addHistory(image->drawImg);
}

void Event::dilate(Image* image)
{
	cv::dilate(image->drawImg, image->drawImg, getMorphMat());
	image->addHistory(image->drawImg);
}

void Event::morphOpen(Image* image)
{
	morphologyEx(image->drawImg, image->drawImg, cv::MORPH_OPEN, getMorphMat());
	image->addHistory(image->drawImg);
}

void Event::morphClose(Image* image)
{
	morphologyEx(image->drawImg, image->drawImg, cv::MORPH_CLOSE, getMorphMat());
	image->addHistory(image->drawImg);
}

void Event::threshold(Image* image)
{
	int thresholdType;
	switch (typeThresh)
	{
		case 0:
			thresholdType = cv::THRESH_OTSU;
			break;
		case 1:
			thresholdType = cv::THRESH_TOZERO_INV;
			break;
		case 2:
			thresholdType = cv::THRESH_TOZERO;
			break;
		default:
			thresholdType = cv::THRESH_TRIANGLE;
			break;
	}
	cv::Mat grayScale;
	cv::cvtColor(image->drawImg, grayScale, cv::COLOR_BGR2GRAY);
	cv::threshold(grayScale, image->drawImg, thresh, maxValue, thresholdType);
	cv::cvtColor(image->drawImg, image->drawImg, cv::COLOR_GRAY2BGR);
	image->addHistory(image->drawImg);
}

void Event::ecHistogram(Image* image)
{
	std::vector<cv::Mat> bgr_mat;
	cv::split(image->drawImg, bgr_mat);
	cv::equalizeHist(bgr_mat[0], bgr_mat[0]);
	cv::equalizeHist(bgr_mat[1], bgr_mat[1]);
	cv::equalizeHist(bgr_mat[2], bgr_mat[2]);
	
	cv::merge(bgr_mat, image->drawImg);
	
	image->addHistory(image->drawImg);
}


void Event::rotation(Image* image, int type) {

	//in event class
	//cv::resize(image->cImg, image->drawImg, cv::Size(), image->zoom, image->zoom);
	int finalAngle = angle;
	if (type == ARBITRARY)
	{
		finalAngle *= (cw==1) ? -1.0 : 1.0;
	}
	cv::Point2f center = cv::Point2f((image->drawImg.cols - 1.0) / 2.0, (image->drawImg.rows - 1.0) / 2.0);

	cv::Mat rot = cv::getRotationMatrix2D(center, finalAngle, 1.0);
	// Create bounding box
	cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), image->drawImg.size(), finalAngle).boundingRect2f();
	// adjust transformation matrix
	rot.at<double>(0, 2) += bbox.width / 2.0 - image->drawImg.cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - image->drawImg.rows / 2.0;

	cv::warpAffine(image->drawImg, image->drawImg, rot, bbox.size());
	//image->createTexture();
	image->addHistory(image->drawImg);

}

void Event::traslate(Image* image, int translateX, int translateY, bool preview) {
	//in event class
	cv::Mat trans_mat = (cv::Mat_<double>(2, 3) << 1, 0, translateX, 0, 1, translateY);
	if (preview)
	{
		cv::warpAffine(image->drawImg, image->preview, trans_mat, image->drawImg.size());
		image->createTexturePrev();
	}
	else {
		cv::warpAffine(image->drawImg, image->drawImg, trans_mat, image->drawImg.size());
		image->addHistory(image->drawImg);
		showPrev = false;
	}
}


void Event::bitReduction(Image* image) {

	// mask used to round the pixel value
	int n = nBits + 1;
	uchar mask = 0xFF << n;
	int div = pow(2, n);
		
	float newColor[256];
	// Haciendo esta operacion se descartarian los bits que no se requieren
	for (int i = 0; i < 256; ++i)
		newColor[i] = (i & mask) + div / 2;
	//newColor[i] = (i & mask);

	std::vector<cv::Mat> bgr_mat;
	cv::split(image->drawImg, bgr_mat);

	const int channels = image->drawImg.channels();
	switch (channels)
	{
	case 1:
	{
		cv::MatIterator_<uchar> it, end;
		for (it = image->drawImg.begin<uchar>(), end = image->drawImg.end<uchar>(); it != end; ++it)
			*it = newColor[*it];
		break;
	}
	case 3:
	{
		cv::MatIterator_<cv::Vec3b> it, end;
		for (it = image->drawImg.begin<cv::Vec3b>(), end = image->drawImg.end<cv::Vec3b>(); it != end; ++it)
		{
			(*it)[0] = newColor[(*it)[0]];
			(*it)[1] = newColor[(*it)[1]];
			(*it)[2] = newColor[(*it)[2]];
			
		}
	}
	}

	image->addHistory(image->drawImg);
}

// fillType false es para vencidad de 4 y true para vencidad de 8
// rangeType false para rango fijo true rango flotante 

void Event::fillImage(Image* image, cv::Vec2i seed , float fillColor[], bool fillType, bool rageType, int loDiff, int upDiff)
{	
	int flags = 4;
	uchar fillValue;

	if (fillType)
		flags += 255 << 8;
	else
		flags += 128 << 4;

	if (rageType)
		flags += cv::FLOODFILL_FIXED_RANGE;

	cv::floodFill(image->drawImg, seed,cv::Scalar(fillColor[2]*255, fillColor[1]*255, fillColor[0]*255),0, cv::Scalar(loDiff, loDiff, loDiff), cv::Scalar(upDiff, upDiff, upDiff), flags);
	

	image->addHistory(image->drawImg);

}

void Event::kMeans(Image* image, int k) 
{
	std::vector<int> labels;
	cv::Mat1f centers(8, 1, CV_32FC1);
    int n = image->drawImg.rows * image->drawImg.cols;
	cv::Mat data = image->drawImg.reshape(1, n);

	data.convertTo(data,CV_32F);
	cv::kmeans(data, k, labels, cv::TermCriteria(), 3, cv::KMEANS_PP_CENTERS, centers);

	for (int i = 0; i < n; ++i)
	{
		data.at<float>(i, 0) = centers(labels[i], 0);
		data.at<float>(i, 1) = centers(labels[i], 1);
		data.at<float>(i, 2) = centers(labels[i], 2);
	}

	cv::Mat result = data.reshape(3, image->drawImg.rows);
	result.convertTo(image->drawImg, CV_8U);
	image->addHistory(image->drawImg);

}

void Event::dithering(Image* image) {

	cv::Mat rgbchannel[3],src;
	image->drawImg.convertTo(src, CV_64F);
	split(src, rgbchannel);
	
	double data[4][4] = { 0,14,3,13,11,5,8,6,2,12,1,15,9, 7,10 ,4 };
	int x, b0, in, b1, vi, y;
	

	for (int k = 0; k < 3; k++) {

		for (int i = 0; i < rgbchannel[k].rows; i++) {

			x = i % 4;

			for (int j = 0; j < rgbchannel[k].cols; j++) {

				y = j % 4;

				in = rgbchannel[k].at<double>(i, j);

				/*if (in < 127)
					b0 = 0;
				else
					b0 = 255;

				vi = in & 0xf;*/
				b0 = in >> 2;

				if (b0 + 1 > 64)
					b1 = 64;
				else
					b1 = b0 + 1;

				vi = in & 64;

				if (vi > data[x][y]) rgbchannel[k].at<double>(i, j) = b1;

				else rgbchannel[k].at<double>(i, j) = b0;
			}
		}
	}

	std::vector<cv::Mat>channels;
	channels.push_back(rgbchannel[0]);
	channels.push_back(rgbchannel[1]);
	channels.push_back(rgbchannel[2]);

	cv::merge(channels, src);
	src.convertTo(image->drawImg, CV_8UC3);
	
	image->addHistory(image->drawImg);
}