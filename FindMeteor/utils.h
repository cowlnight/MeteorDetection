<<<<<<< HEAD
#pragma once

// �إߤ@�Ӹ�Ƨ�
void CreateFolder(std::string folderName);
// ����system command�ñ����^��
std::string ExecCommand(std::string cmd);
// Mean Block Binarization Method
void Binarization(cv::Mat & img, cv::Mat & dst, int blockSize, int dynamicRange, int slidingWindow);
// Get black point mat, used in Mean Block Binarization
cv::Mat GetBlackPointMat(cv::Mat img, int blockSize, int dynamicRange);


=======
#pragma once

// �إߤ@�Ӹ�Ƨ�
void CreateFolder(std::string folderName);
// ����system command�ñ����^��
std::string ExecCommand(std::string cmd);
// Mean Block Binarization Method
void Binarization(cv::Mat & img, cv::Mat & dst, int blockSize, int dynamicRange, int slidingWindow);
// Get black point mat, used in Mean Block Binarization
cv::Mat GetBlackPointMat(cv::Mat img, int blockSize, int dynamicRange);


>>>>>>> CppVersion
