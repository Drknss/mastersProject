#include "Testing.h"


Testing::Testing()
{
	srand(time(NULL));
}


//For Creating a new NN;
Testing::Testing(bool training, int subres)
{
	ctraining = true;
	nnHidenRes = subres;
	Initialize();
	createdNN = false;
}

//For Loading an NN
Testing::Testing(string fName)
{
	Initialize(fName);
}


Testing::~Testing()
{
	inputData.clear();
	delete &inputData;
	TestingTriangles.clear();
	delete &TestingTriangles;
	TestingTriangles.clear();
	delete &TrainingTriangles;
	TestingCircles.clear();
	delete &TestingSquares;
	TrainingSquares.clear();
	delete &TrainingSquares;
	TestingCircles.clear();
	delete &TestingCircles;
	TrainingCircles.clear();
	delete &TrainingCircles;
}

//Training of NN's
void Testing::Initialize()
{
#pragma region 	TestingTriangles

	TestingTriangles.push_back("Triangle3.bmp");
	TestingTriangles.push_back("Triangle6.bmp");
	TestingTriangles.push_back("Triangle9.bmp");
	TestingTriangles.push_back("Triangle12.bmp");
	TestingTriangles.push_back("Triangle15.bmp");

#pragma endregion TestingTriangles

#pragma region TrainingTriangles

	TrainingTriangles.push_back("Triangle.bmp");
	TrainingTriangles.push_back("Triangle1.bmp");
	TrainingTriangles.push_back("Triangle2.bmp");
	TrainingTriangles.push_back("Triangle3.bmp");
	TrainingTriangles.push_back("Triangle4.bmp");
	TrainingTriangles.push_back("Triangle5.bmp");
	TrainingTriangles.push_back("Triangle6.bmp");
	TrainingTriangles.push_back("Triangle7.bmp");
	TrainingTriangles.push_back("Triangle8.bmp");
	TrainingTriangles.push_back("Triangle9.bmp");
	TrainingTriangles.push_back("Triangle10.bmp");
	TrainingTriangles.push_back("Triangle11.bmp");
	TrainingTriangles.push_back("Triangle12.bmp");
	TrainingTriangles.push_back("Triangle13.bmp");
	TrainingTriangles.push_back("Triangle14.bmp");
	TrainingTriangles.push_back("Triangle15.bmp");
	TrainingTriangles.push_back("Triangle16.bmp");
	TrainingTriangles.push_back("Triangle17.bmp");
	TrainingTriangles.push_back("Triangle18.bmp");

#pragma endregion TrainingTriangles

#pragma region TestingSquares

	TestingSquares.push_back("Square3.bmp");
	TestingSquares.push_back("Square6.bmp");
	TestingSquares.push_back("Square9.bmp");
	TestingSquares.push_back("Square12.bmp");
	TestingSquares.push_back("Square15.bmp");

#pragma endregion TestingSquares

#pragma region TrainingSquares

	TrainingSquares.push_back("Square.bmp");
	TrainingSquares.push_back("Square1.bmp");
	TrainingSquares.push_back("Square2.bmp");
	TrainingSquares.push_back("Square3.bmp");
	TrainingSquares.push_back("Square4.bmp");
	TrainingSquares.push_back("Square5.bmp");
	TrainingSquares.push_back("Square6.bmp");
	TrainingSquares.push_back("Square7.bmp");
	TrainingSquares.push_back("Square8.bmp");
	TrainingSquares.push_back("Square9.bmp");
	TrainingSquares.push_back("Square10.bmp");
	TrainingSquares.push_back("Square11.bmp");
	TrainingSquares.push_back("Square12.bmp");
	TrainingSquares.push_back("Square13.bmp");
	TrainingSquares.push_back("Square14.bmp");
	TrainingSquares.push_back("Square15.bmp");
	TrainingSquares.push_back("Square16.bmp");
	TrainingSquares.push_back("Square17.bmp");
	TrainingSquares.push_back("Square18.bmp");

#pragma endregion TrainingSquares

#pragma region TestingCircles

	TestingCircles.push_back("Circle3.bmp");
	TestingCircles.push_back("Circle6.bmp");
	TestingCircles.push_back("Circle9.bmp");
	TestingCircles.push_back("Circle12.bmp");
	TestingCircles.push_back("Circle15.bmp");

#pragma endregion TestingCircles

#pragma region TrainingCircles

	TrainingCircles.push_back("Circle.bmp");
	TrainingCircles.push_back("Circle1.bmp");
	TrainingCircles.push_back("Circle2.bmp");
	TrainingCircles.push_back("Circle3.bmp");
	TrainingCircles.push_back("Circle4.bmp");
	TrainingCircles.push_back("Circle5.bmp");
	TrainingCircles.push_back("Circle6.bmp");
	TrainingCircles.push_back("Circle7.bmp");
	TrainingCircles.push_back("Circle8.bmp");
	TrainingCircles.push_back("Circle9.bmp");
	TrainingCircles.push_back("Circle10.bmp");
	TrainingCircles.push_back("Circle11.bmp");
	TrainingCircles.push_back("Circle12.bmp");
	TrainingCircles.push_back("Circle13.bmp");
	TrainingCircles.push_back("Circle14.bmp");
	TrainingCircles.push_back("Circle15.bmp");
	TrainingCircles.push_back("Circle16.bmp");
	TrainingCircles.push_back("Circle17.bmp");
	TrainingCircles.push_back("Circle18.bmp");

#pragma endregion TrainingCircles

	epoch = 100.f;
	cGuess = 0.f;
	eCount = 0.f;

}

