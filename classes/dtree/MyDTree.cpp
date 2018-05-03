#include "MyDTree.h"


void MyDTree::setWidth(int w)
{
	this->WIDTH = w;
}
void MyDTree::setHeight(int h)
{
	this->HEIGHT = h;
}

int MyDTree::GenerateData(string folder, string dataFile, string labelsFile) {

	for (int i = 0; i < 10; i++) {
		stringstream ss;
		ss << i;
		string source = folder + "/" + ss.str();
		vector<cv::String> fn;
		glob(source, fn, true);
		for (size_t k = 0; k < fn.size(); ++k) {
			Mat src = imread(fn[k], CV_8U);
			Mat resized;
			cv::resize(src, resized, Size(this->WIDTH, this->HEIGHT));
			this->labels.push_back(i);
			Mat imageFloat;
			resized.convertTo(imageFloat, CV_32FC1);
			Mat imageFlattenedFloat = imageFloat.reshape(1, 1);
			this->data.push_back(imageFlattenedFloat);
		}
	}

	FileStorage fsClassifications(labelsFile, FileStorage::WRITE);
	if (fsClassifications.isOpened() == false) {
		std::cout << "error, unable to open training classifications file, exiting program\n\n";
		return -1;
	}
	fsClassifications << "labels" << this->labels;
	fsClassifications.release();

	cv::FileStorage fsTrainingImages(dataFile, FileStorage::WRITE);

	if (fsTrainingImages.isOpened() == false) {
		std::cout << "error, unable to open training images file, exiting program\n\n";
		return -1;
	}
	fsTrainingImages << "data" << this->data;
	fsTrainingImages.release();

	return 0;
}

int MyDTree::loadDataSet(string dataFile, string labelsFile) {

	FileStorage fsClassifications(labelsFile, FileStorage::READ);
	if (fsClassifications.isOpened() == false) {
		std::cout << "error, unable to open training classifications file, exiting program\n\n";
		return -1;
	}

	fsClassifications["labels"] >> this->labels;
	fsClassifications.release();

	FileStorage fsTrainingImages(dataFile, FileStorage::READ);
	if (fsTrainingImages.isOpened() == false) {
		std::cout << "error, unable to open training images file, exiting program\n\n";
		return -1;
	}

	fsTrainingImages["data"] >> this->data;
	fsTrainingImages.release();
	//Ptr<ml::dtree>  DTree(ml::dtree::create());
	return 0;
}

bool MyDTree::train(string modelFile)
{
	
	bool trained=  dtree->train(data, ml::ROW_SAMPLE, labels);
	if (trained)this->dtree->save(modelFile);
	return trained;
}



void MyDTree::load(string path)
{
	this->dtree = Algorithm::load<RTrees>(path);
}

string MyDTree::predict(Mat img)
{
	Mat matResized;
	cv::resize(img, matResized, cv::Size(WIDTH, HEIGHT));
	Mat matFloat;
	matResized.convertTo(matFloat, CV_32FC1);
	cv::Mat matFlattenedFloat = matFloat.reshape(1, 1);

	cv::Mat matChar(0, 0, CV_32F);

	this->dtree->predict(matFlattenedFloat, matChar);     // 
	float floatResult = (float)matChar.at<float>(0, 0);
	stringstream ss;
	ss << floatResult;
	return ss.str();
}

MyDTree::MyDTree()
{
	Ptr<ml::RTrees>  dt(ml::RTrees::create());
	this->dtree = dt;
	
}


MyDTree::~MyDTree()
{
}
