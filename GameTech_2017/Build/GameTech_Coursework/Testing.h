#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <iterator>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

	int LoadBmp(const string filepath);

	void Training(float fps, float ttc);
	void TestingNN(float fps, float ttc);

	void updateDataToFile(float pCent, float fps, float ttc);

	//Setters

	void setTraining(bool t) { ctraining = t; }
	void setSubRes(int r) { nnHidenRes = r; }

	
	double approxRollingAverage(double avg, double input) {
		avg -= avg / 100;
		avg += input / 100;
		return avg;
	}


	bool createdNN;
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

	float epoch;
	float cGuess;
	float eCount;
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

	

	ofstream* filestr;
	neuralNet net;
};


