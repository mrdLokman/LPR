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
	map<string, double> probabilites_classes;

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
<<<<<<< HEAD
=======
	

>>>>>>> 4b9227b58a5d221a72e47b6b9c23fc6d04705bb7

};

