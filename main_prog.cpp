#include "Tests.h"
#include "Pretraitement.h"


void test() {
	Mat src = imread("../input/test.png", 0);
	vector<int> h = histogrammeProjectionHorizontal(src);
	
	int taille = h.size();
	cout << "taille = " << taille <<endl;
	int moy = moyenne(h);
	cout << "moyenne = " << moy << endl;
	int minG = minGauche(h, moy);
	cout << "minGauche = " << minG << endl;
	int minD = minDroit(h, moy);
	cout << "minDroit = " << minD << endl;

	int w = src.cols;
	for (int i = 0; i < w; i++)
	{
		src.at<unsigned char>(minG, i) = 128;
		src.at<unsigned char>(minD, i) = 128;
	}

	imshow("resultat", src);
	imwrite("../input/resultat.png",src);
}


int main1()
{
	Mat src= imread("C:/Users/lool/PFE/PFE/output/segACC/000000.png", 0);
	// the first command-line parameter must be a filename of the binary
	// (black-n-white) image
	

	Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);

	src = src > 1;
	namedWindow("Source", 1);
	imshow("Source", src);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(src, contours, hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,Point(0,0));

	// iterate through all the top-level contours,
	// draw each connected component with its own random color
	int idx = 0;
	for (int i = 0; i < contours.size();i++)
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, i, color, 1, 8, hierarchy,0);
	}

	namedWindow("Components", 1);
	imshow("Components", dst);
	waitKey(0);
	return 0;
}
