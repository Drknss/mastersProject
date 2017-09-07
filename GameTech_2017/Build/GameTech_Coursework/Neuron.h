#pragma once
#include "..\nclgl\GameTimer.h"

#include <vector>

using namespace std;

class Neuron
{
public:
	Neuron();
	void calcResult();
	~Neuron();

	//setters, Potentially used for recreating a saved NN

	//getters, Potentially used for saving a NN
	int* getNeuronID() { return m_pneuronID; };
	bool getInputFlag() { return m_inputFlag; };
	bool getHiddenFlag() { return m_hiddenFlag; };
	bool getOutputFlag() { return m_outputFlag; };

private:

	bool m_inputFlag;
	bool m_hiddenFlag;
	bool m_outputFlag;

	float activationThreshold;

	int* m_pneuronID;

	vector <Neuron> input;
	vector <Neuron> output;
	vector <float> bias;
	//result
};

