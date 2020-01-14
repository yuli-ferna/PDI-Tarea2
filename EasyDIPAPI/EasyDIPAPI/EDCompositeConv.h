
#ifndef __ED_COMPOSITE_CONV__
#define __ED_COMPOSITE_CONV__
#include "EDBaseConv.h"
#include "EDConvolution.h"

class EDCompositeConv : public EDBaseConvolution, public std::list<EDBaseConvolution*>
{
public:

	EDImage* ApplyConvolution(const EDImage& source, EDImage* destiny = nullptr) {


		bool copyAfter = &source == destiny;

		if (!destiny)
		{
			destiny = EDImage::CreateCanvas(source);
		}

		

		for (auto conv : *this)
		{
			conv->ApplyConvolution(*destiny, destiny);
		}

		if (copyAfter)
		{
			// TODO: permitir que el usuario escriba en la misma imagen de entrada
		}

		return destiny;
	}

	static EDCompositeConv* Create()
	{
		EDCompositeConv *conv = new EDCompositeConv();

		return conv;
	}





	~EDCompositeConv();
private:
	EDCompositeConv();
};

#endif // !__ED_COMPOSITE_CONV__


