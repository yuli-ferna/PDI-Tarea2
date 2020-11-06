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
	radius = 35;
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
	split(image->drawImg, rgbchannel);
	
	int data[4][4] = { 0,14,3,13,11,5,8,6,2,12,1,15,9, 7,10 ,4 };
	int x, b0, in, b1, vi, y;
	
	for (int k = 0; k < 3; k++) {

		for (int i = 0; i < rgbchannel[k].rows; i++) {

			x = i % 4;

			for (int j = 0; j < rgbchannel[k].cols; j++) {

				y = j % 4;

				in = rgbchannel[k].at<UCHAR>(i, j);

				b0 = in >> 4;

				if (b0 + 1 > 16)
					b1 = 16;
				else
					b1 = b0 + 1;

				vi = in & 16;

				if (vi > data[x][y]) rgbchannel[k].at<UCHAR>(i, j) = b1;

				else rgbchannel[k].at<UCHAR>(i, j) = b0;
			}
		}
	}

	std::vector<cv::Mat>channels;
	channels.push_back(rgbchannel[0]);
	channels.push_back(rgbchannel[1]);
	channels.push_back(rgbchannel[2]);

	cv::merge(channels, image->drawImg);

	ecHistogram(image);

}
void Event::fourierTransform(Image *image) 
{
	if (!complexImages.empty())
	{
		complexImages.clear();

	}
	//complexImages.clear();
	std::vector<cv::Mat> bgr_mat;
	cv::split(image->drawImg, bgr_mat);

	complexImages.push_back(dft(bgr_mat[0]));
	complexImages.push_back(dft(bgr_mat[1]));
	complexImages.push_back(dft(bgr_mat[2]));
	bgr_mat[0] = EspectrumMag(complexImages[0]);
	bgr_mat[1] = EspectrumMag(complexImages[1]);
	bgr_mat[2] = EspectrumMag(complexImages[2]);
	//cv::Mat magI;
	cv::merge(bgr_mat, magImg);
	//cv::imshow("dft", magImg);
	//image->preview = magImg.clone();
	magImg.convertTo(magImg, CV_8UC3, 255);

	image->drawImg = magImg;
	//imshow("Reconstructed", image->preview);

	image->createTexture();
	showPrev = true;

	showLowPass(image, true);
	//cv::imshow("original", image->drawImg);
}

void Event::MagPreview(Image* image) {
	image->preview = magImg;
	//imshow("Reconstructed", image->preview);
	image->createTexturePrev();
}

std::string Event::type2str(int type) {
	std::string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}
//Antes de usar esta funcion primero se debe ejecutar fourierTransform
void Event::showLowPass(Image* image, bool preview) {
	
	std::vector<cv::Mat> filter;
	std::vector<cv::Mat> complexCopy;

	filter.push_back(complexImages[0].clone());
	filter.push_back(complexImages[1].clone());
	filter.push_back(complexImages[2].clone());
	complexCopy.push_back(complexImages[0]);
	complexCopy.push_back(complexImages[1]);
	complexCopy.push_back(complexImages[2]);

	for (int i = 0; i < 3; i++)
	{
		lowPass(filter[i], radius);
		filter[i] = createImgFilter(filter[i], complexCopy[i]);
	}

	cv::Mat inverseTransform;
	cv::merge(filter, inverseTransform);
	inverseTransform.convertTo(inverseTransform, CV_8UC3, 255);
	if (preview)
	{
		//cv::warpAffine(image->drawImg, image->preview, trans_mat, image->drawImg.size());
		image->preview = inverseTransform;
		//imshow("Reconstructed", image->preview);
		image->createTexturePrev();
	}
	else {
		//cv::warpAffine(image->drawImg, image->drawImg, trans_mat, image->drawImg.size());
		image->drawImg = inverseTransform;
		image->addHistory(image->drawImg);
		showPrev = false;
	}
	//imshow("Reconstructed", inverseTransform);
}
cv::Mat Event::dft(cv::Mat mat) {

	cv::Mat withBorder;
	int m = cv::getOptimalDFTSize(mat.rows);
	int n = cv::getOptimalDFTSize(mat.cols); // on the border add zero values
	cv::copyMakeBorder(mat, withBorder, 0, m - mat.rows, 0, n - mat.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	cv::Mat planes[] = { cv::Mat_<float>(withBorder), cv::Mat::zeros(withBorder.size(), CV_32F) };

	cv::Mat complexI;
	cv::merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
	cv::dft(complexI, complexI, cv::DFT_COMPLEX_OUTPUT);            // this way the result may fit in the source matrix

	return complexI;
}

void Event::lowPass(cv::Mat &complexI, int d) {
	cv::Mat tmp = cv::Mat(complexI.rows, complexI.cols, CV_32F);

	cv::Point centre = cv::Point(complexI.rows / 2, complexI.cols / 2);
	double radius;
	int row = complexI.rows, col = complexI.cols;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			radius = (double)sqrt(pow((i - centre.x), 2.0) + pow((double)(j - centre.y), 2.0));
			if (radius > d) {
				tmp.at<float>(i, j) = (float)0;
			}
			else {
				tmp.at<float>(i, j) = (float)1;
			}

		}
	}

	cv::Mat toMerge[] = { tmp, tmp };
	merge(toMerge, 2, complexI);
	
}

cv::Mat Event::createImgFilter(cv::Mat filter, cv::Mat complexI)
{
	cv::Mat planes[2], imgOutput;
	fftShift(complexI); // rearrage quadrants
	cv::mulSpectrums(complexI, filter, complexI, 0); // multiply 2 spectrums
	fftShift(complexI); // rearrage quadrants
	//cv::Mat espectad = EspectrumMag(complexI);
	//cv::imshow("auxilio", espectad);

	// compute inverse
	idft(complexI, complexI);

	split(complexI, planes);
	normalize(planes[0], imgOutput, 0, 1, cv::NORM_MINMAX);
	//cv::imshow("dft", imgOutput);
	return imgOutput;
}

void Event::fftShift(cv::Mat &magI) {
	// crop the spectrum, if it has an odd number of rows or columns
	magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));
	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;
	cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
	cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
	cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right
	cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

cv::Mat Event::EspectrumMag(cv::Mat complexI){
	cv::Mat magI;
	cv::Mat planes[] = {
		cv::Mat::zeros(complexI.size(), CV_32F),
		cv::Mat::zeros(complexI.size(), CV_32F)
	};

	//cv::Mat planes[] = { cv::Mat_<float>(withBorder), cv::Mat::zeros(withBorder.size(), CV_32F) };

	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	magI = planes[0];
	magI += cv::Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);
	fftShift(magI);
	normalize(magI, magI, 0, 1, cv::NORM_MINMAX); // Transform the matrix with float values into a
											// viewable image form (float between values 0 and 1).
	return magI;
}