#pragma once

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

class Composant
{
public:

	cv::Mat data;
	double positionX;
	double positionY;
	double largeur;
	double hauteur;
	int ordreHorizontal;

	Composant();
	~Composant();
};

