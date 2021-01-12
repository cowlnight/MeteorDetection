<<<<<<< HEAD
/**
* @file MetDetectionAlgo.h
*
* Classes for meteor detection algorithm
*
*/

#ifndef _METDETECTALGO_H_
#define _METDETECTALGO_H_

namespace FindMeteor
{
	
	class CMetDetection : public IAppAlgoITF
	{
	public:
	
		CMetDetection(void) {}
		virtual ~CMetDetection(void) {}
		virtual bool Execute(CMeteorData&);

	
	protected:
		void FindHoughLine(Mat src, vector<Vec4i>& lines, Mat dst);
		void DeleteSmallContour(vector<vector<Point>> &contours);
		void RectCorners(vector<vector<cv::Point>> &contours_in, vector<vector<cv::Point>> &contours_out);
		double Angle(Point pt1, Point pt2, Point pt0);
		void getPointsRange(int &s1, int &h1, const vector<Point> &contours_in_i, map<int, Point> &outputArray);
		void CheckRectContour(vector<vector<Point>> &contours_poly, cv::Mat& dilateImg);
		void CheckContourandHough(vector<vector<Point>> &contour, vector<Vec4i>& lines);



	};

	
}

=======
/**
* @file MetDetectionAlgo.h
*
* Classes for meteor detection algorithm
*
*/

#ifndef _METDETECTALGO_H_
#define _METDETECTALGO_H_

namespace FindMeteor
{
	
	class CMetDetection : public IAppAlgoITF
	{
	public:
	
		CMetDetection(void) {}
		virtual ~CMetDetection(void) {}
		virtual bool Execute(CMeteorData&);

	
	protected:
		void FindHoughLine(Mat src, vector<Vec4i>& lines, Mat dst);
		void DeleteSmallContour(vector<vector<Point>> &contours);
		void RectCorners(vector<vector<cv::Point>> &contours_in, vector<vector<cv::Point>> &contours_out);
		double Angle(Point pt1, Point pt2, Point pt0);
		void getPointsRange(int &s1, int &h1, const vector<Point> &contours_in_i, map<int, Point> &outputArray);
		void CheckRectContour(vector<vector<Point>> &contours_poly, cv::Mat& dilateImg);
		void CheckContourandHough(vector<vector<Point>> &contour, vector<Vec4i>& lines);



	};

	
}

>>>>>>> CppVersion
#endif