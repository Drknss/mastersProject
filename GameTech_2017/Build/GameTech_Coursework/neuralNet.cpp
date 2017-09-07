#include "neuralNet.h"



neuralNet::neuralNet()
{
	Created = false;
}


neuralNet::~neuralNet()
{
}

void neuralNet::Create(int inputSize, int hiddenSize, int outSize, int width, int height) {

	for (int i = 0; i < inputSize; i++)
	{
		inputs.push_back(new inputNeuron());
	}

	for (int j = 0; j < hiddenSize*hiddenSize; j++)
	{
		hidden.push_back(new hiddenNeuron());
	}
	float wPart = (float)width / (float)hiddenSize;
	int wNoR = wPart;
	float wRemain = wPart - (float)wNoR;
	float hPart = (float)height / (float)hiddenSize;
	int hNoR = hPart;
	float hRemain = hPart - (float)hNoR;
	int iCount = 0;

	for (int i = 0; i < hiddenSize; i++)
	{
		for (float h = 0; h < hPart; h++)
		{
			for (int j = 0; j < hiddenSize; j++)
			{			
				for (float w = 0; w < wPart; w++)
				{
					hidden[j+(i*hiddenSize)]->addInput(inputs[iCount]);
					iCount++;
				}
			}
		}
	}

	if (iCount < inputSize) {
		cout << "Error, Too few Connections made to Hidden Nodes" << endl;
		cout << inputSize << endl;
		cout << iCount << endl;
	}

	if (iCount > inputSize) {
		cout << "Error, Too many Connections made to Hidden Nodes" << endl;
		cout << inputSize << endl;
		cout << iCount << endl;
	}

	for (int k = 0; k < outSize; k++)
	{
		outputs.push_back(new outputNeuron());
	}

	for each (hiddenNeuron *h in hidden)
	{
		for each (outputNeuron *o in outputs)
		{
			h->addOutput(o);
			o->addInput(h);
		}		
	}

	Created = true;
}

void neuralNet::Load(string fName) {

}

vector <float*> neuralNet::Update(vector <Vector3*> data)
{
	outputData.clear();
	if (data.size() != inputs.size()) {
		cout << "Incorrect Data size compared to Inputs Size" << endl;
		return outputData;
	}

	int count = 0;

	for each (inputNeuron *n in inputs)
	{
		n->setPixelValues(*data[count]);
	}
	for each (hiddenNeuron *n in hidden)
	{
		n->update();
	}
	for each (outputNeuron *n in outputs)
	{
		n->update();
		float t = n->getOOutput();
		outputData.push_back(&t);
	}
	return outputData;
}

void neuralNet::train(int Expexted, int Actual)
{
	for (int i = 0; i < outputs[Expexted]->getInputs().size();  i++)
	{
		//randomize weight positively by a small random, divided by fitness.
		//if the neurons right allot more than its wrong we only want to change it a very small amount.
		float sr = -1 * (abs(RandomClamped() / outputs[Expexted]->getFitness()));
		outputs[Expexted]->modifyWeight(sr, i);
	}

	for (int i = 0; i < outputs[Actual]->getInputs().size(); i++)
	{
		float sr = (abs(RandomClamped() / outputs[Actual]->getFitness()));
		outputs[Actual]->modifyWeight(sr, i);
	}
}