//If We got file loading of pre built NN's finished
void Testing::Initialize(string fName)
{
#pragma region 	TestingTriangles

	TestingTriangles.push_back("Triangle3.bmp");
	TestingTriangles.push_back("Triangle6.bmp");
	TestingTriangles.push_back("Triangle9.bmp");
	TestingTriangles.push_back("Triangle12.bmp");
	TestingTriangles.push_back("Triangle15.bmp");

#pragma endregion TestingTriangles

#pragma region TrainingTriangles

	TrainingTriangles.push_back("Triangle.bmp");
	TrainingTriangles.push_back("Triangle1.bmp");
	TrainingTriangles.push_back("Triangle2.bmp");
	TrainingTriangles.push_back("Triangle3.bmp");
	TrainingTriangles.push_back("Triangle4.bmp");
	TrainingTriangles.push_back("Triangle5.bmp");
	TrainingTriangles.push_back("Triangle6.bmp");
	TrainingTriangles.push_back("Triangle7.bmp");
	TrainingTriangles.push_back("Triangle8.bmp");
	TrainingTriangles.push_back("Triangle9.bmp");
	TrainingTriangles.push_back("Triangle10.bmp");
	TrainingTriangles.push_back("Triangle11.bmp");
	TrainingTriangles.push_back("Triangle12.bmp");
	TrainingTriangles.push_back("Triangle13.bmp");
	TrainingTriangles.push_back("Triangle14.bmp");
	TrainingTriangles.push_back("Triangle15.bmp");
	TrainingTriangles.push_back("Triangle16.bmp");
	TrainingTriangles.push_back("Triangle17.bmp");
	TrainingTriangles.push_back("Triangle18.bmp");

#pragma endregion TrainingTriangles

#pragma region TestingSquares

	TestingSquares.push_back("Square3.bmp");
	TestingSquares.push_back("Square6.bmp");
	TestingSquares.push_back("Square9.bmp");
	TestingSquares.push_back("Square12.bmp");
	TestingSquares.push_back("Square15.bmp");

#pragma endregion TestingSquares

#pragma region TrainingSquares

	TrainingSquares.push_back("Square.bmp");
	TrainingSquares.push_back("Square1.bmp");
	TrainingSquares.push_back("Square2.bmp");
	TrainingSquares.push_back("Square3.bmp");
	TrainingSquares.push_back("Square4.bmp");
	TrainingSquares.push_back("Square5.bmp");
	TrainingSquares.push_back("Square6.bmp");
	TrainingSquares.push_back("Square7.bmp");
	TrainingSquares.push_back("Square8.bmp");
	TrainingSquares.push_back("Square9.bmp");
	TrainingSquares.push_back("Square10.bmp");
	TrainingSquares.push_back("Square11.bmp");
	TrainingSquares.push_back("Square12.bmp");
	TrainingSquares.push_back("Square13.bmp");
	TrainingSquares.push_back("Square14.bmp");
	TrainingSquares.push_back("Square15.bmp");
	TrainingSquares.push_back("Square16.bmp");
	TrainingSquares.push_back("Square17.bmp");
	TrainingSquares.push_back("Square18.bmp");

#pragma endregion TrainingSquares

#pragma region TestingCircles

	TestingCircles.push_back("Circle3.bmp");
	TestingCircles.push_back("Circle6.bmp");
	TestingCircles.push_back("Circle9.bmp");
	TestingCircles.push_back("Circle12.bmp");
	TestingCircles.push_back("Circle15.bmp");

#pragma endregion TestingCircles

#pragma region TrainingCircles

	TrainingCircles.push_back("Circle.bmp");
	TrainingCircles.push_back("Circle1.bmp");
	TrainingCircles.push_back("Circle2.bmp");
	TrainingCircles.push_back("Circle3.bmp");
	TrainingCircles.push_back("Circle4.bmp");
	TrainingCircles.push_back("Circle5.bmp");
	TrainingCircles.push_back("Circle6.bmp");
	TrainingCircles.push_back("Circle7.bmp");
	TrainingCircles.push_back("Circle8.bmp");
	TrainingCircles.push_back("Circle9.bmp");
	TrainingCircles.push_back("Circle10.bmp");
	TrainingCircles.push_back("Circle11.bmp");
	TrainingCircles.push_back("Circle12.bmp");
	TrainingCircles.push_back("Circle13.bmp");
	TrainingCircles.push_back("Circle14.bmp");
	TrainingCircles.push_back("Circle15.bmp");
	TrainingCircles.push_back("Circle16.bmp");
	TrainingCircles.push_back("Circle17.bmp");
	TrainingCircles.push_back("Circle18.bmp");

#pragma endregion TrainingCircles

	epoch = 100.f;
	cGuess = 0.f;
	eCount = 0.f;

	net.Load(fName);
}

