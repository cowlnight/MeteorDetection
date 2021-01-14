/**
* @file MetDetectionAlgo.cpp
*
* Implementation of meteor detection algorithm
*
*/

#include "../pch.h"
#include "../utils.h"
#include "../AppAlgoITF.h"
#include "../Data/MeteorData.h"
#include "MetDetectionAlgo.h"


using namespace FindMeteor;


bool CMetDetection::Execute(CMeteorData& _data)
{
	Mat srcImg = _data.srcLoadImg;
	Mat canImg, grayImg, dstImg;

	cvtColor(srcImg, grayImg, COLOR_RGB2GRAY);

	Canny(grayImg, canImg, 50, 100, 3);

	///*=== Create mask: remove environment noise ===*///

	cv::Mat maskInput = canImg.clone();
	cv::Mat mask = cv::Mat::zeros(maskInput.size(), maskInput.type());

	cv::Mat removeEnvImg = cv::Mat::zeros(maskInput.size(), maskInput.type());

	cv::circle(mask, cv::Point(maskInput.cols / 2, maskInput.rows / 2), 1100, cv::Scalar(255), -1, 8, 0);
	cv::circle(mask, cv::Point(maskInput.cols / 2, maskInput.rows / 2), 30, cv::Scalar(0), -1, 8, 0);

	Rect rectUp = Rect(0, 0, mask.cols, 100);
	Rect rectDown = Rect(0, mask.rows - 100, mask.cols, mask.rows);

	rectangle(mask, rectUp, Scalar(0), -1, 8, 0);
	rectangle(mask, rectDown, Scalar(0), -1, 8, 0);

	maskInput.copyTo(removeEnvImg, mask);

	///*=== Create mask: remove environment noise ===*///

	
	///*=== Method 1 : find legal line feature ===*///

	Mat houghImg;
	vector<Vec4i> lines;
	FindHoughLine(removeEnvImg, lines, houghImg);
	
	///*=== Method 1: find legal line feature ===*///


	///*=== Method 2: find legal contour feature ===*///
	/// preprocessing for finding contours
	Mat dilateImg, morphImage;
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(5, 5));
	morphologyEx(removeEnvImg, morphImage, MORPH_CLOSE, dilateElement, Point(-1, -1), 2);

	cvtColor(morphImage, dstImg, CV_GRAY2BGR);

	dilateImg = morphImage.clone();

	/// find contours
	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours;

	findContours(dilateImg, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	
	/// identify legal contour
	///1. remove too small area ///
	DeleteSmallContour(contours);
	
	if (contours.size() < 0)		
		return false;

	/// Quadrilateral contours ///
	vector<vector<Point>> contours_rect;
	RectCorners(contours, contours_rect);

	if (contours_rect.size() < 0)
		return false;
	
	/// 2. remove NOT rectangle shape from contours ===//
	CheckRectContour(contours_rect, dilateImg);

	if (contours_rect.size() < 0)
		return false;

	///*=== Method 2: find legal contour feature ===*///


	///*=== Combine 2 conditions: check legal line features and contour features ===*///
	CheckContourandHough(contours_rect,lines);
	
	/*for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];

		line(dstImg, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 1, LINE_AA); 

	}*/
	drawContours(dstImg, contours_rect, -1, Scalar(0, 0, 255), 2);

	///*== create output path &&  save output result ==*///

	if (contours_rect.size() > 0) {

		time_t nowTime;
		tm gmt;
		char timeStr[80];

		time(&nowTime);
		localtime_s(&gmt, &nowTime);
		mktime(&gmt);

		strftime(timeStr, sizeof(timeStr), "%m%d%H%M", &gmt);

		CreateFolder("dump/" + _data.dataSubPath + "/" + timeStr);
		_data.outputFolderPath = "dump/" + _data.dataSubPath + "/" + timeStr + "/";

		imwrite(_data.outputFolderPath + _data.srcFileNameT, srcImg);
		imwrite(_data.outputFolderPath + _data.srcFileName+"_detected.jpg" , dstImg);

		return true;
	}
	else return false;
}


