//
//  main.cpp
//  EasyDIPBenchmark
//
//  Created by Leonardo Monsalve on 1/3/20.
//

#include "main.h"

//#include "EDImage.hpp"
#include "EasyDIPAPI/EDConvolution.h"

int main()
{
    std::cout<< "hola\n";
    
	EDConvolution *conv = EDConvolution::CreateCustom();



	delete conv;
    return 0;
}
