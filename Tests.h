#ifndef TESTS_H
#define TESTS_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<conio.h>

using namespace std;
using namespace cv;


void testExtractionDePlaque(string imgSource, string imgDest, Point coins[]);

void testCorrectionRotation(string imgSource, string imgDest, Point coins[]);

void testCorrectionInclinaison(string imgSource, string imgDest);
void testcorrectionInclinaisonEliminerBordure(string imgSource, string imgDest);

void testSegmentationProjection(string image);
vector<cv::Mat> testSegmentationProjection(Mat bin);
void testSegmentationACC(string image);
vector<Mat> testSegmentationACC(Mat bin);

void testHistogramme(string image);
#endif