/// Method 1
void FindMeteor::CMetDetection::FindHoughLine(Mat src, vector<Vec4i>& lines, Mat dst) {

	cvtColor(src, dst, CV_GRAY2BGR);

	Mat dilateImg_hough, erodeImg_hough;
	Mat dilateElement_hough = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(src, dilateImg_hough, dilateElement_hough, Point(-1, -1),2);
	Mat erodeElement_hough = getStructuringElement(MORPH_RECT, Size(3, 3));
	erode(dilateImg_hough, erodeImg_hough, erodeElement_hough);

	Mat aa = cv::Mat::zeros(dilateImg_hough.size(), dilateImg_hough.type());
	vector<vector<Point>> contours;
	vector<Vec4i> cloudHierarchy;

	findContours(erodeImg_hough, contours, cloudHierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	///*== check cloudy condition ==*///
	int contour_able = 0;
	vector<vector<Point>> cloudContours;

	for (int i = 0; i < contours.size();i++) {
		if (contourArea(contours[i]) > 1550) {
			contour_able = 1;
			cloudContours.push_back(contours[i]);

		}
	}

	drawContours(aa, cloudContours, 0, Scalar(255), -1);
	///*== check cloudy condition ==*///

	///*== Make cloud mask ==*///

	Mat dilation_mask;
	if (contour_able) {
		dilateElement_hough = getStructuringElement(MORPH_RECT, Size(7, 7));
		Mat bitnotImg,dilateTmp;
		bitwise_not(aa, bitnotImg);
		dilate(bitnotImg, dilateTmp, dilateElement_hough);

		dilation_mask = erodeImg_hough.mul(dilateTmp);
		dilation_mask = 255 * dilation_mask;
	}
	else 
	{
		dilation_mask = erodeImg_hough;
	}

	///*== Make cloud mask ==*///

	HoughLinesP(dilation_mask, lines, 3, CV_PI / 180, 100, 50, 20);

	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		double dis = sqrt(pow(lines[i][0] - lines[i][2], 2) + pow(lines[i][1] - lines[i][3], 2));
		if (dis <= 70|| dis > 300) {

			lines.erase(lines.begin() + i);
			i--;
			continue;
		
		}
		line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186, 88, 255), 1, LINE_AA); 

	}
	

}


// Remove too small area contours
void FindMeteor::CMetDetection::DeleteSmallContour(vector<vector<Point>> &contours)
{
	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i], false);
 		if (area < 150.0 ) {
			contours.erase(contours.begin() + i);
			i--;
			continue;
		}

	

	}
}

