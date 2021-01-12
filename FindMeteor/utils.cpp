<<<<<<< HEAD
#include "pch.h"
#include "utils.h"
#include "Data/MeteorData.h"

using namespace std;
using namespace cv;
using namespace FindMeteor;

void CreateFolder(string folderName)
{
	if (_access(folderName.c_str(), 0)) CreateDirectoryA(folderName.c_str(), NULL);
}

string ExecCommand(string cmd)
{
	char buffer[128];
	string result = "";
	FILE* pipe = _popen(cmd.c_str(), "r");
	if (!pipe)
	{
		_pclose(pipe);
		throw runtime_error("popen() failed!");
	}
	try
	{
		while (!feof(pipe))
		{
			if (fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}
	}
	catch (...)
	{
		_pclose(pipe);
		throw;
	}
	_pclose(pipe);

	return result;
}

void Binarization(cv::Mat & img, cv::Mat & dst, int blockSize, int dynamicRange, int slidingWindow)
{
	if (img.type() == CV_8UC3)
		cvtColor(img, img, COLOR_BGR2GRAY);
	int bpHeight = (int)ceil(img.rows / (double)blockSize);
	int bpWidth = (int)ceil(img.cols / (double)blockSize);
	Mat bpMat = GetBlackPointMat(img, blockSize, dynamicRange);

	img.copyTo(dst);

	int winBlock = (slidingWindow - 1) / 2;

	for (int i = 0; i < bpHeight; i++)
	{
		for (int j = 0; j < bpWidth; j++)
		{
			// Black point block mat
			Range bpRowRange = Range(max(i - winBlock, 0), min(i + winBlock, bpHeight));
			Range bpColRange = Range(max(j - winBlock, 0), min(j + winBlock, bpWidth));
			// Pixel average of the black point block
			double avgBP = mean(bpMat(bpRowRange, bpColRange))[0];
			// Image block mat
			Range rowRange = Range(i * blockSize, min((i + 1) * blockSize, dst.rows));
			Range colRange = Range(j * blockSize, min((j + 1) * blockSize, dst.cols));
			Mat subMat = dst(rowRange, colRange);

			threshold(subMat, subMat, avgBP, 255, THRESH_BINARY);
		}
	}
}

Mat GetBlackPointMat(Mat img, int blockSize, int dynamicRange)
{
	int bpHeight = (int)ceil(img.rows / (double)blockSize);
	int bpWidth = (int)ceil(img.cols / (double)blockSize);
	Mat bpMat(bpHeight, bpWidth, CV_8U);

	for (int i = 0; i < img.rows; i += blockSize)
	{
		for (int j = 0; j < img.cols; j += blockSize)
		{
			// Get the black point block mat
			Range rowRange = Range(i, min(i + blockSize, img.rows));
			Range colRange = Range(j, min(j + blockSize, img.cols));
			Mat subMat = img(rowRange, colRange);
			// Get the average pixel value in subMat
			double avgPixel = mean(subMat)[0];
			double minPixel, maxPixel;

			// Get maximum and minimum pixel in subMat
			minMaxLoc(subMat, &minPixel, &maxPixel);

			// Check if the pixel gap between the maximum value and the minimum value is too small
			if (maxPixel - minPixel <= dynamicRange)
			{
				avgPixel = minPixel / 2;

				// If the Block is not located in the left / upper boundary,
				// use the average value of the adjacent Block as the value of this Block
				if (i > 0 && j > 0)
				{
					uchar avgNeighorBP = (uchar)((bpMat.at<uchar>((int)floor((double)i / blockSize) - 1, (int)floor((double)j / blockSize))
						+ bpMat.at<uchar>((int)floor((double)i / blockSize), (int)floor((double)j / blockSize) - 1) * 2
						+ bpMat.at<uchar>((int)floor((double)i / blockSize) - 1, (int)floor((double)j / blockSize) - 1)) / 4.0);
					if (minPixel < avgNeighorBP)
						avgPixel = avgNeighorBP;
				}
			}
			bpMat.at<uchar>((int)floor(i / blockSize), (int)floor(j / blockSize)) = (uchar)avgPixel;
		}
	}
	return bpMat;
}

=======
#include "pch.h"
#include "utils.h"
#include "Data/MeteorData.h"

using namespace std;
using namespace cv;
using namespace FindMeteor;

void CreateFolder(string folderName)
{
	if (_access(folderName.c_str(), 0)) CreateDirectoryA(folderName.c_str(), NULL);
}

string ExecCommand(string cmd)
{
	char buffer[128];
	string result = "";
	FILE* pipe = _popen(cmd.c_str(), "r");
	if (!pipe)
	{
		_pclose(pipe);
		throw runtime_error("popen() failed!");
	}
	try
	{
		while (!feof(pipe))
		{
			if (fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}
	}
	catch (...)
	{
		_pclose(pipe);
		throw;
	}
	_pclose(pipe);

	return result;
}

void Binarization(cv::Mat & img, cv::Mat & dst, int blockSize, int dynamicRange, int slidingWindow)
{
	if (img.type() == CV_8UC3)
		cvtColor(img, img, COLOR_BGR2GRAY);
	int bpHeight = (int)ceil(img.rows / (double)blockSize);
	int bpWidth = (int)ceil(img.cols / (double)blockSize);
	Mat bpMat = GetBlackPointMat(img, blockSize, dynamicRange);

	img.copyTo(dst);

	int winBlock = (slidingWindow - 1) / 2;

	for (int i = 0; i < bpHeight; i++)
	{
		for (int j = 0; j < bpWidth; j++)
		{
			// Black point block mat
			Range bpRowRange = Range(max(i - winBlock, 0), min(i + winBlock, bpHeight));
			Range bpColRange = Range(max(j - winBlock, 0), min(j + winBlock, bpWidth));
			// Pixel average of the black point block
			double avgBP = mean(bpMat(bpRowRange, bpColRange))[0];
			// Image block mat
			Range rowRange = Range(i * blockSize, min((i + 1) * blockSize, dst.rows));
			Range colRange = Range(j * blockSize, min((j + 1) * blockSize, dst.cols));
			Mat subMat = dst(rowRange, colRange);

			threshold(subMat, subMat, avgBP, 255, THRESH_BINARY);
		}
	}
}

Mat GetBlackPointMat(Mat img, int blockSize, int dynamicRange)
{
	int bpHeight = (int)ceil(img.rows / (double)blockSize);
	int bpWidth = (int)ceil(img.cols / (double)blockSize);
	Mat bpMat(bpHeight, bpWidth, CV_8U);

	for (int i = 0; i < img.rows; i += blockSize)
	{
		for (int j = 0; j < img.cols; j += blockSize)
		{
			// Get the black point block mat
			Range rowRange = Range(i, min(i + blockSize, img.rows));
			Range colRange = Range(j, min(j + blockSize, img.cols));
			Mat subMat = img(rowRange, colRange);
			// Get the average pixel value in subMat
			double avgPixel = mean(subMat)[0];
			double minPixel, maxPixel;

			// Get maximum and minimum pixel in subMat
			minMaxLoc(subMat, &minPixel, &maxPixel);

			// Check if the pixel gap between the maximum value and the minimum value is too small
			if (maxPixel - minPixel <= dynamicRange)
			{
				avgPixel = minPixel / 2;

				// If the Block is not located in the left / upper boundary,
				// use the average value of the adjacent Block as the value of this Block
				if (i > 0 && j > 0)
				{
					uchar avgNeighorBP = (uchar)((bpMat.at<uchar>((int)floor((double)i / blockSize) - 1, (int)floor((double)j / blockSize))
						+ bpMat.at<uchar>((int)floor((double)i / blockSize), (int)floor((double)j / blockSize) - 1) * 2
						+ bpMat.at<uchar>((int)floor((double)i / blockSize) - 1, (int)floor((double)j / blockSize) - 1)) / 4.0);
					if (minPixel < avgNeighorBP)
						avgPixel = avgNeighorBP;
				}
			}
			bpMat.at<uchar>((int)floor(i / blockSize), (int)floor(j / blockSize)) = (uchar)avgPixel;
		}
	}
	return bpMat;
}

>>>>>>> CppVersion
