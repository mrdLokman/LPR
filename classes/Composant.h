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
	vector<Vec4i> hierarchie;
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
	vector<double> huMoments;
	double getHauteur();
	double getLargeur();

	bool estDecide();

	double getAttribut(string attributName, double parametre = 0.0);
	vector<double> getAll();
	void calcHuMoments();
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
	//moments de hu 
	double getHu1();
	double getHu2();
	double getHu3();
	double getHu4();
	double getHu5();
	double getHu6();
	double getHu7();
	double getHauteurRelative(double hChar);
	double getEpaisseurMoyenneRelative(double EChar = 1.0);
	double getEpaisseurMoyenne();

	// To Do
	double getContourAproximationScores();
	double getCrossings();
	double getDTW();
};

#endif