#pragma once

#include <string>

#include ".\nclgl\Vector3.h"
#include <ncltech\CommonUtils.h>
#include <time.h>

#include "inputNeuron.h"
#include "hiddenNeuron.h"
#include "outputNeuron.h"

using namespace std;
using namespace CommonUtils;

class neuralNet
{
public:
	neuralNet();
	~neuralNet();
	void Create(int inputSize, int hiddenSize, int outSize, int width, int height);
	void Load(string fName);
	vector <float*> Update(vector <Vector3*> data);

	void clearNet() {
		analysisData.clear();
		analysisData.shrink_to_fit();

		inputs.clear();
		inputs.shrink_to_fit();

		hidden.clear();
		hidden.shrink_to_fit();

		outputs.clear();
		outputs.shrink_to_fit();

		outputData.clear();
		outputData.shrink_to_fit();
	}

	void train(int Expexted, int Actual);

	double RandFloat() { 		
		return (rand()) / (RAND_MAX + 1.0); }
	double RandomClamped() { return RandFloat() - RandFloat(); }

	//setters
	void setInputData(vector <Vector3*> data) { analysisData = data; }
	void increaseFitness(int id) { outputs[id]->incFitness(1); }
	void decreaseFitness(int id) { outputs[id]->incFitness(-1); }
	void setCreated(bool c) { Created = c; }

	//getters
	bool getCreated() {
		return Created;
	}
	bool Created;
private:
	
	vector <Vector3*> analysisData;
	vector <inputNeuron*> inputs;
	vector <hiddenNeuron*> hidden;
	vector <outputNeuron*> outputs;

	vector <float*>	outputData;
};

