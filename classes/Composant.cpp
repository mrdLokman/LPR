#include"Composant.h"


Composant::Composant() {

}

Composant::~Composant() {

}


void Composant::setData(cv::Mat src)
{
	this->data = src;
}

void Composant::setContourExterne()
{
	if (data.empty())return;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat m;
	this->data.copyTo(m);
	threshold(m, m, 100, 255, CV_THRESH_BINARY_INV);
	findContours(m, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	this->contour_externe = contours[0];
}

double Composant::getHauteur()
{
	return (double)(finY - debutY);
}

bool Composant::estDecide() {
	return this->decision;
}

double Composant::getAttribut(string attributName, double parametre){
	if (attributName == "ratio")
		return this->getRatio();
	if (attributName == "densite")
		return this->getDensite();
	if (attributName == "ph")
		return this->getPortionHauteur();
	if (attributName == "hr")
		return this->getHauteurRelative(parametre);
	if (attributName == "nbrContours")
		return this->getNbrContours();
	if (attributName == "surface")
		return this->getSurface();
	if (attributName == "perimetre")
		return this->getPerimeter();
	if (attributName == "yCentreDeMasse")
		return this->getYcentreDeMasse();
	if (attributName == "m00")
		return this->getM00();
	if (attributName == "m01")
		return this->getM01();
	if (attributName == "m10")
		return this->getM10();
	if (attributName == "m11")
		return this->getM11();
	if (attributName == "m02")
		return this->getM02();
	if (attributName == "m20")
		return this->getM20();
	if (attributName == "m12")
		return this->getM12();
	if (attributName == "m21")
		return this->getM21();
	if (attributName == "CAS")
		return this->getContourAproximationScores();
	if (attributName == "cross")
		return this->getCrossings();
	if (attributName == "dtw")
		return this->setDTW();
}

//Calcule des attributs

double Composant::getRatio() {

	if (attributs.find("ratio") == attributs.end())
		attributs["ratio"] = (finX - debutX) / (finY - debutY);

	return attributs["ratio"];
}

double Composant::getDensite() {

	if (attributs.find("densite") == attributs.end()) {
		int nbrPixelNoir = data.rows * data.cols - countNonZero(data);
		attributs["densite"] = ((double)nbrPixelNoir) / ((double)((finX - debutX)*(finY - debutY)));
	}

	return attributs["densite"];
}

double Composant::getPortionHauteur() {
	if (attributs.find("ph") == attributs.end()) {
		attributs["ph"] = ((double)(finY - debutY)) / ((double)data.rows);
	}

	return attributs["ph"];
}

double Composant::getNbrContours()
{
	if (attributs.find("nbrContours") == attributs.end()) {
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		Mat m;
		this->data.copyTo(m);
		threshold(m, m, 100, 255, CV_THRESH_BINARY_INV);
		findContours(m, contours, hierarchy, CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));
		attributs["nbrCnotours"] = contours.size();
		
	}
		return attributs["nbrCnotours"];
}

double Composant::getSurface()
{
	if (attributs.find("surface") == attributs.end()) {
		this->attributs["surface"] = this->contour_externe.empty()? -1:contourArea(this->contour_externe);
	}
	return this->attributs["surface"];
}

double Composant::getPerimeter()
{
	if (attributs.find("perimetre") == attributs.end()) {
		this->attributs["perimetre"] = this->contour_externe.empty() ? -1 : arcLength(this->contour_externe,true);
	}
	return this->attributs["perimetre"];
}

double Composant::getYcentreDeMasse()
{
	if (attributs.find("yCentreDeMasse") == attributs.end()) {
		this->attributs["yCentreDeMasse"] = this->contour_externe.empty() ? -1 : static_cast<float>(getM01() / getM00());
	}
	return this->attributs["yCentreDeMasse"];
}

double Composant::getM00()
{
	if (attributs.find("m00") == attributs.end()) {
		this->attributs["m00"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m00;
	}
	return this->attributs["m00"];
}
double Composant::getM01()
{
	if (attributs.find("m01") == attributs.end()) {
		this->attributs["m01"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m01;
	}
	return this->attributs["m01"];
}
double Composant::getM10()
{
	if (attributs.find("m10") == attributs.end()) {
		this->attributs["m10"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m10;
	}
	return this->attributs["m10"];
}
double Composant::getM11()
{
	if (attributs.find("m11") == attributs.end()) {
		this->attributs["m11"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m11;
	}
	return this->attributs["m11"];
}
double Composant::getM02()
{
	if (attributs.find("m02") == attributs.end()) {
		this->attributs["m02"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m02;
	}
	return this->attributs["m02"];
}
double Composant::getM20()
{
	if (attributs.find("m20") == attributs.end()) {
		this->attributs["m20"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m20;
	}
	return this->attributs["m20"];
}
double Composant::getM21()
{
	if (attributs.find("m21") == attributs.end()) {
		this->attributs["m21"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m21;
	}
	return this->attributs["m21"];
}
double Composant::getM12()
{
	if (attributs.find("m12") == attributs.end()) {
		this->attributs["m12"] = this->contour_externe.empty() ? -1 : cv::moments(this->contour_externe, true).m12;
	}
	return this->attributs["m12"];
}

double Composant::getHauteurRelative(double hChar)
{
	if (attributs.find("hr") == attributs.end()) {
		attributs["hr"] = ((double)(finY - debutY)) / hChar;
	}

	return attributs["hr"];
}


// To Do
double Composant::getContourAproximationScores()
{
	return 0.0;
}

double Composant::getCrossings()
{
	return 0.0;
}

double Composant::setDTW()
{
	return 0.0;
}