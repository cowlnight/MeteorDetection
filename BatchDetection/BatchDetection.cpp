// BatchDecoder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
using namespace std;
using namespace cv;


const char* CmdKey =
"{@PATH        ||input image path}"
"{h help       ||show the help message}";


struct ExecCommandException :public exception
{
	const char * what() const throw ()
	{
		return "Unknown exception raised from ExecCommand()";
	}
};


// Execute the command and return result string
string ExecCommand(const char* cmd)
{
	char buffer[128];
	string result = "";
	FILE* pipe = _popen(cmd, "r");
	if (!pipe) throw runtime_error("popen() failed!");
	try
	{
		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}
	}
	catch (exception& e)
	{
		_pclose(pipe);
		throw e;
	}
	catch (...)
	{
		throw ExecCommandException();
	}
	_pclose(pipe);
	return result;
}

int getdir(string dir, vector<string> &files)
{

	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL)

		files.push_back(string(dirp->d_name));


	closedir(dp);
	return 0;
}

void CreateFolder(string folderName)
{
	if (_access(folderName.c_str(), 0)) CreateDirectoryA(folderName.c_str(), NULL);
}


int main(int argc, const char** argv)
{
	CommandLineParser *cmdParser = new CommandLineParser(argc, argv, CmdKey);

	if (cmdParser->has("help"))
	{
		// If input have "-h" or no image path, terminate program
		cmdParser->printMessage();
		return 0;
	}



	string dataDir;
	vector<string> dataFolders = vector<string>();



	char buff[FILENAME_MAX];
	_getcwd(buff, FILENAME_MAX);
	string sbuff = string(buff);
	replace(sbuff.begin(), sbuff.end(), '\\', '/');
	dataDir = sbuff + string("/data/");
	getdir(dataDir, dataFolders);



	CreateFolder("result");
	string resultFileName = "result/result_";

	time_t nowTime;
	tm gmt;
	char timeStr[80];

	time(&nowTime);
	localtime_s(&gmt, &nowTime);
	mktime(&gmt);

	strftime(timeStr, sizeof(timeStr), "%m%d%H%M", &gmt);


	string programName;
	programName = "FindMeteor.exe";


	// Open file for writing log TODO
	for (int i = 2; i < dataFolders.size(); i++)
	{


		ofstream resultFile;
		resultFile.open(resultFileName + dataFolders[i] + "_" + timeStr + ".csv", ios::app);


		// head
		resultFile << "File Name,Time,Message";
		resultFile << endl;



		vector<string> files = vector<string>();
		getdir(dataDir + dataFolders[i], files);

		for (int j = 2; j < files.size(); j++)
		{
			string files_type = files[j].substr(files[j].rfind("."));
			std::transform(files_type.begin(), files_type.end(), files_type.begin(), [](unsigned char c) { return std::tolower(c); });
			if (files_type == ".jpg" || files_type == ".tif") {  //  load image file

				string imageName;

				imageName = dataDir + dataFolders[i] + "/" + files[j];

				string cmd = programName + " -IMAGE_PATH " + imageName;
				cout << "Detecting " << imageName << endl;



				resultFile << files[j].substr(0, files[j].size() - 4) << ",";

				try
				{
					string outMsg = ExecCommand(cmd.c_str());
					resultFile << outMsg;

					// whether decoded failed or not
					outMsg = outMsg.substr(outMsg.find(",") + 1, outMsg.rfind(",") - outMsg.find(",") - 1);

				}
				catch (exception& e)
				{
					cerr << "[" << e.what() << "] " << endl;
					cerr << "Executed command: " << cmd << endl << endl;
				}
				catch (...)
				{
					cerr << "[ Unknow error on main() ] " << endl;
					cerr << "Executed command: " << cmd << endl << endl;
				}

				resultFile << endl;
			}
		}
		resultFile.close();
	}



	cout << "Detection Complete" << endl;
	system("pause");
	return 0;
}