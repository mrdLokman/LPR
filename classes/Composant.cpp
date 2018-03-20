#include"Composant.h"


Composant::Composant() {

}

Composant::~Composant() {

}

double Composant::ratio(){
	
	if (attributs.find("ratio") == attributs.end())
		attributs["ratio"] = (finX - debutX) / (finY - debutY);
	
	return attributs["ratio"];
}

double Composant::densite(){

	if (attributs.find("densite") == attributs.end()) {
		int nbrPixelBlanc = countNonZero(data);
		attributs["densite"] = 1.0 - ((double)nbrPixelBlanc) / ((double)((finX - debutX)*(finY - debutY)));
	}

	return attributs["densite"];
}

double Composant::portionHauteur() {
	if (attributs.find("ph") == attributs.end()) {
		attributs["ph"] = ((double)(finY - debutY)) / ((double)data.rows);
	}

	return attributs["ph"];
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
	findContours(this->data, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	this->contour_externe = contours[0];
}



double Composant::getNbrContours()
{
	if (attributs.find("nbrContours") == attributs.end()) {
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(this->data, contours, hierarchy, CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));
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

double Composant::getYeentreDeMasse()
{
	this->centre_de_masse = Point2f(static_cast<float>(this->moments.m10 / this->moments.m00), static_cast<float>(this->moments.m01 / this->moments.m00));
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