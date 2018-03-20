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
	double largeur;
	double hauteur;
	double ratio;
	int ordreHorizontal;
	double densite;
	//contour-feature
	int nbr_contours;
	double surface;
	double perimeter;
	vector<double> contour_aproximation_scores;
	//geometrie features
	Point2f centre_de_masse;
	int crossings;
	cv::Moments moments;
	double dtw;
	//*******************************************************methodes()
	
	int ordreHorizontal;
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
	void setPositionX(int);
	void setPositionY(int);
	void setRatio();
	void setOrdreHorizontal(int);
	void setDensite();
	void setNbrContours();
	void setSurface();
	void setPerimeter();
	void setContourAproximationScores();
	void setCentreDeMasse();
	void setCrossings();
	void setMoments();
	void setDTW();//


};

