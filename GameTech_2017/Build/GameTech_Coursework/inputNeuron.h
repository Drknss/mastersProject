#pragma once

#include <vector>
#include "..\nclgl\Vector3.h"

using namespace std;
class inputNeuron
{
public:
	inputNeuron() {};
	~inputNeuron() {};

	void setPixelValues(Vector3 pixel) {		
		rgbData = pixel;
		rgbData.Normalise();
		output = rgbData.Length();
	};

	float getIOutput() { return output; }
private:
	float output;
	Vector3 rgbData;

};

