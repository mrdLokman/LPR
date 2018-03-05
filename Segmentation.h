#ifndef SEGMENTATION_H
#define SEGMENTATION_H


#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include"classes\Composant.h"

#include<vector>
#include<iostream>


using namespace std;
using namespace cv;

vector<Composant> segmentationProjection(Mat plaque);
vector<Composant> segmentationACC(Mat plaque);



#endif