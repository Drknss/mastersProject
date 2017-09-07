#pragma once

#include <vector>

using namespace std;

class hiddenNeuron;
class outputNeuron
{
public:
	outputNeuron() { fitness = 1;
	bias = 0.8;
	};
	~outputNeuron() {};
	void update() {
		float temp = 0;
		for (int i = 0; i < input.size(); i++)
		{
			temp += input[i]->getHOutput() * weights[i];
		}
		temp = temp * bias;

		output = (1 / (1 + exp(-temp)));
	};

	double RandFloat() { return (rand()) / (RAND_MAX + 1.0); }
	double RandomClamped() { return RandFloat() - RandFloat(); }
	float getOOutput() { return output; }
	int	getFitness() { return fitness; }

	void modifyWeight(float weight, int id) { weights[id] += weight; }
	void incFitness(int a) { fitness += a; }
	void addInput(hiddenNeuron* n) { input.push_back(n);
	weights.push_back(RandFloat());
	}

	

	vector <hiddenNeuron*> getInputs() { return input; }

private:
	vector <hiddenNeuron*> input;
	vector <float> weights;

	float bias;
	int fitness;
	float output;
};

