#pragma once

#include "..\nclgl\GameTimer.h"

#include <vector>
#include <fstream>
#include <math.h>

using namespace std;

struct SNeuron
{
	//the number of inputs into the neuron
	int m_NumInputs;

	//the weights for each input
	vector<double> m_vecWeight;

	//ctor
	SNeuron(int NumInputs);

};

struct SNeuronLayer
{
	//the number of neurons in this layer
	int m_NumNeurons;

	//the layer of neurons
	vector<SNeuron> m_vecNeurons;

	SNeuronLayer(int NumNeurons, int NumInputsPerNeuron);
};

class ANN
{
public:

	ANN() {};
	ANN(int iNumInputs, int iNumOutputs, int iNeuronsPerHiddenLayer);
	~ANN();

	void CreateNet();
	//gets the weights from the NN
	vector<double> GetWeights()const;

	//returns the total number of weights in the net
	int GetNumberOfWeights()const;

	//replaces the weights with new ones
	void PutWeights(vector<double> &weights);

	//calculates the outputs from a set of inputs
	vector<double> Update(vector<double> &inputs);

	//sigmoid response curve
	inline double Sigmoid(double activation, double response);

private:

	int m_NumInputs;
	int m_NumOutputs;
	int m_NumHiddenLayers;
	int m_NeuronsPerHiddenLyr;

	double Bias;
	double ActivationResponse;

	//storage for each layer of neurons including the output layer
	vector<SNeuronLayer> m_vecLayers;
};