void Testing::Update(int iterations, int itCom, float fps, float ttc) {
	noIterationsToDo = itCom;
	noIterationsComp = iterations;
	if (ctraining)
	{
		Training( fps,  ttc);
	}
	else {
		TestingNN( fps,  ttc);
	}
}

int Testing::LoadBmp(const string fName)
{
	
	inputData.clear();
	inputData.shrink_to_fit();

	uint8_t* datBuff[2] = {nullptr, nullptr}; // Header buffers

	uint8_t* pixels = nullptr; // Pixels

	BITMAPFILEHEADER* bmpHeader = nullptr; // Header
	BITMAPINFOHEADER* bmpInfo   = nullptr; // Info

	// The file... We open it with it's constructor
	ifstream file(fName, std::ios::binary);
	if (!file)
	{
		std::cout << "Failure to open bitmap file.\n";
		return 1;
	}

	// Allocate byte memory that will hold the two headers
	datBuff[0] = new uint8_t[sizeof(BITMAPFILEHEADER)];
	datBuff[1] = new uint8_t[sizeof(BITMAPINFOHEADER)];

	file.read((char*)datBuff[0], sizeof(BITMAPFILEHEADER));
	file.read((char*)datBuff[1], sizeof(BITMAPINFOHEADER));

	// Construct the values from the buffers
	bmpHeader = (BITMAPFILEHEADER*)datBuff[0];
	bmpInfo = (BITMAPINFOHEADER*)datBuff[1];

	// Check if the file is an actual BMP file
	if (bmpHeader->bfType != 0x4D42)
	{
		std::cout << "File \"" << fName << "\" isn't a bitmap file\n";
		return 2;
	}

	// First allocate pixel memory
	pixels = new uint8_t[bmpInfo->biSizeImage];

	// Go to where image data starts, then read in image data
	file.seekg(bmpHeader->bfOffBits);
	file.read((char*)pixels, bmpInfo->biSizeImage);

	uint8_t tmpRGB = 0; // Swap buffer
	for (unsigned long i = 0; i < bmpInfo->biSizeImage; i += 3)
	{
		tmpRGB = pixels[i];
		pixels[i] = pixels[i + 2];
		pixels[i + 2] = tmpRGB;
		inputData.push_back(&Vector3(int(pixels[i] & 0xff), int(pixels[i + 1] & 0xff), int(pixels[i + 2] & 0xff)));
	}

	// Set width and height to the values loaded from the file
	imgWidth = bmpInfo->biWidth;
	imgHeight = bmpInfo->biHeight;
	nnInputs = bmpInfo->biHeight * bmpInfo->biWidth;

	file.clear();
	file.close();

	return 0;
}

