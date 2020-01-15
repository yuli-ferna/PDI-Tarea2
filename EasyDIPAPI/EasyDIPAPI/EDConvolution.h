#pragma once




#ifndef __ED_CONVOLUTION__
#define __ED_CONVOLUTION__

#include "EDBaseConv.h"

class EDConvolution : public EDBaseConvolution
{
public:
	int width, height;
	int sx, sy;
	int px, py;
	std::vector<float> constant;
	std::vector<float> data;
	EDConvolution();

	static std::vector<float> Normalize(std::vector<float> d)
	{
		float sum = 0;

		for (float val : d)
		{
			sum += val;
		}

		for (float& val : d)
		{
			val /= sum;
		}

		return d;
	}

	EDImage* EDConvolution::ApplyConvolution(const EDImage& source, EDImage* destiny = nullptr) {

		bool isSameImg = &source == destiny;
		if (!destiny || isSameImg)
		{
			destiny = EDImage::CreateCanvas(source);
		}
		else
		{
			EDImage::CreateCanvas(source, destiny);
		}

		int nChannels = source.GetNChannels();
		int imWidth = source.GetWidth() * nChannels;
		int imHeight = source.GetHeight();
		int sxFinal = sx * nChannels;
		int pxStart = px * nChannels;
		for (size_t yy = 0; yy < imHeight; yy += sy)
		{
			for (size_t xx = 0; xx < imWidth; xx += sxFinal)
			{
				float acum[4] = { 0,0,0,0 };
				unsigned char acumDiscrete[4];

				for (size_t cy = 0; cy < height; cy++)
				{
					for (size_t cx = 0, cxc= 0; cx < width; cx++, cxc+= nChannels)
					{
						int fx = static_cast<int>(xx) - pxStart + cxc;
						int fy = static_cast<int>(yy) - py + cy;

						unsigned char* color = source.Get(fx, fy);

						for (size_t cc = 0; cc < nChannels; cc++)
						{
							acum[cc] += color[cc] * data[cy * width + cx] + constant[cc];
						}
					}
				}
				for (size_t cc = 0; cc < nChannels; cc++)
				{
					acumDiscrete[cc] = static_cast<unsigned char>(clamp(255.f, 0.f, acum[cc]));
				}
				destiny->Set(xx, yy, acumDiscrete, nChannels);
			}
		}
	
		
		
		
		
		return destiny;
	}
	static EDConvolution* CreateCustom(std::vector<float> data, std::vector<float> constant, int w, int h, int px, int py, int sx = 1, int sy = 1) {
		EDConvolution* conv = new EDConvolution();

		conv->data = data;
		conv->constant = constant;
		conv->sy = sy;
		conv->sx = sx;
		conv->px = px;
		conv->py = py;
		conv->width = w;
		conv->height = h;

		return conv;
	}
	static EDConvolution* CreateNegative();
	static EDConvolution* CreateCopyPass();

	EDConvolution& operator=(const EDConvolution &conv)
	{
		this->data = conv.data;
		this->constant = conv.constant;
		this->sy = conv.sy;
		this->sx = conv.sx;
		this->px = conv.px;
		this->py = conv.py;
		this->width = conv.width;
		this->height = conv.height;

		return *this;
	}
};


#endif // !__ED_CONVOLUTION__
