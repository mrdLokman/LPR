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

void Composant::setRatio()
{
	this->ratio = this->largeur / this->hauteur;
}

void Composant::setNbrContours()
{
	if (!this->contours.empty())this->nbr_contours = this->contours.size();
	else {
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(this->data, contours, hierarchy, CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));
		this->nbr_contours = contours.size();
	}
}

void Composant::setSurface()
{
	this->surface = contourArea(this->contour_externe);
}

void Composant::setPerimeter()
{
	this->perimeter = arcLength(this->contour_externe, true);
}

void Composant::setCentreDeMasse()
{
	this->centre_de_masse = Point2f(static_cast<float>(this->moments.m10 / this->moments.m00), static_cast<float>(this->moments.m01 / this->moments.m00));
}

void Composant::setMoments()
{
	this->moments = cv::moments(this->contour_externe, true);
}