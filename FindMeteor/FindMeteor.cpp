// FindMeteor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "utils.h"
#include "Data/MeteorData.h"
#include "AppAlgoITF.h"
#include "./Algorithm/MetDetectionAlgo.h"


using namespace FindMeteor;

const char* CmdKey =//C:/Users/840/source/repos/FindMeteor/Assets/data/20201128/2020_08_14__01_07_37.jpg
"{@IMAGE_PATH  ||input image path}"
"{h help       ||show the help message}"
"{o out        |o|output images of every stage}"
"{e error      ||show error message}"
"{t test       ||run program for testing}";


void LoadInputImg(CMeteorData& data) {
	data.srcLoadImg = imread(data.srcFilePath, CV_LOAD_IMAGE_COLOR);
	if (data.srcLoadImg.empty())
		throw "Error while reading image";

	data.srcFileNameT = experimental::filesystem::path(data.srcFilePath).filename().string();
	data.srcFileName = data.srcFileNameT.substr(0, data.srcFileNameT.size() - 4);

	data.filename = data.srcFilePath.substr(0, data.srcFilePath.rfind("/") );


	if (data.m_needOutput)
	{
		data.dataSubPath = data.filename.substr(data.filename.rfind("/") + 1, data.filename.length());
		replace(data.dataSubPath.begin(), data.dataSubPath.end(), '/', '_');


		// Generate output folders
		CreateFolder("dump");
		CreateFolder("dump/" + data.dataSubPath);



	}
}

int main(int argc, const char** argv)
{
	CMeteorData meteorData;

	CommandLineParser *cmdParser = new CommandLineParser(argc, argv, CmdKey);

	///*== cmd parser ==*///
	if (cmdParser->has("help"))
	{
		// If input have "-h" or no image path, terminate program
		cmdParser->printMessage();
		return 0;
	}
	if (!cmdParser->has("@IMAGE_PATH"))
	{
		printf("No input image path\n");
		return -1;
	}
	else {
		meteorData.srcFilePath = cmdParser->get<string>("@IMAGE_PATH");
	}



	meteorData.m_isTesting = cmdParser->has("test");
	meteorData.m_needOutput = cmdParser->has("out");


	try
	{
		///*== Load input Image ==*///
		LoadInputImg(meteorData);

	
		///*== Initialize the algorithmic component of the framework ==*///
		vector<IAppAlgoITF*> algoPipeline;
		algoPipeline.push_back(new CMetDetection());

		clock_t time0 = clock();														///record start time

		string result = "";
		if (algoPipeline[0]->Execute(meteorData))
			result = "true";
		else
			result = "false";
		clock_t time1 = clock();														///record start time

		double detectTime = double(time1 - time0) / (double)CLOCKS_PER_SEC;
		cout << detectTime << "," << result << ",";

	}
	catch (const char str[])
	{
	
		cout << str;
	}
	catch (...)
	{
		cout << "Exception Error.";
	}

	return 0;
}



