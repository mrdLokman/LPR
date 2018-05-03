#pragma once


#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>

#include <map>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;
using namespace ml;

class KNN
{
public:

	KNN();
	~KNN();

	void setParamK(int _k);
	void setWidth(int w);
	void setHeight(int h);

	int GenerateData(string folder, string dataFile, string labelsFile);

	int loadDataset(string dataFile, string labelsFile);
	bool train(string modelFile);

	string predict(Mat img);
	void load(string path);

private:
	int K;
	int WIDTH = 20;
	int HEIGHT = 30;
	Ptr<ml::KNearest>  kNearest;
	Mat labels;
	Mat data;

};

