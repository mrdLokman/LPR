#ifndef COMPOSANT_H
#define COMPOSANT_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

class Composant
{
public:
	Mat data;

	map<string, double> attributs;
	map<string, double> probabilites_classes;
	bool decision = false;
	vector<vector<cv::Point>>contours;
	//vector<Vec4i> hierarchie;
	vector<cv::Point>contour_externe;
	int ordreHorizontal;
	double debutX;
	double finX;
	double debutY;
	double finY;

	Composant();
	~Composant();
    void setData(cv::Mat src);
	void setContourExterne();

	double getHauteur();

	bool estDecide();

	double getAttribut(string attributName, double parametre = 0.0);
	vector<double> getAll();

	double getRatio();
	double getDensite();
	double getPortionHauteur();
	double getNbrContours();
	double getSurface();
	double getPerimeter();
	double getYcentreDeMasse();
	double getM00();
	double getM01();
	double getM10();
	double getM11();
	double getM02();
	double getM20();
	double getM21();
	double getM12();
	double getHauteurRelative(double hChar);

	// To Do
	double getContourAproximationScores();
	double getCrossings();
	double setDTW();
};

#endif