void Testing::readBMP(const string &file)
{
	
	inputData.clear();
	inputData.shrink_to_fit();

	static constexpr size_t HEADER_SIZE = 54;

	ifstream bmp(file, ios::binary);

	array<char, HEADER_SIZE> header;
	bmp.read(header.data(), header.size());

	auto fileSize = *reinterpret_cast<uint32_t *>(&header[2]);
	auto dataOffset = *reinterpret_cast<uint32_t *>(&header[10]);
	auto width = *reinterpret_cast<uint32_t *>(&header[18]);
	auto height = *reinterpret_cast<uint32_t *>(&header[22]);
	auto depth = *reinterpret_cast<uint16_t *>(&header[28]);

	nnInputs = width*height;
	imgWidth = width;
	imgHeight = height;

	//Testing Readouts
	cout << "fileSize: " << fileSize << endl;
	cout << "dataOffset: " << dataOffset << endl;
	cout << "width: " << width << endl;
	cout << "height: " << height << endl;
	cout << "depth: " << depth << "-bit" << endl;

	vector<char> img(dataOffset - HEADER_SIZE);
	bmp.read(img.data(), img.size());

	auto dataSize = ((width * 3 + 3) & (~3)) * height;
	img.resize(dataSize);
	bmp.read(img.data(), img.size());

	char temp = 0;

	for (auto i = dataSize - 4; i >= 0; i -= 3)
	{
		temp = img[i];
		img[i] = img[i + 2];
		img[i + 2] = temp;

		//Testing Pixel Colour Readouts;
		//cout << "R: " << int(img[i] & 0xff) << " G: " << int(img[i + 1] & 0xff) << " B: " << int(img[i + 2] & 0xff) << endl;
		inputData.push_back(&Vector3(int(img[i] & 0xff), int(img[i + 1] & 0xff), int(img[i + 2] & 0xff)));
	}

	bmp.clear();
	bmp.close();
}

