#include "MySVM.h"


void MySVM::setWidth(int w)
{
	this->WIDTH = w;
}
void MySVM::setHeight(int h)
{
	this->HEIGHT = h;
}

int MySVM::GenerateData(string folder, string dataFile, string labelsFile) {

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

int MySVM::loadDataSet(string dataFile, string labelsFile) {

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
	//Ptr<ml::SVM>  MySVM(ml::SVM::create());
	return 0;
}

bool MySVM::train(string modelFile)
{
	bool trained =  svm->train(data, ml::ROW_SAMPLE, labels);
	if(trained)this->svm->save(modelFile);
	return trained;
}

bool MySVM::trainAuto(string modelFile)
{
	cv::Ptr<cv::ml::TrainData> td =cv::ml::TrainData::create(this->data, cv::ml::ROW_SAMPLE, this->labels);
	bool trained=this->svm->trainAuto(td);
	if(trained)this->svm->save(modelFile);
	return trained;
}

void MySVM::load(string path)
{
	this->svm = Algorithm::load<SVM>(path);
}

string MySVM::predict(Mat data)
{
	cv::Mat matChar(0, 0, CV_32F);

	this->svm->predict(data, matChar);     // 
	float floatResult = (float)matChar.at<float>(0, 0);
	stringstream ss;
	ss << floatResult;
	return ss.str();
}

MySVM::MySVM()
{
	Ptr<ml::SVM>  s(ml::SVM::create());
	this->svm = s;
	svm->setKernel(cv::ml::SVM::POLY);
	svm->setGamma(1.0000000000000001e-05);//cv::ml::SVM::POLY);
	//uncomment the next if you choose (POLY / SIGMOID)kernel
	svm->setCoef0(1.0000000000000001e-01);//cv::ml::SVM::POLY);
	//it dosn't work if i uncomment the fellowing instruction
	svm->setType(cv::ml::SVM::C_SVC);
	svm->setDegree(3.4300000000000002e+00);//cv::ml::SVM::POLY);
	svm->setP(cv::ml::SVM::EPS_SVR);
	svm->setNu(0.1);
	svm->setC(1.0000000000000001e-01);
	svm->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6));
}

MySVM::~MySVM()
{
}
