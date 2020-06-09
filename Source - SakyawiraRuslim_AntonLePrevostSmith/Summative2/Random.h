#pragma once
#include <random>

namespace
{
	std::random_device rd;

    //
    // Engines 
    //
    std::mt19937 e2(rd());

    //
    // Distribtuions
    //
    std::uniform_real_distribution<float> dist(0, 1);

    float GenerateFloat()
	{
        return dist(rd);
	}
}
