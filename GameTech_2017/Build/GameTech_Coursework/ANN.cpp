#include "ANN.h"


//Randoms
double RandFloat() { return (rand()) / (RAND_MAX + 1.0); }
double RandomClamped() { return RandFloat() - RandFloat(); }

//Neuron's Method
SNeuron::SNeuron(int NumInputs) : m_NumInputs(NumInputs + 1) {
	//we need an additional weight for the bias hence the +1
	for (int i = 0; i<NumInputs + 1; ++i)
	{
		//set up the weights with an initial random value
		m_vecWeight.push_back(RandomClamped());
	}
}

//Neuron Layer's Method
SNeuronLayer::SNeuronLayer(int NumNeurons, int NumInputsPerNeuron) : m_NumNeurons(NumNeurons) {
	for (int i = 0; i<NumNeurons; ++i)
		m_vecNeurons.push_back(SNeuron(NumInputsPerNeuron));
}

// iNumIn = Resolution of Image, iNumOutputs = potentially always 3, turn left, right, forward, iNeuronsPerHidden = Secondary Resolution
ANN::ANN(int iNumInputs, int iNumOutputs,int iNeuronsPerHiddenLayer)
{
	m_NumInputs = iNumInputs;
	m_NumOutputs = iNumOutputs;
	m_NumHiddenLayers = 1;
	m_NeuronsPerHiddenLyr = iNeuronsPerHiddenLayer;

	CreateNet();
}


ANN::~ANN()
{
}

void ANN::CreateNet() {
	//create the layers of the network
	if (m_NumHiddenLayers > 0)
	{
		//create first hidden layer
		m_vecLayers.push_back(SNeuronLayer(m_NeuronsPerHiddenLyr, m_NumInputs));
		for (int i = 0; i < m_NumHiddenLayers - 1; ++i)
		{
			m_vecLayers.push_back(SNeuronLayer(m_NeuronsPerHiddenLyr, m_NeuronsPerHiddenLyr));
		}

		//create output layer
		m_vecLayers.push_back(SNeuronLayer(m_NumOutputs, m_NeuronsPerHiddenLyr));
	}

	else
	{
		//create output layer
		m_vecLayers.push_back(SNeuronLayer(m_NumOutputs, m_NumInputs));
	}
}

vector<double> ANN::GetWeights() const {
	//this will hold the weights
	vector<double> weights;

	//for each layer
	for (int i = 0; i<m_NumHiddenLayers + 1; ++i)
	{
		//for each neuron
		for (int j = 0; j<m_vecLayers[i].m_NumNeurons; ++j)
		{
			//for each weight
			for (int k = 0; k<m_vecLayers[i].m_vecNeurons[j].m_NumInputs; ++k)
			{
				weights.push_back(m_vecLayers[i].m_vecNeurons[j].m_vecWeight[k]);
			}
		}
	}

	return weights;
}

void ANN::PutWeights(vector<double> &weights) {
	int cWeight = 0;

	//for each layer
	for (int i = 0; i<m_NumHiddenLayers + 1; ++i)
	{
		//for each neuron
		for (int j = 0; j<m_vecLayers[i].m_NumNeurons; ++j)
		{
			//for each weight
			for (int k = 0; k<m_vecLayers[i].m_vecNeurons[j].m_NumInputs; ++k)
			{
				m_vecLayers[i].m_vecNeurons[j].m_vecWeight[k] = weights[cWeight++];
			}
		}
	}

	return;
}

int ANN::GetNumberOfWeights() const {

	int weights = 0;

	//for each layer
	for (int i = 0; i<m_NumHiddenLayers + 1; ++i)
	{
		//for each neuron
		for (int j = 0; j<m_vecLayers[i].m_NumNeurons; ++j)
		{
			//for each weight
			for (int k = 0; k<m_vecLayers[i].m_vecNeurons[j].m_NumInputs; ++k)
				weights++;
		}
	}

	return weights;
}

vector<double> ANN::Update(vector<double> &inputs)
{

	//stores the resultant outputs from each layer
	vector<double> outputs;
	int cWeight = 0;

	//first check that we have the correct amount of inputs
	if (inputs.size() != m_NumInputs)
	{
		//just return an empty vector if incorrect.
		return outputs;
	}

	//For each layer....
	for (int i = 0; i < m_NumHiddenLayers + 1; ++i)
	{
		if (i > 0)
		{
			inputs = outputs;
		}
		outputs.clear();
		cWeight = 0;

		//for each neuron sum the (inputs * corresponding weights).Throw
		//the total at the sigmoid function to get the output.
		for (int j = 0; j<m_vecLayers[i].m_NumNeurons; ++j)
		{
			double netinput = 0;
			int NumInputs = m_vecLayers[i].m_vecNeurons[j].m_NumInputs;

			//for each weight
			for (int k = 0; k<NumInputs - 1; ++k)
			{
				//sum the weights x inputs
				netinput += m_vecLayers[i].m_vecNeurons[j].m_vecWeight[k] *
					inputs[cWeight++];
			}

			//add in the bias
			netinput += m_vecLayers[i].m_vecNeurons[j].m_vecWeight[NumInputs - 1] *
				Bias;

			//we can store the outputs from each layer as we generate them.
			//The combined activation is first filtered through the sigmoid
			//function

			outputs.push_back(Sigmoid(netinput, ActivationResponse));

			cWeight = 0;

		}
	}

	return outputs;

}

double ANN::Sigmoid(double netinput, double response)
{
	return (1 / (1 + exp(-netinput / response)));
}
