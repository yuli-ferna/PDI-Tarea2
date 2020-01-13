#include "EDConvolution.h"

EDConvolution::EDConvolution() {}

EDImage* EDConvolution::ApplyConvolution(const EDImage& source, EDImage* destiny) {


	destiny = EDImage::CreateCanvas(source);

	int nChannels = source.GetNChannels();
	int imWidth = source.GetWidth() * nChannels;
	int imHeight = source.GetHeight();

	for (size_t yy = 0; yy < imHeight; yy += sy)
	{
		for (size_t xx = 0; xx < imWidth; xx += nChannels)
		{
			float acum[4] = { 0,0,0,0 };
			unsigned char acumDiscrete[4];

			for (size_t cy = 0; cy < height; cy++)
			{
				for (size_t cx = 0; cx < width; cx++)
				{
					int fx = static_cast<int>(xx) - px;
					int fy = static_cast<int>(yy) - py;

					unsigned char *color = source.Get(fx, fy);

					for (size_t cc = 0; cc < nChannels; cc++)
					{
						acum[cc] += color[cc] * data[cy * width + cx] + constant[cc];
					}

					for (size_t cc = 0; cc < nChannels; cc++)
					{
						acumDiscrete[cc] = static_cast<unsigned char>(clamp(255.f, 0.f, acum[cc]));
					}
				}
				destiny->Set(xx, yy, acumDiscrete, nChannels);
			}
		}
	}
	return destiny;

}

EDConvolution* EDConvolution::CreateNegative()
{
	return EDConvolution::CreateCustom({ -1.f }, { 255,255,255,0 }, 1, 1, 0, 0);
}

EDConvolution* EDConvolution::CreateCopyPass()
{
	return EDConvolution::CreateCustom({ 1 }, { 0,0,0,0 }, 1, 1, 0, 0);
}

