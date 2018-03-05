#ifndef PRETRAITEMENT_H
#define PRETRAITEMENT_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<conio.h>

using namespace std;
using namespace cv;

enum {
	B_A_GAUSS = 0,
	B_A_MEAN = 1,
	B_G_OTSU = 2,
	B_G_GLOBAL = 3
};

enum {
	No_Blur = 0,
	Blur_GAUSS = 1,
	Blur_MEDIAN = 2
};



Mat extractionDePlaque(Mat src, Point* coins);

Mat correctionRotation(Mat origine, Point* coins);

Mat correctionInclinaison(Mat origine);
Mat correctionInclinaisonEliminerBordure(Mat origine);

vector<int> histogrammeProjectionHorizontal(Mat src);
vector<int> histogrammeProjectionHorizontalGrey(Mat img);
vector<int> histogrammeProjectionVerticalGrey(Mat img);
vector<int> histogrammeProjectionVertical(Mat img);

int segma(vector<int> hist);
int moyenne(vector<int> hist);
int minDroit(vector<int> hist, int moy);
int minGauche(vector<int> hist, int moy);
int maxDroit(vector<int> hist, int moy);
int maxGauche(vector<int> hist, int moy);

Mat mareHistogram(Mat img, int t);
void histogrammeImage(string img);
Mat afficheHistogrammeProjectionHorizontal(vector<int> hist);

Mat binarisation(Mat origine, int type = 0, int filtre = 0);

Mat normalisation(Mat origine, int hauteurNorm);

Mat operationsMorphologiques(Mat origine);
#endif