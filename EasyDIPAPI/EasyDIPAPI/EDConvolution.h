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

	EDImage* ApplyConvolution(const EDImage& source, EDImage* destiny = nullptr);

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
