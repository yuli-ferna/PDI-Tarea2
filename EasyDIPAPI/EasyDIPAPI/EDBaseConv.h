#ifndef __ED_BASE_CONVOLUTION__
#define __ED_BASE_CONVOLUTION__

#include "EDImage.h"


/**
* asdasd
* 
*/
class EDBaseConvolution
{
protected:
	EDBaseConvolution();
public:
	virtual EDImage* ApplyConvolution(const EDImage& source, EDImage* destiny = nullptr) = 0;
};


#endif // !__ED_CONVOLUTION__