// Get 4 points in rectangle
void FindMeteor::CMetDetection::RectCorners(vector<vector<cv::Point>> &contours_in, vector<vector<cv::Point>> &contours_out)
{

	for (int i = 0; i < contours_in.size(); i++) {

		Rect m_rect = boundingRect(contours_in.at(i));
		RotatedRect box = minAreaRect(contours_in.at(i));
		Size2f boxArea = box.size;
		Point2f boxCenter = box.center;

		double contoursArea = contourArea(contours_in.at(i), false);
	

		Point2f rotRect[4];
		box.points(rotRect);

		int upNum = 0, downNum = 0, leftNum = 0, rightNum = 0;
		int upID = -1, downID = -1, leftID = -1, rightID = -1;
		double disUp = DBL_MAX, disDown = DBL_MAX, disLeft = DBL_MAX, disRight = DBL_MAX;
		Point P_up(0, 0), P_down(0, 0), P_left(0, 0), P_right(0, 0);

		vector<Point> c_rect;
		double alpha;

		// 找離外接矩形四個角點最近的角點
		for (int k = 0; k < contours_in[i].size(); k++) {

			if (k == 0)
			{
				alpha = Angle(contours_in[i][contours_in[i].size() - 1], contours_in[i][k + 1], contours_in[i][k]);
			}
			else if (k == contours_in[i].size() - 1)
			{
				alpha = Angle(contours_in[i][k - 1], contours_in[i][0], contours_in[i][k]);
			}
			else
			{
				alpha = Angle(contours_in[i][k - 1], contours_in[i][k + 1], contours_in[i][k]);
			}

			if (alpha >= 134) {
				contours_in[i].erase(contours_in[i].begin() + k);
				k--;
				continue;
			}

			if (norm(contours_in[i][k] - Point(rotRect[0])) < disLeft) {
				disLeft = norm(contours_in[i][k] - Point(rotRect[0].x, rotRect[0].y));
				P_left = contours_in[i][k];
				leftID = k;
			}
			if (norm(contours_in[i][k] - Point(rotRect[1])) < disUp) {
				disUp = norm(contours_in[i][k] - Point(rotRect[1].x, rotRect[1].y));
				P_up = contours_in[i][k];
				upID = k;
			}
			if (norm(contours_in[i][k] - Point(rotRect[2])) < disRight) {
				disRight = norm(contours_in[i][k] - Point(rotRect[2].x, rotRect[2].y));
				P_right = contours_in[i][k];
				rightID = k;
			}
			if (norm(contours_in[i][k] - Point(rotRect[3])) < disDown) {
				disDown = norm(contours_in[i][k] - Point(rotRect[3].x, rotRect[3].y));
				P_down = contours_in[i][k];
				downID = k;
			}

		}

		vector<Point> points;

		points.push_back(P_left);
		points.push_back(P_up);
		points.push_back(P_right);
		points.push_back(P_down);

		//find the maximum angle in 4 corners' contour
		double alpha_left = Angle(P_up, P_down, P_left);
		double alpha_up = Angle(P_left, P_right, P_up);
		double alpha_right = Angle(P_up, P_down, P_right);
		double alpha_down = Angle(P_left, P_right, P_down);
		double alpha_max = alpha_left > alpha_up ? alpha_left : alpha_up;
		alpha_max = alpha_max > alpha_right ? alpha_max : alpha_right;
		alpha_max = alpha_max > alpha_down ? alpha_max : alpha_down;


		double disH = fabs(norm(P_left - P_up) - norm(P_down - P_right));
		double disW = fabs(norm(P_right - P_up) - norm(P_down - P_left));

		double cornerIOU_1 = contourArea(points, false) / contourArea(contours_in.at(i), false);
		points.clear();

		// optimization
		if (contours_in[i].size() > 4) {

			double iouThreshhold = cornerIOU_1;
			int offsetParam = 2;
			map<int, Point> pa, pb, pc, pd;
			// IOU=intersection/(areaA+areaB-intersection)

			int s1 = leftID - offsetParam, h1 = leftID + offsetParam;
			int s2 = upID - offsetParam, h2 = upID + offsetParam;
			int s3 = rightID - offsetParam, h3 = rightID + offsetParam;
			int s4 = downID - offsetParam, h4 = downID + offsetParam;
			//////get Points index range
			getPointsRange(s1, h1, contours_in[i], pa);
			getPointsRange(s2, h2, contours_in[i], pb);
			getPointsRange(s3, h3, contours_in[i], pc);
			getPointsRange(s4, h4, contours_in[i], pd);
			map<int, Point>::iterator iter_a, iter_b, iter_c, iter_d;
			vector<vector<Point>> candiRects;
			int paraGroupIndex = -1;

			for (iter_c = pc.begin(); iter_c != pc.end(); iter_c++)
			{
				for (iter_b = pb.begin(); iter_b != pb.end(); iter_b++)
				{
					for (iter_a = pa.begin(); iter_a != pa.end(); iter_a++)
					{
						for (iter_d = pd.begin(); iter_d != pd.end(); iter_d++)
						{
						
							if ((iter_b->first < iter_a->first && iter_a->first < iter_d->first && iter_d->first < iter_c->first) ||
								(iter_c->first < iter_b->first && iter_b->first < iter_a->first && iter_a->first < iter_d->first)) {
								vector<Point> candiRect;
								candiRect.push_back(iter_a->second);
								candiRect.push_back(iter_b->second);
								candiRect.push_back(iter_c->second);
								candiRect.push_back(iter_d->second);

								candiRects.push_back(candiRect);
								candiRect.clear();
							}
						}
					}
				}
			}


			for (int r = 0; r < candiRects.size(); r++) {

				double cornerIOU = contourArea(candiRects.at(r), false) / contoursArea;

				if (cornerIOU > iouThreshhold) {
					double disRectH = fabs(norm(candiRects[r][0] - candiRects[r][1]) - norm(candiRects[r][3] - candiRects[r][2]));
					double disRectW = fabs(norm(candiRects[r][2] - candiRects[r][1]) - norm(candiRects[r][3] - candiRects[r][0]));

					double alpha_1 = Angle(candiRects[r][1], candiRects[r][3], candiRects[r][0]);
					double alpha_2 = Angle(candiRects[r][0], candiRects[r][2], candiRects[r][1]);
					double alpha_3 = Angle(candiRects[r][1], candiRects[r][3], candiRects[r][2]);
					double alpha_4 = Angle(candiRects[r][0], candiRects[r][2], candiRects[r][3]);
					double temAlpha_max = alpha_1 > alpha_2 ? alpha_1 : alpha_2;
					temAlpha_max = temAlpha_max > alpha_3 ? temAlpha_max : alpha_3;
					temAlpha_max = temAlpha_max > alpha_4 ? temAlpha_max : alpha_4;
					if (fabs(alpha_max - 90) < 10)alpha_max = 90;

					
					double maxDisH = disH > 6 ? disH : 6;
					double maxDisW = disW > 12 ? disW : 12;

					if (fabs(disH - disRectH) <= maxDisH && fabs(disW - disRectW) <= maxDisW && fabs(alpha_max - temAlpha_max) <= 5) {
					
						iouThreshhold = cornerIOU;
						P_left = candiRects[r][0];
						P_up = candiRects[r][1];
						P_right = candiRects[r][2];
						P_down = candiRects[r][3];

					}
				}
			}


			candiRects.erase(candiRects.begin(), candiRects.end());

			pa.erase(pa.begin(), pa.end());
			pb.erase(pb.begin(), pb.end());
			pc.erase(pc.begin(), pc.end());
			pd.erase(pd.begin(), pd.end());

		}



		//clockwise：P_left->P_up->P_right->P_down
		c_rect.push_back(P_left);
		c_rect.push_back(P_up);
		c_rect.push_back(P_right);
		c_rect.push_back(P_down);

		contours_out.push_back(c_rect);


	}

}

