#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <iterator>
#include <time.h>

#include "neuralNet.h"

#include <ncltech\NCLDebug.h>

using namespace std;

enum shape {
	triangle,
	circle,
	square
};

class Testing
{
public:
	Testing();
	Testing(bool training, int subres);
	Testing(string fName);
	~Testing();
	void Initialize();
	void Initialize(string fName);
	void Update(int iterations, int itCom, float fps, float ttc);
	void readBMP(const string &file);

	void Training(float fps, float ttc);
	void TestingNN(float fps, float ttc);

	void updateDataToFile(float pCent, float fps, float ttc);

	//Setters

	void setTraining(bool t) { ctraining = t; }
	void setSubRes(int r) { nnHidenRes = r; }

private:

	bool ctraining;

	int nnInputs;
	int imgWidth;
	int imgHeight;
	int noIterationsToDo;
	int noIterationsComp;

	int imageRes;
	int nnHidenRes;
	int completedCycles;

	int epoch;
	int cGuess;
	int eCount;
	float accPCent;

	shape presented;
	shape guessed;

	string currentFile;

	vector<Vector3*> inputData;

	vector <string> TestingTriangles;
	vector <string> TrainingTriangles;
	vector <string> TestingSquares;
	vector <string> TrainingSquares;
	vector <string> TestingCircles;
	vector <string> TrainingCircles;

	neuralNet* net;

	ofstream* filestr;

};

