#pragma once

#include <vector>
#include "..\nclgl\Vector3.h"

#include "inputNeuron.h"



using namespace std;
class outputNeuron;
class hiddenNeuron
{
public:
	hiddenNeuron() { bias = 0.8; };
	~hiddenNeuron() {};

	void update() {
		float temp = 0;
		for (int i = 0; i < input.size(); i++)
		{
			temp += input[i]->getIOutput() * weights[i];
		}
		temp = temp * bias;

		data = (1 / (1 + exp(-temp)));
	};

	void addInput(inputNeuron* n) { input.push_back(n);
	weights.push_back(RandFloat());
	}
	void addOutput(outputNeuron* o) { output.push_back(o); }

	float getHOutput() { return data; }
	double RandFloat() { return (rand()) / (RAND_MAX + 1.0); }
	double RandomClamped() { return RandFloat() - RandFloat(); }

	vector <inputNeuron*> getInputs() { return input; }
	void modifyWeight(float weight, int id) { weights[id] += weight; }

private:
	float bias;
	float data;
	vector <inputNeuron*> input;
	vector <float> weights;
	vector <outputNeuron*> output;
};