// The Angle between three points
double FindMeteor::CMetDetection::Angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	double angle_line = (dx1*dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2 * dy2) + 1e-10);
	return acos(angle_line) * 180 / CV_PI;
}

// Get point range to optimize iou
void FindMeteor::CMetDetection::getPointsRange(int &s1, int &h1, const vector<Point> &contours_in_i, map<int, Point> &outputArray)
{
	for (int a = s1; a <= h1; a++) {
		if (a < 0)
		{
			a = 0;
			h1 = h1 + 1;
		}

		if (s1 < 0 && a == h1 && s1 + contours_in_i.size() >= 0) /// "if"maybe have problem
		{

			a = s1 + contours_in_i.size();
			h1 = contours_in_i.size() - 1;
			s1 = 0;
		}


		if (a > contours_in_i.size() - 1) {
			a = 0;
			h1 = h1 % contours_in_i.size();
		}
		outputArray[a] = contours_in_i[a];
	}
}

//check whether the contour is rectangle or not
void FindMeteor::CMetDetection::CheckRectContour(vector<vector<Point>> &contours_poly, cv::Mat& dilateImg)
{

	for (int i = 0; i < contours_poly.size(); i++) {


		if (fabs(norm(contours_poly[i][0] - contours_poly[i][1])) > fabs(norm(contours_poly[i][0] - contours_poly[i][3]))) {
			/// The long edge of the rectangle should be longer than the short edge. 
			if (fabs(norm(contours_poly[i][0] - contours_poly[i][1])) < 3.0 * fabs(norm(contours_poly[i][0] - contours_poly[i][3]))) {
				contours_poly.erase(contours_poly.begin() + i);
				i--;
				continue;
			}
			if (fabs(norm(contours_poly[i][0] - contours_poly[i][1])) < 3.0 * fabs(norm(contours_poly[i][1] - contours_poly[i][2]))) {
				contours_poly.erase(contours_poly.begin() + i);
				i--;
				continue;
			}

			/// The difference of short edges should less than 10px AND The short edge of rectangle should less than 15px
			if (fabs(norm(contours_poly[i][0] - contours_poly[i][3]) - norm(contours_poly[i][1] - contours_poly[i][2])) > 10.0|| 
				fabs(norm(contours_poly[i][0] - contours_poly[i][3]))>15.0 || fabs(norm(contours_poly[i][1] - contours_poly[i][2]))>15.0) {
				contours_poly.erase(contours_poly.begin() + i);
				i--;
				continue;
			}

			/// The long edge of rectangle should NOT be greater than 75px 
			if (fabs(norm(contours_poly[i][0] - contours_poly[i][1])) <= 75.0 || fabs(norm(contours_poly[i][2] - contours_poly[i][3])) <=75.0) {
				contours_poly.erase(contours_poly.begin() + i);
				i--;
				continue;
			}
		}
		else {
			/// The long edge of the rectangle should be longer than the short edge. 
			if (fabs(norm(contours_poly[i][0] - contours_poly[i][3])) < 3.0 * fabs(norm(contours_poly[i][0] - contours_poly[i][1]))) {
				contours_poly.erase(contours_poly.begin() + i);
				i--;
				continue;
			}
			if (fabs(norm(contours_poly[i][0] - contours_poly[i][3])) < 3.0 * fabs(norm(contours_poly[i][2] - contours_poly[i][3]))) {
				contours_poly.erase(contours_poly.begin() + i);
				i--;
				continue;
			}

			/// The difference of short edges should less than 10px AND The short edge of rectangle should less than 15px
			if (fabs(norm(contours_poly[i][0] - contours_poly[i][1]) - norm(contours_poly[i][2] - contours_poly[i][3])) > 10.0 ||
				fabs(norm(contours_poly[i][0] - contours_poly[i][1])) > 15.0 || fabs(norm(contours_poly[i][2] - contours_poly[i][3])) > 15.0) {
				contours_poly.erase(contours_poly.begin() + i);
				i--;
				continue;
			}
			/// The long edge of rectangle should NOT be greater than 75px 
			if (fabs(norm(contours_poly[i][0] - contours_poly[i][3])) <= 75.0 || fabs(norm(contours_poly[i][1] - contours_poly[i][2])) <= 75.0) {
				contours_poly.erase(contours_poly.begin() + i);
				i--;
				continue;
			}
		}

		///*== filling rate in a contour ==*///
		int samePixel = 0;
		int allPixel = 0;

		Rect m_rect = boundingRect(contours_poly.at(i));

		for (int nX = m_rect.x; nX < (m_rect.x + m_rect.width); ++nX)
		{
			for (int nY = m_rect.y; nY < (m_rect.y + m_rect.height); ++nY)
			{
				Point2f tempoint(nX, nY);

				if (pointPolygonTest(contours_poly[i], tempoint, 0) >= 0) {
					allPixel++;
					Point2f tempoint(nX, nY);
					if (dilateImg.at<uchar>(nY, nX) == 255)
						samePixel++;
				}
			}
		}


		if (double(samePixel) / double(allPixel) < 0.5&&double(samePixel)<500)
		{
			contours_poly.erase(contours_poly.begin() + i);
			i--;
			continue;
		}
	
	}

}

// check whether a line is in (near) a contour or not
void FindMeteor::CMetDetection::CheckContourandHough(vector<vector<Point>>& contour, vector<Vec4i>& lines)
{
	vector<Point2f> midPoint;
	for (int i = 0; i < lines.size(); i++) {
		midPoint.push_back(Point((lines[i][0] + lines[i][2]) / 2, (lines[i][1] + lines[i][3]) / 2));
	}
	for (int i = 0; i < contour.size(); i++) {
		bool isInContour = false;
		for (int j = 0; j < lines.size(); j++) {
			if (fabs(pointPolygonTest(contour[i], midPoint[j], 1)) < 1.5) {
				isInContour = true;
				break;
				
			}
		}
		if (!isInContour) {
			contour.erase(contour.begin() + i);
			i--;
			continue;
		}
	}
}
