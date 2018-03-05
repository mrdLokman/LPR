#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace cv;
using namespace std;

cv::Mat display_images(std::vector<cv::Mat>& vecMat, int windowHeight, int nRows);