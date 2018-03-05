#pragma once
#include <vector>

#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

class Selector
{
private:
	vector <cv::Point>points;
public:
	Selector();
	~Selector();
	void addPoint(cv::Point);
	vector<cv::Point>*getPoints();
	static void selectPointsOnClick(int event, int x, int y, int flags, void * vec);
	Point* getPlaqueCorners(Mat img);
	Mat cropImage(Mat src, Point* coins);

};


