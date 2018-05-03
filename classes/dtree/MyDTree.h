#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/ml/ml.hpp"
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;
using namespace ml;
class MyDTree
{
public:
	MyDTree();
	~MyDTree();
	void setWidth(int w);
	void setHeight(int h);

	int GenerateData(string folder, string dataFile, string labelsFile);
	int loadDataSet(string dataFile, string labelsFile);
	bool train(string modelFile);

	void load(string path);
	string predict(Mat img);

private:
	int WIDTH = 20;
	int HEIGHT = 30;
	Ptr<ml::RTrees>dtree;
	Mat labels;
	Mat data;

};

