#pragma once

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

class Composant
{
public:

	cv::Mat data;
	//double positionX;
	//double positionY;
	//double largeur;
	//double hauteur;
	int ordreHorizontal;
	double debutX;
	double finX;
	double debutY;
	double finY;
	

	Composant();
	~Composant();

	double ratio();
	double surface();
	double densite();


};

