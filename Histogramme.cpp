#include "Histogramme.h"

Mat NormImage(Mat src)
{
	Mat  dst;
	equalizeHist(src, dst);
	return dst;
}
Mat imgHist(Mat hist, float scaleX = 1, float scaleY = 1) {
	double maxVal = 0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);
	int rows = 64; //default height size
	int cols = hist.rows; //get the width size from the histogram
	Mat histImg = Mat::zeros(rows*scaleX, cols*scaleY, CV_8UC3);
	//for each bin
	for (int i = 0; i<cols - 1; i++) {
		float histValue = hist.at<float>(i, 0);
		float nextValue = hist.at<float>(i + 1, 0);
		Point pt1 = Point(i*scaleX, rows*scaleY);
		Point pt2 = Point(i*scaleX + scaleX, rows*scaleY);
		Point pt3 = Point(i*scaleX + scaleX, (rows - nextValue*rows / maxVal)*scaleY);
		Point pt4 = Point(i*scaleX, (rows - nextValue*rows / maxVal)*scaleY);

		int numPts = 5;
		Point pts[] = { pt1, pt2, pt3, pt4, pt1 };

		fillConvexPoly(histImg, pts, numPts, Scalar(255, 255, 255));
	}
	return histImg;
}


