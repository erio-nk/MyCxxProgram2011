#include <iostream>
#include <cmath>	// for fmod

float roundDegree(float x)
{
	if (x >= 0.f) {
		return fmod(x, 360.f);
	} else {
		return 360.f - fmod(-x, 360.f);
	}
	return x;
}

int main()
{
	std::cout 
		<< roundDegree(10.f)	<< std::endl
		<< roundDegree(120.30f) << std::endl
		<< roundDegree(400.f)	<< std::endl
		<< roundDegree(900.58f)	<< std::endl
		<< roundDegree(-10.90f)	<< std::endl
		<< roundDegree(-370.f)	<< std::endl
		<< roundDegree(-900.f)	<< std::endl
		;

	return 0;
}