void Testing::Training(float fps, float ttc) {
	int v1 = rand() % 3;
	int v2;
	switch (v1)
	{
	case 0:
		v2 = rand() % TrainingTriangles.size();
		currentFile = TrainingTriangles[v2];
		break;
	case 1:
		v2 = rand() % TrainingCircles.size();
		currentFile = TrainingCircles[v2];
		break;
	case 2:
		v2 = rand() % TrainingSquares.size();
		currentFile = TrainingSquares[v2];
		break;
	default:
		break;
	}

	presented = (shape)v1;

	LoadBmp(currentFile);

	//readBMP(currentFile);

	//bool t = net->getCreated();

	if (!net.getCreated()) {
		net.clearNet();
		net.Create((imgHeight * imgWidth), nnHidenRes, 3, imgWidth, imgHeight);
	}

	vector <float*> temp;
	temp.swap(net.Update(inputData));
	float temp1 = *temp[0];
	float temp2 = *temp[1];
	float temp3 = *temp[2];

	if (temp1 > temp2 && temp1 > temp3) {
		guessed = triangle;
	}
	else if (temp2 > temp1 && temp2 > temp3) {
		guessed = circle;
	}
	else if (temp3 > temp2 && temp3 > temp1) {
		guessed = square;
	}
	else {
	guessed = (shape)(rand() % 3);
	}

	if (guessed != presented) {
		net.train((int)presented, (int)guessed);
	}
	else {
		cGuess++;
		net.increaseFitness((int)guessed);
	}

	temp.clear();

	eCount++;
	accPCent = (approxRollingAverage(accPCent, cGuess / eCount));

	if (eCount >= epoch)
	{
		updateDataToFile(accPCent, fps, ttc);
		if ( accPCent >= 0.96f) {
				ctraining = false;
		}		
		cGuess = 0;
		eCount = 0;
	}
}

void Testing::TestingNN(float fps, float ttc) {
	int v1 = rand() % 3;
	int v2;
	switch (v1)
	{
	case 0:
		v2 = rand() % TestingTriangles.size();
		currentFile = TestingTriangles[v2];
		break;
	case 1:
		v2 = rand() % TestingCircles.size();
		currentFile = TestingCircles[v2];
		break;
	case 2:
		v2 = rand() % TestingSquares.size();
		currentFile = TestingSquares[v2];
		break;
	default:
		break;
	}

	presented = (shape)v1;

	LoadBmp(currentFile);

	//readBMP(currentFile);

	if (!net.getCreated()) {
		net.clearNet();
		net.Create((imgHeight * imgWidth), nnHidenRes, 3, imgWidth, imgHeight);
	}

	vector <float*> temp;
	temp = net.Update(inputData);
	float temp1 = *temp[0];
	float temp2 = *temp[1];
	float temp3 = *temp[2];

	if (temp1 > temp2 && temp1 > temp3) {
		guessed = triangle;
	}
	else if (temp2 > temp1 && temp2 > temp3) {
		guessed = circle;
	}
	else if (temp3 > temp2 && temp3 > temp1) {
		guessed = square;
	}
	else {
		guessed = (shape)(rand() % 3);
	}
	
	temp.clear();
	eCount++;

	if (eCount >= epoch)
	{
		updateDataToFile(accPCent, fps, ttc);
		eCount = 0;
	}


}

void Testing::updateDataToFile(float pCent, float fps, float ttc) {
	//FName Format - Number Of Iterations Ran For - Hidden Neuron Res
	ofstream log(to_string(noIterationsComp) + "-" + to_string(nnHidenRes) + ".txt", std::ios_base::app | std::ios_base::out);
	log << "Currently Training? = " + to_string(ctraining) + " Current No. It = " + to_string(noIterationsToDo+1)
		+ " Current FPS Avg = " + to_string(fps) + " Current Time To Compute = "
		+ to_string(ttc) + "ms Current Percentage Acc = " + to_string((pCent*100)) + "\n";
	
	NCLDebug::Log("Currently Training? = " + to_string(ctraining));
	NCLDebug::Log("Current No.It = " + to_string(noIterationsToDo+1));
	NCLDebug::Log("Current FPS Avg = " + to_string(fps));
	NCLDebug::Log("Current Time To Compute = " + to_string(ttc) + "ms");
	NCLDebug::Log("Current Percentage Acc = " + to_string(pCent));
}