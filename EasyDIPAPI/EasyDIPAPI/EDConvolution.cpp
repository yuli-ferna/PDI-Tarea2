#include "EDConvolution.h"

EDConvolution::EDConvolution() {}



EDConvolution* EDConvolution::CreateNegative()
{
	return EDConvolution::CreateCustom({ -1.f }, { 255,255,255,0 }, 1, 1, 0, 0);
}

EDConvolution* EDConvolution::CreateCopyPass()
{
	return EDConvolution::CreateCustom({ 1 }, { 0,0,0,0 }, 1, 1, 0, 0);
}

