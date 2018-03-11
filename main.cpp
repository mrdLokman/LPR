#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Histogramme.h"
#include "classes/Selector.h"
#include "Pretraitement.h"
#include "Tests.h"
#include"display.h"
#include <vector>
#include <iostream>
using namespace std;
using namespace cv;

//*******************************************************************img source


 //string cars = "C:/Users/lool/Desktop/PFE/dataset image/cars/*.bmp";
 string cars = "C:/Users/Merdji/Documents/Visual Studio 2015/Projects/cars";

//*****************************************************************
 

 int main() {

	 cv::String path(cars); //select only bmp
	 vector<cv::String> fn;
	 vector<cv::Mat>windows;
	 cv::glob(path, fn, true); // recurse
	 for (size_t k = 0; k < fn.size(); ++k)
	 {
		 cv::Mat src = cv::imread(fn[k],0);
		 windows.clear();
		 if (src.empty()) continue; //only proceed if sucsessful		 
		 //cvtColor(src, src, CV_BGR2GRAY);
		 //-----------------------------------------------------------------
		 Selector selector;
		 //---------------------------------------------------crop the img src to get plaque
		 cv::Point* points = selector.getPlaqueCorners(src);
		 if (points == NULL)return -1;
		 int s = 0;
		 for (size_t i = 0; i < 4; i++)
		 {
			 s += points[i].x;
			 cout << points[i].x << "," << points[i].y << endl;
		 }
		 if (s == 0)continue;

		 Mat p = imread(fn[k], 0);
		 line(p, points[0], points[1], Scalar(0, 0, 255));
		 line(p, points[1], points[2], Scalar(0, 0, 255));
		 line(p, points[2], points[3], Scalar(0, 0, 255));
		 line(p, points[3], points[0], Scalar(0, 0, 255));
		 imshow("plaque", p);

		 imwrite("input/1-source.png",p);
		 // traitement de la plaque 

		 //1-extraction de la Plaque
		 Mat crop = extractionDePlaque(src, points);
		 imwrite("input/2-plaque.png", crop);
		 windows.push_back(crop);

		 //2-rotation plaque
		 Mat rotCrop = correctionRotation(crop, points);
		 imwrite("input/3-rotation.png",rotCrop);
		 windows.push_back(rotCrop);

		 //3-ajuter la plaque (rotation)
		 Mat ajustee = correctionInclinaisonEliminerBordure(rotCrop);
		 imwrite("input/4-ajustee.png",ajustee);
		 windows.push_back(ajustee);

		 //4-ameliorer le contraste de la plaque
		 Mat sharped;

		 sharped = unsharpFiltre2(ajustee);
		 windows.push_back(sharped);
		 imwrite("input/5-sharped1.png", sharped);

		 //filtrePersonnel(ajustee, sharped, 50, Size(5, 2));
	     //windows.push_back(sharped);
		 //imwrite("input/5-sharped2.png", sharped);

		 //5- egalisation d'histogramme
			//calcule histogramme source
		 MatND hist;
		 Mat histImg;
		 int nbins = 256;
		 int hsize[] = { nbins };
		 float range[] = { 0, 255 };
		 const float *ranges[] = { range };
		 int chnls[] = { 0 };

		 calcHist(&sharped, 1, chnls, Mat(), hist, 1, hsize, ranges);
		 histImg = imgHist(hist, 3, 3);
		 windows.push_back(histImg);
			//normalisation
		 //Mat normCrop = NormImage(ajustee);
		 //windows.push_back(normCrop);
			//calcule histogramme image normalisée
		 //calcHist(&normCrop, 1, chnls, Mat(), hist, 1, hsize, ranges);
		 //histImg = imgHist(hist, 3, 3);
		 //windows.push_back(histImg);
		 
		 //5-nomalisation

		 Mat normalisee = normalisation(sharped, 50);
		 windows.push_back(normalisee);
		 imwrite("input/6-normalisee.png", normalisee);

		 //5-binarisation

		 //Mat binaire_eq_hist_gauss = binarisation(normCrop,B_A_GAUSS,Blur_GAUSS);
		 //windows.push_back(binaire_eq_hist_gauss);
		 //Mat hist_normalisee = normalisation(binaire_eq_hist_gauss, 50);


		 Mat binaire = binarisation(normalisee, B_G_OTSU, No_Blur);
		 windows.push_back(binaire);
		 imwrite("input/7-binaire.png", binaire);

		 //6-operations morphoogiques
		 Mat resultat = operationsMorphologiques(binaire);
		 windows.push_back(resultat);
		 imwrite("input/8-oprsMorphs.png", resultat);
		

		 // affichage résultats
		 imshow("pretraitement", display_images(windows, 1000, 6));


		 //Segmentation

		// testSegmentationProjection("input/binaire.png");
		 imshow("segmentation projection", display_images(testSegmentationProjection(resultat),80,1));
		 //testSegmentationACC("input/binaire.png");
		 imshow("segmentation CCA", display_images(testSegmentationACC(resultat), 80, 1));

		 

		 waitKey(0);
		 cvDestroyWindow("pretraitement");
		 cvDestroyWindow("segmentation CCA");
		 cvDestroyWindow("segmentation projection");
		 cvDestroyWindow("plaque");
		

	 }
	 

 }





