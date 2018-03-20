#pragma once

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include <map>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

class Composant
{
public:
	Mat data;

	map<string, double> attributs;

	vector<vector<cv::Point>>contours;
	//vector<Vec4i> hierarchie;
	vector<cv::Point>contour_externe;
	//***********
	//features
	//***********
	
	int ordreHorizontal;
	
	//*******************************************************methodes()
	

	map<string, double> probabilites_classes;


	double debutX;
	double finX;
	double debutY;
	double finY;
	

	Composant();
	~Composant();

	double ratio();
	double densite();
	double portionHauteur();

	void setData(cv::Mat src);
	void setContourExterne();
	double getNbrContours();
	double getSurface();
	double getPerimeter();
	double getContourAproximationScores();
	double getYeentreDeMasse();
	double getM00();
	double getM01();
	double getM10();
	double getM11();
	double getM02();
	double getM20();
	double getM21();
	double getM12();
	double getCrossings();
	double setDTW();//

};

