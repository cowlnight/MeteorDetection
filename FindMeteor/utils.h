<<<<<<< HEAD
#pragma once

// 建立一個資料夾
void CreateFolder(std::string folderName);
// 執行system command並接收回應
std::string ExecCommand(std::string cmd);
// Mean Block Binarization Method
void Binarization(cv::Mat & img, cv::Mat & dst, int blockSize, int dynamicRange, int slidingWindow);
// Get black point mat, used in Mean Block Binarization
cv::Mat GetBlackPointMat(cv::Mat img, int blockSize, int dynamicRange);


=======
#pragma once

// 建立一個資料夾
void CreateFolder(std::string folderName);
// 執行system command並接收回應
std::string ExecCommand(std::string cmd);
// Mean Block Binarization Method
void Binarization(cv::Mat & img, cv::Mat & dst, int blockSize, int dynamicRange, int slidingWindow);
// Get black point mat, used in Mean Block Binarization
cv::Mat GetBlackPointMat(cv::Mat img, int blockSize, int dynamicRange);


>>>>>>> CppVersion
